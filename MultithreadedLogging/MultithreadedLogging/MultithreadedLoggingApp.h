#pragma once
#include "stdafx.h"
#include "resource.h"

class MultiThreadedLoggingApp : public CWinApp
{
public:
	MultiThreadedLoggingApp() {};

	virtual BOOL InitInstance();
	virtual int ExitInstance();

	afx_msg void  OnConfigObserver();   //afxwin.h type modifier for message handlers
	afx_msg void OnLogGui(void);
	afx_msg void OnLogCom();
	afx_msg void OnLogUserActivity();

	DECLARE_MESSAGE_MAP();  //afxwin.h Window message map handling macro


protected:
	HMENU m_hMDIMenu;

private:

};