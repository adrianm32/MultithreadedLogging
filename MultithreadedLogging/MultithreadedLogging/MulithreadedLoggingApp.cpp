#include "stdafx.h"

#include <io.h>
#include <fcntl.h>   //file control
#include "MultithreadedLoggingApp.h"
#include "MainFrame.h"

BEGIN_MESSAGE_MAP(MultiThreadedLoggingApp, CWinApp)
	ON_COMMAND(ID_OBSERVER_CONFIGURE, OnConfigObserver)
	ON_COMMAND(ID_MESSAGES_GENERATEGUIMESSAGES, OnLogGui)
	ON_COMMAND(ID_MESSAGES_GENERATECOMMESSAGES, OnLogCom)
	ON_COMMAND(ID_MESSAGES_GENERATEUSERACTIVITYMESSAGES, OnLogUserActivity)
END_MESSAGE_MAP()


//Singleton app object.
MultiThreadedLoggingApp theApp;

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


	pFrame->ShowWindow(m_nCmdShow);
	pFrame->UpdateWindow();

	return TRUE;

}

int MultiThreadedLoggingApp::ExitInstance()
{
	

	// Stop the log output generator threads
	/*saLogger[0].End(true);
	saLogger[1].End(true);
	saLogger[2].End(true);*/

	//LogDistributor::Instance().End(true);

	return CWinApp::ExitInstance();
}

void MultiThreadedLoggingApp::OnConfigObserver()
{
	CPropertySheet sheet(ID_OBSERVER_CONFIGURE);
	/*PpCfgObserver ppCfgObserver;

	sheet.AddPage(&ppCfgObserver);*/

	sheet.DoModal();
}

void MultiThreadedLoggingApp::OnLogCom()
{

}

void MultiThreadedLoggingApp::OnLogGui()
{

}

void MultiThreadedLoggingApp::OnLogUserActivity()
{

}
