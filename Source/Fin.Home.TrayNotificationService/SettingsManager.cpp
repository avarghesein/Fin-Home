

#include "stdafx.h"
#include "SettingsManager.h"


#define GET_BOOL_SETTING(variable)	key = STR(variable); \
									stat = GetSetting(key,value); \
									if(!stat || value == EMPTY) \
									{ \
										config.##variable = false; \
									} \
									else \
									{ \
										TRIM(value); \
										value.MakeLower(); \
										config.##variable = value == STR(true); \
									} \

#define GET_DTM_SETTING(variable)	key = STR(variable); \
									stat = GetSetting(key,value); \
									config.##variable = GetDateTime(value); \

#define GET_INT_SETTING(variable,defaultvalue)	key = STR(variable); \
									stat = GetSetting(key,value); \
									iCnt = _stscanf(value, _T("%d"), &config.##variable); \
									if (iCnt != 1) \
									{ \
										config.##variable = defaultvalue; \
									} \
									
#define GET_STR_SETTING(variable)	key = STR(variable); \
									stat = GetSetting(key,value); \
									config.##variable = value; \

#define SET_BOOL_SETTING(variable)	key = STR(variable); \
									value = config.##variable ? STR(true):STR(false); \
									stat = SetSetting(key,value); \

#define SET_DTM_SETTING(variable)	key = STR(variable); \
									value = GetDateTimeString(config.##variable); \
									stat = SetSetting(key,value); \

#define SET_INT_SETTING(variable)	key = STR(variable); \
									value.Format(_T("%d"),config.##variable); \
									stat = SetSetting(key,value); \

#define SET_STR_SETTING(variable)	key = STR(variable); \
									value = config.##variable; \
									stat = SetSetting(key,value); \

bool CSettingsManager::UpdateData(SConfig& config, bool isLoading)
{
	bool stat;
	CString key;
	CString value;
	int iCnt = 0;

	
	if(isLoading)
	{	
		GET_BOOL_SETTING(ShowInSystemTray)
		GET_BOOL_SETTING(AutoStartup)
		GET_BOOL_SETTING(UnNotifyTillNextTime)
		GET_BOOL_SETTING(UnNotifyTillNextDTM)
		GET_DTM_SETTING(UnNotifyTillNext_DTM_VALUE)

		GET_INT_SETTING(AlertTimeInterval, CONST_ALERT_CHECK_TIMEOUT)		
		GET_BOOL_SETTING(OnCountCheckAlerts)
		GET_INT_SETTING(AlertCheckCount, CONST_ALERT_CHECK_COUNT)
		
		GET_BOOL_SETTING(AutoBackup)
		GET_INT_SETTING(BackupIntervalDays, CONST_BKUP_INTERVAL_DAYS)	
		GET_STR_SETTING(BackupPath)
		GET_BOOL_SETTING(BackupStatus)
		GET_DTM_SETTING(LastBackupDate)
	}
	else
	{
		SET_BOOL_SETTING(ShowInSystemTray)
		SET_BOOL_SETTING(AutoStartup)
		SET_BOOL_SETTING(UnNotifyTillNextTime)
		SET_BOOL_SETTING(UnNotifyTillNextDTM)
		SET_DTM_SETTING(UnNotifyTillNext_DTM_VALUE)
	
		SET_INT_SETTING(AlertTimeInterval)
		SET_BOOL_SETTING(OnCountCheckAlerts)
		SET_INT_SETTING(AlertCheckCount)

		SET_BOOL_SETTING(AutoBackup)
		SET_INT_SETTING(BackupIntervalDays)
		SET_STR_SETTING(BackupPath)
		SET_BOOL_SETTING(BackupStatus)		
		SET_DTM_SETTING(LastBackupDate);

	}

	if(!isLoading)
	{
		// Save the auto startup, registry option
		try
		{
			// Save the auto start up option...
			CRegKey RegKey;
			// Create/Open the required regkey
			bool bSuccess = ERROR_SUCCESS == RegKey.Create(  HKEY_CURRENT_USER,
															 T("Software\\Microsoft\\Windows\\CurrentVersion\\Run") );
			// If we successfully opened the registry key
			if( bSuccess )
			{
				// If user selected the auto start option
				if( config.AutoStartup )
				{
					// Register our exe in the registry for auto startup	
					bSuccess = ERROR_SUCCESS == RegKey.SetValue( CSettingsManager::GetEXEPath() , REG_AUTOSTARTUP_NAME );
				}
				else
				{
					// Remove the auto startup option from the registry, if its already there
					RegKey.DeleteValue( REG_AUTOSTARTUP_NAME );
				}
				// Close the registry key...
				RegKey.Close();
			}
		}
		catch( ... ) 
		{
		}
	}
	return true;
}

CString CSettingsManager::GetDateTimeString(CTime& dt)
{
	CString fmt = dt.Format(T("%d-%B-%Y %I:%M:%S %p"));
	return fmt;
}

CTime CSettingsManager::GetDateTime(CString& dtStr)
{
	CTime oleDT;
	if(dtStr == EMPTY || !oleDT.ParseDateTime(dtStr))
	{
		return CTime::GetCurrentTime();
	}
	return oleDT;
}

bool CSettingsManager::GetSetting(CString& key,CString& value)
{
	TCHAR out[1000];
	out[0] = _T('\0');

	GetPrivateProfileString(INI_SECTION_NAME,key,_T(""),out,1000,INI_FILE_NAME);
	value = out;
	return true;
}

bool CSettingsManager::SetSetting(CString& key, CString& value)
{
	BOOL stat = WritePrivateProfileString(INI_SECTION_NAME,key,value,INI_FILE_NAME); 
	return (bool)stat;
}

CString& CSettingsManager::GetEXEPath()
{
	static CString exePath;

	if(exePath != EMPTY)
	{
		return exePath;
	}
	TCHAR szExePath[ 1024 ];
	GetModuleFileName( 0, szExePath, 1024 );
	exePath = szExePath;
	exePath += _T(" -Startup"); //Run in service mode
	return exePath;
}

CString& CSettingsManager::GetStartupPath()
{
	static CString startupPath;

	if(startupPath != EMPTY)
	{
		return startupPath;
	}

	CString exeFile = GetEXEPath();
	int nIndex = exeFile.ReverseFind( '\\' );
	startupPath = exeFile.Left( nIndex + 1 );
	return startupPath;
}


/*------------------------------------------------------------------
   FirstDriveFromMask( unitmask )

   Description
     Finds the first valid drive letter from a mask of drive letters.
     The mask must be in the format bit 0 = A, bit 1 = B, bit 2 = C, 
     and so on. A valid drive letter is defined when the 
     corresponding bit is set to 1.

   Returns the first drive letter that was found.
--------------------------------------------------------------------*/

TCHAR CSettingsManager::GetDriveLetterFromMask( ULONG unitmask )
{
	TCHAR i;

	for (i = 0; i < 26; ++i)
	{
		if (unitmask & 0x1)
		  break;

		unitmask = unitmask >> 1;
	}

	return( i + T('A'));
}

#ifndef INVALID_FILE_ATTRIBUTES
	#define INVALID_FILE_ATTRIBUTES ((DWORD)-1)
#endif

bool CSettingsManager::CopyDBFile(CString& targetFolder, CString& srcFileName, TCHAR driveInserted)
{
	TRIM(targetFolder)
	TRIM(srcFileName)

	if(targetFolder == EMPTY) return false;
	if(srcFileName == EMPTY) return false;

	if(driveInserted != 0)
	{
		TCHAR drive = targetFolder.GetAt(0);
		if(drive != driveInserted) return false; 
	}

	DWORD dwAttribs = GetFileAttributes(srcFileName);

	if(INVALID_FILE_ATTRIBUTES == dwAttribs) return false;

	CString targetFileName = targetFolder;
	dwAttribs = GetFullPathName(srcFileName,0,0,0);
	TCHAR* buff = new TCHAR[dwAttribs + 1];
	TCHAR* fileName = 0;
	dwAttribs = GetFullPathName(srcFileName,dwAttribs,buff,&fileName);
	targetFileName += CSTR("\\") + fileName;
	delete[] buff;

	TCHAR *srcFileNameLng = new TCHAR[srcFileName.GetLength() + 2]; 
	TCHAR *targetFileNameLng = new TCHAR[targetFileName.GetLength() + 2]; 
	_tcscpy(srcFileNameLng,srcFileName); 
	_tcscpy(targetFileNameLng,targetFileName);

	srcFileNameLng[srcFileName.GetLength() + 1] = T('\0'); 
	targetFileNameLng[targetFileName.GetLength() + 1] = T('\0'); 

	SHFILEOPSTRUCT copyStruct = {0};
	copyStruct.hwnd = 0;
	copyStruct.wFunc = FO_COPY;
	copyStruct.pFrom = srcFileNameLng;
	copyStruct.pTo = targetFileNameLng;

	copyStruct.fFlags = FOF_NOCONFIRMMKDIR | FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_SILENT;

	int ret = SHFileOperation(&copyStruct);

	delete[] srcFileNameLng;
	delete[] targetFileNameLng;

	dwAttribs = GetFileAttributes(targetFileName);
	
	if(INVALID_FILE_ATTRIBUTES == dwAttribs) return false;

	return ret == ERROR_SUCCESS;
}

bool CSettingsManager::IsBackupNeeded(SConfig& config_i)
{
	if(!config_i.AutoBackup) return false;
	
	if(config_i.BackupStatus)
	{
		CTime curTime = CTime::GetCurrentTime();
		CTime lstBkup = config_i.LastBackupDate;

		if(lstBkup <= curTime)
		{
			CTimeSpan spanElpsed = curTime - lstBkup;			
			if(abs(spanElpsed.GetDays()) < config_i.BackupIntervalDays) return false;		
		}
	}
	return true;
}

bool CSettingsManager::TakeBackup(SConfig& config_io,CString& dbPath, TCHAR driveInserted)
{
	if(!IsBackupNeeded(config_io)) return false;
	
	if(CopyDBFile(config_io.BackupPath,dbPath,driveInserted))
	{
		config_io.BackupStatus = true;
		config_io.LastBackupDate = CTime::GetCurrentTime();
		return true;
	}
	else
	{
		config_io.BackupStatus = false;
		return false;
	}
}

