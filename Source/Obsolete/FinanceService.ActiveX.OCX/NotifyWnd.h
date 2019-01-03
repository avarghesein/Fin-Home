// NotifyWnd.h: interface for the CNotifyWnd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NOTIFYWND_H__A0F92166_FB2C_49AF_BA78_0A68AC262FC0__INCLUDED_)
#define AFX_NOTIFYWND_H__A0F92166_FB2C_49AF_BA78_0A68AC262FC0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
class CBmpButton;

//define function pointer
typedef DWORD (WINAPI *pSetLayeredWindowAttributes)(HWND, DWORD, BYTE, DWORD);

typedef CWinTraits <WS_CLIPCHILDREN | WS_POPUP |WS_VISIBLE ,0 > CNotificationWinTraits;
class CNotifyWnd : public CWindowImpl<CNotifyWnd,CWindow,CNotificationWinTraits>
{
	COLORREF m_refWndColor;
	UINT m_nWidth;
	UINT m_nHeight;
	HWND m_hParenWnd;
	BYTE m_nOpacity;
	BYTE m_nCurrentOpacity;
	BOOL m_bTimerActive;
	unsigned int m_nShowTime;
	CBmpButton* m_pButton;
public:
	CNotifyWnd(HWND hParentWnd=NULL,int nWidth=100,int nHeight=80,unsigned int nShowTime=100,BYTE nOpacity=255 );
	~CNotifyWnd();
	DECLARE_WND_CLASS(_T("CNotifyWnd"))
	BEGIN_MSG_MAP(CNotifier)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		MESSAGE_HANDLER(WM_MOUSELEAVE, OnMouseLeave)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnMouseDown)
		//reflect notifications to child
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()
	void CreateNotifyWindow();
private:
	LRESULT ChangeOpacity(BYTE iFactor);
	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDrawItem(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMouseDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

private:
	pSetLayeredWindowAttributes _setLayeredWindowAttributes;
	HMODULE _user32DLL;
};

#endif // !defined(AFX_NOTIFYWND_H__A0F92166_FB2C_49AF_BA78_0A68AC262FC0__INCLUDED_)
