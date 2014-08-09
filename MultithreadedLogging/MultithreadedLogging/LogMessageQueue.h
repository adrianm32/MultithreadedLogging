#pragma once

#include "stdafx.h"

#include <queue>


#include "LogMessage.h"

class LogMessageQueue
{
public:
	LogMessageQueue();
	~LogMessageQueue();

	void operator += (const LogMessage & rLogMessage);

	LogMessage GetMessage();

private:
	bool m_bCriticalSectionInitialized;
	CRITICAL_SECTION m_CriticalSection;

	//the message queue itself
	std::queue<LogMessage> m_Queue;

};
