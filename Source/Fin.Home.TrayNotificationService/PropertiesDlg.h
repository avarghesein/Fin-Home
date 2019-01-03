#if !defined(_ATL_PROPERTIESDLG_H)
#define _ATL_PROPERTIESDLG_H

#if _MSC_VER > 1000
#pragma once
#endif
//

#include "FinanceDB.h"
#include "PropertySheet.h"
#include "General.h"
#include "Advanced.h"
#include "CreditsDebits.h"

class PropertiesDlg : public CMessageFilter, public CDialogImpl<PropertiesDlg>
{
public:

	enum { IDD = IDD_DIALOG_CONFIG };

	~PropertiesDlg();

	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		return CDialogImpl<PropertiesDlg>::IsDialogMessage(pMsg);
	}

	// Maps
    BEGIN_MSG_MAP_EX(PropertiesDlg)
        COMMAND_ID_HANDLER_EX(IDC_CONFG_APPLY, OnApply)
		COMMAND_ID_HANDLER_EX(IDC_CONFG_OK, OnOK)		
		COMMAND_ID_HANDLER_EX(IDC_OPEN_DB, OnOpenDb)
		COMMAND_ID_HANDLER_EX(IDC_CONFG_CANCEL, OnCancel)
		
		MSG_WM_INITDIALOG(OnInitDialog)
		MSG_WM_SIZE(OnSize) 
		MSG_WM_CLOSE(OnClose)
		MSG_WM_DESTROY(OnDestroy) 
    END_MSG_MAP()

protected:

	BOOL OnInitDialog ( HWND hwndFocus, LPARAM lParam );
	void OnApply(UINT uCode, int nID, HWND hwndCtrl);
	void OnCancel(UINT uCode, int nID, HWND hwndCtrl);
	void OnOK(UINT uCode, int nID, HWND hwndCtrl);	
	void OnOpenDb(UINT uCode, int nID, HWND hwndCtrl);
	void OnDestroy();
	void OnClose();
	void OnSize(UINT nType, CSize size);

protected:
	CFinanceDB _financeDB;

public:
	// property pages. Pass in dialog and control resource IDs
	General m_General;
	Advanced m_adv;
	//CCreditsDebits m_creditsDebits;

    CPropView m_PropertySheet;
};

#endif
