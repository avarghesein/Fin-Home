// PropertiesDlg.cpp : implementation file
//

#pragma once

#include "stdafx.h"

#include "FinanceDlg.h"

#include "SettingsManager.h"
#include "PropertiesDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PropertiesDlg dialog


PropertiesDlg::~PropertiesDlg()
{
	m_General.DestroyWindow();
	m_Properties.DestroyWindow();
}

void PropertiesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(PropertiesDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(PropertiesDlg, CDialog)
	//{{AFX_MSG_MAP(PropertiesDlg)
	ON_BN_CLICKED(IDC_CONFG_APPLY, OnApply)
	ON_BN_CLICKED(IDC_CONFG_OK, OnOk)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_OPEN_DB, OnOpenDb)
	ON_BN_CLICKED(IDC_CONFG_CANCEL, OnCancel)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



BOOL PropertiesDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	// Add the general and color tab pages to the property sheet or tab control
    m_Properties.AddPage( &m_General );
	m_Properties.AddPage( &m_adv );
    
	// Create the Tab Control
	// NOTE: Extended style 'WS_EX_CONTROLPARENT' is a must here, to show a modal dialog
	// From a Tab Control / Tab Page. Otherwise it will hang if we call any DoModal.
	// Ex: suppose if we are showing a ColorSelection dialog.
	m_Properties.Create( this, WS_CHILD | WS_VISIBLE, WS_EX_CONTROLPARENT );

    CRect rcClient;
    GetClientRect( &rcClient );

    const int SIZE_ADJUST_WIDTH = 35;
    const int SIZE_ADJUST_HEIGHT = 55;
	// Properly align the tab control with in the form
    m_Properties.MoveWindow( 0, 0 , rcClient.Width() - SIZE_ADJUST_WIDTH, rcClient.Height() - SIZE_ADJUST_HEIGHT );
    CTabCtrl* pCtrl = m_Properties.GetTabControl();
    pCtrl->MoveWindow( 0, 0, rcClient.Width() - SIZE_ADJUST_WIDTH, rcClient.Height() - SIZE_ADJUST_HEIGHT );
	// Set the general settings tab page as default
    m_Properties.SetActivePage( &m_General );


    CRect rcWindow;
    GetWindowRect( &rcWindow );

    int nScreenWidth = ::GetSystemMetrics( SM_CXSCREEN );
    int nScreenHeight = ::GetSystemMetrics( SM_CYSCREEN );

    int nDX = ( nScreenWidth / 2 )- ( rcWindow.Width() / 2 );
    int nDY = ( nScreenHeight / 2 ) - ( rcWindow.Height() / 2 );
	// Show our window in the centre of the screen
    MoveWindow( nDX, nDY, rcWindow.Width() - 25, rcWindow.Height());
	SetIcon( AfxGetApp()->LoadIcon( IDR_MAINFRAME ), TRUE );	
	return FALSE;      
}

void PropertiesDlg::OnApply() 
{
	bool stat = m_adv.GetConfig(_globalConfigObject); 
	if(stat)
	{
		CSettingsManager setMgr;
		setMgr.UpdateData(_globalConfigObject,false); 
		::PostMessage(AfxGetApp()->GetMainWnd()->m_hWnd,UM_FINANCE,UP_SETTINGS_CHANGE,0);
	}
}


void PropertiesDlg::OnCancel() 
{
    SendMessage(WM_CLOSE); 
}

void PropertiesDlg::OnOk() 
{
	OnApply();
    SendMessage(WM_CLOSE); 
}

void PropertiesDlg::OnClose() 
{
	// Apply the changes first, then close the form
	::PostMessage(AfxGetApp()->GetMainWnd()->m_hWnd,UM_FINANCE,UP_DESKTOP_PROPERTY_CLOSE,0);

	DestroyWindow();
}

BOOL PropertiesDlg::Create(UINT nID, CWnd * pWnd)
{ 
	return CDialog::Create(nID,pWnd);
}

void PropertiesDlg::PostNcDestroy()
{
	//delete this;
}

void PropertiesDlg::OnOpenDb() 
{
	CString path,param;
	_financeDB.GetFinanceHomeApp(path,param); 
	ShellExecute(NULL, "open", path, param, NULL, SW_SHOWNORMAL);	
}

void PropertiesDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	switch (nType)
    {
        case SIZE_MINIMIZED:
            SendMessage(WM_CLOSE); 
            break;
    }
}
