
#if !defined(_ATL_PROPERTIESSHEET_H)
#define _ATL_PROPERTIESSHEET_H

#if _MSC_VER > 1000
#pragma once
#endif

class CPropView : public CPropertySheetImpl<CPropView>
{
public:

	BEGIN_MSG_MAP(CPropView)
		CHAIN_MSG_MAP(CPropertySheetImpl<CPropView>)
	END_MSG_MAP()

	BOOL PreTranslateMessage(MSG* pMsg);

	// constructor. At least one page must be added before window is created
	CPropView();

	// override the callback procedure and add a pre-creation message
	// handler so that we can change the window style
	static int CALLBACK PropSheetCallback(HWND hWnd, UINT uMsg, LPARAM lParam);
};

#endif
