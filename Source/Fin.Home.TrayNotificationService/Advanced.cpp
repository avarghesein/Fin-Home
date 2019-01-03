
#include "stdafx.h"
#include "Advanced.h"


Advanced::Advanced() : 
CPropertyPageImpl<Advanced>( Advanced::IDD )
{
	_dataError = false;

	_chkHideAlertsNextTime = FALSE;
	_chkHideAlertsDTM = FALSE;
	_chkAutoStart = FALSE;
	_txtAlertTimeInterval = 0;
	_chkSystemTrayOn = FALSE;
	_chkOnCountCheckAlerts = FALSE;
	_txtAlertCheckCount = 0;
	_txtBackupPath = EMPTY;
	_txtBackupIntervalDays = 0;
	_chkAutoBackup = FALSE;
}

Advanced::~Advanced()
{
}

void Advanced::OnDataExchangeError(UINT nCtrlID, BOOL bSave)
{
	if(bSave)
	{
		// Override to display an error message
		::MessageBox(m_hWnd,T("Please correct the validations"),T("Validation Errors!"),MB_OK);  		
		_dataError = true;
	}
	::SetFocus(GetDlgItem(nCtrlID));
}

void Advanced::OnDataValidateError(UINT nCtrlID, BOOL bSave, _XData& /*data*/)
{
	OnDataExchangeError(nCtrlID,bSave);
}

BOOL Advanced::OnInitDialog ( HWND hwndFocus, LPARAM lParam )
{
	AttachConfig(_globalConfigObject);
	return FALSE;  
}

void Advanced::OnBrowseBackupPath(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	DoDataExchange(true);

	CFolderDialog folderDlg(m_hWnd,T("Select backup folder..."));
	folderDlg.SetInitialFolder(_txtBackupPath);
	if(folderDlg.DoModal(m_hWnd) == IDOK)
	{
		_txtBackupPath = folderDlg.GetFolderPath();  
		DoDataExchange(false);
	}
	::DestroyWindow(folderDlg.m_hWnd);
}

bool Advanced::AttachConfig(SConfig& config)
{
	_dataError = false;

	SYSTEMTIME sysDateTime = { 0 };

	sysDateTime.wYear = config.UnNotifyTillNext_DTM_VALUE.GetYear(); 
	sysDateTime.wMonth = config.UnNotifyTillNext_DTM_VALUE.GetMonth(); 
	sysDateTime.wDay = config.UnNotifyTillNext_DTM_VALUE.GetDay(); 
	sysDateTime.wHour = config.UnNotifyTillNext_DTM_VALUE.GetHour(); 
	sysDateTime.wMinute = config.UnNotifyTillNext_DTM_VALUE.GetMinute(); 
	sysDateTime.wSecond = config.UnNotifyTillNext_DTM_VALUE.GetSecond();

	CDateTimePickerCtrl _selectedTime;
	_selectedTime.Attach(GetDlgItem(IDC_CTL_TIME));
	CMonthCalendarCtrl _calendarControl;
	_calendarControl.Attach(GetDlgItem(IDC_CTL_CAL));

	_calendarControl.SetCurSel(&sysDateTime); 
	_selectedTime.SetSystemTime(GDT_VALID, &sysDateTime);
	_chkHideAlertsNextTime = config.UnNotifyTillNextTime;
	_chkHideAlertsDTM = config.UnNotifyTillNextDTM;
	_chkAutoStart = config.AutoStartup; 
	_txtAlertTimeInterval = config.AlertTimeInterval;
	_chkSystemTrayOn = config.ShowInSystemTray; 
	_chkOnCountCheckAlerts = config.OnCountCheckAlerts; 
	_txtAlertCheckCount = config.AlertCheckCount; 
	_txtBackupPath = config.BackupPath;
	_txtBackupIntervalDays = config.BackupIntervalDays;
	_chkAutoBackup = config.AutoBackup;

	DoDataExchange(false);

	_selectedTime.Detach();
	_calendarControl.Detach();

	return !_dataError;
}

bool  Advanced::GetConfig(SConfig& config)
{
	_dataError = false;

	CDateTimePickerCtrl _selectedTime;
	_selectedTime.Attach(GetDlgItem(IDC_CTL_TIME));
	CMonthCalendarCtrl _calendarControl;
	_calendarControl.Attach(GetDlgItem(IDC_CTL_CAL));

	DoDataExchange(true);

	SYSTEMTIME sysDate = { 0 };
	SYSTEMTIME sysTime = { 0 };

	_calendarControl.GetCurSel(&sysDate);
	_selectedTime.GetSystemTime(&sysTime);
	
	CTime time(
		sysDate.wYear,
		sysDate.wMonth,
		sysDate.wDay,
		sysTime.wHour,
		sysTime.wMinute,
		sysTime.wSecond);

	config.UnNotifyTillNext_DTM_VALUE = time;
	config.UnNotifyTillNextTime = _chkHideAlertsNextTime;
	config.UnNotifyTillNextDTM  = _chkHideAlertsDTM; 
	config.AutoStartup = _chkAutoStart; 
	config.ShowInSystemTray = _chkSystemTrayOn;
	config.AlertTimeInterval = _txtAlertTimeInterval;
	config.OnCountCheckAlerts = _chkOnCountCheckAlerts; 
	config.AlertCheckCount = _txtAlertCheckCount; 
	
	CString oldPath = config.BackupPath, newPath = _txtBackupPath;
	TRIM(oldPath)
	TRIM(newPath)
	oldPath.MakeLower();
	newPath.MakeLower();
	if(oldPath != newPath) config.BackupStatus = false;

	config.BackupPath = _txtBackupPath;
	config.BackupIntervalDays = _txtBackupIntervalDays;
	config.AutoBackup = _chkAutoBackup;

	_selectedTime.Detach();
	_calendarControl.Detach();

	return !_dataError;
}
