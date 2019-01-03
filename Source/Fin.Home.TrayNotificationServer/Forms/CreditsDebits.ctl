VERSION 5.00
Object = "{CDE57A40-8B86-11D0-B3C6-00A0C90AEA82}#1.0#0"; "MSDATGRD.OCX"
Object = "{8E27C92E-1264-101C-8A2F-040224009C02}#7.0#0"; "MSCAL.OCX"
Begin VB.UserControl CreditsDebits 
   BackStyle       =   0  'Transparent
   ClientHeight    =   6420
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   8595
   BeginProperty Font 
      Name            =   "Tahoma"
      Size            =   8.25
      Charset         =   0
      Weight          =   400
      Underline       =   0   'False
      Italic          =   0   'False
      Strikethrough   =   0   'False
   EndProperty
   ScaleHeight     =   6420
   ScaleWidth      =   8595
   Begin VB.CommandButton cmdDelete 
      Caption         =   "Delete"
      Height          =   330
      Left            =   6840
      TabIndex        =   5
      Top             =   2160
      Width           =   1635
   End
   Begin VB.TextBox txtKey 
      Height          =   285
      Left            =   6975
      TabIndex        =   17
      Top             =   180
      Visible         =   0   'False
      Width           =   420
   End
   Begin VB.CommandButton cmdSave 
      Caption         =   "Save"
      Height          =   330
      Left            =   5130
      TabIndex        =   4
      Top             =   2160
      Width           =   1635
   End
   Begin VB.TextBox txtCat 
      Height          =   285
      Left            =   6525
      TabIndex        =   16
      Top             =   180
      Visible         =   0   'False
      Width           =   285
   End
   Begin VB.CheckBox chkOnetime 
      Caption         =   "Onetime?"
      DataField       =   "IS_ONETIME"
      DataMember      =   "IS_ONETIME"
      DataSource      =   "adodcDebitCredits"
      Height          =   285
      Left            =   5490
      TabIndex        =   7
      Top             =   1755
      Width           =   1185
   End
   Begin VB.CheckBox chkMarked 
      Caption         =   "Marked?"
      DataField       =   "IS_MARKED"
      DataMember      =   "IS_MARKED"
      DataSource      =   "adodcDebitCredits"
      Height          =   285
      Left            =   4410
      TabIndex        =   6
      Top             =   1755
      Width           =   915
   End
   Begin VB.CheckBox chkDebit 
      Caption         =   "Debit?"
      DataField       =   "IS_DEBIT"
      BeginProperty DataFormat 
         Type            =   5
         Format          =   ""
         HaveTrueFalseNull=   1
         TrueValue       =   "True"
         FalseValue      =   "False"
         NullValue       =   ""
         FirstDayOfWeek  =   0
         FirstWeekOfYear =   0
         LCID            =   1033
         SubFormatType   =   7
      EndProperty
      DataMember      =   "IS_DEBIT"
      DataSource      =   "adodcDebitCredits"
      Height          =   285
      Left            =   3420
      TabIndex        =   3
      Top             =   1755
      Value           =   1  'Checked
      Width           =   825
   End
   Begin VB.TextBox txtAmount 
      DataField       =   "AMOUNT"
      BeginProperty DataFormat 
         Type            =   1
         Format          =   "0.00"
         HaveTrueFalseNull=   0
         FirstDayOfWeek  =   0
         FirstWeekOfYear =   0
         LCID            =   1033
         SubFormatType   =   1
      EndProperty
      DataMember      =   "AMOUNT"
      DataSource      =   "adodcDebitCredits"
      Height          =   285
      Left            =   4320
      MaxLength       =   10
      TabIndex        =   1
      Top             =   990
      Width           =   1995
   End
   Begin VB.TextBox txtDesc 
      DataField       =   "TRANS_DESC"
      DataMember      =   "TRANS_DESC"
      DataSource      =   "adodcDebitCredits"
      Height          =   285
      Left            =   4320
      MaxLength       =   255
      TabIndex        =   0
      Top             =   630
      Width           =   4110
   End
   Begin VB.Frame frmAddUpdate 
      Height          =   495
      Left            =   3360
      TabIndex        =   12
      Top             =   30
      Width           =   2925
      Begin VB.OptionButton chkAdd 
         Caption         =   "Edit"
         Height          =   255
         Index           =   1
         Left            =   1305
         TabIndex        =   9
         Top             =   180
         Width           =   675
      End
      Begin VB.OptionButton chkAdd 
         Caption         =   "Add"
         Height          =   255
         Index           =   0
         Left            =   90
         TabIndex        =   8
         Top             =   195
         Value           =   -1  'True
         Width           =   675
      End
   End
   Begin VB.ComboBox cboCategory 
      Height          =   315
      Left            =   4320
      Style           =   2  'Dropdown List
      TabIndex        =   2
      Top             =   1350
      Width           =   4140
   End
   Begin MSDataGridLib.DataGrid dgdCreditDebits 
      Height          =   3735
      Left            =   90
      TabIndex        =   11
      Top             =   2565
      Width           =   8415
      _ExtentX        =   14843
      _ExtentY        =   6588
      _Version        =   393216
      AllowUpdate     =   0   'False
      AllowArrows     =   -1  'True
      Enabled         =   -1  'True
      ColumnHeaders   =   -1  'True
      HeadLines       =   1
      RowHeight       =   16
      TabAction       =   2
      RowDividerStyle =   1
      FormatLocked    =   -1  'True
      BeginProperty HeadFont {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Tahoma"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Tahoma"
         Size            =   9
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ColumnCount     =   8
      BeginProperty Column00 
         DataField       =   "TIME_STAMP"
         Caption         =   "TIME_STAMP"
         BeginProperty DataFormat {6D835690-900B-11D0-9484-00A0C91110ED} 
            Type            =   1
            Format          =   "0"
            HaveTrueFalseNull=   0
            FirstDayOfWeek  =   0
            FirstWeekOfYear =   0
            LCID            =   1033
            SubFormatType   =   1
         EndProperty
      EndProperty
      BeginProperty Column01 
         DataField       =   "DATE"
         Caption         =   "DATE"
         BeginProperty DataFormat {6D835690-900B-11D0-9484-00A0C91110ED} 
            Type            =   1
            Format          =   "MM/dd/yyyy"
            HaveTrueFalseNull=   0
            FirstDayOfWeek  =   0
            FirstWeekOfYear =   0
            LCID            =   1033
            SubFormatType   =   3
         EndProperty
      EndProperty
      BeginProperty Column02 
         DataField       =   "TRANS_DESC"
         Caption         =   "Details"
         BeginProperty DataFormat {6D835690-900B-11D0-9484-00A0C91110ED} 
            Type            =   0
            Format          =   ""
            HaveTrueFalseNull=   0
            FirstDayOfWeek  =   0
            FirstWeekOfYear =   0
            LCID            =   1033
            SubFormatType   =   0
         EndProperty
      EndProperty
      BeginProperty Column03 
         DataField       =   "AMOUNT"
         Caption         =   "Amount"
         BeginProperty DataFormat {6D835690-900B-11D0-9484-00A0C91110ED} 
            Type            =   1
            Format          =   "0.00"
            HaveTrueFalseNull=   0
            FirstDayOfWeek  =   0
            FirstWeekOfYear =   0
            LCID            =   1033
            SubFormatType   =   1
         EndProperty
      EndProperty
      BeginProperty Column04 
         DataField       =   "IS_DEBIT"
         Caption         =   "Debit?"
         BeginProperty DataFormat {6D835690-900B-11D0-9484-00A0C91110ED} 
            Type            =   5
            Format          =   ""
            HaveTrueFalseNull=   1
            TrueValue       =   "Yes"
            FalseValue      =   "No"
            NullValue       =   ""
            FirstDayOfWeek  =   0
            FirstWeekOfYear =   0
            LCID            =   1033
            SubFormatType   =   7
         EndProperty
      EndProperty
      BeginProperty Column05 
         DataField       =   "CATEGORY_CODE"
         Caption         =   "Category Code"
         BeginProperty DataFormat {6D835690-900B-11D0-9484-00A0C91110ED} 
            Type            =   0
            Format          =   ""
            HaveTrueFalseNull=   0
            FirstDayOfWeek  =   0
            FirstWeekOfYear =   0
            LCID            =   1033
            SubFormatType   =   0
         EndProperty
      EndProperty
      BeginProperty Column06 
         DataField       =   "IS_MARKED"
         Caption         =   "Marked?"
         BeginProperty DataFormat {6D835690-900B-11D0-9484-00A0C91110ED} 
            Type            =   5
            Format          =   "M/d/yyyy"
            HaveTrueFalseNull=   1
            TrueValue       =   "Yes"
            FalseValue      =   "No"
            NullValue       =   "No"
            FirstDayOfWeek  =   0
            FirstWeekOfYear =   0
            LCID            =   1033
            SubFormatType   =   7
         EndProperty
      EndProperty
      BeginProperty Column07 
         DataField       =   "IS_ONETIME"
         Caption         =   "Onetime?"
         BeginProperty DataFormat {6D835690-900B-11D0-9484-00A0C91110ED} 
            Type            =   5
            Format          =   ""
            HaveTrueFalseNull=   1
            TrueValue       =   "Yes"
            FalseValue      =   "No"
            NullValue       =   "No"
            FirstDayOfWeek  =   0
            FirstWeekOfYear =   0
            LCID            =   1033
            SubFormatType   =   7
         EndProperty
      EndProperty
      SplitCount      =   1
      BeginProperty Split0 
         RecordSelectors =   0   'False
         BeginProperty Column00 
            Locked          =   -1  'True
            ColumnWidth     =   0
         EndProperty
         BeginProperty Column01 
            Locked          =   -1  'True
            ColumnWidth     =   0
         EndProperty
         BeginProperty Column02 
            Locked          =   -1  'True
            ColumnWidth     =   3600
         EndProperty
         BeginProperty Column03 
            Locked          =   -1  'True
            ColumnWidth     =   1200.189
         EndProperty
         BeginProperty Column04 
            Locked          =   -1  'True
            ColumnWidth     =   705.26
         EndProperty
         BeginProperty Column05 
            Locked          =   -1  'True
            ColumnWidth     =   900.284
         EndProperty
         BeginProperty Column06 
            Locked          =   -1  'True
            ColumnWidth     =   750.047
         EndProperty
         BeginProperty Column07 
            Locked          =   -1  'True
            ColumnWidth     =   794.835
         EndProperty
      EndProperty
   End
   Begin MSACAL.Calendar calDate 
      Bindings        =   "CreditsDebits.ctx":0000
      DataField       =   "[DATE]"
      BeginProperty DataFormat 
         Type            =   1
         Format          =   "MM/dd/yyyy"
         HaveTrueFalseNull=   0
         FirstDayOfWeek  =   0
         FirstWeekOfYear =   0
         LCID            =   1033
         SubFormatType   =   3
      EndProperty
      DataMember      =   "[DATE]"
      Height          =   2415
      Left            =   0
      TabIndex        =   10
      Top             =   0
      Width           =   3135
      _Version        =   524288
      _ExtentX        =   5530
      _ExtentY        =   4260
      _StockProps     =   1
      BackColor       =   -2147483633
      Year            =   2011
      Month           =   11
      Day             =   28
      DayLength       =   1
      MonthLength     =   2
      DayFontColor    =   0
      FirstDay        =   7
      GridCellEffect  =   0
      GridFontColor   =   10485760
      GridLinesColor  =   -2147483632
      ShowDateSelectors=   -1  'True
      ShowDays        =   -1  'True
      ShowHorizontalGrid=   -1  'True
      ShowTitle       =   0   'False
      ShowVerticalGrid=   -1  'True
      TitleFontColor  =   10485760
      ValueIsNull     =   0   'False
      BeginProperty DayFont {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      BeginProperty GridFont {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      BeginProperty TitleFont {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
   End
   Begin VB.Label lblCat 
      Caption         =   "Category:"
      Height          =   195
      Left            =   3420
      TabIndex        =   15
      Top             =   1395
      Width           =   825
   End
   Begin VB.Label lblAmount 
      Caption         =   "Amount:"
      Height          =   195
      Left            =   3420
      TabIndex        =   14
      Top             =   1035
      Width           =   870
   End
   Begin VB.Label lblDetail 
      Caption         =   "Details:"
      Height          =   240
      Left            =   3420
      TabIndex        =   13
      Top             =   630
      Width           =   870
   End
End
Attribute VB_Name = "CreditsDebits"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Dim m_categories() As String
Dim m_rsCreditDebits As ADODB.Recordset

Private Sub ResetCreditDebitRecordSet(Optional isCleanup As Boolean = False)
    If Not (m_rsCreditDebits Is Nothing) Then
        Set dgdCreditDebits.DataSource = Nothing
        m_rsCreditDebits.Close
        Set m_rsCreditDebits = Nothing
    End If
    
    If Not isCleanup Then
        Set m_rsCreditDebits = qryGetAllCreditsDebits
        Set dgdCreditDebits.DataSource = m_rsCreditDebits
        Call calDate_AfterUpdate
    End If
End Sub

Private Sub calDate_AfterUpdate()
    m_rsCreditDebits.Filter = "DATE = '" & calDate.value & "'"
    Call ClearFields
End Sub

Private Sub cboCategory_Click()
    If cboCategory.ListIndex >= 0 Then
        Dim Index As Integer
        Index = cboCategory.ItemData(cboCategory.ListIndex)
        txtCat.Text = m_categories(Index)
        txtCat.DataChanged = True
    End If
End Sub

Private Sub cmdDelete_Click()
  If Trim(txtKey.Text) = Empty Then
    MsgBox "Select an item from the list to delete!"
    txtDesc.SetFocus
    Exit Sub
  End If
  
  If MsgBox("Are you sure to delete this item?", vbYesNo, "Confirm!") = vbNo Then
    Exit Sub
  End If
  Call Delete(txtKey.Text)
  ResetCreditDebitRecordSet
  
  Call ClearFields
  
  txtDesc.SetFocus
End Sub

Private Sub cmdSave_Click()

    Dim isAdd As Boolean
    Dim item As DebitCreditRecord
    
    isAdd = chkAdd(0).value <> 0
    
    If Trim(txtDesc.Text) = Empty Then
        MsgBox "Details cannot be empty!"
        txtDesc.SetFocus
        Exit Sub
    End If
    
    If Trim(txtAmount.Text) = Empty Or val(txtAmount.Text) <= 0 Then
        MsgBox "Amount should be a valid decimal!"
        txtAmount.SetFocus
        Exit Sub
    End If
    
    If cboCategory.ListIndex <= -1 Then
        MsgBox "Select a valid category!"
        cboCategory.SetFocus
        Exit Sub
    End If
    
    If Not isAdd Then
        If Trim(txtKey.Text) = Empty Then
            MsgBox "Select an item from the list to update!"
            txtDesc.SetFocus
            Exit Sub
        End If
    End If
    
    If Not isAdd Then
        item.m_ItemKey = CLng(txtKey.Text)
    Else
        item.m_ItemKey = CLng(DateDiff("s", CDate("01/01/2000 12:00 AM"), Now()))
    End If
    item.m_ItemDesc = txtDesc.Text
    item.m_IsDebit = chkDebit.value <> 0
    item.m_Date = calDate.value
    item.m_CategoryCode = txtCat.Text
    item.m_BookMark = chkMarked.value <> 0
    item.m_OneTime = chkOnetime.value <> 0
    item.m_Amount = CDec(txtAmount.Text)

    If isAdd Then
        Call Add(item)
    Else
        Call Update(item)
    End If
    
    ResetCreditDebitRecordSet
    
    m_rsCreditDebits.Find ("TIME_STAMP=" & item.m_ItemKey)
    
    If isAdd Then
        Call ClearFields
    End If
    txtDesc.SetFocus
End Sub

Private Sub ClearFields()
    chkAdd(0).value = 1
    txtKey.Text = Empty
    txtDesc.Text = Empty
    txtAmount.Text = Empty
    cboCategory.ListIndex = -1
    txtCat.Text = Empty
    chkDebit.value = 1
    chkOnetime.value = 0
    chkMarked.value = 0
End Sub

Private Sub dgdCreditDebits_RowColChange(LastRow As Variant, ByVal LastCol As Integer)
On Error Resume Next

    Dim i As Integer
    For i = 0 To dgdCreditDebits.SelBookmarks.Count - 1 Step 1
        dgdCreditDebits.SelBookmarks.Remove (i)
    Next i
    
    dgdCreditDebits.SelBookmarks.Add (dgdCreditDebits.RowBookmark(dgdCreditDebits.Row))
    
    Call BindGridToControls
End Sub

Private Sub txtDesc_KeyDown(KeyCode As Integer, Shift As Integer)
    If KeyCode = vbKeyReturn Then
        txtAmount.SetFocus
    End If
End Sub

Private Sub txtAmount_KeyDown(KeyCode As Integer, Shift As Integer)
    If KeyCode = vbKeyReturn Then
        cboCategory.SetFocus
    End If
End Sub

Private Sub cboCategory_KeyDown(KeyCode As Integer, Shift As Integer)
    If KeyCode = vbKeyReturn Then
        Call cmdSave_Click
     End If
End Sub


Private Sub UserControl_Initialize()

    calDate.value = Now
    ResetCreditDebitRecordSet
    
    Dim rs As ADODB.Recordset
    Set rs = qryGetAllCategories
    
    ReDim Preserve m_categories(rs.RecordCount)
    Dim Index As Integer: Index = 0
    While Not rs.EOF
        cboCategory.AddItem rs!CAT_NAME
        
        m_categories(Index) = rs!CAT_CODE
        cboCategory.ItemData(cboCategory.NewIndex) = Index
        Index = Index + 1
        
        rs.MoveNext
    Wend
    rs.Close
    Set rs = Nothing
    
    'txtDesc.SetFocus
End Sub

Private Sub BindGridToControls()
 With dgdCreditDebits
    txtKey.Text = .Columns("TIME_STAMP").value
    txtDesc.Text = .Columns("Details").value
    txtAmount.Text = .Columns("Amount").value
    txtCat.Text = .Columns("Category Code").value
    
    Dim i As Integer
    For i = 0 To cboCategory.ListCount Step 1
        If m_categories(cboCategory.ItemData(i)) = txtCat.Text Then
            cboCategory.ListIndex = i
            Exit For
        End If
    Next i
    
    chkDebit.value = IIf(.Columns("Debit?").value, 1, 0)
    chkOnetime.value = IIf(.Columns("Onetime?") = "Yes", 1, 0)
    chkMarked.value = IIf(.Columns("Marked?") = "Yes", 1, 0)
    
    chkAdd(1).value = True
 End With
End Sub

Private Sub UserControl_Terminate()
    Call ResetCreditDebitRecordSet(True)
End Sub
