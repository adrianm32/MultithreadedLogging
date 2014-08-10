#include "stdafx.h"

#include <io.h>
#include <fcntl.h>   //file control
#include "MultithreadedLoggingApp.h"
#include "MainFrame.h"
#include "LogDistributor.h"
#include "LoggerThread.h"
#include "ConsoleObserver.h"


BEGIN_MESSAGE_MAP(MultiThreadedLoggingApp, CWinApp)
	ON_COMMAND(ID_OBSERVER_CONFIGURE, OnConfigObserver)
	ON_COMMAND(ID_MESSAGES_GENERATEGUIMESSAGES, OnLogGui)
	ON_COMMAND(ID_MESSAGES_GENERATECOMMESSAGES, OnLogCom)
	ON_COMMAND(ID_MESSAGES_GENERATEUSERACTIVITYMESSAGES, OnLogUserActivity)
END_MESSAGE_MAP()


//Singleton app object.
MultiThreadedLoggingApp theApp;

//threads to generate log output.
static LoggerThread saLogger[3];   // it contains objects of type LoggerThread automatically. 

BOOL MultiThreadedLoggingApp::InitInstance()
{
	CMDIFrameWnd * pFrame = new MainFrame;
	m_pMainWnd = pFrame;

	// create main MDI frame window
	if (!pFrame->LoadFrame(IDR_MENU1))
	{
		return FALSE;
	}

	//Create a console Window
	int hCrt;
	FILE *hf;

	::AllocConsole();   //consoleapi.h    allocates a new console for the calling process

	hCrt = _open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);
	hf = _fdopen(hCrt, "w");
	*stdout = *hf;   //wchar.h stdstream defined.
	setvbuf(stdout, nullptr, _IONBF, 0);  //set stream buffer.  _IONBF  no buffering for IO. Output is written immediately.

	//Create the instance and intiliaze its critical section and start .
	LogDistributor::Instance().Initialize().Start();

	saLogger[0].Start();
	saLogger[1].Start();
	saLogger[2].Start();



	pFrame->ShowWindow(m_nCmdShow);
	pFrame->UpdateWindow();

	return TRUE;

}

int MultiThreadedLoggingApp::ExitInstance()
{
	

	// Stop the log output generator threads
	saLogger[0].End(true);
	saLogger[1].End(true);
	saLogger[2].End(true);

	LogDistributor::Instance().End(true);

	return CWinApp::ExitInstance();
}

void MultiThreadedLoggingApp::OnConfigObserver()
{

	LogDistributor::Instance() += new ConsoleObserver(LogMessage::evUserActivity, LogMessage::evTrace);
	LogDistributor::Instance() += new ConsoleObserver(LogMessage::evCOM, LogMessage::Level::evEvent);
	//LogDistributor::Instance() += new ConsoleObserver(LogMessage::evGUI, LogMessage::evTrace);
}

void MultiThreadedLoggingApp::OnLogCom()
{
	LogDistributor::Instance().Log(LogMessage(LogMessage::evCOM, LogMessage::evEvent,
		"This is my Com-Error-Output."));

}

void MultiThreadedLoggingApp::OnLogGui()
{
	LogDistributor::Instance().Log(LogMessage(LogMessage::evGUI, LogMessage::evTrace,
		"This is my GUI-Trace-Output."));

}

void MultiThreadedLoggingApp::OnLogUserActivity()
{
	LogDistributor::Instance().Log(LogMessage(LogMessage::evUserActivity,
		LogMessage::evTrace,
		"This is my UserActivity-Event-Output."));

}
