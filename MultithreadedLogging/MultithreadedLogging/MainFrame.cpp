#include "stdafx.h"

#include "MultithreadedLoggingApp.h"
#include "MainFrame.h"


IMPLEMENT_DYNAMIC(MainFrame, CMDIFrameWnd)   //implements RuntimeClass

BEGIN_MESSAGE_MAP(MainFrame, CMDIFrameWnd)
	ON_WM_CREATE()
//	ON_COMMAND(IDM_CONFIG_OBSERVER, &MainFrame::OnConfigObserver)
//	ON_COMMAND(IDM_LOG_COM, &MainFrame::OnLogCom)
//	ON_COMMAND(IDM_LOG_GUI, &MainFrame::OnLogGui)
//	ON_COMMAND(IDM_LOG_USERACTIVITY, &MainFrame::OnLogUseractivity)
END_MESSAGE_MAP()


MainFrame::~MainFrame()
{
}

//gets called by the ON_WM_CREATE message map macro.
int MainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}

	return 0;
}

BOOL MainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CMDIFrameWnd::PreCreateWindow(cs))
	{
		return FALSE;
	}

	return TRUE;
}



