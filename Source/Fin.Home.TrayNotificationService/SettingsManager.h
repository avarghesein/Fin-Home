
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

	TCHAR GetDriveLetterFromMask( ULONG unitmask );

	bool CopyDBFile(CString& targetFolder, CString& srcFileName, TCHAR driveInserted = 0);
	bool TakeBackup(SConfig& config_io,CString& dbPath, TCHAR driveInserted = 0);
	bool IsBackupNeeded(SConfig& config_i);
};