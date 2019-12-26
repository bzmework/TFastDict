VERSION 5.00
Begin VB.Form frmMain 
   Caption         =   "字典测试"
   ClientHeight    =   6825
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   12180
   BeginProperty Font 
      Name            =   "Tahoma"
      Size            =   8.25
      Charset         =   0
      Weight          =   400
      Underline       =   0   'False
      Italic          =   0   'False
      Strikethrough   =   0   'False
   EndProperty
   LinkTopic       =   "Form1"
   ScaleHeight     =   455
   ScaleMode       =   3  'Pixel
   ScaleWidth      =   812
   StartUpPosition =   1  '所有者中心
   Begin VB.TextBox txtCount 
      Height          =   375
      Left            =   8610
      MaxLength       =   10
      TabIndex        =   4
      Text            =   "100000"
      Top             =   690
      Width           =   3075
   End
   Begin VB.CommandButton Command5 
      Caption         =   "清除"
      Height          =   495
      Left            =   120
      TabIndex        =   3
      Top             =   6210
      Width           =   1725
   End
   Begin VB.CommandButton Command4 
      Caption         =   "微软 写入-读取-删除 写入测试"
      BeginProperty Font 
         Name            =   "宋体"
         Size            =   9
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   555
      Left            =   8610
      TabIndex        =   2
      Top             =   2340
      Width           =   3075
   End
   Begin VB.ListBox lstOut 
      Height          =   5325
      Left            =   150
      TabIndex        =   1
      Top             =   630
      Width           =   8085
   End
   Begin VB.CommandButton Command1 
      Caption         =   "写入-读取-删除 测试"
      BeginProperty Font 
         Name            =   "宋体"
         Size            =   9
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   555
      Left            =   8610
      TabIndex        =   0
      Top             =   1590
      Width           =   3075
   End
   Begin VB.Label Label1 
      Caption         =   "测试数量"
      Height          =   255
      Left            =   8610
      TabIndex        =   5
      Top             =   390
      Width           =   975
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Sub Command1_Click()
    Dim oTime As New cTiming
    Dim dic As New TFastDict.FastDict
    Dim t As Single
    Dim i As Long
    Dim v As String
    Dim n As Long
    
    n = Val(txtCount.Text)
    
    t = oTime.Elapsed
    For i = 1 To n
        If Not dic.Add("k你好" & i, "hello") Then
            lstOut.AddItem "增加到" & i & "失败,内存申请失败"
            Exit For
        End If
    Next
    
    lstOut.AddItem "自定字典增加" & n & ",消耗时间: " & oTime.Elapsed - t & " 毫秒,字典中实际项目数量" & dic.Count
    DoEvents
    
    
    Dim ps As Long
    
    t = oTime.Elapsed
    For i = 1 To n
        v = dic.Item("k你好" & i)
        If IsEmpty(v) Then
            ps = ps + 1
        End If
    Next
    
    lstOut.AddItem "自定字典读取" & n & ",消耗时间: " & oTime.Elapsed - t & " 毫秒,发生碰撞" & ps
    DoEvents
    
    Dim q As Variant

    t = oTime.Elapsed
    q = dic.Query("*公司*") '支持通配符*和?

    lstOut.AddItem "自定字典从" & n & "中查询1次,消耗时间: " & oTime.Elapsed - t & " 毫秒,"
    DoEvents
    
    
    t = oTime.Elapsed
    For i = 1 To n
        dic.Remove "k你好" & i
    Next
    
    lstOut.AddItem "自定字典删除" & n & ",消耗时间: " & oTime.Elapsed - t & " 毫秒"
    DoEvents
    
    Set dic = Nothing
    Set oTime = Nothing
    
End Sub

Private Sub Command4_Click()
    Dim oTime As New cTiming
    Dim dic As New Scripting.Dictionary
    Dim t As Single
    Dim i As Long
    Dim v As String
    Dim n As Long
    
    n = Val(txtCount.Text)
    
    t = oTime.Elapsed
    For i = 1 To n
        dic.Add "k" & i, "hello"
    Next
    
    lstOut.AddItem "微软字典增加" & n & ",消耗时间: " & oTime.Elapsed - t & " 毫秒"
    DoEvents
    
    
    t = oTime.Elapsed
    For i = 1 To n
        v = dic.Item("k" & i)
    Next
    
    lstOut.AddItem "微软字典读取" & n & ",消耗时间: " & oTime.Elapsed - t & " 毫秒"
    DoEvents
    
    
    t = oTime.Elapsed
    For i = 1 To n
        dic.Remove "k" & i
    Next
    
    lstOut.AddItem "微软字典删除" & n & ",消耗时间: " & oTime.Elapsed - t & " 毫秒"
    DoEvents
    
    Set dic = Nothing
    
    
End Sub

Private Sub Command5_Click()
    lstOut.Clear
End Sub


