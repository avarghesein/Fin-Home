#include "resource.h"
#include "NotifyWnd.h"

class CBmpButton :public CWindowImpl<CBmpButton>
{
	UINT m_BitmapId[3];
	UINT m_nCurrentBmp;
public:
	CBmpButton(UINT*);
	virtual ~CBmpButton();
    DECLARE_WND_SUPERCLASS( _T("BitmapButton"), _T("Button") )
    BEGIN_MSG_MAP(CBmpButton)
        MESSAGE_HANDLER(WM_CREATE, OnCreate)
        MESSAGE_HANDLER(WM_CLOSE, OnClose)
        MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
        MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		MESSAGE_HANDLER(WM_MOUSELEAVE, OnMouseLeave)
		MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUP)
		//to avoid flicker
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
		//parent will reflect this message
        MESSAGE_HANDLER(OCM_DRAWITEM, OnDrawItem)
		DEFAULT_REFLECTION_HANDLER()
    END_MSG_MAP()
private:
    LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDrawItem(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnLButtonUP(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};
