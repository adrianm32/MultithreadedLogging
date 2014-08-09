#pragma once

#include "stdafx.h"

class MainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(MainFrame)   //afx.h helper for defining CRuntimeClass compatible classes
public:
	MainFrame() {};
	virtual ~MainFrame();

	virtual BOOL PreCreateWindow(CREATESTRUCT & cs);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	DECLARE_MESSAGE_MAP();

private:

public:
//	afx_msg void OnConfigObserver();
//	afx_msg void OnLogCom();
//	afx_msg void OnLogGui();
//	afx_msg void OnLogUseractivity();
};