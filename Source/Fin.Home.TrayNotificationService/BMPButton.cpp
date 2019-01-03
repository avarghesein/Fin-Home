
#include "stdafx.h"
#include "BMPButton.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBmpButton::CBmpButton(UINT * pBitmapID)
{
	m_nCurrentBmp=m_BitmapId[0]=pBitmapID[0];
	m_BitmapId[1]=pBitmapID[1];
	m_BitmapId[2]=pBitmapID[2];
}

CBmpButton::~CBmpButton()
{

}
LRESULT CBmpButton::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	ModifyStyle(0,BS_OWNERDRAW);
	return 1;
}
LRESULT CBmpButton::OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//just to avoid flickering
	return 1;
}
LRESULT CBmpButton::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    DestroyWindow();
    return 0;
}
LRESULT CBmpButton::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_nCurrentBmp=m_BitmapId[2];
	Invalidate();
	return 1;
}
LRESULT CBmpButton::OnDrawItem(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	DRAWITEMSTRUCT* stDrawItem=(DRAWITEMSTRUCT*)lParam;
	//create memory device context
	HDC hMemdDC=CreateCompatibleDC(stDrawItem->hDC);
	HBITMAP hBmp=LoadBitmap(_Module.m_hInst,MAKEINTRESOURCE(m_nCurrentBmp)); 

	HBITMAP hPrevBmp=(HBITMAP) SelectObject(hMemdDC,hBmp);
	StretchBlt(stDrawItem->hDC, 
				 0, 0, 
				 (stDrawItem->rcItem.right-stDrawItem->rcItem.left),
				 (stDrawItem->rcItem.bottom-stDrawItem->rcItem.top), 
				 hMemdDC, 
				 0, 0, 
				 10,10, 
				 SRCCOPY); 

	SelectObject(hMemdDC,hPrevBmp);
	DeleteObject(hBmp);
	ReleaseDC(hMemdDC);
	return 1;
}
LRESULT CBmpButton::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (wParam != MK_LBUTTON)
	{
		//if left mouse button is not down, then use second bitmap
		m_nCurrentBmp=m_BitmapId[1];
	}
	TRACKMOUSEEVENT stMouseEvent;
	//to check Mouse leave event
	stMouseEvent.cbSize=sizeof(stMouseEvent);
	stMouseEvent.dwFlags=TME_LEAVE;
	stMouseEvent.dwHoverTime=HOVER_DEFAULT;
	stMouseEvent.hwndTrack=m_hWnd;
	TrackMouseEvent(&stMouseEvent);

	Invalidate();
	return 0;
}
LRESULT CBmpButton::OnLButtonUP(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (m_nCurrentBmp != m_BitmapId[0])
	{
		m_nCurrentBmp=m_BitmapId[1];
		Invalidate();
		HWND hParent=GetParent();
		CWindow wTemp;
		wTemp.Attach(hParent);
		wTemp.PostMessage(WM_CLOSE);
	}
	return 0;
}
LRESULT CBmpButton::OnMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_nCurrentBmp=m_BitmapId[0];
	Invalidate();
	return 0;
}




