// MyClockDlg.cpp : implementation file
//

#pragma once

#include "stdafx.h"
#include "FinanceDlg.h"
#include "PropertiesDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef IDC_HAND
    #define IDC_HAND MAKEINTRESOURCE(32649)
#endif

BEGIN_MESSAGE_MAP(CFinWindow, CWnd)
    //{{AFX_MSG_MAP(CFinWindow)
    ON_WM_TIMER()
	ON_WM_CONTEXTMENU()
    ON_COMMAND(ID_MAIN_EXIT, OnMainExit)
    ON_COMMAND(ID_MAIN_OPTIONS, OnMainOptions)
	ON_MESSAGE(UM_FINANCE, OnUserMessage)
	ON_MESSAGE(UM_TRAYMENU, OnSysTrayMsg )
    ON_WM_DESTROY()
	ON_WM_ENDSESSION()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CFinWindow::CFinWindow() : 
	_pWndNotify(0),
	_pWndProperty(0),
	_isPropertyBoxOpened(false)
{ }

BOOL CFinWindow::Create(CWnd*& mainWind)
{
	if( 0 != m_hWnd )
	{
		return TRUE;
	}

	LPCTSTR pstrOwnerClass = AfxRegisterWndClass( 0 ) ;

	//Check this application is up already?
	HWND hWnd  = ::FindWindow(pstrOwnerClass,FIN_UNIQUE_WINDOW_NAME);	
	if(hWnd && ::IsWindow(hWnd))
	{
		//If up ask older application to open the property window
		::PostMessage(hWnd,UM_FINANCE,UP_DESKTOP_NOTIFY_CLICK,0); 
		return FALSE;
	}

	BOOL ret = CreateEx( 0, pstrOwnerClass, FIN_UNIQUE_WINDOW_NAME, WS_POPUP, CW_USEDEFAULT,
								   CW_USEDEFAULT,
								   CW_USEDEFAULT,
								   CW_USEDEFAULT,
								   NULL, 0 );

	if (ret == TRUE)
	{
		ShowWindow(SW_HIDE);
		UpdateWindow();
		mainWind = this;

		memset( &m_stNotifyData, 0, sizeof( m_stNotifyData ));
		m_stNotifyData.cbSize = sizeof( m_stNotifyData );
		m_stNotifyData.uID = 101;
		m_stNotifyData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
		m_stNotifyData.hIcon =  ::LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_RED ));

		m_stNotifyData.hWnd = m_hWnd;
		AfxLoadString( IDS_APP_NAME ,m_stNotifyData.szTip, 64 );
		m_stNotifyData.uCallbackMessage = UM_TRAYMENU;

		_isPropertyBoxOpened = false;
		_settingMgr.UpdateData(_globalConfigObject,true);

		OnUserMessage(UP_SETTINGS_CHANGE,0);

		bool isStartupWaitTimer = false;
		int interval = GetAlertTimerInterval(isStartupWaitTimer); 
		if(interval > 0 && !isStartupWaitTimer)
		{
			OnTimer(MSG_AlERT_CHECK_TIMER);
		}
		else
		{
			EnableAlertTimer(true);
		}
	}
	return ret;
}

bool CFinWindow::EnableAlertTimer(bool on)
{
	KillTimer(MSG_AlERT_STARTUP_TIMER);
	KillTimer(MSG_AlERT_CHECK_TIMER);

	if(on)
	{
		if(!_pWndProperty && !_pWndNotify)
		{
			bool isStartupWaitTimer = false;
			int interval = GetAlertTimerInterval(isStartupWaitTimer);  

			if( interval > 0 )
			{
				SetTimer( 
					isStartupWaitTimer ? 
						MSG_AlERT_STARTUP_TIMER : MSG_AlERT_CHECK_TIMER,
					interval * 1000, 0 );
			}
		}		
	}	
	return true;
}

void CFinWindow::OnTimer( UINT nIDEvent )
{
	bool isAlert = false;

	switch(nIDEvent)
	{
	case MSG_AlERT_STARTUP_TIMER:
		{
			EnableAlertTimer(true);
		}
		break;

	case MSG_AlERT_CHECK_TIMER:

		EnableAlertTimer(false);
		
		if(!_pWndProperty && !_pWndNotify)
		{
			isAlert = _financeDB.GetAlerts(_policyDues,_invDues,_tskDues); 
		}

		if(isAlert)
		{
			DesktopNotification(true);			
		}
		else
		{
			EnableAlertTimer(true);
		}
		break;

	default:
		break;
	}
    CWnd::OnTimer(nIDEvent);
}


void CFinWindow::OnMainExit()
{
	::Shell_NotifyIcon( NIM_DELETE, &m_stNotifyData );
}

void CFinWindow::OnMainOptions()
{
	SetForegroundWindow();
	ShowPropertyBox(true);
}

