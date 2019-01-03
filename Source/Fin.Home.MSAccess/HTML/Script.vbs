Option Explicit

'Constants Date Specific
Const Year_Code = "Y"
Const Quarater_Code = "Q"
Const Month_Code = "M"
Const Week_Code = "W"
Const Day_Code = "D"
Const Hour_Code = "H"
Const Min_Code = "N"
Const Sec_Code = "S"

Function GetConnection()
    Dim cn
    Set cn = CreateObject("ADODB.Connection")
    cn.Open "Provider=MSDATASHAPE;Data Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" & GetDBPath()
    Set GetConnection = cn
End Function

Function OpenRecordSet(cn, sql)
    Dim rs
    Set rs = CreateObject("ADODB.Recordset")
    rs.Open sql, cn, 2, 3, 1
    Set OpenRecordSet = rs
End Function

Function DateTimeNow()
    DateTimeNow = Now
End Function

Function Format(dateVal,formatTag)
    If IsNull(dateVal) Or Trim(dateVal) = Empty Then
        Format = "N/A"
        Exit Function
    End If
    dim rep
    rep = formatTag
    rep = Replace(rep,"mmm",left(MonthName(Month(dateVal)),3))
    rep = Replace(rep,"MMM",ucase(left(MonthName(Month(dateVal)),3)))
    rep = Replace(rep,"MM",Month(dateVal))
    rep = Replace(rep,"dd",Day(dateVal))
    rep = Replace(rep,"yyyy",Year(dateVal))
    rep = Replace(rep,"HH",Hour(dateVal))
    rep = Replace(rep,"nn",Second(dateVal))
        
    If Hour(dateVal) >= 12 Then
        rep = Replace(rep,"AM","PM")
    Else
        rep = Replace(rep,"PM","AM")
    End If 

    Format = rep
End Function

Function Nz(value, defaultValue)
    If IsNull(value) Then
        Nz = Trim(defaultValue)
    Else
        Nz = Trim(value)
    End If
End Function

Function IIf(test,trueVal,falseFal)
    If test = true Then
        IIf = trueVal
    Else
        IIf = falseFal
    End If
End Function

Function GetVBDateTimeUnit(unitType, ByRef unit)
    Dim vbUnit

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

Function GetFinanceDateTimeUnit(unitType)
    Dim vbUnit

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

Function AddDateTimeBasedOnUnits(unit, unitType, time)
    Dim vbUnit
    vbUnit = GetVBDateTimeUnit(unitType, unit)
    AddDateTimeBasedOnUnits = DateAdd(vbUnit, unit, time)
End Function

Function GetDateDiffString(dateDiffCode)
    Dim units
    Set units = CreateObject("Scripting.Dictionary")
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

Function AdjustDateTimeOnFinanceUnit(dteTme, unitType)
    Dim adjDte
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

Function GetFinanceUnitsToSecondsLookup()
    Dim minSec: minSec = 60
    Dim hrSec: hrSec = minSec * 60
    Dim daySec: daySec = hrSec * 24
    Dim weekSec: weekSec = daySec * 7
    Dim monSec: monSec = daySec * 30
    Dim quaSec: quaSec = monSec * 3
    Dim yrSec: yrSec = daySec * 365
    
    Dim units
    
    Set units = CreateObject("Scripting.Dictionary")
    
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

Function GetDateDiffInUnits(dateLarge, dateSmall)
    Dim units
    Set units = CreateObject("Scripting.Dictionary")
    
    Set units = GetFinanceUnitsToSecondsLookup()
    Dim val
    Dim unt
    Dim quo
    Dim dt1
    Dim dt2
    
    For Each unt In units.Keys
        dt1 = AdjustDateTimeOnFinanceUnit(dateSmall, CStr(unt))
        dt2 = AdjustDateTimeOnFinanceUnit(dateLarge, CStr(unt))
        val = DateDiff("s", dt2, dt1)
        quo = CLng(val \ units(unt))
        If quo <> 0 Then
            GetDateDiffInUnits = Abs(quo) & "+ " & unt
            Exit Function
        End If
    Next
End Function

Function GetTimeUnitToGo(tskTime, STATUS)
    If Not STATUS Then
        Dim diff
        diff = GetDateDiffInUnits(tskTime, DateTimeNow)
        If Len(diff) >= 2 Then
            diff = "~" & Left(diff, Len(diff) - 1) & " " & GetDateDiffString(Right(diff, 1))
        End If
        GetTimeUnitToGo = diff
    End If
End Function

