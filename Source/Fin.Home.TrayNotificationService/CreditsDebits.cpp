// CreditsDebits.cpp : implementation file
//

#include "stdafx.h"
#include "CreditsDebits.h"

CCreditsDebits::CCreditsDebits() : 
CPropertyPageImpl<CCreditsDebits>( CCreditsDebits::IDD )
{
}

CCreditsDebits::~CCreditsDebits()
{
}

BOOL CCreditsDebits::OnInitDialog(HWND hwndFocus, LPARAM lParam ) 
{
	ModifyStyle(0, WS_EX_NOPARENTNOTIFY, SWP_FRAMECHANGED);
	
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CCreditsDebits::PreTranslateMessage(MSG* pMsg) 
{
	if((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN))
	{
		if(pMsg->hwnd != m_hWnd)
		{
			::SendMessage(pMsg->hwnd,pMsg->message,pMsg->wParam,pMsg->lParam);
			return TRUE;
		}
	}
	return TRUE;
	//return CPropertyPageImpl<CCreditsDebits>::PreTranslateMessage(pMsg);
}