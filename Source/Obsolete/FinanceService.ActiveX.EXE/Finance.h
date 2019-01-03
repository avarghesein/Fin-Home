
#if !defined(_AFX_FIN_H)
#define _AFX_FIN_H

#if _MSC_VER > 1000
#pragma once
#endif 

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"
#include "FinanceDlg.h"

class CFinApp : public CWinApp
{
public:
	CFinApp();

	//{{AFX_VIRTUAL(CFinApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CFinApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

    private:

	CFinWindow m_wndOwner;
};

//{{AFX_INSERT_LOCATION}}

#endif 
