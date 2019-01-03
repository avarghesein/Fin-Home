// MyClock.cpp : Defines the class behaviors for the application.
//

#pragma once

#include "stdafx.h"
#include "Finance.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFinApp

BEGIN_MESSAGE_MAP(CFinApp, CWinApp)
	//{{AFX_MSG_MAP(CFinApp)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFinApp construction

CFinApp::CFinApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CFinApp object

CComModule _Module;
CFinApp theApp;
SConfig _globalConfigObject;

/////////////////////////////////////////////////////////////////////////////
// CFinApp initialization

BOOL CFinApp::InitInstance()
{
	::CoInitialize(NULL);

	_Module.Init(NULL, AfxGetInstanceHandle());	

	AfxEnableControlContainer();

	bool serviceMode = false;

	CString* pcmdLine = new CString( m_lpCmdLine );
	pcmdLine->MakeLower();
	serviceMode = pcmdLine->FindOneOf(T("-startup")) >= 0;
	delete pcmdLine;

    if (!m_wndOwner.Create(m_pMainWnd, serviceMode)) return FALSE;

	theApp.Run();

	m_wndOwner.DestroyWindow();

	_Module.Term();

	return FALSE;
}