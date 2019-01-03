				

#include "stdafx.h"
#include "ReadOnlyEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReadOnlyEdit

CReadOnlyEdit::CReadOnlyEdit()
{
	//default text color
	m_crText = RGB(0,0,0);
}

CReadOnlyEdit::~CReadOnlyEdit()
{
	//delete brush
	if(m_brBackGnd.m_hBrush) m_brBackGnd.DeleteObject();
}

HBRUSH CReadOnlyEdit::OnCtlColor(CDCHandle dc, CEdit edit)
{
	dc.SetTextColor(m_crText);
	//set the text's background color
	dc.SetBkColor(m_crBackGnd);
	//return the brush used for background this sets control background
	return (HBRUSH)m_brBackGnd;
}


void CReadOnlyEdit::SetBackColor(COLORREF rgb)
{
	//set background color ref (used for text's background)
	m_crBackGnd = rgb;	
	//free brush
	if(m_brBackGnd.m_hBrush) m_brBackGnd.DeleteObject();
	//set brush to new color
	m_brBackGnd.CreateSolidBrush(rgb);
	
	//redraw
	if(m_hWnd) Invalidate(TRUE);
}


void CReadOnlyEdit::SetTextColor(COLORREF rgb)
{
	//set text color ref
	m_crText = rgb;

	//redraw
	if(m_hWnd) Invalidate(TRUE);
}
