Attribute VB_Name = "GlobalProcedures"
Option Explicit

Function OpenConnection() As ADODB.Connection
    Set OpenConnection = GetConnection
End Function

Function CloseConnection(cn As ADODB.Connection)
    cn.Close
End Function

Function OpenCommand(cn As ADODB.Connection, sql As String)
    cn.Execute (sql)
End Function

Function OpenRecordSet(cn As ADODB.Connection, sql As String) As ADODB.Recordset
    Dim rs As New ADODB.Recordset
    With rs
        .ActiveConnection = cn
        .CursorLocation = adUseClient
        .CursorType = adOpenDynamic
        .LockType = adLockOptimistic
        .Open sql
    End With
    Set OpenRecordSet = rs
End Function

Function Format(dateVal As Date, formatTag As String) As String
    If IsNull(dateVal) Or Trim(dateVal) = Empty Then
        Format = "N/A"
        Exit Function
    End If
    Format = Strings.Format(dateVal, formatTag)
End Function

Function Nz(value, defaultValue)
    If IsNull(value) Then
        Nz = Trim(defaultValue)
    Else
        Nz = Trim(value)
    End If
End Function

Public Function GetVBDateTimeUnit(unitType As String, ByRef unit As Double) As String
    Dim vbUnit As String

    Select Case LCase(Left(Trim(unitType), 3))
        Case Empty, "sec"
            vbUnit = "s"
        Case "min"
            vbUnit = "n"
        Case "hou"
            vbUnit = "h"
        Case "day"
            vbUnit = "d"
        Case "wee"
            unit = unit * 7
            vbUnit = "d"
        Case "mon"
            vbUnit = "m"
        Case "qua"
            unit = unit * 4
            vbUnit = "m"
        Case "yea"
            vbUnit = "yyyy"
    End Select
    
    GetVBDateTimeUnit = vbUnit
End Function

Public Function GetFinanceDateTimeUnit(unitType As String) As String
    Dim vbUnit As String

    Select Case LCase(Left(Trim(unitType), 3))
        Case Empty, "sec"
            vbUnit = Sec_Code
        Case "min"
            vbUnit = Min_Code
        Case "hou"
            vbUnit = Hour_Code
        Case "day"
            vbUnit = Day_Code
        Case "wee"
            vbUnit = Week_Code
        Case "mon"
            vbUnit = Month_Code
        Case "qua"
            vbUnit = Quarater_Code
        Case "yea"
            vbUnit = Year_Code
    End Select
    
    GetFinanceDateTimeUnit = vbUnit
End Function

Public Function AddDateTimeBasedOnUnits(unit As Double, unitType As String, time As Date) As Date
    Dim vbUnit As String
    vbUnit = GetVBDateTimeUnit(unitType, unit)
    AddDateTimeBasedOnUnits = DateAdd(vbUnit, unit, time)
End Function

Public Function GetDateDiffString(dateDiffCode As String) As String
    Dim units As New Dictionary
    
    units.Add Year_Code, "Years"
    units.Add Quarater_Code, "Quarters"
    units.Add Month_Code, "Months"
    units.Add Week_Code, "Weeks"
    units.Add Day_Code, "Days"
    units.Add Hour_Code, "Hours"
    units.Add Min_Code, "Minitues"
    units.Add Sec_Code, "Seconds"
    
    GetDateDiffString = units(dateDiffCode)
End Function

Public Function AdjustDateTimeOnFinanceUnit(dteTme As Date, unitType As String) As Date
    Dim adjDte As Date
    adjDte = dteTme
    Select Case unitType
        Case Min_Code
            adjDte = CDate(Format(dteTme, "MM/dd/yyyy HH:nn") & ":00 AM")
            
        Case Hour_Code
            adjDte = CDate(Format(dteTme, "MM/dd/yyyy HH") & ":00:00 AM")
            
        Case Day_Code, Week_Code, Month_Code, Quarater_Code, Year_Code
            adjDte = CDate(Format(dteTme, "MM/dd/yyyy") & " 00:00:00 AM")
    End Select
    AdjustDateTimeOnFinanceUnit = adjDte
End Function

