
#if !defined(WTL_CREDITSDEBITS_H_INCLUDED_)
#define WTL_CREDITSDEBITS_H_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

#include "Resource.h"
#include <atlddx.h>

class CCreditsDebits : public CPropertyPageImpl<CCreditsDebits>, public CWinDataExchange<CCreditsDebits>
{
public:
	enum { IDD = IDD_DEBITS_CREDITS };

	CCreditsDebits();
	~CCreditsDebits();

	// Maps
    BEGIN_MSG_MAP_EX(CCreditsDebits)
        MSG_WM_INITDIALOG(OnInitDialog)
        CHAIN_MSG_MAP(CPropertyPageImpl<CCreditsDebits>)
    END_MSG_MAP()

	BOOL OnInitDialog ( HWND hwndFocus, LPARAM lParam );
	virtual BOOL PreTranslateMessage(MSG* pMsg);

};

#endif // !defined(WTL_CREDITSDEBITS_H_INCLUDED_)
