#if !defined(_AFX_PROPERTIESDLG_H)
#define _AFX_PROPERTIESDLG_H

#if _MSC_VER > 1000
#pragma once
#endif
//

#include "FinanceDB.h"

#include "General.h"
#include "Advanced.h"
#include "CreditsDebits.h"

class PropertiesDlg : public CDialog
{
public:
	//PropertiesDlg(CWnd* pParent = NULL);  

	PropertiesDlg() {}

	BOOL Create(UINT nID, CWnd * pWnd);
	virtual void PostNcDestroy();


	~PropertiesDlg();

	//{{AFX_DATA(PropertiesDlg)
	enum { IDD = IDD_DIALOG_CONFIG };
	//}}AFX_DATA


	//{{AFX_VIRTUAL(PropertiesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:

	//{{AFX_MSG(PropertiesDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnApply();
	afx_msg void OnCancel();
	afx_msg void OnOK();
	afx_msg void OnClose();
	afx_msg void OnOpenDb();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

    CPropertySheet m_Properties;

protected:
	CFinanceDB _financeDB;

public:
    General m_General;
	Advanced m_adv;
	CCreditsDebits m_creditsDebits;
};

//{{AFX_INSERT_LOCATION}}

#endif
