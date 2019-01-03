
#pragma once

#define STR(x) _T(#x)
#define T(x) _T(x)
#define CSTR(x) CString(T(x))
#define EMPTY _T("")

#define TRIM(value) value.TrimLeft();  \
				    value.TrimRight(); 

//User window messages
#define UM_FINANCE WM_USER + 201

	#define UP_DESKTOP_NOTIFY_CLICK   0
	#define UP_DESKTOP_NOTIFY_CLOSE   1
	#define UP_DESKTOP_PROPERTY_CLOSE 2
	#define UP_SETTINGS_CHANGE        3

#define UM_TRAYMENU WM_USER + 202


//Timer Settings
#define MSG_AlERT_STARTUP_TIMER   1
#define MSG_AlERT_CHECK_TIMER     2
#define CONST_ALERT_CHECK_TIMEOUT 5


//Settings Config
#define INI_FILE_NAME _T("Finance@Service.ini")
#define INI_SECTION_NAME _T("NOTIFY")
#define REG_AUTOSTARTUP_NAME _T("Finance@Service")

#define FIN_UNIQUE_WINDOW_NAME _T("C6E07C35-72D5-40d9-B1C3-9EB538818B6E")


struct SConfig
{
public:
	bool ShowInSystemTray;
	int AlertTimeInterval;
	bool AutoStartup;
	bool UnNotifyTillNextTime;

	bool UnNotifyTillNextDTM;	
	CTime UnNotifyTillNext_DTM_VALUE;
};

extern SConfig _globalConfigObject;