Public Function GetFinanceUnitsToSecondsLookup() As Dictionary
    Dim minSec As Double: minSec = 60
    Dim hrSec As Double: hrSec = minSec * 60
    Dim daySec As Double: daySec = hrSec * 24
    Dim weekSec As Double: weekSec = daySec * 7
    Dim monSec As Double: monSec = daySec * 30
    Dim quaSec As Double: quaSec = monSec * 3
    Dim yrSec As Double: yrSec = daySec * 365
    
    Dim units As New Dictionary
    units.Add Year_Code, yrSec
    units.Add Quarater_Code, quaSec
    units.Add Month_Code, monSec
    units.Add Week_Code, weekSec
    units.Add Day_Code, daySec
    units.Add Hour_Code, hrSec
    units.Add Min_Code, minSec
    units.Add Sec_Code, 1
    
    Set GetFinanceUnitsToSecondsLookup = units
End Function

Public Function GetDateDiffInUnits(dateLarge As Date, dateSmall As Date) As String
    Dim units As New Dictionary
    Set units = GetFinanceUnitsToSecondsLookup()
    Dim val As Double
    Dim unt As Variant
    Dim quo As Long
    Dim dt1 As Date
    Dim dt2 As Date
    
    For Each unt In units.Keys
        dt1 = AdjustDateTimeOnFinanceUnit(dateSmall, CStr(unt))
        dt2 = AdjustDateTimeOnFinanceUnit(dateLarge, CStr(unt))
        val = DateDiff("s", dt2, dt1)
        quo = CLng(val \ units(unt))
        If quo <> 0 Then
            GetDateDiffInUnits = Abs(quo) & "+ " & unt
            Exit Function
        End If
    Next unt
End Function

Public Function GetTimeUnitToGo(tskTime As Date, STATUS As Boolean) As String
    If Not STATUS Then
        Dim diff As String
        diff = GetDateDiffInUnits(tskTime, DateTimeNow)
        If Len(diff) >= 2 Then
            diff = "~" & Left(diff, Len(diff) - 1) & " " & GetDateDiffString(Right(diff, 1))
        End If
        GetTimeUnitToGo = diff
    End If
End Function

Public Function GetNthDateTerm( _
startDate As Date, endDate As Date, dateToFit As Date, _
commonDiff As Long, typeOfDiff As String, Optional forceNPlus1 As Boolean = True, _
Optional ifReminderNPlus1 As Boolean = False) As Variant

    Dim nthTerm As Long
    Dim lastTerm As Long
    
    Dim n As Long
    
    nthTerm = DateDiff(typeOfDiff, startDate, dateToFit)
    
    If nthTerm < 0 Then
        GetNthDateTerm = Null
        Exit Function
    End If
    
    lastTerm = DateDiff(typeOfDiff, startDate, endDate)
    lastTerm = lastTerm \ commonDiff + 1

    n = nthTerm \ commonDiff + 1
    
    If forceNPlus1 Then
        n = n + 1
    Else
        If ifReminderNPlus1 And ((nthTerm Mod commonDiff) > 0) Then
            n = n + 1
        End If
    End If
    
    If n > lastTerm Then
        GetNthDateTerm = Null
        Exit Function
    End If
    
    Dim dteFitted As Date
    dteFitted = DateAdd(typeOfDiff, (n - 1) * commonDiff, startDate)
    If (Not forceNPlus1) And (Not ifReminderNPlus1) And (dteFitted > dateToFit) Then
        n = n - 1
    End If
    
    If n < 0 Then
        GetNthDateTerm = Null
    Else
        GetNthDateTerm = n
    End If
End Function

Public Function GetNthDate( _
startDate As Date, endDate As Date, dateToFit As Date, _
commonDiff As Long, typeOfDiff As String, ByRef datesCount As Variant, _
Optional forceNPlus1 As Boolean = True, Optional ifReminderNPlus1 As Boolean = False) As Variant
 
    Dim fittedTerm As Long
    
    datesCount = GetNthDateTerm(startDate, endDate, dateToFit, commonDiff, typeOfDiff, forceNPlus1, ifReminderNPlus1)
        
    If IsNull(datesCount) Then
        GetNthDate = Null
        Exit Function
    End If
    fittedTerm = (datesCount - 1) * commonDiff
    GetNthDate = DateAdd(typeOfDiff, fittedTerm, startDate)
End Function

Function GetStatusString(statusCode)
   
    Select Case statusCode
        Case "ND"
            GetStatusString = "No Delay"
        Case "TD"
            GetStatusString = "Today!"
        Case "AD"
            GetStatusString = "To Delay"
        Case "D"
            GetStatusString = "Delayed"
        Case "C"
            GetStatusString = "Completed"
        Case "NA"
            GetStatusString = "Disabled"
        Case Else
            GetStatusString = "Unknown"
    End Select

End Function

