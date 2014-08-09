#pragma once
#include "stdafx.h"

class ThreadBase
{
public:
	//Thread function passed to BeginThreadEx when creating the thread
	static unsigned int __stdcall RunProc(void * pInstance)
	{
		return (static_cast<ThreadBase *>(pInstance)->Run());
	}

	ThreadBase();
	virtual ~ThreadBase() { Cleanup(true); }

	HANDLE Handle();

	void Start(const int iPriority = THREAD_PRIORITY_NORMAL);

	void End(const bool bCalledFromOutside);

protected:
	virtual bool PreOperation() { return true; }

	virtual void Operation() = 0;

	virtual void PostOperation() {}

	CRITICAL_SECTION & GetCriticalSection() { return m_CriticalSection;  }


private:
	int Run();
	void Cleanup(const bool bDeleteCriticalSection);

	bool GetContinue();
	void SetContinue(const bool val);

	unsigned m_uThreadId;
	HANDLE m_hThread;
	bool m_bContinue;
	bool m_bCriticalSectionInitialized;
	CRITICAL_SECTION  m_CriticalSection;
};