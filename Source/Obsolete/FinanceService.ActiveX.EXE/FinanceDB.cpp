
#pragma once

#include "stdafx.h"
#include "FinanceDB.h"

#include <Wtypes.h>
#import "C:\program files\common files\system\ado\msado15.dll" no_namespace rename("EOF","adoEOF")

#import "..\FinanceServer\Finance@Server.exe" no_namespace


bool CFinanceDB::GetAlerts(CString& policy, CString& investment, CString& tasks)
{
	try
	{
		BSTR szPol = 0,szInv = 0,szTsk = 0;

		_FinanceServerPtr p(__uuidof(FinanceServer));
		
		if(p)
		{
			VARIANT_BOOL stat = p->GetAlerts(&szPol,&szInv,&szTsk); 

			policy = szPol;
			investment = szInv;
			tasks = szTsk; 

			SysFreeString(szPol); 
			SysFreeString(szInv); 
			SysFreeString(szTsk);

			p.Release(); 

			return (bool)stat;
		}
	}
	catch(...) {}
    return false;
}

bool CFinanceDB::GetFinanceHomeApp(CString& appName,CString& inputParams)
{
	try
	{
		_FinanceServerPtr p(__uuidof(FinanceServer));
		
		if(p)
		{
			BSTR app = 0;
			BSTR params = 0;
			VARIANT_BOOL stat = p->GetFinanceHomeApp(&app,&params); 

			appName = app;
			inputParams = params;

			SysFreeString(app);
			SysFreeString(params);

			p.Release(); 

			return (bool)stat;
		}
	}
	catch(...) {}
    return false;
}

CString CFinanceDB::GetAllAlerts(void)
{
	CString pol,inv,tsk,all;

	GetAlerts(pol, inv, tsk);
	
	all = pol + _T("\r\n"); 
	all += inv + _T("\r\n"); 
	all += tsk + _T("\r\n"); 

	return all;
}