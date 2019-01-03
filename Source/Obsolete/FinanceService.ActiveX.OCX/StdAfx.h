
#if !defined(_AFX_STDAFX_H)
#define _AFX_STDAFX_H

#if _MSC_VER > 1000
#pragma once
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif
#undef WINVER  
#define WINVER  0x0500 

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

//#include "AggressiveOptimize.h"

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#include <afxpriv.h>
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <atlbase.h>
extern CComModule _Module;
#include <atlcom.h>
#include <atlwin.h>

#include "Constants.h"

//{{AFX_INSERT_LOCATION}}

#endif
