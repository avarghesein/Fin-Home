// CreditsDebits.cpp : implementation file
//

#include "stdafx.h"
#include "CreditsDebits.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCreditsDebits property page

IMPLEMENT_DYNCREATE(CCreditsDebits, CPropertyPage)

CCreditsDebits::CCreditsDebits() : CPropertyPage(CCreditsDebits::IDD)
{
	//{{AFX_DATA_INIT(CCreditsDebits)
	//}}AFX_DATA_INIT
}

CCreditsDebits::~CCreditsDebits()
{
}

void CCreditsDebits::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCreditsDebits)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCreditsDebits, CPropertyPage)
	//{{AFX_MSG_MAP(CCreditsDebits)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCreditsDebits message handlers

BOOL CCreditsDebits::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	//_CreditsDebitsControl.StartInitialize();

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
	return CPropertyPage::PreTranslateMessage(pMsg);
}
