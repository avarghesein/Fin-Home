//{{AFX_INCLUDES()
#include "creditsdebitscontrol.h"
//}}AFX_INCLUDES
#if !defined(AFX_CREDITSDEBITS_H__E048EA2E_53A0_48C1_A6F0_1E269A382672__INCLUDED_)
#define AFX_CREDITSDEBITS_H__E048EA2E_53A0_48C1_A6F0_1E269A382672__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Resource.h"

class CCreditsDebits : public CPropertyPage
{
	DECLARE_DYNCREATE(CCreditsDebits)

// Construction
public:
	CCreditsDebits();
	~CCreditsDebits();

// Dialog Data
	//{{AFX_DATA(CCreditsDebits)
	enum { IDD = IDD_DEBITS_CREDITS };
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CCreditsDebits)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCreditsDebits)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CREDITSDEBITS_H__E048EA2E_53A0_48C1_A6F0_1E269A382672__INCLUDED_)
