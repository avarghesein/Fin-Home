// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__87B3EBE1_1512_4443_AEB1_F927C5ED07B9__INCLUDED_)
#define AFX_STDAFX_H__87B3EBE1_1512_4443_AEB1_F927C5ED07B9__INCLUDED_

// Change these values to use different versions
#define WINVER		0x0400
//#define _WIN32_WINNT	0x0400

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif

#define _WIN32_IE	0x0400
#define _RICHEDIT_VER	0x0100

#include "AggressiveOptimize.h"

#include <atlbase.h>
#include <atlapp.h>

extern CAppModule _Module;

#include <atlwin.h>

#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>
#include <atlctrlw.h>
#include <atlmisc.h>
#include <atlcrack.h>
#include <atlddx.h>

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#include "TimeCore.h"
#include "Constants.h"

#endif // !defined(AFX_STDAFX_H__87B3EBE1_1512_4443_AEB1_F927C5ED07B9__INCLUDED_)
