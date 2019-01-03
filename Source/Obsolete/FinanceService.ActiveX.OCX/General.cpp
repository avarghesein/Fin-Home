// General.cpp : implementation file
//

#pragma once

#include "stdafx.h"
#include "General.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// General property page

IMPLEMENT_DYNCREATE(General, CPropertyPage)

General::General() : CPropertyPage(General::IDD)
{
	//{{AFX_DATA_INIT(General)
	_txtAlert = _T("");
	//}}AFX_DATA_INIT

}

General::~General()
{
    
}

void General::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(General)
	DDX_Control(pDX, IDC_EDT_ALERT, _edtCtlAlert);
	DDX_Text(pDX, IDC_EDT_ALERT, _txtAlert);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(General, CPropertyPage)
	//{{AFX_MSG_MAP(General)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


#include "PropertiesDlg.h"

BOOL General::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	_edtCtlAlert.SetBackColor(RGB(255,255,255)); 
	_edtCtlAlert.SetTextColor(RGB(0,0,0));

	UpdateData(FALSE); 
	return FALSE;  
}

BOOL General::OnSetActive() 
{
	_alertString = _financeDB.GetAllAlerts(); 
	_txtAlert = _alertString;

	UpdateData(FALSE); 

	return CPropertyPage::OnSetActive();
}
