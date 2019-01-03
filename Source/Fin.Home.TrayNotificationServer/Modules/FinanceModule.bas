Attribute VB_Name = "FinanceDB"

Option Explicit

Public Function FindPolicyDue(ByVal policyID As String, ByRef delayDate As String, ByRef pendingAmount As Double) As String
    Dim STATUS As String: STATUS = "ND" 'Not delayed
    pendingAmount = 0
    delayDate = Empty
    If Trim(policyID) <> Empty Then
        Dim cn As ADODB.Connection
        Set cn = GetConnection()
        Dim ds As ADODB.Recordset
        Set ds = OpenRecordSet(cn, "SELECT * FROM " & qryPolicyMaster() & " WHERE POL_ID = '" & policyID & "'")
        
        Dim polInterval As Long
        Dim polInitialAmt As Double
        Dim polPremiumAmt As Double
        Dim polDurationMonths As Long
        Dim polStartDate As Date
        Dim polOrgStartDate As Date
        Dim polEndDate As Date
        Dim polStatus As String
        
        Dim polAlertOn As Boolean
        Dim polSnooz As Long
        Dim polSnoozUnit As String
        Dim polCompleted As Boolean
        
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
        
        Dim netPaid As Double
        Dim lastDatePaid As Date
        
        Dim dsSub As ADODB.Recordset
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
        
        Dim amtTotalPolicy As Double
        amtTotalPolicy = polInitialAmt
        
        Dim n As Variant
        Dim dt As Variant
        
        If polInterval > 0 Then
            dt = GetNthDate(polStartDate, polEndDate, polEndDate, polInterval, "m", n, False, True)
            If Not IsNull(n) Then
                amtTotalPolicy = amtTotalPolicy + polPremiumAmt * IIf(n >= 1, n, 0)
            End If
        End If
        
        If netPaid >= amtTotalPolicy Then
            pendingAmount = amtTotalPolicy - netPaid
            FindPolicyDue = "C"
            Exit Function
        End If
        
        Dim curDt As Date
        curDt = CDate(Format(DateTimeNow, "MM/dd/yyyy"))
        
        Dim shouldExit As Boolean
        
        If curDt < polOrgStartDate Then
            pendingAmount = IIf(polInitialAmt > 0, polInitialAmt, polPremiumAmt) - netPaid
            delayDate = Format(polOrgStartDate, "mmm/dd/yyyy")
            shouldExit = True
        ElseIf curDt < polStartDate Then
            pendingAmount = polInitialAmt + IIf(polInterval > 0, polPremiumAmt, 0) - netPaid
            delayDate = Format(polStartDate, "mmm/dd/yyyy")
            shouldExit = True
        End If
        
        Dim dateTillNow As Date
        
        If shouldExit Then
            dateTillNow = CDate(delayDate)
            STATUS = GetScheduledTaskStatus(dateTillNow, polSnooz, polSnoozUnit, polCompleted, polAlertOn)
            FindPolicyDue = STATUS
            Exit Function
        End If
        
        Dim amtToPaid As Double
        
        amtToPaid = polInitialAmt
        dateTillNow = polStartDate
        
        If polInterval > 0 Then
            dt = GetNthDate(polStartDate, polEndDate, curDt, polInterval, "m", n, False, False)
            'Cumilate intervals till today
            If Not IsNull(n) Then
                amtToPaid = amtToPaid + polPremiumAmt * IIf(n >= 1, n, 0)
            End If
            
            If Not IsNull(dt) Then
                dateTillNow = dt
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

Public Function GetInvestmentStatus( _
tskTime As Variant, snooz As Variant, snoozeUnit As Variant, STATUS As Variant, alert As Variant) As String

    GetInvestmentStatus = "NA"
    
    Dim invTime As Date
    Dim invAlertOn As Boolean
    Dim invSnooz As Long
    Dim invSnoozUnit As String
    Dim invCompleted As Boolean
    Dim invStatus As String
    
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

Public Function GetScheduledTaskStatus( _
tskTime As Date, snooz As Long, snoozeUnit As Variant, STATUS As Boolean, alert As Boolean) As String
    
    Dim statRep As String
    Dim snoozeUnitStr As String
    
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
            Dim curDte As Date: curDte = DateTimeNow
            curDte = AdjustDateTimeOnFinanceUnit(curDte, GetFinanceDateTimeUnit(snoozeUnitStr))
            tskTime = AdjustDateTimeOnFinanceUnit(tskTime, GetFinanceDateTimeUnit(snoozeUnitStr))
            
            Dim ret As Long
            ret = DateDiff("s", tskTime, curDte)
            If ret = 0 Then
                statRep = "TD"
            ElseIf ret > 0 Then
                statRep = "D"
            Else
                Dim snoozeTime As Date
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