Function GetNthDateTerm( _
startDate, endDate, dateToFit, _
commonDiff, typeOfDiff, forceNPlus1, _
ifReminderNPlus1)

    Dim nthTerm
    Dim lastTerm
    
    Dim n
    
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
    
    Dim dteFitted
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

Function GetNthDate( _
startDate, endDate, dateToFit, _
commonDiff, typeOfDiff, ByRef datesCount, _
forceNPlus1, ifReminderNPlus1)
 
    Dim fittedTerm
    
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

Function FindPolicyDue(ByVal policyID, ByRef delayDate, ByRef pendingAmount)
    Dim STATUS: STATUS = "ND"  'Not delayed
    pendingAmount = 0
    delayDate = Empty
    If Trim(policyID) <> Empty Then
        Dim cn
        Set cn = GetConnection()
        Dim ds
        Set ds = OpenRecordSet(cn, "SELECT * FROM " & qryPolicyMaster() & " WHERE POL_ID = '" & policyID & "'")
        
        Dim polInterval
        Dim polInitialAmt
        Dim polPremiumAmt
        Dim polDurationMonths
        Dim polStartDate
        Dim polOrgStartDate
        Dim polEndDate
        Dim polStatus
        
        Dim polAlertOn
        Dim polSnooz
        Dim polSnoozUnit
        Dim polCompleted
        
        If Not ds.EOF Then
            polInterval = ds.Fields("POL_INTERVAL_MONTHS").value
            polInitialAmt = ds.Fields("INITIAL_AMOUNT").value
            polPremiumAmt = ds.Fields("PREMIUM_AMOUNT").value
            polDurationMonths = ds.Fields("POL_DURATION_MONTHS").value
            polStartDate = ds.Fields("START_DATE").value
            polOrgStartDate = polStartDate
            polEndDate = ds.Fields("END_DATE").value
            polStatus = Nz(ds.Fields("STATUS").value, Empty)
            
            polAlertOn = Nz(ds.Fields("POL_ALERT").value, False)
            polSnooz = Nz(ds.Fields("POL_SNOOZE").value, 0)
            polSnoozUnit = Nz(ds.Fields("POL_SNOOZE_UNIT").value, Empty)
            polCompleted = Not (Trim(UCase(polStatus)) = "ACTIVE")
        End If
        
        If polInterval > 0 And polInitialAmt > 0 Then
            polStartDate = DateAdd("m", polInterval, polStartDate)
        End If
        
        If LCase(Trim(polStatus)) <> "active" Then
            FindPolicyDue = LCase(Trim(polStatus))
            cn.Close
            Exit Function
        End If
        
        Dim netPaid
        Dim lastDatePaid
        
        Dim dsSub
        Set dsSub = OpenRecordSet(cn, "SELECT PAID_DATE FROM " & qryPolicyTransactions() & " WHERE POL_ID = '" & policyID & "' ORDER BY PAID_DATE DESC")
        'There is no payment made
        If Not dsSub.EOF Then
            lastDatePaid = dsSub.Fields("PAID_DATE").value
            
            Set dsSub = OpenRecordSet(cn, "SELECT SUM(PAID_AMOUNT) AS SUM_AMT FROM " & qryPolicyTransactions() & " WHERE POL_ID = '" & policyID & "'")
            netPaid = dsSub.Fields("SUM_AMT").value
        Else 'This is not the first payment
            netPaid = 0
        End If
        
        cn.Close
        
        Dim amtTotalPolicy
        amtTotalPolicy = polInitialAmt
        
        Dim n
        Dim DT
        
        If polInterval > 0 Then
            DT = GetNthDate(polStartDate, polEndDate, polEndDate, polInterval, "m", n, False, True)
            If Not IsNull(n) Then
                amtTotalPolicy = amtTotalPolicy + polPremiumAmt * IIf(n >= 1, n, 0)
            End If
        End If
        
        If netPaid >= amtTotalPolicy Then
            pendingAmount = amtTotalPolicy - netPaid
            FindPolicyDue = "C"
            Exit Function
        End If
        
        Dim curDt
        curDt = CDate(Format(DateTimeNow, "MM/dd/yyyy"))
        
        Dim shouldExit
        
        If curDt < polOrgStartDate Then
            pendingAmount = IIf(polInitialAmt > 0, polInitialAmt, polPremiumAmt) - netPaid
            delayDate = Format(polOrgStartDate, "mmm/dd/yyyy")
            shouldExit = True
        ElseIf curDt < polStartDate Then
            pendingAmount = polInitialAmt + IIf(polInterval > 0, polPremiumAmt, 0) - netPaid
            delayDate = Format(polStartDate, "mmm/dd/yyyy")
            shouldExit = True
        End If
        
        Dim dateTillNow
        
        If shouldExit Then
            dateTillNow = CDate(delayDate)
            STATUS = GetScheduledTaskStatus(dateTillNow, polSnooz, polSnoozUnit, polCompleted, polAlertOn)
            FindPolicyDue = STATUS
            Exit Function
        End If
        
        Dim amtToPaid
        
        amtToPaid = polInitialAmt
        dateTillNow = polStartDate
        
        If polInterval > 0 Then
            DT = GetNthDate(polStartDate, polEndDate, curDt, polInterval, "m", n, False, False)
            'Cumilate intervals till today
            If Not IsNull(n) Then
                amtToPaid = amtToPaid + polPremiumAmt * IIf(n >= 1, n, 0)
            End If
            
            If Not IsNull(DT) Then
                dateTillNow = DT
            Else
                dateTillNow = polEndDate
            End If
        End If
        
        'If we have additional amout to pay?
        If netPaid < amtToPaid Then
            STATUS = "D" 'Delayed
            delayDate = Format(dateTillNow, "mmm/dd/yyyy")
            pendingAmount = amtToPaid - netPaid
        ElseIf polInterval <= 0 Then
            pendingAmount = amtTotalPolicy - netPaid
            If pendingAmount <= 0 Then
                STATUS = "C"
            Else
                delayDate = Format(dateTillNow, "mmm/dd/yyyy")
                STATUS = "D"
            End If
        Else
            STATUS = "ND" 'Not Delayed
            If dateTillNow < curDt Then
                amtToPaid = amtToPaid + polPremiumAmt
                dateTillNow = DateAdd("m", polInterval, dateTillNow)
                If dateTillNow > polEndDate Then
                    dateTillNow = polEndDate
                End If
            End If
            
            pendingAmount = amtToPaid - netPaid
            
            If pendingAmount > 0 Then
                delayDate = Format(dateTillNow, "mmm/dd/yyyy")
                If DateDiff("d", dateTillNow, curDt) = 0 Then
                    STATUS = "TD"
                ElseIf DateDiff("d", dateTillNow, curDt) > 0 Then
                    STATUS = "D"
                Else
                    STATUS = GetScheduledTaskStatus(dateTillNow, polSnooz, polSnoozUnit, polCompleted, polAlertOn)
                End If
            Else
                Dim quotient
                quotient = Abs(pendingAmount) \ polPremiumAmt + 1
                dateTillNow = DateAdd("m", polInterval * quotient, dateTillNow)
                If dateTillNow > polEndDate Then
                    dateTillNow = polEndDate
                End If
                delayDate = Format(dateTillNow, "mmm/dd/yyyy")
                pendingAmount = IIf(netPaid + polPremiumAmt <= amtTotalPolicy, polPremiumAmt, amtTotalPolicy - netPaid)
            End If
        End If
    End If
    FindPolicyDue = STATUS
