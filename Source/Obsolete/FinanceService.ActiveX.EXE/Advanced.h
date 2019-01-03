//{{AFX_INCLUDES()
#include "calendar.h"
//}}AFX_INCLUDES
#if !defined(AFX_ADVANCED1_H__C757F08D_7008_458B_8B3C_68739C088F4C__INCLUDED_)
#define AFX_ADVANCED1_H__C757F08D_7008_458B_8B3C_68739C088F4C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/////////////////////////////////////////////////////////////////////////////
// Advanced dialog

class Advanced : public CPropertyPage
{
	DECLARE_DYNCREATE(Advanced)

// Construction
public:
	Advanced();
	~Advanced();

// Dialog Data
	//{{AFX_DATA(Advanced)
	enum { IDD = IDD_ADV };
	CTime	_selectedTime;
	BOOL	_chkHideAlertsNextTime;
	BOOL	_chkHideAlertsDTM;
	CCalendar	_calendarControl;
	BOOL	m_chkAutoStart;
	int		_txtAlertTimeInterval;
	BOOL	_chkSystemTrayOn;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(Advanced)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(Advanced)
		virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	bool AttachConfig(SConfig&);

public:
	bool GetConfig(SConfig&);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADVANCED1_H__C757F08D_7008_458B_8B3C_68739C088F4C__INCLUDED_)
