Attribute VB_Name = "DebitsCredits"

Public Type DebitCreditRecord
    m_ItemKey As Long
    m_Date As Date
    m_ItemDesc As String
    m_Amount As Double
    m_IsDebit As Boolean
    m_CategoryCode As String
    m_OneTime As Boolean
    m_BookMark As Boolean
End Type

Public Function qryGetAllCategories() As ADODB.Recordset
    Dim cn As ADODB.connection
    Set cn = GetConnection
    
    Dim sql As String
    sql = "SELECT CAT_CODE, CAT_NAME FROM CATEGORY_MASTER ORDER BY CAT_NAME"

    Set qryGetAllCategories = OpenRecordSet(cn, sql)
End Function

Public Function qryGetAllCreditsDebits() As ADODB.Recordset
   Dim cn As ADODB.connection
    Set cn = GetConnection
    
    Dim sql As String
    sql = _
    "SELECT DEBITS_CREDITS.TIME_STAMP," & _
    "DATE,TRANS_DESC,AMOUNT,IS_DEBIT,CATEGORY_CODE,DEBITS_CREDITS_BOOKMARK.IS_MARKED," & _
    "DEBITS_CREDITS_ONETIME.IS_ONETIME " & _
    "FROM (DEBITS_CREDITS LEFT JOIN DEBITS_CREDITS_BOOKMARK ON DEBITS_CREDITS_BOOKMARK.TIME_STAMP=DEBITS_CREDITS.TIME_STAMP) LEFT JOIN DEBITS_CREDITS_ONETIME ON DEBITS_CREDITS_ONETIME.TIME_STAMP=DEBITS_CREDITS.TIME_STAMP"

    Set qryGetAllCreditsDebits = OpenRecordSet(cn, sql)
End Function


Function UpdateOneTimeAndBookMark(item As DebitCreditRecord, connection As ADODB.connection)
    Dim cmd As String
    cmd = "DELETE FROM DEBITS_CREDITS_BOOKMARK WHERE TIME_STAMP=" & item.m_ItemKey
    Call OpenCommand(connection, cmd)

    cmd = "DELETE FROM DEBITS_CREDITS_ONETIME WHERE TIME_STAMP=" & item.m_ItemKey
    Call OpenCommand(connection, cmd)

    If (item.m_OneTime = True) Then
        cmd = "INSERT INTO DEBITS_CREDITS_ONETIME(TIME_STAMP,IS_ONETIME) VALUES(" & item.m_ItemKey & ",true)"
        Call OpenCommand(connection, cmd)
    End If

    If (item.m_BookMark = True) Then
        cmd = "INSERT INTO DEBITS_CREDITS_BOOKMARK(TIME_STAMP,IS_MARKED) VALUES(" & item.m_ItemKey & ",true)"
        Call OpenCommand(connection, cmd)
    End If
End Function

Function Add(item As DebitCreditRecord)
On Error GoTo XIT
    Dim con As ADODB.connection
    Set con = OpenConnection()
    
    con.BeginTrans
    
    Dim cmd As String
    
    cmd = "INSERT INTO DEBITS_CREDITS([DATE],TRANS_DESC,AMOUNT,IS_DEBIT,TIME_STAMP,CATEGORY_CODE) " & _
        "VALUES(#" & Format(item.m_Date, "MM/dd/yyyy") & "#" & _
        ",'" & item.m_ItemDesc & "'," & item.m_Amount & "," & item.m_IsDebit & "," & _
        item.m_ItemKey & ",'" & item.m_CategoryCode & "')"

        Call OpenCommand(con, cmd)
        Call UpdateOneTimeAndBookMark(item, con)
    
    con.CommitTrans
    Call CloseConnection(con)
    Exit Function
    
XIT:
    MsgBox Err.Description
    con.RollbackTrans
    Call CloseConnection(con)
End Function

Function Update(item As DebitCreditRecord)
On Error GoTo XIT
    Dim con As ADODB.connection
    Set con = OpenConnection()
    
    con.BeginTrans
    
    Dim cmd As String
    
    cmd = "UPDATE DEBITS_CREDITS SET " & _
        "[DATE] = #" & Format(item.m_Date, "MM/dd/yyyy") & "#, " & _
        "TRANS_DESC = '" & item.m_ItemDesc & "', AMOUNT = " & item.m_Amount & ", IS_DEBIT=" & item.m_IsDebit & ", " & _
         "CATEGORY_CODE = '" & item.m_CategoryCode & "' WHERE TIME_STAMP=" & item.m_ItemKey

        Call OpenCommand(con, cmd)
        Call UpdateOneTimeAndBookMark(item, con)
   
    con.CommitTrans
    Call CloseConnection(con)
    Exit Function
    
XIT:
    MsgBox Err.Description
    con.RollbackTrans
    Call CloseConnection(con)
End Function

Function Delete(itemKey As String)
On Error GoTo XIT
    Dim con As ADODB.connection
    Set con = OpenConnection()
    
    con.BeginTrans
    
    Dim cmd As String
    
        cmd = "DELETE FROM DEBITS_CREDITS_BOOKMARK WHERE TIME_STAMP=" & itemKey
        Call OpenCommand(con, cmd)

        cmd = "DELETE FROM DEBITS_CREDITS_ONETIME WHERE TIME_STAMP=" & itemKey
        Call OpenCommand(con, cmd)

        cmd = "DELETE FROM DEBITS_CREDITS WHERE TIME_STAMP=" & itemKey
        Call OpenCommand(con, cmd)
        
    con.CommitTrans
    Call CloseConnection(con)
    Exit Function
    
XIT:
    MsgBox Err.Description
    con.RollbackTrans
    Call CloseConnection(con)
End Function


