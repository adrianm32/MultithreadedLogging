#include "stdafx.h"
#include "LogMessageQueue.h"


LogMessageQueue::LogMessageQueue() : m_bCriticalSectionInitialized(false)
{
	::InitializeCriticalSection(&m_CriticalSection);

	m_bCriticalSectionInitialized = true;

}

LogMessageQueue::~LogMessageQueue()
{
	printf("Queue Size : %ld \n", m_Queue.size());

	if (m_bCriticalSectionInitialized)
	{
		::DeleteCriticalSection(&m_CriticalSection);
		m_bCriticalSectionInitialized = false;
	}
}

void LogMessageQueue::operator+=(const LogMessage & rLogMessage)
{
	::EnterCriticalSection(&m_CriticalSection);

	m_Queue.push(rLogMessage);

	::LeaveCriticalSection(&m_CriticalSection);
}

LogMessage LogMessageQueue::GetMessage()
{
	LogMessage oRet(LogMessage::evAll, LogMessage::evError, "No Log Message in the Queue");

	::EnterCriticalSection(&m_CriticalSection);

	if (m_Queue.empty())
	{
		ASSERT(false); //this should not happen.
	}
	else
	{
		oRet = m_Queue.front();
		m_Queue.pop();
	}

	::LeaveCriticalSection(&m_CriticalSection);

	return  oRet;
}