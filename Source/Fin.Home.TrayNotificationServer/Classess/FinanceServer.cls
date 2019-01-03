VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "FinanceServer"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True

Function GetAlerts(ByRef policy As String, ByRef investment As String, ByRef tasks As String) As Boolean
    GetAlerts = FinanceDB.GetAlerts(policy, investment, tasks)
End Function

Function GetConnection() As ADODB.connection
    Set GetConnection = GlobalObjects.GetConnection()
End Function

Function qryScheduledTaskStatus(cn As ADODB.connection) As ADODB.Recordset
    Set qryScheduledTaskStatus = FinanceDB.qryScheduledTaskStatus(cn)
End Function

Function qryInvestmentDetails(cn As ADODB.connection) As ADODB.Recordset
    Set qryInvestmentDetails = FinanceDB.qryInvestmentDetails(cn)
End Function

Function qryPolicyDues(cn As ADODB.connection) As ADODB.Recordset
    Set qryPolicyDues = FinanceDB.qryPolicyDues(cn)
End Function

Function GetFinanceHomeApp(ByRef app As String, ByRef params As String) As Boolean
    app = "MSACCESS.EXE"
    params = "/nostartup """ & GlobalObjects.GetFrontEndPath() & """"
    GetFinanceHomeApp = True
End Function

Function GetFinanceDBPath(ByRef dbPath As String) As Boolean
    dbPath = GetDBPath()
    GetFinanceDBPath = True
End Function