Function GetAlertString(cn As ADODB.Connection, qry As ADODB.Recordset, nameFld As String, dateFld As String, statusFld As String, alertFld As String) As String
    Dim rs  As ADODB.Recordset
    Dim val As String
    Dim cnt As Long
    
    Set rs = qry
    
    rs.Filter = _
    "(" & statusFld & "='D' AND " & alertFld & "<>0) or (" & statusFld & "='AD' AND " & alertFld & "<>0) or (" & statusFld & "='TD' AND " & alertFld & "<>0)"
    
    rs.Sort = dateFld & " ASC"
    
    Dim dt As String
    Dim STAT As String
    Dim BYT As String
    
    val = Empty
    cnt = 0
    If Not rs.EOF Then
        rs.MoveFirst
        While Not rs.EOF
            dt = Format(Nz(rs(dateFld).value, Empty), "MMM/dd/yyyy")
            STAT = GetStatusString(Nz(rs(statusFld).value, Empty))
            BYT = GetTimeUnitToGo(Nz(rs(dateFld).value, Empty), False)
            cnt = cnt + 1
            val = val & cnt & ". " & Nz(rs(nameFld).value, Empty) _
            & "   " & dt _
            & "   (" & STAT _
            & ":" & BYT & ")" _
            & vbCrLf
            rs.MoveNext
        Wend
        rs.Close
    End If
    GetAlertString = Trim(val)
End Function

Public Function GetAlerts(ByRef policy As String, ByRef investment As String, ByRef tasks As String) As Boolean
    Dim cn As ADODB.Connection
    Set cn = GetConnection()
    
    policy = Trim(GetAlertString(cn, qryPolicyDues(cn), "POLICY_NAME", "DUE_DATE", "POL_STAT", "POL_ALERT"))
    investment = Trim(GetAlertString(cn, qryInvestmentDetails(cn), "INVESTMENT_NAME", "END_DATE", "INV_STATUS", "INV_ALERT"))
    tasks = Trim(GetAlertString(cn, qryScheduledTaskStatus(cn), "TSK_NAME", "TSK_TIME", "STATUS", "TSK_ALERT"))
    
    cn.Close
    
    GetAlerts = policy <> Empty Or investment <> Empty Or tasks <> Empty
End Function

Function qryPolicyMaster() As String

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

Function qryPolicyTransactions() As String

qryPolicyTransactions = _
" (SELECT POLICY_TRANSACTION.POL_ID, POLICY_TRANSACTION.PAID_AMOUNT, " _
& "POLICY_TRANSACTION.PAID_DATE, POLICY_TRANSACTION.NEXT_DUE_DATE, " _
& "POLICY_TRANSACTION.REMARKS " _
& "FROM POLICY_TRANSACTION) qryPolicyTransactions " _

End Function

Function qryPolicyDues(cn As ADODB.Connection) As ADODB.Recordset

Dim sql As String
sql = _
"SHAPE { SELECT POL_ID, POL_NAME+' ('+POL_ID+')' AS POLICY_NAME, START_DATE, END_DATE, " _
& "POL_TOT_RETURN,STATUS, POL_ALERT " _
& "FROM POLICY_MASTER} APPEND NEW adChar(20) DUE_DATE, NEW adChar(20) DUE_PAY , NEW adChar(20) POL_STAT"

    Dim rs As ADODB.Recordset
    Set rs = OpenRecordSet(cn, sql)
    If rs.BOF Then
        Set qryPolicyDues = rs
        Exit Function
    End If
    While (Not rs.EOF)
             
        Dim dt As String
        Dim amt As Double
        Dim STAT: STAT = FindPolicyDue(rs.Fields("POL_ID").value, dt, amt)
        
        Dim dte As String
        dte = Trim(dt)
    
        If IsNull(dte) Or dte = Empty Then
            rs.Fields("DUE_DATE").value = Null
        Else
            rs.Fields("DUE_DATE").value = CDate(dt)
        End If
        
        rs.Fields("DUE_PAY").value = CStr(amt)
        rs.Fields("POL_STAT").value = STAT
        rs.Update
        rs.MoveNext
    Wend
    rs.MoveFirst
    Set qryPolicyDues = rs
End Function

Function qryInvestmentDetails(cn As ADODB.Connection) As ADODB.Recordset

Dim sql As String
sql = _
"SHAPE {SELECT ' ('+INV_ID+') '+INV_NAME AS INVESTMENT_NAME, START_DATE, END_DATE, " _
& "INV_TOTAL_RETURN, INTEREST_RATE, INV_DURATION_MONTHS, INV_AMOUNT, " _
& "INV_SNOOZE,INV_SNOOZE_UNIT,STATUS,INV_ALERT " _
& "FROM INVESTMENT_MASTER " _
& "WHERE STATUS='ACTIVE'} APPEND NEW adChar(20) INV_STATUS"

    Dim rs As ADODB.Recordset
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

Function qryScheduledTaskStatus(cn As ADODB.Connection) As ADODB.Recordset

Dim sql As String
sql = _
"SHAPE {SELECT " _
& "TASK_SCHEDULER.TSK_NAME, TASK_SCHEDULER.TSK_DESC, TASK_SCHEDULER.TSK_TIME, " _
& "TASK_SCHEDULER.TSK_SNOOZE, TASK_SCHEDULER.TSK_SNOOZE_UNIT, " _
& "TASK_SCHEDULER.TSK_COMPLETED, TASK_SCHEDULER.TSK_RECURRING_NEXT, " _
& "TASK_SCHEDULER.TSK_RECURRING_UNIT, TASK_SCHEDULER.TSK_ALERT " _
& "FROM TASK_SCHEDULER} APPEND NEW adChar(20) STATUS"

    Dim rs As ADODB.Recordset
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



