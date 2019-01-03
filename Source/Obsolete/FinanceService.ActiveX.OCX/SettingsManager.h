
#pragma once

class CSettingsManager
{
protected:
	bool GetSetting(CString&,CString&);
	bool SetSetting(CString&,CString&);

public:
	bool UpdateData(SConfig&,bool isLoading = true);
	
	CString GetDateTimeString(CTime&);
	CTime GetDateTime(CString&);

	static CString& GetEXEPath();
	static CString& GetStartupPath();
};