bool CFinWindow::ShowPropertyBox(bool isCreate)
{
	if(_pWndProperty)
	{
		if(IsWindow(_pWndProperty->m_hWnd))
		{
			if(isCreate)
			{
				_pWndProperty->ShowWindow(SW_NORMAL);
				_pWndProperty->UpdateWindow();
				return true;
			}
			else
			{
				_pWndProperty->DestroyWindow(); 
			}
		}
		delete _pWndProperty;
		_pWndProperty = 0;
	}

	if(isCreate)
	{
		DesktopNotification(false);	

		_pWndProperty = new PropertiesDlg();   
		_pWndProperty->Create(IDD_DIALOG_CONFIG,this); 
		_pWndProperty->ShowWindow(SW_NORMAL);
		_pWndProperty->UpdateWindow();
	}
	return true;
}

bool CFinWindow::DesktopNotification(bool isCreate)
{
	if(_pWndNotify)
	{
		if (IsWindow(_pWndNotify->m_hWnd))
		{
			_pWndNotify->DestroyWindow();			
		}
		delete _pWndNotify;
		_pWndNotify = 0;
	}

	if(isCreate)
	{
		_pWndNotify=new CNotifyWnd(m_hWnd,300,100);
		_pWndNotify->CreateNotifyWindow();
	}
	return true;
}

LRESULT CFinWindow::OnUserMessage(WPARAM wParam, LPARAM lParam )
{
	switch(wParam)
	{
		case UP_DESKTOP_NOTIFY_CLICK:
			{							
				ShowPropertyBox(true);
				EnableAlertTimer(false);
			}
		break;

		case UP_DESKTOP_NOTIFY_CLOSE:
			{
				DesktopNotification(false);	
				EnableAlertTimer(true);
			}
		break;

		case UP_DESKTOP_PROPERTY_CLOSE:
			{
				ShowPropertyBox(false);

				_isPropertyBoxOpened = true;

				EnableAlertTimer(true);
			}
		break;

		case UP_SETTINGS_CHANGE:
			{
				//Show tray icon, if configured
				::Shell_NotifyIcon( NIM_DELETE, &m_stNotifyData );
				if(_globalConfigObject.ShowInSystemTray)
				{
					::Shell_NotifyIcon( NIM_ADD, &m_stNotifyData );
				}
			}
		break;
	}
    
    return 0;
}
void CFinWindow::OnDestroy()
{
	// Deletes shell notify icon
    ::Shell_NotifyIcon( NIM_DELETE, &m_stNotifyData );

	CWnd::OnDestroy();
}

void CFinWindow::OnEndSession( BOOL bEnding ) 
{
	if(bEnding)
	{
	}

	CWnd::OnEndSession(bEnding); 
}

int CFinWindow::GetAlertTimerInterval(bool& isStartupWaitTimer)
{
	isStartupWaitTimer = false;

	SConfig config = _globalConfigObject;

	if(config.UnNotifyTillNextTime && _isPropertyBoxOpened) 
	{
		return -1;
	}
	
	if(config.UnNotifyTillNextDTM)
	{
		CTime nextAlertTime = config.UnNotifyTillNext_DTM_VALUE;
		CTime curTime = CTime::GetCurrentTime(); 

		if(nextAlertTime <= curTime)
		{
			_globalConfigObject.UnNotifyTillNextDTM = false;
			_globalConfigObject.UnNotifyTillNext_DTM_VALUE = curTime;
			_settingMgr.UpdateData(_globalConfigObject,false);

			return config.AlertTimeInterval;
		}

		CTimeSpan tmeSpn = nextAlertTime - curTime;
		
		LONG secs = tmeSpn.GetTotalSeconds(); 

		if(secs > 0)
		{
			isStartupWaitTimer = true;
			return secs;
		}
	}

	return config.AlertTimeInterval; 
}

LRESULT CFinWindow::OnSysTrayMsg( WPARAM wParam, LPARAM lParam )
{
    if( WM_LBUTTONDBLCLK == lParam )
    {
       OnMainOptions();
    }
    else if( WM_RBUTTONDOWN == lParam )
    {
        CMenu menu;
        SetForegroundWindow();
        menu.LoadMenu( IDR_FIN_MENU );
        CPoint pt;
        ::GetCursorPos( &pt );
        menu.GetSubMenu( 0 )->TrackPopupMenu( TPM_RIGHTALIGN, pt.x, pt.y, this );
    }
    else if( WM_LBUTTONDOWN == lParam )
    {
        OnMainOptions();
    }
    return 0;
}

void CFinWindow::OnContextMenu(CWnd* pWnd, CPoint point)
{
	OnSysTrayMsg( 0, WM_RBUTTONDOWN );
    return;
}
