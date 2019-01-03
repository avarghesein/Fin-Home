#if !defined(_AFX_GENERAL_H)
#define _AFX_GENERAL_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Resource.h"

#include "ReadOnlyEdit.h"
#include "FinanceDB.h"

class General : public CPropertyPage
{
	DECLARE_DYNCREATE(General)

public:
	General();
	~General();

	//{{AFX_DATA(General)
	enum { IDD = IDD_GENERAL };
	CReadOnlyEdit	_edtCtlAlert;
	CString	_txtAlert;
	//}}AFX_DATA


	//{{AFX_VIRTUAL(General)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	//}}AFX_VIRTUAL


protected:

	//{{AFX_MSG(General)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CString _alertString;

protected:
	CFinanceDB _financeDB;

};

//{{AFX_INSERT_LOCATION}}

#endif
