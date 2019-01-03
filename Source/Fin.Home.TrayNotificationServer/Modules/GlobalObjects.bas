Attribute VB_Name = "GlobalObjects"
Option Explicit

'Constants Date Specific
Public Const Year_Code As String = "Y"
Public Const Quarater_Code As String = "Q"
Public Const Month_Code As String = "M"
Public Const Week_Code As String = "W"
Public Const Day_Code As String = "D"
Public Const Hour_Code As String = "H"
Public Const Min_Code As String = "N"
Public Const Sec_Code As String = "S"

Public m_gFinanceServer As FinanceServer

Function GetDBPath() As String
    GetDBPath = app.Path & "\Finance@Home_DB.mdb"
End Function

Function GetFrontEndPath() As String
    GetFrontEndPath = app.Path & "\Finance@Home.mdb"
End Function

Function GetConnection() As ADODB.connection
    Dim cn As New ADODB.connection
    cn.Open "Provider=MSDATASHAPE;Data Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" & GetDBPath()
    Set GetConnection = cn
End Function

Function DateTimeNow() As Date
    DateTimeNow = Now
End Function

Public Function GetFinanceServer() As FinanceServer
    If m_gFinanceServer Is Nothing Then
        Set m_gFinanceServer = New FinanceServer
    End If
    Set GetFinanceServer = m_gFinanceServer
End Function

