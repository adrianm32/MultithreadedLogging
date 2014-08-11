#pragma once 

#include <stdio.h>

#include "LogObserver.h"
#include "LogMessage.h"

class ConsoleObserver : public LogObserver
{
public:
	ConsoleObserver(const int iArea, const LogMessage::Level nLevel) : LogObserver(iArea, nLevel, Console)
	{}

protected:
	virtual void Write(const LogMessage & rMessage)
	{
		printf_s("ConsoleObserver (%ld): %s\n", ::GetCurrentThreadId(), rMessage.GetText().c_str());
	}


private:

};
