#include "stdafx.h"

#include "LogDistributor.h"
#include "LogObserver.h"

#include <algorithm>  //algos like for_each, sorting, heap, findmin etc.

/*
Defines Standard Library functions that help construct function objects—also known as functors—and their binders. A function object is an object of a type that defines operator(). A function object can be a function pointer, but more typically, the object is used to store additional information that can be accessed during a function call.
*/
#include <functional> 

LogDistributor & LogDistributor::Instance()
{
	static LogDistributor instance;   //static, so only one instance.  singleton.
	return instance;
}

LogDistributor::~LogDistributor()
{
	// Free the log semaphore
	if (m_hLogSemaphore != INVALID_HANDLE_VALUE)
	{
		::CloseHandle(m_hLogSemaphore);
		m_hLogSemaphore = INVALID_HANDLE_VALUE;
	}

	// If there is no critical section, the vector containing the observers
	// cannot be handled too.
	if (!m_bCriticalSectionInitialized)
	{
		ASSERT(m_vecObserver.empty()); // there shouldn't exist any more observer
		return;
	}


	while (!m_vecObserver.empty())
	{
		*this -= *m_vecObserver.begin();   //*this deref itself.  
	}

	::DeleteCriticalSection(&m_CriticalSection);

	m_bCriticalSectionInitialized = false;
}


LogDistributor & LogDistributor::Initialize()
{
	::InitializeCriticalSection(&m_CriticalSection);

	m_bCriticalSectionInitialized = true;

	m_hLogSemaphore = ::CreateSemaphore(NULL, 0, 0xfffffff, NULL);

	if (m_hLogSemaphore == NULL)
	{
		m_hLogSemaphore = INVALID_HANDLE_VALUE;
		throw (INVALID_HANDLE_VALUE);
	}

	return (*this);   //*this deref itself.
}


void LogDistributor::operator+= (LogObserver * pObserver)
{
	bool bStartObserver = false;

	::EnterCriticalSection(&m_CriticalSection);

	itObserverVector itEnd = m_vecObserver.end();  // vector end is the past-the-end theoretical element.
	itObserverVector itFind = std::find(m_vecObserver.begin(), itEnd, pObserver);

	if (itFind != itEnd)    // if it does not find the element, then std::find returns the last element which is the past-the-end theoretical element.
	{
		goto End;
	}

	m_vecObserver.push_back(pObserver);
	bStartObserver = true;

End:
	::LeaveCriticalSection(&m_CriticalSection);

	if (bStartObserver)
	{
		pObserver->Start();
	}

}

void LogDistributor::operator -= (const LogObserver * pObserver)
{
	LogObserver * pObserverToStop = nullptr;

	::EnterCriticalSection(&m_CriticalSection);

	itObserverVector itEnd = m_vecObserver.end();
	itObserverVector itFind = std::find(m_vecObserver.begin(), itEnd, pObserver);

	if (itFind != itEnd)
	{
		pObserverToStop = *itFind;   //iterator is a pointer to the item in the vector. so need to deref it.
		m_vecObserver.erase(itFind);   //removes element from vector.
	}

	::LeaveCriticalSection(&m_CriticalSection);

	if (!pObserverToStop)
	{
		return;
	}

	pObserverToStop->AddMessage(LogMessage(LogMessage::evLogInternal, LogMessage::evEvent, ""));

	::ReleaseSemaphore(pObserverToStop->GetSemaphoreHandle(), 1L, nullptr);

	pObserverToStop->GetProcessQueueToEnd() ? ::WaitForSingleObject(pObserverToStop->Handle(), INFINITY)  //wait for the observer thread to finish processing all its messages.
		: pObserverToStop->End(true);

	delete pObserverToStop;
}

void LogDistributor::Log(const LogMessage & rMessage)
{

	m_oMessageQueue += rMessage;

	::ReleaseSemaphore(m_hLogSemaphore, 1L, nullptr);  //release this to signal the distributor to start processing this message.
}

void LogDistributor::Operation()
{
	if (::WaitForSingleObject(m_hLogSemaphore, evWaitTimeout) != WAIT_OBJECT_0)
	{
		return;
	}

	LogMessage message(m_oMessageQueue.GetMessage());  //define a class with its ctor

	::EnterCriticalSection(&m_CriticalSection);

	std::for_each(m_vecObserver.begin(), m_vecObserver.end(), std::bind2nd(CreateLogEntry(), message)); //iterate through each LogObserver and let it add message to its queuee if the message is for that observer.

	::LeaveCriticalSection(&m_CriticalSection);

}