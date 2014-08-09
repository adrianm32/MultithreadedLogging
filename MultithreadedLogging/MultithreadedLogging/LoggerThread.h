#pragma once

#include "ThreadBase.h"

class LoggerThread : public ThreadBase
{
public:
	LoggerThread() : m_sTimeMultiplicator(++m_ssTimeMultiplicator),
		m_lMessageNo(0L)
	{}

protected:
	virtual bool PreOperation() override
	{
		m_dwThreadId = ::GetCurrentThreadId();
		return true;
	}

	virtual void Operation() override;

private:
	// Multiplicator of the sleep time between generating a new message
	static short m_ssTimeMultiplicator;

	// Multiplicator of the sleep time between generating a new message
	const short m_sTimeMultiplicator;

	// Number of the generated messages
	long m_lMessageNo;

	// Id of the thread
	DWORD m_dwThreadId;

};
