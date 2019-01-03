#if !defined(_WTL_GENERAL_H)
#define _WTL_GENERAL_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Resource.h"

#include <atlddx.h>

#include "ReadOnlyEdit.h"
#include "FinanceDB.h"


class General : public CPropertyPageImpl<General>,public CWinDataExchange<General>
{
public:
    enum { IDD = IDD_GENERAL };

    // Construction
    
	General();
	~General();

    // Maps
    BEGIN_MSG_MAP_EX(General)
        MSG_WM_INITDIALOG(OnInitDialog)
		MSG_WM_CHAR(OnChar)
		MSG_WM_DESTROY(OnDestroy);
		CHAIN_MSG_MAP_MEMBER(_edtCtlAlert) 
        CHAIN_MSG_MAP(CPropertyPageImpl<General>)
    END_MSG_MAP()

    BEGIN_DDX_MAP(General)
        DDX_TEXT(IDC_EDT_ALERT, _txtAlert)
    END_DDX_MAP()
    
    // Message handlers
    BOOL OnInitDialog ( HWND hwndFocus, LPARAM lParam );
	void OnDestroy();
	void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);

    // Notification handlers
    int  OnSetActive();
    BOOL OnKillActive();

public:
	CString _alertString;

protected:
	CString	_txtAlert;
	CReadOnlyEdit	_edtCtlAlert;
	CFinanceDB _financeDB;
};

#endif
