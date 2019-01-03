
#pragma once

#include "stdafx.h"
#include "PropertySheet.h"

BOOL CPropView::PreTranslateMessage(MSG* pMsg)
{
	return FALSE;
}

// constructor. At least one page must be added before window is created
CPropView::CPropView()
{
	// disable Apply button
	m_psh.dwFlags |= PSH_NOAPPLYNOW;			
}

// override the callback procedure and add a pre-creation message
// handler so that we can change the window style
int CALLBACK CPropView::PropSheetCallback(HWND hWnd, UINT uMsg, LPARAM lParam)
{
	// dialog template is available and changeable pre-creation
	if(uMsg == PSCB_PRECREATE)
	{
		LPDLGTEMPLATE lpDT = (LPDLGTEMPLATE)lParam;

		// remove dialog border styles
		lpDT->style -= DS_MODALFRAME | WS_POPUP | WS_CAPTION | WS_SYSMENU;

		// add child window and clipping styles
		lpDT->style |= WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

		return 0;
	}
	else // call inherited method
		return CPropertySheetImpl<CPropView>::PropSheetCallback(hWnd, uMsg, lParam);
}




