

#include "stdafx.h"
#include "SettingsManager.h"

bool CSettingsManager::UpdateData(SConfig& config, bool isLoading)
{
	bool stat;
	CString key;
	CString value;

	if(isLoading)
	{	
		key = STR(ShowInSystemTray);		
		stat = GetSetting(key,value);
		if(!stat || value == EMPTY)
		{
			config.ShowInSystemTray = false;
		}
		else
		{
			TRIM(value);
			value.MakeLower(); 
			config.ShowInSystemTray = value == STR(true);
		}

		key = STR(AutoStartup);		
		stat = GetSetting(key,value);
		if(!stat || value == EMPTY)
		{
			config.AutoStartup = false;
		}
		else
		{
			TRIM(value);
			value.MakeLower(); 
			config.AutoStartup = value == STR(true);
		}

		key = STR(UnNotifyTillNextTime);		
		stat = GetSetting(key,value);
		if(!stat || value == EMPTY)
		{
			config.UnNotifyTillNextTime = false;
		}
		else
		{
			TRIM(value);
			value.MakeLower(); 
			config.UnNotifyTillNextTime = value == STR(true);
		}

		key = STR(UnNotifyTillNextDTM);		
		stat = GetSetting(key,value);
		if(!stat || value == EMPTY)
		{
			config.UnNotifyTillNextDTM = false;
		}
		else
		{
			TRIM(value);
			value.MakeLower(); 
			config.UnNotifyTillNextDTM = value == STR(true);
		}

		key = STR(UnNotifyTillNext_DTM_VALUE);		
		stat = GetSetting(key,value);
		config.UnNotifyTillNext_DTM_VALUE = GetDateTime(value); 

		key = STR(AlertTimeInterval);		
		stat = GetSetting(key,value);
		int iCnt = _stscanf(value, _T("%d"), &config.AlertTimeInterval); 
		if (iCnt != 1) 
		{   
			config.AlertTimeInterval = CONST_ALERT_CHECK_TIMEOUT;
		} 
	}
	else
	{
		key = STR(ShowInSystemTray);
		value = config.ShowInSystemTray ? STR(true):STR(false);
		stat = SetSetting(key,value);

		key = STR(AutoStartup);
		value = config.AutoStartup ? STR(true):STR(false);
		stat = SetSetting(key,value);

		key = STR(UnNotifyTillNextTime);
		value = config.UnNotifyTillNextTime ? STR(true):STR(false);
		stat = SetSetting(key,value);

		key = STR(UnNotifyTillNextDTM);
		value = config.UnNotifyTillNextDTM ? STR(true):STR(false);
		stat = SetSetting(key,value);

		key = STR(UnNotifyTillNext_DTM_VALUE);		
		value = GetDateTimeString(config.UnNotifyTillNext_DTM_VALUE);
		stat = SetSetting(key,value);

		key = STR(AlertTimeInterval);		
		value.Format(_T("%d"),config.AlertTimeInterval);  
		stat = SetSetting(key,value);
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
	COleDateTime oleDT;
	if(dtStr == EMPTY || !oleDT.ParseDateTime(dtStr))
	{
		return CTime::GetCurrentTime();
	}

	CTime time;
	SYSTEMTIME st;        
	if(oleDT.GetAsSystemTime(st))          
	{
		time = st;
	} 
	else
	{
		time = CTime::GetCurrentTime();
	}
	return time;
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
	return stat;
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
