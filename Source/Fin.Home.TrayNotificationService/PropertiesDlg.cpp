// PropertiesDlg.cpp : implementation file
//

#pragma once

#include "stdafx.h"
#include "SettingsManager.h"
#include "PropertiesDlg.h"

PropertiesDlg::~PropertiesDlg()
{
}

void PropertiesDlg::OnDestroy()
{
	//m_PropertySheet.RemovePage(1);
	//m_PropertySheet.RemovePage(0);	

	m_PropertySheet.DestroyWindow();
}

BOOL PropertiesDlg::OnInitDialog(HWND hwndFocus, LPARAM lParam) 
{
	m_General.SetTitle("Alerts");
	m_adv.SetTitle("Settings...");
	//m_creditsDebits.SetTitle("Credits/Debits");

	m_PropertySheet.AddPage( m_General );	
	m_PropertySheet.AddPage( m_adv );
	//m_Properties.AddPage( &m_creditsDebits );

	m_PropertySheet.Create(m_hWnd);
	m_PropertySheet.SetParent(m_hWnd);

	m_PropertySheet.ModifyStyle(0,WS_CHILD | WS_VISIBLE); 
	// remove dialog modal frame, set client edge style
	m_PropertySheet.ModifyStyleEx(0,WS_EX_CONTROLPARENT);

	//Remove OK, Cancel Buttons
	CWindow(m_PropertySheet.GetDlgItem(IDOK)).ShowWindow(SW_HIDE); 
	CWindow(m_PropertySheet.GetDlgItem(IDCANCEL)).ShowWindow(SW_HIDE);


    CRect rcClient;
    GetClientRect( &rcClient );

    const int SIZE_ADJUST_WIDTH = 35;
    const int SIZE_ADJUST_HEIGHT = 55;
	// Properly align the tab control with in the form
    m_PropertySheet.MoveWindow( 5, 5 , rcClient.Width() - SIZE_ADJUST_WIDTH, rcClient.Height() - SIZE_ADJUST_HEIGHT );
    CTabCtrl pCtrl = m_PropertySheet.GetTabControl();
    pCtrl.MoveWindow( 0, 0, rcClient.Width() - SIZE_ADJUST_WIDTH, rcClient.Height() - SIZE_ADJUST_HEIGHT );

	CString pol,inv,tsk;
	bool stat = _financeDB.GetAlerts(pol,inv,tsk);
	if(stat)
	{
		// Set the general settings tab page as default
		m_PropertySheet.SetActivePage( 0 );
	}
	else
	{
		m_PropertySheet.SetActivePage( 1 );
	}

    CRect rcWindow;
    GetWindowRect( &rcWindow );

    int nScreenWidth = ::GetSystemMetrics( SM_CXSCREEN );
    int nScreenHeight = ::GetSystemMetrics( SM_CYSCREEN );

    int nDX = ( nScreenWidth / 2 )- ( rcWindow.Width() / 2 );
    int nDY = ( nScreenHeight / 2 ) - ( rcWindow.Height() / 2 );
	// Show our window in the centre of the screen
    MoveWindow( nDX, nDY, rcWindow.Width() - 25, rcWindow.Height());
	//SetIcon( AfxGetApp()->LoadIcon( IDR_MAINFRAME ), TRUE );
	SetMsgHandled(FALSE);
	return FALSE;   
}

void PropertiesDlg::OnApply(UINT uCode, int nID, HWND hwndCtrl) 
{
	SConfig localConfig = _globalConfigObject;
	bool stat = m_adv.GetConfig(localConfig); 
	if(stat)
	{
		_globalConfigObject = localConfig;
		CSettingsManager setMgr;
		setMgr.UpdateData(_globalConfigObject,false); 
		::PostMessage(_globalObjects.m_hMainWnd,UM_FINANCE,UP_SETTINGS_CHANGE,0);

		if(IDC_CONFG_OK == nID)
		{
			SendMessage(WM_CLOSE); 
		}
	}
}

void PropertiesDlg::OnCancel(UINT uCode, int nID, HWND hwndCtrl) 
{
    SendMessage(WM_CLOSE); 
}

void PropertiesDlg::OnOK(UINT uCode, int nID, HWND hwndCtrl) 
{
	OnApply(uCode,nID,hwndCtrl );
}

void PropertiesDlg::OnClose() 
{
	// Apply the changes first, then close the form
	::PostMessage(_globalObjects.m_hMainWnd,UM_FINANCE,UP_DESKTOP_PROPERTY_CLOSE,0);
	DestroyWindow();
}

void PropertiesDlg::OnOpenDb(UINT uCode, int nID, HWND hwndCtrl) 
{
	CString path,param;
	_financeDB.GetFinanceHomeApp(path,param); 
	ShellExecute(NULL, "open", path, param, NULL, SW_SHOWNORMAL);	
}

void PropertiesDlg::OnSize(UINT nType, CSize size) 
{
	switch (nType)
    {
        case SIZE_MINIMIZED:
            SendMessage(WM_CLOSE); 
            break;
    }
}

