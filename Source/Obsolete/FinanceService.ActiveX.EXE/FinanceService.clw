; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=PropertiesDlg
LastTemplate=CPropertyPage
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "financeservice.h"
LastPage=0

ClassCount=5
Class1=CFinApp
Class2=CFinWindow
Class3=General
Class4=PropertiesDlg

ResourceCount=4
Resource1=IDD_ADV
Resource2=IDD_DIALOG_CONFIG
Resource3=IDD_GENERAL
Class5=Advanced
Resource4=IDR_FIN_MENU

[CLS:CFinApp]
Type=0
BaseClass=CWinApp
HeaderFile=Finance.h
ImplementationFile=Finance.cpp

[CLS:CFinWindow]
Type=0
BaseClass=CWnd
HeaderFile=FinanceDlg.h
ImplementationFile=FinanceDlg.cpp

[CLS:General]
Type=0
BaseClass=CPropertyPage
HeaderFile=General.h
ImplementationFile=General.cpp
LastObject=General
Filter=D
VirtualFilter=idWC

[CLS:PropertiesDlg]
Type=0
BaseClass=CDialog
HeaderFile=PropertiesDlg.h
ImplementationFile=PropertiesDlg.cpp
Filter=D
VirtualFilter=dWC
LastObject=PropertiesDlg

[DLG:IDD_GENERAL]
Type=1
Class=General
ControlCount=2
Control1=IDI_YELLOW,static,1342177283
Control2=IDC_EDT_ALERT,edit,1352665220

[DLG:IDD_DIALOG_CONFIG]
Type=1
Class=PropertiesDlg
ControlCount=4
Control1=IDC_CONFG_APPLY,button,1342177280
Control2=IDC_CONFG_OK,button,1342177280
Control3=IDC_CONFG_CANCEL,button,1342177280
Control4=IDC_OPEN_DB,button,1342177280

[MNU:IDR_FIN_MENU]
Type=1
Class=?
Command1=ID_MAIN_OPTIONS
Command2=ID_MAIN_EXIT
CommandCount=2

[DLG:IDD_ADV]
Type=1
Class=Advanced
ControlCount=12
Control1=IDC_CHK_HIDEALERTS,button,1342242819
Control2=IDC_STATIC,static,1342308352
Control3=IDC_CHK_HIDEALERTS_DTM,button,1342242819
Control4=IDC_STATIC,static,1342308352
Control5=IDC_CTL_TIME,SysDateTimePick32,1342242873
Control6=IDC_CTL_CAL,{8E27C92B-1264-101C-8A2F-040224009C02},1342242816
Control7=IDC_CHK_AUTOSTART,button,1342242819
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_EDT_ALERT_INTVL,edit,1350631552
Control11=IDC_CHK_TRAY,button,1342242819
Control12=IDC_STATIC,static,1342308352

[CLS:Advanced]
Type=0
HeaderFile=Advanced.h
ImplementationFile=Advanced.cpp
BaseClass=CPropertyPage
Filter=D
LastObject=IDC_CHK_TRAY
VirtualFilter=idWC

