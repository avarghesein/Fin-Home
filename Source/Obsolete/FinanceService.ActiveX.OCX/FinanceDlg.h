

#if !defined(_FIN_DIALOG_INCLUDED_)
#define _FIN_DIALOG_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

#include "Resource.h"
#include "FinanceDB.h"
#include "NotifyWnd.h"
#include "PropertiesDlg.h"

#include "SettingsManager.h"

class CFinWindow : public CWnd 
{
public:
	CFinWindow();
	BOOL Create(CWnd*& mainWind,bool serviceMode = true);

    //{{AFX_DATA(CFinWindow)
	//}}AFX_DATA

    //{{AFX_VIRTUAL(CFinWindow)
    //}}AFX_VIRTUAL

protected:
    //{{AFX_MSG(CFinWindow)
    afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg LRESULT OnSysTrayMsg( WPARAM wParam, LPARAM lParam );
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
    afx_msg void OnMainExit();
    afx_msg void OnMainOptions();
	afx_msg LRESULT OnUserMessage( WPARAM wParam, LPARAM lParam );	
	afx_msg void OnEndSession(BOOL bEnding);
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()

protected: 
	bool DesktopNotification(bool isCreate);
	bool ShowPropertyBox(bool isCreate);
	bool EnableAlertTimer(bool on);
	
	int GetAlertTimerInterval(bool& isStartupWaitTimer);

private:
	NOTIFYICONDATA m_stNotifyData;

	CNotifyWnd* _pWndNotify;
	PropertiesDlg* _pWndProperty;

	CFinanceDB _financeDB;
	CSettingsManager _settingMgr;

public:
	CString _policyDues;
	CString _invDues;
	CString _tskDues;

	bool _isPropertyBoxOpened;
};

//{{AFX_INSERT_LOCATION}}

#endif
