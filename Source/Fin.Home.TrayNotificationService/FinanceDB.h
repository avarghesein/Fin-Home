
#pragma once

class CFinanceDB
{
public:
	CString GetAllAlerts(void);
	bool GetAlerts(CString& policy, CString& investment, CString& tasks);
	bool GetFinanceHomeApp(CString&,CString&);
	bool GetFinanceDBPath(CString&);
};

