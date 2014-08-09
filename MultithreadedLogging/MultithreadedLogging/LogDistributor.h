#pragma once

#include <vector>

#include "ThreadBase.h"
#include "LogMessage.h"

class LogDistributor : public ThreadBase
{
public:
	LogDistributor();
	~LogDistributor();

private:

};

LogDistributor::LogDistributor()
{
}

LogDistributor::~LogDistributor()
{
}