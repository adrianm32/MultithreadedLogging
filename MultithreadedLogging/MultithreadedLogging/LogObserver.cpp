#include "stdafx.h"

#include "LogObserver.h"
#include "LogDistributor.h"

LogObserver::LogObserver(const int iArea, const LogMessage::Level nLevel, const Type nType) :
m_iArea(iArea),
m_nLevel(nLevel),
m_nType(nType),
m_hLogSemaphore(INVALID_HANDLE_VALUE),
m_bProcessQueueToEnd(false)
{
	m_hLogSemaphore = ::CreateSemaphore(NULL, 0, 0xfffffff, NULL);


	if (m_hLogSemaphore == NULL)
	{
		m_hLogSemaphore = INVALID_HANDLE_VALUE;
		throw(INVALID_HANDLE_VALUE);
	}

}

LogObserver::~LogObserver()
{
	if (m_hLogSemaphore != INVALID_HANDLE_VALUE)
	{
		::CloseHandle(m_hLogSemaphore);
		m_hLogSemaphore = INVALID_HANDLE_VALUE;
	}
}

void LogObserver::SetProcessQueueToEnd
(
const bool bProcessQueueToEnd /* = true */ // new value
)
{
	::EnterCriticalSection(&GetCriticalSection());

	m_bProcessQueueToEnd = bProcessQueueToEnd;

	::LeaveCriticalSection(&GetCriticalSection());
}


bool LogObserver::GetProcessQueueToEnd()
{
	bool bRet;

	::EnterCriticalSection(&GetCriticalSection());

	bRet = m_bProcessQueueToEnd;

	::LeaveCriticalSection(&GetCriticalSection());

	return (bRet);
}


void LogObserver::Operation()
{
	switch (::WaitForSingleObject(m_hLogSemaphore, INFINITE))
	{
		case WAIT_OBJECT_0:
		{
							  LogMessage oMessage(m_oMessageQueue.GetMessage());

							  Write(oMessage);
		}
		default:
			break;
	}
}
