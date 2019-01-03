; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "financeservice.h"
LastPage=0

ClassCount=4
Class1=General
Class2=PropertiesDlg
Class3=Advanced
Class4=CCreditsDebits

ResourceCount=7
Resource1=IDD_GENERAL
Resource3=IDR_MAINFRAME
Resource4=IDD_ADV
Resource5=IDD_DIALOG_CONFIG
Resource6=IDD_DEBITS_CREDITS
Resource7=IDR_FIN_MENU

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
ControlCount=1
Control1=IDC_EDT_ALERT,edit,1352665220

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
Command2=ID_MAIN_RESET
Command3=ID_MAIN_EXIT
CommandCount=3

[DLG:IDD_ADV]
Type=1
Class=Advanced
ControlCount=16
Control1=IDC_CHK_HIDEALERTS,button,1342242819
Control2=IDC_STATIC,static,1342308352
Control3=IDC_CHK_HIDEALERTS_DTM,button,1342242819
Control4=IDC_STATIC,static,1342308352
Control5=IDC_CTL_TIME,SysDateTimePick32,1342242873
Control6=IDC_CHK_AUTOSTART,button,1342242819
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_EDT_ALERT_INTVL,edit,1350631552
Control10=IDC_CHK_TRAY,button,1342242819
Control11=IDC_STATIC,static,1342308352
Control12=IDC_CTL_CAL,SysMonthCal32,1342242840
Control13=IDC_CHK_ONCOUNTCHECKALERT,button,1342242819
Control14=IDC_STATIC,static,1342308352
Control15=IDC_EDT_ALERT_CHKCOUNT,edit,1350631552
Control16=IDC_STATIC,static,1342308352

[DLG:IDD_DEBITS_CREDITS]
Type=1
Class=CCreditsDebits
ControlCount=0