End Function

Function GetInvestmentStatus( _
tskTime, snooz, snoozeUnit, STATUS, alert)

    GetInvestmentStatus = "NA"
    
    Dim invTime
    Dim invAlertOn
    Dim invSnooz
    Dim invSnoozUnit
    Dim invCompleted
    Dim invStatus
    
    invStatus = Nz(STATUS, Empty)
    
    Select Case Trim(UCase(invStatus))
        Case "COMPLETED"
            GetInvestmentStatus = "C"
            Exit Function
            
        Case "NOT ACTIVE"
            GetInvestmentStatus = "NA"
            Exit Function
            
    End Select
    
    If IsNull(tskTime) Then
        Exit Function
    End If
    invTime = tskTime
    
    invSnooz = Nz(snooz, 0)
    If invSnooz <= 0 Then
        Exit Function
    End If
    invAlertOn = Nz(alert, False)
    If Not invAlertOn Then
        Exit Function
    End If
    
    invSnoozUnit = Nz(snoozeUnit, Empty)
    invCompleted = Not (Trim(UCase(invStatus)) = "ACTIVE")
    
    GetInvestmentStatus = GetScheduledTaskStatus(invTime, invSnooz, invSnoozUnit, invCompleted, invAlertOn)
End Function

Function GetScheduledTaskStatus( _
tskTime, snooz, snoozeUnit, STATUS, alert)
    
    Dim statRep
    Dim snoozeUnitStr
    
    statRep = "ND"
    
    snooz = IIf(IsNull(snooz), 0, snooz)
    
    If IsNull(snoozeUnit) Then
        snoozeUnitStr = Empty
    Else
        snoozeUnitStr = CStr(snoozeUnit)
    End If
    
    If Not alert Then
        statRep = "NA"
    Else
        If STATUS Then
             statRep = "C"
        Else
            Dim curDte: curDte = DateTimeNow
            curDte = AdjustDateTimeOnFinanceUnit(curDte, GetFinanceDateTimeUnit(snoozeUnitStr))
            tskTime = AdjustDateTimeOnFinanceUnit(tskTime, GetFinanceDateTimeUnit(snoozeUnitStr))
            
            Dim ret
            ret = DateDiff("s", tskTime, curDte)
            If ret = 0 Then
                statRep = "TD"
            ElseIf ret > 0 Then
                statRep = "D"
            Else
                Dim snoozeTime
                snoozeTime = AddDateTimeBasedOnUnits(-snooz, snoozeUnitStr, tskTime)
                ret = DateDiff("s", snoozeTime, curDte)
                If ret >= 0 Then
                    statRep = "AD"
                End If
            End If
        End If
    End If

    GetScheduledTaskStatus = statRep
