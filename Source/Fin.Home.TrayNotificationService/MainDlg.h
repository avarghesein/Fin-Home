
#if !defined(WTL_MAINDLG__INCLUDED)
#define WTL_MAINDLG__INCLUDED

#if _MSC_VER >= 1000
#pragma once
#endif 

#include "Resource.h"
#include "SettingsManager.h"
#include "FinanceDB.h"
#include "NotifyWnd.h"
#include "PropertiesDlg.h"

typedef CWinTraits<WS_OVERLAPPEDWINDOW, WS_EX_APPWINDOW> MyWndStyle;
class CMainDlg;
typedef CWindowImpl<CMainDlg,CWindow,MyWndStyle> MyBaseWin;

class CMainDlg : 
		public MyBaseWin,
		public CMessageFilter, 
		public CIdleHandler
{
public:
	DECLARE_WND_CLASS(FINANCE_UNIQUE_OBJECT_NAME)

	BEGIN_MSG_MAP_EX(CMainDlg)	
		MSG_WM_TIMER(OnTimer)
		MSG_WM_CONTEXTMENU(OnContextMenu)
		MSG_WM_DESTROY(OnDestroy)
		MSG_WM_ENDSESSION(OnEndSession)
		MSG_WM_DEVICECHANGE(OnDeviceChange) 

		COMMAND_ID_HANDLER_EX(ID_MAIN_EXIT, OnMainExit)
		COMMAND_ID_HANDLER_EX(ID_MAIN_OPTIONS, OnMainOptions)
		COMMAND_ID_HANDLER_EX(ID_MAIN_RESET, OnMainReset)

		MESSAGE_HANDLER_EX(UM_FINANCE,OnUserMessage)
		MESSAGE_HANDLER_EX(UM_TRAYMENU, OnSysTrayMsg )
	END_MSG_MAP()

	BOOL Create(bool serviceMode = true);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle(void);	

	CMainDlg(void);

protected:
	void OnTimer(UINT_PTR nIDEvent);
	void OnContextMenu(CWindow wnd, CPoint point);
	void OnMainExit(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnMainReset(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnMainOptions(UINT uNotifyCode, int nID, CWindow wndCtl);
	LRESULT OnUserMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnSysTrayMsg(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void OnDestroy(void);
	void OnEndSession(BOOL bEnding, UINT uLogOff);
	BOOL OnDeviceChange(UINT nEventType, DWORD dwData);

	bool DesktopNotification(bool isCreate);
	bool ShowPropertyBox(bool isCreate);
	bool EnableAlertTimer(bool on);	
	int GetAlertTimerInterval(bool& isStartupWaitTimer);
	bool QuitIfPossible(void);
	void TakeBackup(void);

private:
	CMenu _contextMenu;

	NOTIFYICONDATA m_stNotifyData;

	CNotifyWnd* _pWndNotify;
	PropertiesDlg* _pWndProperty;

	CSettingsManager _settingMgr;
	CFinanceDB _financeDB;
	
	CString _policyDues;
	CString _invDues;
	CString _tskDues;

	bool _timersDisabledForFuture;
	bool _isPropertyBoxOpened;
	int _alertCheckedCount;
	bool _trayIconVisible;
};

#endif // !defined(WTL_MAINDLG__INCLUDED)
