
#include "stdafx.h"
#include "resource.h"
#include "maindlg.h"

#include <dbt.h>


#ifndef IDC_HAND
    #define IDC_HAND MAKEINTRESOURCE(32649)
#endif

BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{		
	return MyBaseWin::IsDialogMessage(pMsg);
}

BOOL CMainDlg::OnIdle()
{
	return FALSE;
}

BOOL CMainDlg::Create(bool serviceMode)
{
	if( 0 != m_hWnd )
	{
		return TRUE;
	}

	//Check this application is up already?
	HWND hWnd  = ::FindWindow(FINANCE_UNIQUE_OBJECT_NAME, FINANCE_UNIQUE_OBJECT_NAME);	
	if(hWnd && ::IsWindow(hWnd))
	{
		//If up ask older application to open the property window
		::PostMessage(hWnd,UM_FINANCE,UP_DESKTOP_NOTIFY_CLICK,0); 
		return FALSE;
	}	
	
	//start the application
	HWND ret = MyBaseWin::Create(0,rcDefault,FINANCE_UNIQUE_OBJECT_NAME,SW_HIDE); 
	
	if (ret)
	{ 
		//Our main window is hidden
		MyBaseWin::ShowWindow(SW_HIDE); 
		UpdateWindow();

		//Load the menu
		_contextMenu.LoadMenu( IDR_FIN_MENU );

		//struct for Tray icon
		memset( &m_stNotifyData, 0, sizeof( m_stNotifyData ));
		m_stNotifyData.cbSize = sizeof( m_stNotifyData );
		m_stNotifyData.uID = 101;
		m_stNotifyData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
		//m_stNotifyData.hIcon =  ::LoadIcon(_Module.GetResourceInstance(),MAKEINTRESOURCE(IDI_YELLOW ));
		//m_stNotifyData.hIcon =  ::LoadIcon(0,IDI_WARNING); 
		//int cnt = ExtractIconEx(STR(EXPLORER.EXE),-110,&m_stNotifyData.hIcon,0,1);
		m_stNotifyData.hIcon = ExtractIcon(_Module.GetResourceInstance(),STR(EXPLORER.EXE),-110); 

		m_stNotifyData.hWnd = m_hWnd;
		LoadString(_Module.GetModuleInstance(),IDS_APP_NAME ,m_stNotifyData.szTip, 64 );
		m_stNotifyData.uCallbackMessage = UM_TRAYMENU;
		//Property box is not opened
		_isPropertyBoxOpened = false;
		//No alert check has been done
		_alertCheckedCount = 0;
		//Timers are enabled by default
		_timersDisabledForFuture = false;
		//Get the configuration from ini file
		_settingMgr.UpdateData(_globalConfigObject,true);
		//Say we have loaded settings and act accordingly
		OnUserMessage(UM_FINANCE, UP_SETTINGS_CHANGE,0);
		//If user has directly clicked the application? Directly open the property box
		//Else the application is in service mode, do not show property box initially
		if( serviceMode )
		{
			//If service mode, setup the timers
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
		else
		{
			//user mode, show property box
			SendMessage(UM_FINANCE,UP_DESKTOP_NOTIFY_CLICK,0);
		}
	}

	return ret != FALSE;
}

CMainDlg::CMainDlg(void) : 
	_pWndNotify(0),
	_pWndProperty(0),
	_isPropertyBoxOpened(false),
	_alertCheckedCount(0),
	_trayIconVisible(false),
	_timersDisabledForFuture(false)
{ }

bool CMainDlg::EnableAlertTimer(bool on)
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
					isStartupWaitTimer ? MSG_AlERT_STARTUP_TIMER : MSG_AlERT_CHECK_TIMER,
					interval * 1000 );
			}
			else
			{
				QuitIfPossible();
			}
		}		
	}	
	return true;
}

bool CMainDlg::QuitIfPossible(void)
{
	if(!_timersDisabledForFuture) return false;
	if(_trayIconVisible && _globalConfigObject.ShowInSystemTray) return false;
	if(_pWndProperty || _pWndNotify) return false;
	if(_settingMgr.IsBackupNeeded(_globalConfigObject)) return false;

	CloseWindow(m_hWnd);
	DestroyWindow(); 

	return true;
}

void CMainDlg::OnTimer(UINT_PTR nIDEvent)
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

			if(_globalConfigObject.OnCountCheckAlerts) ++_alertCheckedCount;
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

	SetMsgHandled(FALSE);
}


void CMainDlg::OnMainExit(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	::Shell_NotifyIcon( NIM_DELETE, &m_stNotifyData );
	_trayIconVisible = false;
}

void CMainDlg::OnMainOptions(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	SendMessage(UM_FINANCE,UP_DESKTOP_NOTIFY_CLICK,0);
}

void CMainDlg::OnMainReset(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	_alertCheckedCount = 0;
	_isPropertyBoxOpened = false;
}

bool CMainDlg::ShowPropertyBox(bool isCreate)
{
	if(_pWndProperty)
	{
		if(::IsWindow(_pWndProperty->m_hWnd))
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
		_pWndProperty->Create(NULL);
		_pWndProperty->SetIcon(m_stNotifyData.hIcon);   
		//_pWndProperty->SetForegroundWindow(); 
		_pWndProperty->ShowWindow(SW_NORMAL);
		_pWndProperty->UpdateWindow();
	}
	return true;
}

