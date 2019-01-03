
#if !defined(WTL_ADVANCED_H_INCLUDED_)
#define WTL_ADVANCED_H_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

#include "Resource.h"
#include <atlddx.h>

class Advanced : public CPropertyPageImpl<Advanced>, public CWinDataExchange<Advanced>
{
	public:
    enum { IDD = IDD_ADV };

    // Construction    
	Advanced();
	~Advanced();

	// Maps
    BEGIN_MSG_MAP_EX(Advanced)
        MSG_WM_INITDIALOG(OnInitDialog)
        CHAIN_MSG_MAP(CPropertyPageImpl<Advanced>)
		COMMAND_ID_HANDLER_EX(IDC_BTN_BRWS_BKUPPTH,OnBrowseBackupPath) 
    END_MSG_MAP()

	BEGIN_DDX_MAP(Advanced)	
		DDX_CHECK(IDC_CHK_HIDEALERTS, _chkHideAlertsNextTime)
		DDX_CHECK(IDC_CHK_HIDEALERTS_DTM, _chkHideAlertsDTM)		
		DDX_CHECK(IDC_CHK_AUTOSTART, _chkAutoStart)
		DDX_UINT(IDC_EDT_ALERT_INTVL, _txtAlertTimeInterval)
		DDX_CHECK(IDC_CHK_TRAY, _chkSystemTrayOn)
		DDX_CHECK(IDC_CHK_ONCOUNTCHECKALERT, _chkOnCountCheckAlerts)
		DDX_UINT(IDC_EDT_ALERT_CHKCOUNT, _txtAlertCheckCount)
		DDX_TEXT(IDC_EDT_BKUP_PTH,_txtBackupPath) 
		DDX_UINT(IDC_EDT_BKUP_DAYS, _txtBackupIntervalDays)
		DDX_CHECK(IDC_CHK_AUTOBACKUP, _chkAutoBackup)
    END_DDX_MAP()

	// Message handlers
    BOOL OnInitDialog ( HWND hwndFocus, LPARAM lParam );
	void OnBrowseBackupPath(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnDataExchangeError(UINT nCtrlID, BOOL /*bSave*/);
	void OnDataValidateError(UINT nCtrlID, BOOL /*bSave*/, _XData& /*data*/);

protected:
	bool AttachConfig(SConfig&);

public:
	bool GetConfig(SConfig&);

public:		
	BOOL	_chkHideAlertsNextTime;
	BOOL	_chkHideAlertsDTM;	
	BOOL	_chkAutoStart;
	int		_txtAlertTimeInterval;
	BOOL	_chkSystemTrayOn;
	BOOL	_chkOnCountCheckAlerts;
	int		_txtAlertCheckCount;
	int		_txtBackupIntervalDays;
	BOOL	_chkAutoBackup;
	CString _txtBackupPath;

	bool _dataError;
};

#endif // !defined(WTL_ADVANCED_H_INCLUDED_)
