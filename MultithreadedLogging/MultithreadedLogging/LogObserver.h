#pragma once

#include "ThreadBase.h"
#include "LogMessage.h"
#include "LogMessageQueue.h"

class LogObserver : public ThreadBase
{
public:

	enum Type {Console, Window, File};

	virtual ~LogObserver();


	// Returns the area(s) to observe
	int GetArea() const { return (m_iArea); }

	// Returns the level to observe
	LogMessage::Level GetLevel() const { return (m_nLevel); }

	// Returns the observer's type
	Type GetType() const { return (m_nType); }

	// Adds a message to the message queue; thread safe
	LogObserver & AddMessage(const LogMessage & rMessage)
	{
		m_oMessageQueue += rMessage; return (*this);
	}

	// Returns the handle to the (log message) semaphore
	HANDLE GetSemaphoreHandle() const { return (m_hLogSemaphore); }

	// Sets the value of m_bProcessQueueToEnd; thread safe
	void SetProcessQueueToEnd
		(
		const bool bProcessQueueToEnd = true // new value
		);

	// Returns the value of m_bProcessQueueToEnd; thread safe
	bool GetProcessQueueToEnd();

protected:
	LogObserver(const int iArea, const LogMessage::Level nLevel, const Type nType);

	virtual bool PreOperation() override { return true; }

	virtual void Operation() override;	

	virtual void Write(const LogMessage & rMessage) = 0;

private:
	LogObserver();

	const int m_iArea;

	const LogMessage::Level m_nLevel;  //can only be initialized in ctor and then cannot be modified.

	const Type m_nType;

	LogMessageQueue m_oMessageQueue;

	HANDLE m_hLogSemaphore;

	bool m_bProcessQueueToEnd;




};