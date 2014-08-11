#include "stdafx.h"
#include "ThreadBase.h"

#include <process.h>


ThreadBase::ThreadBase() : m_uThreadId(0),
m_hThread(INVALID_HANDLE_VALUE),
m_bContinue(true),
m_bCriticalSectionInitialized(false)
{

	::InitializeCriticalSection(&m_CriticalSection);

	m_bCriticalSectionInitialized = true;
}

void ThreadBase::Cleanup(const bool bDeleteCriticalSection)
{
	if (m_bCriticalSectionInitialized && bDeleteCriticalSection)
	{
		::DeleteCriticalSection(&m_CriticalSection);
		m_bCriticalSectionInitialized = false;

	}

	if (INVALID_HANDLE_VALUE != m_hThread)
	{
		::CloseHandle(m_hThread);
		m_hThread = INVALID_HANDLE_VALUE;
	}

	m_uThreadId = 0;
}

HANDLE ThreadBase::Handle()
{
	HANDLE hRet;

	::EnterCriticalSection(&m_CriticalSection);

	hRet = m_hThread;

	::LeaveCriticalSection(&m_CriticalSection);

	return hRet;
}




int ThreadBase::Run()
{
	if (!PreOperation())
	{
		PostOperation();
		return  -1;
	}

	while (GetContinue())
	{
		Operation();
	}

	PostOperation();

	return S_OK;
}

void ThreadBase::Start(const int iPriority)
{
	End(true);

	SetContinue(true);

	m_hThread = reinterpret_cast<HANDLE> (::_beginthreadex(nullptr, 0, ThreadBase::RunProc, this, 1, &m_uThreadId));

	if (m_hThread != INVALID_HANDLE_VALUE )
	{
		::SetThreadPriority(m_hThread, iPriority);
	}
}

void ThreadBase::End(const bool bCalledFromOutside)
{
	if (m_hThread == INVALID_HANDLE_VALUE)
	{
		return;
	}

	SetContinue(false);

	// The thread cannot wait for itself when stopping. Also, CleanUp cannot be called
	// while the thread itself has not finished.
	if (!bCalledFromOutside)
	{
		return;
	}

	::WaitForSingleObject(m_hThread, INFINITE);  //called by main thread. Waits until this thread is done executing.

	Cleanup(false);
}

bool ThreadBase::GetContinue()
{
	bool bRet;

	::EnterCriticalSection(&m_CriticalSection);

	bRet = m_bContinue;

	::LeaveCriticalSection(&m_CriticalSection);

	return (bRet);
}

void ThreadBase::SetContinue
(
const bool bContinue // Value to set
)
{
	::EnterCriticalSection(&m_CriticalSection);

	m_bContinue = bContinue;

	::LeaveCriticalSection(&m_CriticalSection);
}
