// Advanced1.cpp : implementation file
//

#include "stdafx.h"
#include "finance.h"
#include "Advanced.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Advanced property page

IMPLEMENT_DYNCREATE(Advanced, CPropertyPage)

Advanced::Advanced() : CPropertyPage(Advanced::IDD)
{
	//{{AFX_DATA_INIT(Advanced)
	_selectedTime = 0;
	_chkHideAlertsNextTime = FALSE;
	_chkHideAlertsDTM = FALSE;
	m_chkAutoStart = FALSE;
	_txtAlertTimeInterval = 0;
	_chkSystemTrayOn = FALSE;
	//}}AFX_DATA_INIT
}

Advanced::~Advanced()
{
}

void Advanced::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Advanced)
	DDX_DateTimeCtrl(pDX, IDC_CTL_TIME, _selectedTime);
	DDX_Check(pDX, IDC_CHK_HIDEALERTS, _chkHideAlertsNextTime);
	DDX_Check(pDX, IDC_CHK_HIDEALERTS_DTM, _chkHideAlertsDTM);
	DDX_Control(pDX, IDC_CTL_CAL, _calendarControl);
	DDX_Check(pDX, IDC_CHK_AUTOSTART, m_chkAutoStart);
	DDX_Text(pDX, IDC_EDT_ALERT_INTVL, _txtAlertTimeInterval);
	DDX_Check(pDX, IDC_CHK_TRAY, _chkSystemTrayOn);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Advanced, CPropertyPage)
	//{{AFX_MSG_MAP(Advanced)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

#include "PropertiesDlg.h"

BOOL Advanced::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	AttachConfig(_globalConfigObject);
	return FALSE;  
}

bool Advanced::AttachConfig(SConfig& config)
{
	_calendarControl.SetYear(config.UnNotifyTillNext_DTM_VALUE.GetYear());  
	_calendarControl.SetMonth(config.UnNotifyTillNext_DTM_VALUE.GetMonth());  
	_calendarControl.SetDay(config.UnNotifyTillNext_DTM_VALUE.GetDay());  

	_selectedTime = config.UnNotifyTillNext_DTM_VALUE;
	_chkHideAlertsNextTime = config.UnNotifyTillNextTime;
	_chkHideAlertsDTM = config.UnNotifyTillNextDTM;
	m_chkAutoStart = config.AutoStartup; 
	_txtAlertTimeInterval = config.AlertTimeInterval;
	_chkSystemTrayOn = config.ShowInSystemTray; 

	UpdateData(FALSE);
	return true;
}

bool  Advanced::GetConfig(SConfig& config)
{
	UpdateData(TRUE);

	CTime time(
		_calendarControl.GetYear(),
		_calendarControl.GetMonth(),
		_calendarControl.GetDay(),
		_selectedTime.GetHour(),
		_selectedTime.GetMinute(),
		_selectedTime.GetSecond());

	config.UnNotifyTillNext_DTM_VALUE = time;
	config.UnNotifyTillNextTime = _chkHideAlertsNextTime;
	config.UnNotifyTillNextDTM  = _chkHideAlertsDTM; 
	config.AutoStartup = m_chkAutoStart; 
	config.ShowInSystemTray = _chkSystemTrayOn;
	config.AlertTimeInterval = _txtAlertTimeInterval;

	return true;
}
