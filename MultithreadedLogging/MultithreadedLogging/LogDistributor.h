#pragma once

#include <vector>

#include "ThreadBase.h"
#include "LogMessage.h"
#include "LogObserver.h"
#include "LogMessageQueue.h"

class LogDistributor : public ThreadBase
{
public:

	static LogDistributor & Instance();   //singleton , return value is by ref.  Returns the reference to the one and only logdistributor.

	 //private ctor for singleton.

	~LogDistributor();

	LogDistributor & Initialize();  //init the critical section and create the log semaphore.

	void operator += (LogObserver * pObserver);  //adds an observer to the list and starts it.

	void operator -= (const LogObserver * pObserver); //removes an observer from the list ,stops it and frees resources.

	void Log(const LogMessage & rMessage);  //adds a message to the distributor's message queue.

	int GetNumberOfObservers() const
	{
		return m_vecObserver.size();
	}

	const LogObserver * GetObserver(const int nObserver)
	{
		return m_vecObserver[nObserver];
	}

protected:
	virtual void Operation() override;
private:
	enum { evWaitTimeout = 1000 };


	LogDistributor() : m_bCriticalSectionInitialized(false), m_hLogSemaphore(INVALID_HANDLE_VALUE)
	{}

	typedef std::vector<LogObserver *> ObserverVector;
	typedef std::vector<LogObserver *>::iterator itObserverVector;

	//functor to tell the appropriate Observer to log the message and then release its semaphore.
	struct	CreateLogEntry : public std::binary_function<LogObserver *, LogMessage, bool>
	{
		bool operator () (LogObserver * pLogObserver, const LogMessage & rMessage) const
		{
			if (pLogObserver->GetArea() == rMessage.GetArea() && pLogObserver->GetLevel() == rMessage.GetLevel())
			{
				pLogObserver->AddMessage(rMessage);
				::ReleaseSemaphore(pLogObserver->GetSemaphoreHandle(), 1L, nullptr);
				return true;
			}
			return false;
		}

	};



	ObserverVector m_vecObserver;

	bool m_bCriticalSectionInitialized;
	CRITICAL_SECTION m_CriticalSection;

	//Threadsafe queue of all messages to distribute.
	LogMessageQueue m_oMessageQueue;

	HANDLE m_hLogSemaphore;

};