End Function

Function GetAlertString(cn, qry, nameFld, dateFld, statusFld, alertFld)
    Dim rs
    Dim val
    Dim cnt
    
    Set rs = qry
    
    rs.Filter = _
    "(" & statusFld & "='D' AND " & alertFld & "<>0) or (" & statusFld & "='AD' AND " & alertFld & "<>0) or (" & statusFld & "='TD' AND " & alertFld & "<>0)"
    
    rs.Sort = dateFld & " ASC"
    
    Dim DT
    Dim STAT
    Dim BYT
    
    val = Empty
    cnt = 0
    If Not rs.EOF Then
        rs.MoveFirst
        While Not rs.EOF
            DT = Format(Nz(rs(dateFld).Value, Empty), "MMM/dd/yyyy")
            STAT = GetStatusString(Nz(rs(statusFld).Value, Empty))
            BYT = GetTimeUnitToGo(Nz(rs(dateFld).Value, Empty), False)
            cnt = cnt + 1
            val = val & cnt & ". " & Nz(rs(nameFld).Value, Empty) _
            & "   " & DT _
            & "   (" & STAT _
            & ":" & BYT & ")" _
            & "<br/>"
            rs.MoveNext
        Wend
        rs.Close
    End If
    GetAlertString = Trim(val)
End Function

Sub SetAlerts()
    Dim pol
    Dim inv
    Dim task
    Dim stat
    
    dim uiItem
    
    set uiItem = GetItem("divAlerts")
    uiItem.innerHTML = Empty
    
    stat = GetAlerts(pol, inv, task)
    
    if stat = True then
        dim str
        'str = "<b>Alerts!</b><br/>"
        if pol <> Empty then
            str = str & "<br/><b>Policies</b><br/>"
            str = str & pol
        end if
        if inv <> Empty then
            str = str & "<br/><b>Investments</b><br/>"
            str = str & inv
        end if
        if task <> Empty then
            str = str & "<br/><b>Tasks</b><br/>"
            str = str & task
        end if
        uiItem.innerHTML = str
    end if
    
End Sub

Function GetAlerts(ByRef policy, ByRef investment, ByRef tasks)
    Dim cn
    Set cn = GetConnection()
    
    policy = Trim(GetAlertString(cn, qryPolicyDues(cn), "POLICY_NAME", "DUE_DATE", "POL_STAT", "POL_ALERT"))
    investment = Trim(GetAlertString(cn, qryInvestmentDetails(cn), "INVESTMENT_NAME", "END_DATE", "INV_STATUS", "INV_ALERT"))
    tasks = Trim(GetAlertString(cn, qryScheduledTaskStatus(cn), "TSK_NAME", "TSK_TIME", "STATUS", "TSK_ALERT"))
    
    cn.Close
    
    GetAlerts = policy <> Empty Or investment <> Empty Or tasks <> Empty
End Function

Function qryPolicyMaster()

qryPolicyMaster = _
" (SELECT POLICY_MASTER.POL_NAME, POLICY_MASTER.POL_ID," _
& "POLICY_MASTER.POL_REMARKS, POLICY_MASTER.POL_INTERVAL_MONTHS," _
& "POLICY_MASTER.INITIAL_AMOUNT, POLICY_MASTER.PREMIUM_AMOUNT," _
& "POLICY_MASTER.POL_TOT_RETURN, POLICY_MASTER.POL_DURATION_MONTHS," _
& "POLICY_MASTER.START_DATE, POLICY_MASTER.END_DATE, POLICY_MASTER.STATUS," _
& "POLICY_MASTER.POL_SNOOZE, POLICY_MASTER.POL_SNOOZE_UNIT," _
& "POLICY_MASTER.POL_ALERT " _
& "FROM POLICY_MASTER) qryPolicyMaster "

