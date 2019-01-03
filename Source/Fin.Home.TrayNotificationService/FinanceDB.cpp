
#pragma once

#include "stdafx.h"
#include "FinanceDB.h"

#include <Wtypes.h>

#pragma warning( push )
#pragma warning(disable:4146)
#import "C:\program files\common files\system\ado\msado15.dll" no_namespace rename("EOF","adoEOF")
#pragma warning( pop )

#import "..\FinanceServer\Finance@Server.exe" no_namespace

bool CFinanceDB::GetAlerts(CString& policy, CString& investment, CString& tasks)
{
	try
	{
		BSTR szPol = 0,szInv = 0,szTsk = 0;

		_FinanceFactoryPtr pFactory(__uuidof(FinanceFactory));	
		
		if(pFactory)
		{
			_FinanceServerPtr p = pFactory->GetFinanceServerObject();

			VARIANT_BOOL stat = p->GetAlerts(&szPol,&szInv,&szTsk); 

			policy = szPol;
			investment = szInv;
			tasks = szTsk; 

			TRIM(policy)
			TRIM(investment)
			TRIM(tasks)

			SysFreeString(szPol); 
			SysFreeString(szInv); 
			SysFreeString(szTsk);

			pFactory.Release(); 

			return (bool)stat;
		}
	}
	catch(...) 
	{
		
	}
    return false;
}

bool CFinanceDB::GetFinanceHomeApp(CString& appName,CString& inputParams)
{
	try
	{
		_FinanceFactoryPtr pFactory(__uuidof(FinanceFactory));	
		
		if(pFactory)
		{
			_FinanceServerPtr p = pFactory->GetFinanceServerObject();

			BSTR app = 0;
			BSTR params = 0;
			VARIANT_BOOL stat = p->GetFinanceHomeApp(&app,&params); 

			appName = app;
			inputParams = params;

			SysFreeString(app);
			SysFreeString(params);

			pFactory.Release(); 

			return (bool)stat;
		}
	}
	catch(...) {}
    return false;
}

bool CFinanceDB::GetFinanceDBPath(CString& dbPath)
{
	try
	{
		_FinanceFactoryPtr pFactory(__uuidof(FinanceFactory));	
		
		if(pFactory)
		{
			_FinanceServerPtr p = pFactory->GetFinanceServerObject();

			BSTR path = 0;
			VARIANT_BOOL stat = p->GetFinanceDBPath(&path); 

			dbPath = path;

			SysFreeString(path);
			pFactory.Release(); 

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
	
	all = pol;
	if(pol != EMPTY) all += _T("\r\n\r\n");

	all += inv; 
	if(inv != EMPTY) all += _T("\r\n\r\n");

	all += tsk; 
	if(tsk != EMPTY) all += _T("\r\n\r\n");

	return all;
}