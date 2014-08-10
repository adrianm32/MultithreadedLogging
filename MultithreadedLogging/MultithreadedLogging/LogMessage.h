#pragma once

#include <string>
#include <vector>


class LogMessage
{
public:
	enum Area
	{
		evLogInternal = 0,
		evGUI = 1,
		evCOM = 2,
		evUserActivity = 4,
		evAll = 7
	};

	enum Level
	{
		evError,
		evEvent,
		evTrace,
		evDebug
	};

	LogMessage (
		const Area nArea,
		const Level nLevel,
		char * pszText
		) : m_nArea(nArea), m_nLevel(nLevel), m_strText(pszText)
	{};

	LogMessage
		(
		const Area nArea,                                 // The message area
		const Level nLevel,                               // The message level
		std::string & strText                                  // The message text
		)
		: m_nArea(nArea), m_nLevel(nLevel), m_strText(strText)
	{ }

	//copy ctor
	LogMessage(const LogMessage & rSrc)
		:m_nArea(rSrc.m_nArea), m_nLevel(rSrc.m_nLevel), m_strText(rSrc.m_strText)
	{}

	// Returns the message text
	std::string GetText() const { return (m_strText); }

	// Returns the message area
	Area GetArea() const { return (m_nArea); }

	// Returns the message level
	Level GetLevel() const { return (m_nLevel); }

	

private:
	std::string m_strText;

	Area m_nArea;
	Level m_nLevel;

};
