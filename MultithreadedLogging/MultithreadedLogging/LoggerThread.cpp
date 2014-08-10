#include "stdafx.h"

#include "LoggerThread.h"
#include "LogMessage.h"
#include "LogDistributor.h"

#include <sstream>   //string stream helper


short LoggerThread::m_ssTimeMultiplicator = 0;



void LoggerThread::Operation()
{
	std::ostringstream message;

	message << ++m_lMessageNo << " sent by Thread ID " << m_dwThreadId;

	LogDistributor::Instance().Log(LogMessage (LogMessage::evGUI, LogMessage::evTrace, message.str()));

	::Sleep(75 * m_sTimeMultiplicator);
}