
#if !defined(WTL_READONLYEDIT_H_NCLUDED)
#define WTL_READONLYEDIT_H_NCLUDED

#if _MSC_VER > 1000
#pragma once
#endif 

#include <atlcrack.h>

class CReadOnlyEdit : public CWindowImpl<CReadOnlyEdit, CEdit>
{
typedef CWindowImpl<CReadOnlyEdit, CEdit> CEditEx;
// Construction
public:
	CReadOnlyEdit();

public:			
	void SetTextColor(COLORREF rgb);
	void SetBackColor(COLORREF rgb);
	virtual ~CReadOnlyEdit();

protected:
	COLORREF m_crText;
	COLORREF m_crBackGnd;
	CBrush m_brBackGnd;

	BEGIN_MSG_MAP_EX(CReadOnlyEdit)
		MSG_WM_CTLCOLORSTATIC(OnCtlColor)
    END_MSG_MAP()

	HBRUSH OnCtlColor(CDCHandle dc, CEdit edit);
};

#endif // !defined(WTL_READONLYEDIT_H_NCLUDED)
