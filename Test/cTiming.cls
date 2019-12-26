VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "cTiming"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
'================================================
' Class:         cTiming.cls
' Author:        -
' Dependencies:  None
' Last revision: -
'================================================

Option Explicit

'-- API:

Private Declare Function QueryPerformanceCounter Lib "kernel32" (lpPerformanceCount As Currency) As Long
Private Declare Function QueryPerformanceFrequency Lib "kernel32" (lpFrequency As Currency) As Long

'//

'-- Private variables:
Private m_Frequency As Currency
Private m_Start     As Currency
Private m_Now       As Currency
Private m_Available As Boolean



'========================================================================================
' Class
'========================================================================================

Private Sub Class_Initialize()

    m_Available = (QueryPerformanceFrequency(m_Frequency) <> 0)
    
    If (m_Available) Then
        Debug.Print "Ticks/sec: "; m_Frequency * 10000
      Else
        Debug.Print "Performance Counter not available"
    End If
End Sub

'========================================================================================
' Methods
'========================================================================================

Friend Sub Reset()
    QueryPerformanceCounter m_Start
End Sub

Friend Function Elapsed() As Double
    Call QueryPerformanceCounter(m_Now)
    If (m_Available) Then
        Elapsed = 1000 * (m_Now - m_Start) / m_Frequency
    End If
End Function
