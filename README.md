# Fin@Home
A demo finance application featuring Desktop, Web and Native Mobile [Android, Java Feature Phone (J2ME) ] Platforms...

The below technologies have been used to build this application;

# Moble
* Android Native Programming through Android SDK
* Feature Phone Programming through J2ME/Java Midlet SDK

# Desktop
* Microsoft Access 2007+ Database Technologies
* Visual C++ 6 (MFC, ATL, WTL - COM/Active X Technologies)
* Visual Basic 6 (Active X Technology)
* HTML/CSS

Note: Though I've added the projects into a Visual Studio 2013 Solution, Above projects requires Visual Studio 6 Compilers. You could upgrade the projects to the latest Visual Studio if you want, though it requires some effort, as MFC/ATL technologies has now some structural changes (i.e CString is now common for both ATL/MFC and such C++ Header names changed in latest Visual Studio Versions)

# How to Use The Application/Navigate the Source

PROJECT STRUCTURE
![alt Source](https://github.com/avarghesein/Fin-Home/blob/master/Source/Screenshots/FinHome.ProjectStructure.jpg)

Navigate the projects as Numbered

# 1. Desktop Main Application (Finance@Home.mdb Application) - No Build Required
This is a Microsoft Access file, which you can directly open given Microsoft Office (Access) 2007+ installed on your system. This suite contains feature to add your daily expenditures, bank investments, life insurance policies, scheduled tasks etc. Reporting based on each of the above categories have been provided for viewing a consolidated summary

![alt Main Application](https://github.com/avarghesein/Fin-Home/blob/master/Source/Screenshots/FinHome.DesktopMDBApplication.jpg)

# 2. Desktop Web Application (Finance@Home.html Page) - No Build Required
This is an easy approach to use the application directly in a IE WebPage, without Microsoft Office Application installed on your system. It internally uses ActiveX and ODBC technologies to talk to the Finance@Home database 

![alt Web Application](https://github.com/avarghesein/Fin-Home/blob/master/Source/Screenshots/FinHome.DesktopHTMLPage.jpg)

# 3-4. Desktop Tray Notification for Live Alers (TrayNotificatonComponents) - Build using Visual Studio 6 (VB6, VC++ 6 compilers)*
If you would like to get Live notifications (Say if your policy have a due or a scheduled tasks is already gone past), this components will provide you the feature. It will give you OUTLOOK kind desktop pop up notifications, clicking on which will open detailed Report.
It has build using two components

a. Notification Active X Server - VB6

A VB Active X Exe Server talks to the underlying Finance@Home Access details to fetch alerts. It then provide those details through a COM Interface to the consumers (eg. Notification Desktop Service mentioned below)

b. Notification Desktop Service - VC++6 with ATL/WTL dependency

The Desktop Notification Service (ATL/WTL Visual C++ Windows GUI Application). It fetch the notification details from "a. Notification Active X Server", and will display to the user.

![alt Notification Server](https://github.com/avarghesein/Fin-Home/blob/master/Source/Screenshots/FinHome.DesktopLiveNotification.jpg)

# 5. Android Mobile Native Application. - Build using NetBeans, Android SDK

You could directly install this sub application to your Android Mobile,so that daily activities can be recorded on the Go (instead of using the Desktop Machine every now and then to enter the daily details, which is not practical). This application uses an 'SQLite' light database to store the details in the mobile. Once you've enough details you could then export the details as a Text file from the Mobile. Copy the text file to your desktop (through USB/Wifi), and Import the Text file directly in to the 1. Finance@Home.mdb Office Access Main Application

Use NetBeans, Android SDK to build the application. Copy the APK and the template SQLite file (provided inside the source) to your Android Mobile through USB, Install the APK file to start using the application

![alt Android App](https://github.com/avarghesein/Fin-Home/blob/master/Source/Screenshots/AndroidMobileHome.jpg)
![alt Android NewEntry](https://github.com/avarghesein/Fin-Home/blob/master/Source/Screenshots/AndroidNewEntry.jpg)
![alt Android NewEntry2](https://github.com/avarghesein/Fin-Home/blob/master/Source/Screenshots/AndroidNewEntry2.jpg)

# 6. Feature Mobile Native Application. - Build using NetBeans, J2ME - Java Midlet SDK

You could directly install this sub application to your Feature Mobile (J2ME supported),so that daily activities can be recorded on the Go (instead of using the Desktop Machine every now and then to enter the daily details, which is not practical). This application uses an J2ME record store to store the details in the mobile. Once you've enough details you could then export the details as a Text file from the Mobile. Copy the text file to your desktop (through USB/Wifi), and Import the Text file directly in to the 1. Finance@Home.mdb Office Access Main Application

Use NetBeans, J2ME SDK to build the application. Copy the JAR file to your Feature Mobile through USB, Install the JAR file to start using the application




