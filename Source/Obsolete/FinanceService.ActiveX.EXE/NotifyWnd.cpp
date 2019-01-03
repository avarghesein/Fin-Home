// NotifyWnd.cpp: implementation of the CNotifyWnd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NotifyWnd.h"
#include "BMPButton.h"

const UINT TIMER_ID = 1;

#ifndef WS_EX_LAYERED
#define WS_EX_LAYERED           0x00080000
#define LWA_COLORKEY            0x00000001
#define LWA_ALPHA               0x00000002
#endif 


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
LRESULT CNotifyWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if ( ModifyStyleEx(0,WS_EX_LAYERED ))
	{
		SetTimer(TIMER_ID ,30);
		m_bTimerActive=TRUE;
	}
	m_refWndColor= RGB(255,239,198); //GetSysColor(COLOR_ACTIVECAPTION);
	//pass these bitmaps in this sequence, ie. Normal pos.,positin while ,mouse move,
	// button presed position
	UINT nBMPs[3]={IDB_BMP_NORMAL,IDB_BMP_HOVER,IDB_BMP_PRESS};
	m_pButton=new CBmpButton(nBMPs);
	RECT r1 ;
	r1.top=5;
	r1.left=m_nWidth-15;
	r1.right= m_nWidth-5;
	r1.bottom= 15;

	m_pButton->Create(m_hWnd,r1);
	m_pButton->ShowWindow(SW_SHOW);
	m_pButton->UpdateWindow();
    return 1;
}
LRESULT CNotifyWnd::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	static BYTE nIncrement=1;
	if (m_nCurrentOpacity == m_nOpacity)
	{
		//now start to dissappear
		nIncrement = - 1;
	}
	if ( (m_nCurrentOpacity += nIncrement) == 0 )
	{
		//done, kill timer, and  close window
		KillTimer(TIMER_ID );
		m_bTimerActive=FALSE;
		m_nCurrentOpacity=0;
		nIncrement=1;

		CWindow wTemp;
		wTemp.Attach(m_hWnd);
		wTemp.SendMessage(WM_CLOSE);
	}
	else
	{
		ChangeOpacity(m_nCurrentOpacity);
	}
	return 0;
}
LRESULT CNotifyWnd::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if ( ! IsIconic() )
	{
		PAINTSTRUCT ps;
		HDC hDC = BeginPaint(&ps);
		RECT rcClient;
		GetClientRect(&rcClient);

		HPEN pen = CreatePen(PS_SOLID,5,RGB(0,0,0));
		 HGDIOBJ old = SelectObject(hDC,pen);
		 
		Rectangle(hDC,rcClient.left ,rcClient.top,rcClient.right,rcClient.bottom);      
		SelectObject(hDC,old);
		DeleteObject(pen);

		rcClient.left += 2;
		rcClient.top += 2;
		rcClient.bottom -= 2;
		rcClient.right -=2 ; 

		//use color, used by system for coloring titile bar
		HBRUSH hbr=CreateSolidBrush(m_refWndColor);
		FillRect(hDC,&rcClient,hbr);
		//done with brush
		DeleteObject(hbr);

		GetClientRect(&rcClient);

		rcClient.top =(rcClient.bottom/2) - 10 ;
		rcClient.bottom-=10;
		
			//make text background transparent
		SetBkMode(hDC,TRANSPARENT);

		DrawText(hDC,_T("Financial Alerts! Click Here To Open..."),-1,&rcClient, DT_CENTER);
		 
		EndPaint(&ps);
		bHandled=TRUE;
	}
	return 1;
}
void CNotifyWnd::CreateNotifyWindow()
{
	RECT rcClient;
	::SystemParametersInfo(SPI_GETWORKAREA, 0, &rcClient, 0);
	rcClient.bottom=rcClient.bottom;
	rcClient.right=rcClient.right;
	rcClient.top=rcClient.bottom-m_nHeight;
	rcClient.left=rcClient.right-m_nWidth;
	Create(m_hParenWnd,rcClient);
	SetForegroundWindow(m_hWnd);  
	ShowWindow(SW_SHOW);
	SetWindowPos(CWnd::wndTopMost.m_hWnd,rcClient.left,rcClient.top,
               m_nWidth,m_nHeight, SWP_SHOWWINDOW );
}
LRESULT CNotifyWnd::ChangeOpacity(BYTE iFactor)
{
	BOOL bRes=_setLayeredWindowAttributes(m_hWnd,RGB(255,255,255), iFactor, LWA_COLORKEY | LWA_ALPHA);
	return 0;
}

LRESULT CNotifyWnd::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	::PostMessage(m_hParenWnd,UM_FINANCE,UP_DESKTOP_NOTIFY_CLOSE,0); 
	DestroyWindow();	
	return 1;
}
LRESULT CNotifyWnd::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_pButton->DestroyWindow();
	delete m_pButton;
	return 1;
}

LRESULT CNotifyWnd::OnMouseDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	::PostMessage(m_hParenWnd,UM_FINANCE,UP_DESKTOP_NOTIFY_CLICK,0);  
	CloseWindow(m_hWnd); 	
	return 1;
}

LRESULT CNotifyWnd::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(m_bTimerActive)
	{
		KillTimer(TIMER_ID);
		m_bTimerActive=FALSE;
		ChangeOpacity(m_nOpacity);
		m_nCurrentOpacity=m_nOpacity;
		//fortunately this we can get help from foe setting mouse leave event
		TRACKMOUSEEVENT stMouseEvent;
		//to check Mouse leave event
		stMouseEvent.cbSize=sizeof(stMouseEvent);
		stMouseEvent.dwFlags=TME_LEAVE;
		stMouseEvent.dwHoverTime=HOVER_DEFAULT;
		stMouseEvent.hwndTrack=m_hWnd;
		TrackMouseEvent(&stMouseEvent);
		HCURSOR hCursor=LoadCursor(NULL,IDC_HAND);
		SetClassLong(m_hWnd,GCL_HCURSOR,(LONG) hCursor);   // new cursor 
	}
	return 0;
}
LRESULT CNotifyWnd::OnMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	ATLASSERT(m_bTimerActive == FALSE);
	m_bTimerActive=SetTimer(TIMER_ID ,30);
	HCURSOR hCursor=GetCursor();
	SetClassLong(m_hWnd,GCL_HCURSOR,(LONG) hCursor);   // new cursor 
	return 0;
}

CNotifyWnd::CNotifyWnd(HWND hParentWnd,int nWidth,int nHeight,unsigned int nShowTime,BYTE nOpacity)
:m_hParenWnd(hParentWnd),
m_nWidth(nWidth),
m_nHeight(nHeight),
m_nShowTime(nShowTime),
m_nOpacity(nOpacity),
m_nCurrentOpacity(0),
_setLayeredWindowAttributes(0),
_user32DLL(0)
{
	_user32DLL = LoadLibrary (_T("user32"));
	if (_user32DLL)
	{
		_setLayeredWindowAttributes = (pSetLayeredWindowAttributes) GetProcAddress(_user32DLL,_T("SetLayeredWindowAttributes"));
	}
}

CNotifyWnd::~CNotifyWnd()
{
	FreeLibrary(_user32DLL);	
}
