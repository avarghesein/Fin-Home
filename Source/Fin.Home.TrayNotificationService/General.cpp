// General.cpp : implementation file
//

#pragma once

#include "stdafx.h"
#include "General.h"

General::General() : 
CPropertyPageImpl<General>( General::IDD )
{
	_txtAlert = _T("");
}

General::~General()
{
    
}

void General::OnDestroy()
{
	_edtCtlAlert.Detach();
}

BOOL General::OnInitDialog(HWND hwndFocus, LPARAM lParam) 
{	 
	HWND hWnd = GetDlgItem(IDC_EDT_ALERT);
	_edtCtlAlert.Attach(hWnd);
	_edtCtlAlert.SetBackColor(RGB(255,255,255)); 
	_edtCtlAlert.SetTextColor(RGB(0,0,0));

	DoDataExchange(false);
	return FALSE;  
}

BOOL General::OnKillActive() 
{
	return true;
}

int General::OnSetActive() 
{
	_alertString = _financeDB.GetAllAlerts(); 
	_txtAlert = _alertString;

	DoDataExchange(false);

	return 	CPropertyPageImpl<General>::OnSetActive();
}

void General::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
}