bool CMainDlg::DesktopNotification(bool isCreate)
{
	if(_pWndNotify)
	{
		if (::IsWindow(_pWndNotify->m_hWnd))
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

void CMainDlg::TakeBackup(void)
{
	if(_settingMgr.IsBackupNeeded(_globalConfigObject))
	{
		CString dbPath;
		if(_financeDB.GetFinanceDBPath(dbPath))
		{
			_settingMgr.TakeBackup(_globalConfigObject,dbPath);
			_settingMgr.UpdateData(_globalConfigObject,false); 
		}
	}
}

LRESULT CMainDlg::OnUserMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
		case UP_DESKTOP_NOTIFY_CLICK:
			{	
				EnableAlertTimer(false);
				ShowPropertyBox(true);				
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
				_trayIconVisible = false;
				if(_globalConfigObject.ShowInSystemTray)
				{
					::Shell_NotifyIcon( NIM_ADD, &m_stNotifyData );
					_trayIconVisible = true;
				}

				TakeBackup();
			}
		break;

		case UP_MEDIA_INSERT:
			{			
				TakeBackup();
				QuitIfPossible();
			}
		break;
	}
	
    return 0;
}
void CMainDlg::OnDestroy(void)
{
	EnableAlertTimer(false);

	// Deletes shell notify icon
    ::Shell_NotifyIcon( NIM_DELETE, &m_stNotifyData );
	_trayIconVisible = false;

	_contextMenu.DestroyMenu(); 
	
	DesktopNotification(false);
	ShowPropertyBox(false);
	DestroyIcon(m_stNotifyData.hIcon);  
	
	PostQuitMessage(0);

	SetMsgHandled(FALSE);
}

void CMainDlg::OnEndSession( BOOL bEnding, UINT uLogOff ) 
{
	if(bEnding)
	{
		CloseWindow(m_hWnd);
		DestroyWindow();
	}
	SetMsgHandled(FALSE);
}

int CMainDlg::GetAlertTimerInterval(bool& isStartupWaitTimer)
{
	isStartupWaitTimer = false;

	SConfig config = _globalConfigObject;

	_timersDisabledForFuture = true;

	//if configured to show propertybox onetime and the box is already opened?
	if(config.UnNotifyTillNextTime && _isPropertyBoxOpened) 
	{
		return -1;
	}

	if(config.OnCountCheckAlerts && config.AlertCheckCount <= _alertCheckedCount)
	{
		return -1;
	}

	_timersDisabledForFuture = false;

	if(config.UnNotifyTillNextDTM)
	{
		CTime nextAlertTime = config.UnNotifyTillNext_DTM_VALUE;
		CTime curTime = CTime::GetCurrentTime(); 
		//if benchmark time is already elapsed?
		if(nextAlertTime <= curTime)
		{
			_globalConfigObject.UnNotifyTillNextDTM = false;
			_globalConfigObject.UnNotifyTillNext_DTM_VALUE = curTime;
			_settingMgr.UpdateData(_globalConfigObject,false);

			return config.AlertTimeInterval;
		}

		CTimeSpan tmeSpn = nextAlertTime - curTime;
		
		LONG secs = tmeSpn.GetTotalSeconds(); 
		//If benchmark time (alert disable period) is further ahead of current time
		//wait until that time, before checking for alerts
		if(secs > 0)
		{
			if(secs >= (CONST_STARTUP_TIMER_NONEED_HOURS * 60 * 60))
			{
				_timersDisabledForFuture = true;
				return -1;
			}
			
			isStartupWaitTimer = true;
			return secs;
		}
	}

	return config.AlertTimeInterval; 
}

LRESULT CMainDlg::OnSysTrayMsg(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if( WM_LBUTTONDBLCLK == lParam )
    {
       OnMainOptions(0,0,CWindow(m_hWnd));
    }
    else if( WM_RBUTTONDOWN == lParam )
    {        
        SetForegroundWindow(m_hWnd);        
        CPoint pt;
        ::GetCursorPos( &pt );
        _contextMenu.GetSubMenu( 0 ).TrackPopupMenu(TPM_RIGHTALIGN, pt.x, pt.y, m_hWnd );
    }
    else if( WM_LBUTTONDOWN == lParam )
    {
		OnMainOptions(0,0,CWindow(m_hWnd));
		//if(!_pWndProperty || !::IsWindow(_pWndProperty->m_hWnd)) 
		//{
		//	OnMainOptions();
		//}
		//else
		//{
		//	::SendMessage(_pWndProperty->m_hWnd,WM_CLOSE,0,0);   
		//}        
    }
    return 0;
}

void CMainDlg::OnContextMenu(CWindow wnd, CPoint point)
{
	OnSysTrayMsg( UM_TRAYMENU, WM_RBUTTONDOWN,0 );
    return;
}

BOOL CMainDlg::OnDeviceChange(UINT nEventType, DWORD dwData)
{
	PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR)dwData;

	switch(nEventType )
	{
		case DBT_DEVICEARRIVAL:
			PostMessage(UM_FINANCE, UP_MEDIA_INSERT,0);

			/*if (lpdb -> dbch_devicetype == DBT_DEVTYP_VOLUME)
			{				
				PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME)lpdb;

				if (lpdbv -> dbcv_flags & DBTF_MEDIA)
				{
					TCHAR driveLetter = _settingMgr.GetDriveLetterFromMask(lpdbv ->dbcv_unitmask);
				}
			}*/
			break;

		case DBT_DEVICEREMOVECOMPLETE:
			PostMessage(UM_FINANCE, UP_MEDIA_EJECT,0);
			break;

		default:
			break;
	}

	SetMsgHandled(FALSE);
	return true;
}