End Function

Function qryPolicyTransactions()

qryPolicyTransactions = _
" (SELECT POLICY_TRANSACTION.POL_ID, POLICY_TRANSACTION.PAID_AMOUNT, " _
& "POLICY_TRANSACTION.PAID_DATE, POLICY_TRANSACTION.NEXT_DUE_DATE, " _
& "POLICY_TRANSACTION.REMARKS " _
& "FROM POLICY_TRANSACTION) qryPolicyTransactions " _

End Function

Function qryPolicyDues(cn)

Dim sql
sql = _
"SHAPE { SELECT POL_ID, POL_NAME+' ('+POL_ID+')' AS POLICY_NAME, START_DATE, END_DATE, " _
& "POL_TOT_RETURN,STATUS, POL_ALERT " _
& "FROM POLICY_MASTER} APPEND NEW adChar(20) DUE_DATE, NEW adChar(20) DUE_PAY , NEW adChar(20) POL_STAT"

    Dim rs
    Set rs = OpenRecordSet(cn, sql)
    If rs.BOF Then
        Set qryPolicyDues = rs
        Exit Function
    End If
    While (Not rs.EOF)        
             
        Dim DT
        Dim amt
        Dim STAT : STAT = FindPolicyDue(rs.Fields("POL_ID").value, DT, amt)
        
        Dim dte  
        dte = Trim(DT)
    
        If IsNull(dte) Or dte = Empty Then
            rs.Fields("DUE_DATE").value = null
        Else
            rs.Fields("DUE_DATE").value = CDate(DT)
        End If        
        
        rs.Fields("DUE_PAY").value = CStr(amt)
        rs.Fields("POL_STAT").value = STAT
        rs.Update
        rs.MoveNext
    Wend
    rs.MoveFirst
    Set qryPolicyDues = rs
End Function

Function qryInvestmentDetails(cn)

Dim sql
sql = _
"SHAPE {SELECT ' ('+INV_ID+') '+INV_NAME AS INVESTMENT_NAME, START_DATE, END_DATE, " _
& "INV_TOTAL_RETURN, INTEREST_RATE, INV_DURATION_MONTHS, INV_AMOUNT, " _
& "INV_SNOOZE,INV_SNOOZE_UNIT,STATUS,INV_ALERT " _
& "FROM INVESTMENT_MASTER " _
& "WHERE STATUS='ACTIVE'} APPEND NEW adChar(20) INV_STATUS"

    Dim rs
    Set rs = OpenRecordSet(cn, sql)
    If rs.BOF Then
        Set qryInvestmentDetails = rs
        Exit Function
    End If
    While (Not rs.EOF)
        rs.Fields("INV_STATUS").value = GetInvestmentStatus( _
        rs.Fields("END_DATE").value, _
        rs.Fields("INV_SNOOZE").value, _
        rs.Fields("INV_SNOOZE_UNIT").value, _
        rs.Fields("STATUS").value, _
        rs.Fields("INV_ALERT").value)
        
        rs.Update
        rs.MoveNext
    Wend
    rs.MoveFirst
    Set qryInvestmentDetails = rs
End Function

Function qryScheduledTaskStatus(cn)

Dim sql
sql = _
"SHAPE {SELECT " _
& "TASK_SCHEDULER.TSK_NAME, TASK_SCHEDULER.TSK_DESC, TASK_SCHEDULER.TSK_TIME, " _
& "TASK_SCHEDULER.TSK_SNOOZE, TASK_SCHEDULER.TSK_SNOOZE_UNIT, " _
& "TASK_SCHEDULER.TSK_COMPLETED, TASK_SCHEDULER.TSK_RECURRING_NEXT, " _
& "TASK_SCHEDULER.TSK_RECURRING_UNIT, TASK_SCHEDULER.TSK_ALERT " _
& "FROM TASK_SCHEDULER} APPEND NEW adChar(20) STATUS"

    Dim rs
    Set rs = OpenRecordSet(cn, sql)
    If rs.BOF Then
        Set qryScheduledTaskStatus = rs
        Exit Function
    End If
    While (Not rs.EOF)
        rs.Fields("STATUS").value = GetScheduledTaskStatus( _
        rs.Fields("TSK_TIME").value, _
        rs.Fields("TSK_SNOOZE").value, _
        rs.Fields("TSK_SNOOZE_UNIT").value, _
        rs.Fields("TSK_COMPLETED").value, _
        rs.Fields("TSK_ALERT").value)
        
        rs.Update
        rs.MoveNext
    Wend
    rs.MoveFirst
    Set qryScheduledTaskStatus = rs
End Function

