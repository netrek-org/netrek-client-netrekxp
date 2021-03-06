VERSION 5.00
Begin VB.Form Form3 
   BackColor       =   &H80000007&
   Caption         =   "Netrek Configuration Utility"
   ClientHeight    =   10770
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   10950
   Icon            =   "Form3.frx":0000
   LinkTopic       =   "Form3"
   ScaleHeight     =   10770
   ScaleWidth      =   10950
   Begin VB.CheckBox Check5 
      BackColor       =   &H80000007&
      Caption         =   "Enable Sound"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H8000000E&
      Height          =   195
      Left            =   7440
      TabIndex        =   88
      ToolTipText     =   "Turns your sound on.  For better quality enable layered stereo sound."
      Top             =   8640
      Value           =   1  'Checked
      Width           =   1695
   End
   Begin VB.CheckBox Check6 
      BackColor       =   &H00000000&
      Caption         =   "Enable DET Circle"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00FFFFFF&
      Height          =   195
      Left            =   7440
      TabIndex        =   87
      ToolTipText     =   "Shows a circle around your ship, you can Destroy Enemy Torpedoes that are in this circle."
      Top             =   8160
      Width           =   2415
   End
   Begin VB.CheckBox Check9 
      BackColor       =   &H00000000&
      Caption         =   "Show Your Speed"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H8000000E&
      Height          =   195
      Left            =   7440
      MaskColor       =   &H00FFFFFF&
      TabIndex        =   86
      ToolTipText     =   "Shows your speed next to your ship on the local map."
      Top             =   8400
      Value           =   1  'Checked
      Width           =   2055
   End
   Begin VB.CheckBox Check4 
      BackColor       =   &H80000007&
      Caption         =   "Show Warpstreaks"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H8000000E&
      Height          =   195
      Left            =   7440
      TabIndex        =   85
      ToolTipText     =   "Shows star streaks when tawrping to your base."
      Top             =   7920
      Value           =   1  'Checked
      Width           =   2175
   End
   Begin VB.CheckBox Check3 
      BackColor       =   &H80000007&
      Caption         =   "Show Stars"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H8000000E&
      Height          =   195
      Left            =   7440
      TabIndex        =   84
      ToolTipText     =   "Shows stars in the background on your local map."
      Top             =   8880
      Value           =   1  'Checked
      Width           =   1695
   End
   Begin VB.CheckBox Check1 
      BackColor       =   &H00000000&
      Caption         =   "Enable Classic Mode"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00FFFFFF&
      Height          =   255
      Left            =   7440
      TabIndex        =   83
      Top             =   9120
      Width           =   2415
   End
   Begin VB.ComboBox Combo2 
      Height          =   315
      ItemData        =   "Form3.frx":08CA
      Left            =   8880
      List            =   "Form3.frx":08D4
      TabIndex        =   82
      Text            =   "Torpedoes"
      Top             =   5520
      Width           =   1935
   End
   Begin VB.TextBox Text1 
      Height          =   285
      Index           =   30
      Left            =   9960
      MaxLength       =   1
      TabIndex        =   80
      Text            =   "W"
      Top             =   6960
      Width           =   495
   End
   Begin VB.TextBox Text1 
      Height          =   285
      Index           =   29
      Left            =   9960
      MaxLength       =   1
      TabIndex        =   79
      Text            =   "Z"
      Top             =   6600
      Width           =   495
   End
   Begin VB.TextBox Text1 
      Height          =   285
      Index           =   28
      Left            =   9960
      MaxLength       =   1
      TabIndex        =   78
      Text            =   "V"
      Top             =   6240
      Width           =   495
   End
   Begin VB.OptionButton Option2 
      BackColor       =   &H00000000&
      Caption         =   "Netrek Default Keymap"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00FFFFFF&
      Height          =   255
      Left            =   240
      TabIndex        =   71
      Top             =   3600
      Width           =   2535
   End
   Begin VB.OptionButton Option1 
      BackColor       =   &H00000000&
      Caption         =   "Netrek XP 2009 Keymap"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00FFFFFF&
      Height          =   255
      Left            =   240
      TabIndex        =   70
      Top             =   3360
      Value           =   -1  'True
      Width           =   2415
   End
   Begin VB.TextBox Text1 
      Height          =   285
      Index           =   27
      Left            =   6360
      MaxLength       =   1
      TabIndex        =   68
      Text            =   "y"
      Top             =   9000
      Width           =   495
   End
   Begin VB.TextBox Text1 
      Height          =   285
      Index           =   26
      Left            =   6360
      MaxLength       =   1
      TabIndex        =   67
      Text            =   "j"
      Top             =   8640
      Width           =   495
   End
   Begin VB.TextBox Text1 
      Height          =   285
      Index           =   25
      Left            =   6360
      MaxLength       =   1
      TabIndex        =   66
      Text            =   "p"
      Top             =   8280
      Width           =   495
   End
   Begin VB.CommandButton Command2 
      Caption         =   "Cancel"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Left            =   4200
      TabIndex        =   62
      Top             =   10080
      Width           =   1095
   End
   Begin VB.CheckBox Check2 
      BackColor       =   &H80000012&
      Caption         =   "Enable Tip Window"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H8000000E&
      Height          =   195
      Left            =   7440
      TabIndex        =   60
      ToolTipText     =   $"Form3.frx":08EC
      Top             =   7680
      Value           =   1  'Checked
      Width           =   2175
   End
   Begin VB.TextBox Text1 
      Height          =   285
      Index           =   24
      Left            =   6360
      MaxLength       =   1
      TabIndex        =   58
      Text            =   "e"
      Top             =   7800
      Width           =   495
   End
   Begin VB.TextBox Text1 
      Height          =   285
      Index           =   23
      Left            =   6360
      MaxLength       =   1
      TabIndex        =   56
      Text            =   "u"
      Top             =   5760
      Width           =   495
   End
   Begin VB.TextBox Text1 
      Height          =   285
      Index           =   22
      Left            =   2160
      MaxLength       =   1
      TabIndex        =   55
      Text            =   "4"
      Top             =   10080
      Width           =   495
   End
   Begin VB.TextBox Text1 
      Height          =   285
      Index           =   21
      Left            =   2160
      MaxLength       =   1
      TabIndex        =   54
      Text            =   "s"
      Top             =   9720
      Width           =   495
   End
   Begin VB.TextBox Text1 
      Height          =   285
      Index           =   20
      Left            =   2160
      MaxLength       =   1
      TabIndex        =   53
      Text            =   "2"
      Top             =   9360
      Width           =   495
   End
   Begin VB.TextBox Text1 
      Height          =   285
      Index           =   19
      Left            =   2160
      MaxLength       =   1
      TabIndex        =   52
      Text            =   "0"
      Top             =   9000
      Width           =   495
   End
   Begin VB.TextBox Text1 
      Height          =   285
      Index           =   18
      Left            =   2160
      MaxLength       =   1
      TabIndex        =   51
      Text            =   "t"
      Top             =   5400
      Width           =   495
   End
   Begin VB.TextBox Text1 
      Height          =   285
      Index           =   17
      Left            =   6360
      MaxLength       =   11
      TabIndex        =   50
      Text            =   "Space Bar"
      Top             =   6600
      Width           =   975
   End
   Begin VB.TextBox Text1 
      Height          =   285
      Index           =   16
      Left            =   6360
      MaxLength       =   1
      TabIndex        =   49
      Text            =   "v"
      Top             =   6240
      Width           =   495
   End
   Begin VB.TextBox Text1 
      Height          =   285
      Index           =   15
      Left            =   6360
      MaxLength       =   1
      TabIndex        =   48
      Text            =   "q"
      Top             =   9360
      Width           =   495
   End
   Begin VB.TextBox Text1 
      Height          =   285
      Index           =   14
      Left            =   6360
      MaxLength       =   1
      TabIndex        =   47
      Text            =   "i"
      Top             =   5400
      Width           =   495
   End
   Begin VB.TextBox Text1 
      Height          =   285
      Index           =   13
      Left            =   6360
      MaxLength       =   1
      TabIndex        =   46
      Text            =   "r"
      Top             =   5040
      Width           =   495
   End
   Begin VB.TextBox Text1 
      Height          =   285
      Index           =   12
      Left            =   6360
      MaxLength       =   1
      TabIndex        =   45
      Text            =   "n"
      Top             =   4680
      Width           =   495
   End
   Begin VB.TextBox Text1 
      Height          =   285
      Index           =   11
      Left            =   2160
      MaxLength       =   1
      TabIndex        =   44
      Text            =   "R"
      Top             =   6960
      Width           =   495
   End
   Begin VB.TextBox Text1 
      Height          =   285
      Index           =   10
      Left            =   2160
      MaxLength       =   1
      TabIndex        =   43
      Text            =   "#"
      Top             =   8640
      Width           =   495
   End
   Begin VB.TextBox Text1 
      Height          =   285
      Index           =   9
      Left            =   2160
      MaxLength       =   1
      TabIndex        =   42
      Text            =   "a"
      Top             =   8280
      Width           =   495
   End
   Begin VB.TextBox Text1 
      Height          =   285
      Index           =   8
      Left            =   6360
      MaxLength       =   1
      TabIndex        =   41
      Text            =   "d"
      Top             =   7440
      Width           =   495
   End
   Begin VB.TextBox Text1 
      Height          =   285
      Index           =   7
      Left            =   6360
      MaxLength       =   1
      TabIndex        =   40
      Text            =   "g"
      Top             =   7080
      Width           =   495
   End
   Begin VB.TextBox Text1 
      Height          =   285
      Index           =   6
      Left            =   2160
      MaxLength       =   1
      TabIndex        =   39
      Text            =   "z"
      Top             =   7800
      Width           =   495
   End
   Begin VB.TextBox Text1 
      Height          =   285
      Index           =   5
      Left            =   2160
      MaxLength       =   1
      TabIndex        =   38
      Text            =   "x"
      Top             =   7440
      Width           =   495
   End
   Begin VB.TextBox Text1 
      Height          =   285
      Index           =   4
      Left            =   2160
      MaxLength       =   1
      TabIndex        =   37
      Text            =   "c"
      Top             =   6600
      Width           =   495
   End
   Begin VB.TextBox Text1 
      Height          =   285
      Index           =   3
      Left            =   2160
      MaxLength       =   1
      TabIndex        =   36
      Text            =   "b"
      Top             =   6240
      Width           =   495
   End
   Begin VB.TextBox Text1 
      Height          =   285
      Index           =   2
      Left            =   2160
      MaxLength       =   1
      TabIndex        =   34
      Text            =   "w"
      Top             =   5880
      Width           =   495
   End
   Begin VB.TextBox Text1 
      Height          =   285
      Index           =   1
      Left            =   2160
      MaxLength       =   1
      TabIndex        =   33
      Text            =   "P"
      Top             =   5040
      Width           =   495
   End
   Begin VB.ComboBox Combo1 
      Height          =   315
      ItemData        =   "Form3.frx":099E
      Left            =   8880
      List            =   "Form3.frx":09A8
      TabIndex        =   32
      Text            =   "Phasers"
      Top             =   5160
      Width           =   1935
   End
   Begin VB.CommandButton Command1 
      Caption         =   "Finish"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Left            =   5640
      TabIndex        =   21
      Top             =   10080
      Width           =   1095
   End
   Begin VB.TextBox Text1 
      Height          =   285
      Index           =   0
      Left            =   2160
      MaxLength       =   1
      TabIndex        =   20
      Text            =   "f"
      Top             =   4680
      Width           =   495
   End
   Begin VB.Image Image3 
      Height          =   1080
      Left            =   7680
      Picture         =   "Form3.frx":09C0
      Top             =   9360
      Visible         =   0   'False
      Width           =   1950
   End
   Begin VB.Image Image2 
      Height          =   1080
      Left            =   7680
      Picture         =   "Form3.frx":7842
      Top             =   9360
      Width           =   1950
   End
   Begin VB.Label Label46 
      BackColor       =   &H80000007&
      Caption         =   "Middle Button"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H8000000E&
      Height          =   255
      Left            =   7560
      TabIndex        =   81
      Top             =   5520
      Width           =   1215
   End
   Begin VB.Label Label45 
      BackColor       =   &H80000007&
      Caption         =   "Close Special Windows Key"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H8000000E&
      Height          =   255
      Left            =   7560
      TabIndex        =   77
      Top             =   7080
      Width           =   2295
   End
   Begin VB.Label Label44 
      BackColor       =   &H80000007&
      Caption         =   "Lower Volume Key"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H8000000E&
      Height          =   255
      Left            =   7560
      TabIndex        =   76
      Top             =   6720
      Width           =   1575
   End
   Begin VB.Label Label43 
      BackColor       =   &H80000007&
      Caption         =   "Raise Volume Key"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H8000000E&
      Height          =   255
      Left            =   7560
      TabIndex        =   75
      Top             =   6360
      Width           =   1575
   End
   Begin VB.Label Label42 
      BackColor       =   &H00000000&
      Caption         =   "play.netrek.org"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   6
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00FFFFFF&
      Height          =   135
      Left            =   5160
      TabIndex        =   74
      Top             =   10560
      Width           =   1455
   End
   Begin VB.Label Label41 
      BackColor       =   &H00000000&
      Caption         =   $"Form3.frx":E6C4
      ForeColor       =   &H00FFFFFF&
      Height          =   615
      Left            =   3000
      TabIndex        =   73
      Top             =   3120
      Width           =   7575
   End
   Begin VB.Label Label40 
      BackColor       =   &H80000008&
      Caption         =   "Choose Keymap Style:"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00C0C000&
      Height          =   375
      Left            =   120
      TabIndex        =   72
      Top             =   3000
      Width           =   2775
   End
   Begin VB.Label Label39 
      BackColor       =   &H00000000&
      Caption         =   $"Form3.frx":E7F5
      ForeColor       =   &H00FFFFFF&
      Height          =   495
      Left            =   120
      TabIndex        =   69
      Top             =   2400
      Width           =   10695
   End
   Begin VB.Label Label38 
      BackColor       =   &H80000007&
      Caption         =   "Docking Permissions"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H8000000E&
      Height          =   255
      Left            =   4080
      TabIndex        =   65
      Top             =   9120
      Width           =   1935
   End
   Begin VB.Label Label37 
      BackColor       =   &H80000007&
      Caption         =   "Messaging"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H8000000E&
      Height          =   255
      Left            =   4080
      TabIndex        =   64
      Top             =   8760
      Width           =   1215
   End
   Begin VB.Label Label36 
      BackColor       =   &H80000007&
      Caption         =   "Planet List"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H8000000E&
      Height          =   255
      Left            =   4080
      TabIndex        =   63
      Top             =   8400
      Width           =   1095
   End
   Begin VB.Label Label34 
      BackColor       =   &H80000008&
      Caption         =   "Misc Options:"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00C0C000&
      Height          =   375
      Left            =   7440
      TabIndex        =   61
      Top             =   6000
      Width           =   2055
   End
   Begin VB.Label Label32 
      BackColor       =   &H80000007&
      Caption         =   "Tractor/Pressor Toggle"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H8000000E&
      Height          =   375
      Left            =   4080
      TabIndex        =   59
      Top             =   7800
      Width           =   2175
   End
   Begin VB.Label Label24 
      BackColor       =   &H80000007&
      Caption         =   "War Declaration"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H8000000E&
      Height          =   255
      Left            =   4080
      TabIndex        =   57
      Top             =   5760
      Width           =   1455
   End
   Begin VB.Image Image1 
      Height          =   1065
      Left            =   2160
      Picture         =   "Form3.frx":E912
      Top             =   0
      Width           =   6315
   End
   Begin VB.Label Label31 
      BackColor       =   &H80000007&
      Caption         =   "Quit"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H8000000E&
      Height          =   255
      Left            =   4080
      TabIndex        =   35
      Top             =   9480
      Width           =   615
   End
   Begin VB.Label Label33 
      BackColor       =   &H80000008&
      Caption         =   "Mouse Controls:"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00C0C000&
      Height          =   375
      Left            =   7560
      TabIndex        =   31
      Top             =   4680
      Width           =   2055
   End
   Begin VB.Line Line4 
      BorderColor     =   &H80000009&
      X1              =   8400
      X2              =   8400
      Y1              =   4560
      Y2              =   4320
   End
   Begin VB.Label Label30 
      BackColor       =   &H80000007&
      Caption         =   "Left Button"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H8000000E&
      Height          =   255
      Left            =   7560
      TabIndex        =   30
      Top             =   5160
      Width           =   1215
   End
   Begin VB.Line Line3 
      BorderColor     =   &H80000009&
      X1              =   120
      X2              =   8400
      Y1              =   4320
      Y2              =   4320
   End
   Begin VB.Line Line2 
      BorderColor     =   &H80000009&
      X1              =   120
      X2              =   120
      Y1              =   4560
      Y2              =   4320
   End
   Begin VB.Line Line1 
      BorderColor     =   &H80000009&
      X1              =   120
      X2              =   8400
      Y1              =   4560
      Y2              =   4560
   End
   Begin VB.Label Label29 
      BackColor       =   &H80000012&
      Caption         =   "Key to Assign"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00808000&
      Height          =   255
      Left            =   6480
      TabIndex        =   29
      Top             =   4320
      Width           =   1575
   End
   Begin VB.Label Label28 
      BackColor       =   &H80000012&
      Caption         =   "Control"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00FFFF00&
      Height          =   255
      Left            =   4080
      TabIndex        =   28
      Top             =   4320
      Width           =   855
   End
   Begin VB.Label Label27 
      BackColor       =   &H80000012&
      Caption         =   "Key to Assign"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00808000&
      Height          =   255
      Left            =   2160
      TabIndex        =   27
      Top             =   4320
      Width           =   1575
   End
   Begin VB.Label Label26 
      BackColor       =   &H80000012&
      Caption         =   "Control"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00FFFF00&
      Height          =   255
      Left            =   480
      TabIndex        =   26
      Top             =   4320
      Width           =   855
   End
   Begin VB.Label Label25 
      BackColor       =   &H80000008&
      Caption         =   "Keyboard Controls:"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00C0C000&
      Height          =   255
      Left            =   120
      TabIndex        =   25
      Top             =   3960
      Width           =   2535
   End
   Begin VB.Label Label23 
      BackColor       =   &H80000012&
      Caption         =   $"Form3.frx":247E4
      BeginProperty Font 
         Name            =   "Microsoft Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00FFFFFF&
      Height          =   1215
      Left            =   120
      TabIndex        =   24
      Top             =   1080
      Width           =   10695
   End
   Begin VB.Label Label22 
      BackColor       =   &H80000007&
      Caption         =   "Warp 0"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H8000000E&
      Height          =   255
      Left            =   480
      TabIndex        =   23
      Top             =   9000
      Width           =   975
   End
   Begin VB.Label Label21 
      BackColor       =   &H80000007&
      Caption         =   "Warp 4"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H8000000E&
      Height          =   255
      Left            =   480
      TabIndex        =   22
      Top             =   10080
      Width           =   855
   End
   Begin VB.Label Label20 
      BackColor       =   &H80000007&
      Caption         =   "Pickup Armies"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H8000000E&
      Height          =   255
      Left            =   480
      TabIndex        =   19
      Top             =   7800
      Width           =   1335
   End
   Begin VB.Label Label19 
      BackColor       =   &H80000007&
      Caption         =   "Pressor On"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H8000000E&
      Height          =   255
      Left            =   4080
      TabIndex        =   18
      Top             =   7080
      Width           =   1095
   End
   Begin VB.Label Label18 
      BackColor       =   &H80000007&
      Caption         =   "Warp 3"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H8000000E&
      Height          =   255
      Left            =   480
      TabIndex        =   17
      Top             =   9720
      Width           =   975
   End
   Begin VB.Label Label17 
      BackColor       =   &H80000007&
      Caption         =   "Refit to New Ship"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H8000000E&
      Height          =   255
      Left            =   4080
      TabIndex        =   16
      Top             =   4680
      Width           =   1695
   End
   Begin VB.Label Label16 
      BackColor       =   &H80000007&
      Caption         =   "MaxWarp"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H8000000E&
      Height          =   255
      Left            =   480
      TabIndex        =   15
      Top             =   8280
      Width           =   975
   End
   Begin VB.Label Label15 
      BackColor       =   &H80000007&
      Caption         =   "Half Speed"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H8000000E&
      Height          =   255
      Left            =   480
      TabIndex        =   14
      Top             =   8640
      Width           =   1215
   End
   Begin VB.Label Label14 
      BackColor       =   &H80000007&
      Caption         =   "Target Information"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H8000000E&
      Height          =   255
      Left            =   4080
      TabIndex        =   13
      Top             =   5400
      Width           =   1935
   End
   Begin VB.Label Label13 
      BackColor       =   &H80000007&
      Caption         =   "Tractor On"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H8000000E&
      Height          =   255
      Left            =   4080
      TabIndex        =   12
      Top             =   7440
      Width           =   975
   End
   Begin VB.Label Label12 
      BackColor       =   &H80000007&
      Caption         =   "Warp 2"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H8000000E&
      Height          =   255
      Left            =   480
      TabIndex        =   11
      Top             =   9360
      Width           =   1095
   End
   Begin VB.Label Label11 
      BackColor       =   &H80000007&
      Caption         =   "Repair"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H8000000E&
      Height          =   255
      Left            =   480
      TabIndex        =   10
      Top             =   6960
      Width           =   1455
   End
   Begin VB.Label Label10 
      BackColor       =   &H80000007&
      Caption         =   "Lock onto Target"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H8000000E&
      Height          =   255
      Left            =   4080
      TabIndex        =   9
      Top             =   5040
      Width           =   1935
   End
   Begin VB.Label Label9 
      BackColor       =   &H80000007&
      Caption         =   "Plasma"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H8000000E&
      Height          =   255
      Left            =   480
      TabIndex        =   8
      Top             =   5400
      Width           =   855
   End
   Begin VB.Label Label8 
      BackColor       =   &H80000007&
      Caption         =   "Phasers"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H8000000E&
      Height          =   255
      Left            =   480
      TabIndex        =   7
      Top             =   5040
      Width           =   735
   End
   Begin VB.Label Label7 
      BackColor       =   &H80000007&
      Caption         =   "Drop Armies"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H8000000E&
      Height          =   255
      Left            =   480
      TabIndex        =   6
      Top             =   7440
      Width           =   1215
   End
   Begin VB.Label Label6 
      BackColor       =   &H80000007&
      Caption         =   "Detonate Enemy Torps"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H8000000E&
      Height          =   255
      Left            =   4080
      TabIndex        =   5
      Top             =   6600
      Width           =   2055
   End
   Begin VB.Label Label5 
      BackColor       =   &H80000007&
      Caption         =   "Detonate Own Torps"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H8000000E&
      Height          =   255
      Left            =   4080
      TabIndex        =   4
      Top             =   6240
      Width           =   1815
   End
   Begin VB.Label Label4 
      BackColor       =   &H80000007&
      Caption         =   "Bomb"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H8000000E&
      Height          =   255
      Left            =   480
      TabIndex        =   3
      Top             =   6240
      Width           =   495
   End
   Begin VB.Label Label3 
      BackColor       =   &H80000007&
      Caption         =   "Cloak"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H8000000E&
      Height          =   255
      Left            =   480
      TabIndex        =   2
      Top             =   6600
      Width           =   615
   End
   Begin VB.Label Label2 
      BackColor       =   &H80000007&
      Caption         =   "Shield"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H8000000E&
      Height          =   255
      Left            =   480
      TabIndex        =   1
      Top             =   5880
      Width           =   615
   End
   Begin VB.Label Label1 
      BackColor       =   &H80000007&
      Caption         =   "Torpedoes"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H8000000E&
      Height          =   255
      Left            =   480
      TabIndex        =   0
      Top             =   4680
      Width           =   1095
   End
End
Attribute VB_Name = "Form3"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'Copyright (C) 2007  Joe Evango - Netrek XP 2009 Configuration Program v1.5
'
'This program is free software; you can redistribute it and/or
'modify it under the terms of the GNU General Public License
'as published by the Free Software Foundation; either version 2
'of the License, or (at your option) any later version.
'
'This program is distributed in the hope that it will be useful,
'but WITHOUT ANY WARRANTY; without even the implied warranty of
'MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
'GNU General Public License for more details.
'
'You should have received a copy of the GNU General Public License
'along with this program; if not, write to the Free Software
'Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
'
'To contact the author email joe@playnetrek.org
'
' Program notes from Joe 4/15/07-
' Code contains some comments, pretty easy to follow the logic.
'
' This utility writes to the netrekrc file and assumes it is in located the same folder as the utility so if the
' netrekrc file does not exist the utility will not run properly and error out.  When it is run it will also save
' your old netrekrc file as netrek.sav.
'
' Since Netrek code has always been freely shared in the community I will do the same with this config utility.
' Please feel free to help improve the utility and/or include the utility as an add-on if you are working on
' a Windows client.  Also feel free to use the logic in this code to create a config utility for another OS.  It
' took a while to put the keymap portion of this together but it works well. I only ask that if any modifications
' are made you also make your code changes available to the community and give me a mention as a contributor
'
' Special thanks to Bill Barcerski for helping me out with the Classic Mode configuration addition and helping test
'
' Program notes from Joe - 5/15/07
' Removed the repeated append functionality and replaced it with an imported rc file with default setting.
' Options available in the config utility are added at the bottom of the file.  These options are excluded from the
' imported file and appended only once to the bottom of the imported rc file.  Options are now only specified one time.
' If a netrekrc.txt file already exists it is save in the netrek root install folder as netrek.sav.
' In order for the program to function there will need to be a file named netrekrc.txt in the root of where it is executed
' and a file named netrekrc.cfg in the docs folder.  Fixed several bugs that have been reported.


'swaps the classic mode/xp mode images when the checkbox value changes

Private Sub Check1_Click()
    If (Check1.Value = vbChecked) Then
         Image2.Visible = False
         Image3.Visible = True
  Else
         Image3.Visible = False
         Image2.Visible = True
  End If
End Sub




Private Sub Command1_Click()

'Saves your current netrekrc.txt to netrekrc.sav and copies a static netrek config file from the docs directory
    
    FileCopy ".\netrekrc.txt", ".\netrekrc.sav"
    FileCopy ".\docs\netrekrc.cfg", ".\netrekrc.txt"

    
    Dim warp As Integer
    Dim det As Integer
    Dim speed As Integer
    Dim sound As Integer
    Dim stars As Integer
         
    Dim warptxt As String
    Dim dettxt As String
    Dim speedtxt As String
    Dim soundtxt As String
    Dim starstxt As String

    Dim FileName As String
    Dim Application As String
    Dim keymap As String
    Dim buttonmap As String
    Dim torpedo As String
    Dim phaser As String
    Dim shield As String
    Dim bomb As String
    Dim cloak As String
    Dim drop As String
    Dim pickup As String
    Dim tractor As String
    Dim pressor As String
    Dim maxwarp As String
    Dim halfspeed As String
    Dim repair As String
    Dim refit As String
    Dim tlock As String
    Dim info As String
    Dim quit As String
    Dim detown As String
    Dim detenemy As String
    Dim plasma As String
    Dim warp0 As String
    Dim warp2 As String
    Dim warp3 As String
    Dim warp4 As String
    Dim war As String
    Dim tractoroff As String
    Dim planetlist As String
    Dim docking As String
    Dim messaging As String
        
        
    Dim tips As Integer
    Dim tiptxt As String
    
    Dim volumeup As String
    Dim volumedown As String
    Dim windowclose As String
    
    Dim classicmode As Integer
    
    
    Dim mode1 As String
    Dim mode2 As String
    Dim mode3 As String
    Dim mode4 As String
    Dim mode5 As String
    Dim mode6 As String
    Dim mode7 As String
    Dim mode8 As String
    Dim mode9 As String
    Dim mode10 As String
    Dim mode11 As String
        
    Dim print1 As String
    Dim print2 As String
    
    Dim keymap1 As String
    Dim keymap2 As String
    Dim keymap3 As String
    Dim keymap4 As String
    Dim keymap5 As String
    Dim keymap6 As String
    Dim keymap7 As String
    Dim keymap8 As String
    Dim keymap9 As String
    Dim keymap10 As String
    Dim keymap11 As String
    Dim keymap12 As String
    Dim keymap13 As String
    Dim keymap14 As String
    Dim keymap15 As String
    Dim keymap16 As String
    Dim keymap17 As String
    Dim keymap18 As String
    Dim keymap19 As String
    Dim keymap20 As String
    Dim keymap21 As String
    Dim keymap22 As String
    Dim keymap23 As String
    Dim keymap24 As String
    Dim keymap25 As String
    Dim keymap26 As String
    Dim keymap27 As String
    Dim keymap28 As String
    
    Dim keymap29 As String
    Dim keymap30 As String
    Dim keymap31 As String
    
    
    Dim keytag1 As String
    Dim keytag2 As String
    
    Dim a As Integer
    Dim b As Integer
    Dim c As Integer
    Dim d As Integer
       
    
    Dim duplicate As Integer
    Dim speckey As Integer
    Dim blank As Integer
    
    Dim currentdate As String
    
    currentdate = Date
    fnum3 = FreeFile
    fnum2 = FreeFile
    
    
    torpedo = "t"
    phaser = "p"
    shield = "s"
    bomb = "b"
    cloak = "c"
    drop = "x"
    pickup = "z"
    tractor = "_"
    pressor = "^"
    maxwarp = "%"
    halfspeed = "#"
    repair = "R"
    refit = "r"
    tlock = "l"
    info = "i"
    quit = "q"
    detown = "D"
    detenemy = "d"
    plasma = "f"
    warp0 = "0"
    warp2 = "2"
    warp3 = "3"
    warp4 = "4"
    war = "w"
    tractoroff = "y"
    planetlist = "P"
    docking = "e"
    messaging = "m"
    volumeup = "V"
    volumedown = "v"
    windowclose = " "
    
    FileName = ".\controls.txt"
    Application = "Notepad.exe"
        
    
   If (Check2.Value = vbChecked) Then
        tips = 1
   End If
   
   
    If tips = 1 Then
        tiptxt = "showHints: on"
    End If
    
    If tips = 0 Then
        tiptxt = "showHints: off"
    End If
    
     If (Check4.Value = vbChecked) Then
         warp = 1
    End If
   
   
    If warp = 1 Then
        warptxt = "warpStreaks: on"
    End If
    
    If warp <> 1 Then
        warptxt = "warpStreaks: off"
    End If
    
    
   If (Check6.Value = vbChecked) Then
        det = 1
   End If
   
    If det = 1 Then
        dettxt = "detCircle: on"
    End If
    
    If det <> 1 Then
        dettxt = "detCircle: off"
    End If
    
               
               
   If (Check9.Value = vbChecked) Then
        speed = 1
   End If
      
   If speed = 1 Then
        speedtxt = "showMySpeed: on"
   End If
    
   If tips <> 1 Then
        speedtxt = "showMySpeed: off"
   End If
               
               
               
   If (Check5.Value = vbChecked) Then
        sound = 1
   End If
    
   If sound = 1 Then
        soundtxt = "sound: on"
   End If
    
   If sound <> 1 Then
        soundtxt = "sound: off"
   End If
               
               
               
   If (Check3.Value = vbChecked) Then
        stars = 1
   End If
   
   
    If stars = 1 Then
        starstxt = "showStars: on"
    End If
    
    If stars <> 1 Then
        starstxt = "showStars: off"
    End If
              
'Keymap configuration
    
    If torpedo = Text1(0) Then
       keymap1 = ""
    Else
       keymap1 = Text1(0) + torpedo
    End If
    
    If phaser = Text1(1) Then
       keymap2 = ""
    Else
       keymap2 = Text1(1) + phaser
    End If
    
    If shield = Text1(2) Then
       keymap3 = ""
    Else
       keymap3 = Text1(2) + shield
    End If
    
    If bomb = Text1(3) Then
       keymap4 = ""
    Else
       keymap4 = Text1(3) + bomb
    End If
    
    If cloak = Text1(4) Then
       keymap5 = ""
    Else
       keymap5 = Text1(4) + cloak
    End If
    
    If drop = Text1(5) Then
       keymap6 = ""
    Else
       keymap6 = Text1(5) + drop
    End If
    
    If pickup = Text1(6) Then
       keymap7 = ""
    Else
       keymap7 = Text1(6) + pickup
    End If
    
    If pressor = Text1(7) Then
       keymap8 = ""
    Else
       keymap8 = Text1(7) + pressor
    End If
    
    If tractor = Text1(8) Then
       keymap9 = ""
    Else
       keymap9 = Text1(8) + tractor
    End If
    
    If maxwarp = Text1(9) Then
       keymap10 = ""
    Else
       keymap10 = Text1(9) + maxwarp
    End If
    
    If halfspeed = Text1(10) Then
       keymap11 = ""
    Else
       keymap11 = Text1(10) + halfspeed
    End If
    
    If repair = Text1(11) Then
       keymap12 = ""
    Else
       keymap12 = Text1(11) + repair
    End If
    
    If refit = Text1(12) Then
       keymap13 = ""
    Else
       keymap13 = Text1(12) + refit
    End If
    
    If tlock = Text1(13) Then
       keymap14 = ""
    Else
       keymap14 = Text1(13) + tlock
    End If
    
    If info = Text1(14) Then
       keymap15 = ""
    Else
       keymap15 = Text1(14) + info
    End If
    
    If quit = Text1(15) Then
       keymap16 = ""
    Else
       keymap16 = Text1(15) + quit
    End If
    
    If detown = Text1(16) Then
       keymap17 = ""
    Else
       keymap17 = Text1(16) + detown
    End If
    
    If detenemy = Text1(17) Then
       keymap18 = ""
    Else
       If Text1(17) <> "Space Bar" Then
            keymap18 = Text1(17) + detenemy
    Else
        If Text1(17) = "Space Bar" Then
            keymap18 = " " + detenemy
    End If
    End If
    End If
    
    If plasma = Text1(18) Then
       keymap19 = ""
    Else
       keymap19 = Text1(18) + plasma
    End If
    
    If warp0 = Text1(19) Then
       keymap20 = ""
    Else
       keymap20 = Text1(19) + warp0
    End If
    
    If warp2 = Text1(20) Then
       keymap21 = ""
    Else
       keymap21 = Text1(20) + warp2
    End If
    
    If warp3 = Text1(21) Then
       keymap22 = ""
    Else
       keymap22 = Text1(21) + warp3
    End If
    
    If warp4 = Text1(22) Then
       keymap23 = ""
    Else
       keymap23 = Text1(22) + warp4
    End If
    If war = Text1(23) Then
       keymap24 = ""
    Else
       keymap24 = Text1(23) + war
    End If
    If tractoroff = Text1(24) Then
       keymap25 = ""
    Else
       keymap25 = Text1(24) + tractoroff
    End If
    
    
    If planetlist = Text1(25) Then
       keymap26 = ""
    Else
       keymap26 = Text1(25) + planetlist
    End If
    If messaging = Text1(26) Then
       keymap27 = ""
    Else
       keymap27 = Text1(26) + messaging
       End If
    If docking = Text1(27) Then
       keymap28 = ""
    Else
       keymap28 = Text1(27) + docking
    End If
    If volumeup = Text1(28) Then
       keymap29 = ""
    Else
       keymap29 = Text1(28) + volumeup
    End If
    If volumedown = Text1(29) Then
       keymap30 = ""
    Else
       keymap30 = Text1(29) + volumedown
    End If
    If windowsclose = Text1(30) Then
       keymap31 = ""
    Else
       keymap31 = Text1(30) + windowclose
    End If
    
    If (Check1.Value = vbChecked) Then
       classicmode = 1
    Else
       classicmode = 0
    End If
      
    speckey = 0
    duplicate = 0
    blank = 0
    
    For a = 0 To 30
       For b = 0 To 30
            If a <> b And Text1(a) = Text1(b) Then
               duplicate = 1
               Exit For
            End If
       Next b
            If duplicate = 1 Then
                MsgBox "The key '" + Text1(a) + "' is defined more than once.  This will cause problems while playing.  Please remove the duplicate controls and click on Finish."
                Exit For
            End If
    Next a
       
     
       For c = 0 To 30
           If Text1(c) = "h" Then
               speckey = 1
           End If
           If speckey = 1 Then
                MsgBox "You cannot use the 'h' key.  This is used for help.  Please assign a different key."
           Exit For
          End If
       Next c
       
       For d = 0 To 30
           If Text1(d) = "" Then
               blank = 1
           End If
           If blank = 1 Then
                MsgBox "Blank values are not allowed.  If you meant to assign a key to the space bar please go to the key you would like to assign and enter a space by pressing the space bar"
           Exit For
          End If
       Next d
    
    
       If duplicate = 0 And speckey = 0 And blank = 0 Then
       ' Open the file for append.
       Open ".\netrekrc.txt" For Append As fnum2
       
       ' Add the command.
       If Combo1 = "Torpedoes" And Combo2 = "Phasers" Then
           buttonmap = "buttonmap:" + "1" + Text1(0) + "2" + Text1(1)
       End If
       If Combo1 = "Phasers" And Combo2 = "Torpedoes" Then
           buttonmap = "buttonmap:" + "1" + Text1(1) + "2" + Text1(0)
       End If
       
       If Combo1 = "Phasers" And Combo2 = "Phasers" Then
           buttonmap = "buttonmap:" + "1" + Text1(1) + "2" + Text1(1)
       End If
       If Combo1 = "Torpedoes" And Combo2 = "Torpedoes" Then
           buttonmap = "buttonmap:" + "1" + Text1(0) + "2" + Text1(0)
       End If
      
'Classic Mode / XP Mode options
       If classicmode = 1 Then
          mode1 = "colorClient: 0"
          mode2 = "colorWeapons: off"
          mode4 = "planetBitmap: 0"
          mode5 = "planetBitmapGalaxy: 0"
          mode6 = "planetHighlighting: off"
          mode10 = "useLite: off"
       Else
          mode1 = "colorClient: 4"
          mode2 = "colorWeapons: on"
          mode4 = "planetBitmap: 3"
          mode5 = "planetBitmapGalaxy: 3"
          mode6 = "planetHighlighting: on"
          mode10 = "useLite: on"
       End If
       
       
       keymap = "keymap:" + keymap1 + keymap2 + keymap3 + keymap4 + keymap5 + keymap6 + keymap7 + keymap8 + keymap9 + keymap10 + keymap11 + keymap12 + keymap13 + keymap14 + keymap15 + keymap16 + keymap17 + keymap18 + keymap19 + keymap20 + keymap21 + keymap22 + keymap23 + keymap24 + keymap25 + keymap26 + keymap27 + keymap28 + keymap29 + keymap30 + keymap31
       keytag1 = "###Configuration below was added on - " + currentdate
       
       Print #fnum2, Chr(13)
       Print #fnum2, Chr(13)
       Print #fnum2, keytag1
       Print #fnum2, Chr(13)
       Print #fnum2, "# Key mapping"
       Print #fnum2, "# Define you keymap here.  The format is newkey defaultkey.  For example, the"
       Print #fnum2, "# default key for shield toggle is 's', if you want to remap shield toggle to"
       Print #fnum2, "# 'q', you would put 'qs' in your keymap.  Shields would still be mapped to"
       Print #fnum2, "# 's' as well as now being on 'q'.  Adding a mapping doesn't delete the old"
       Print #fnum2, "# one.  If you want shields on 'w' as well, put 'ws' in your keymap.  If you"
       Print #fnum2, "# had instead put 'wq', it would have mapped quit, the default action of 'q',"
       Print #fnum2, "# onto 'w'."
       Print #fnum2, keymap
       Print #fnum2, Chr(13)
       Print #fnum2, "# Mouse button mapping"
       Print #fnum2, "# Button map lets you map the mouse buttons.  Unfortunately, it works in a"
       Print #fnum2, "# different way than keymap.  The format is <mousebutton> <key>.  To map"
       Print #fnum2, "# shields onto button 1 (the left one), you would put '1s' in your keymap."
       Print #fnum2, "# The buttonmap works through the keymap, so if you have shields mapped to"
       Print #fnum2, "# 'q', putting '1q' would make button 1 shields and not quit.  Buttons 4 and"
       Print #fnum2, "# 5 are support for Microsoft Xbuttons.  If you have a wheel mouse, the up"
       Print #fnum2, "# wheel is button 6 and the down wheel is button 7.  There is also support"
       Print #fnum2, "# for shifted, controled, and control-shifted mouse actions, see the netrek"
       Print #fnum2, "# help file for more details."
       Print #fnum2, buttonmap
       Print #fnum2, Chr(13)
       Print #fnum2, "# Show hints window"
       Print #fnum2, tiptxt
       Print #fnum2, Chr(13)
       Print #fnum2, "# What type of ship bitmaps to use"
       Print #fnum2, "# 0 - mono"
       Print #fnum2, "# 1 - new color bitmaps (default)"
       Print #fnum2, "# 2 - old color bitmaps"
       Print #fnum2, "# 3 - shaded old color bitmaps"
       Print #fnum2, "# 4 - experimental high res bitmaps"
       Print #fnum2, mode1
       Print #fnum2, Chr(13)
       Print #fnum2, "# Use colored bitmaps for torps and plasmas"
       Print #fnum2, mode2
       Print #fnum2, Chr(13)
       Print #fnum2, "# Type of planet bitmaps on local map"
       Print #fnum2, "# 0 - Bronco (default)"
       Print #fnum2, "# 1 - Moo"
       Print #fnum2, "# 2 - Rabbitear"
       Print #fnum2, "# 3 - New color"
       Print #fnum2, mode4
       Print #fnum2, Chr(13)
       Print #fnum2, "# Type of planet bitmaps on galactic map"
       Print #fnum2, "# 0 - Bronco (default)"
       Print #fnum2, "# 1 - Moo"
       Print #fnum2, "# 2 - Rabbitear"
       Print #fnum2, "# 3 - New color"
       Print #fnum2, mode5
       Print #fnum2, Chr(13)
       
       Print #fnum2, "# Highlight galactic planets by race and army status"
       Print #fnum2, mode6
       Print #fnum2, "# Use beeplite"
       Print #fnum2, mode10

       
       Print #fnum2, Chr(13)
       Print #fnum2, "# Enable layered, stereo sound"
       Print #fnum2, soundtxt
       Print #fnum2, Chr(13)
       Print #fnum2, "# Draw background stars"
       Print #fnum2, starstxt
       Print #fnum2, Chr(13)
       Print #fnum2, "# Draw warp streaks while transwarping to starbase"
       Print #fnum2, warptxt
       Print #fnum2, Chr(13)
       Print #fnum2, "# Show det circle around your ship"
       Print #fnum2, dettxt
       Print #fnum2, Chr(13)
       Print #fnum2, "# Show my speed on local"
       Print #fnum2, speedtxt
       Close fnum2
       
       ' Opens the controls you chose in notepad for reference
       Open ".\controls.txt" For Output As #fnum3
       Print #fnum3, "These are the controls you have chosen:"
       Print #fnum3, Chr(13)
       Print #fnum3, Chr(13)
       Print #fnum3, "Mouse Controls:"
       Print #fnum3, Chr(13)
       Print #fnum3, "Left Button     -  " + Combo1
       Print #fnum3, "Middle Button   -  " + Combo2
       Print #fnum3, "Right Button    -  " + "Steer"
       Print #fnum3, Chr(13)
       Print #fnum3, Chr(13)
       Print #fnum3, Chr(13)
       Print #fnum3, "Keyboard Controls:"
       Print #fnum3, Chr(13)
       Print #fnum3, "Messaging            - " + Text1(26) + "  - Be sure to hold your mouse cursor over"
       Print #fnum3, "                          " + "the Galaxy or Tactical maps when you press '" + Text1(26) + "'."
       Print #fnum3, "                           " + "After pressing '" + Text1(26) + "' type 'A' to send messages to"
       Print #fnum3, "                           " + "the All Window, type 'T' to send messages"
       Print #fnum3, "                           " + "to the Team Window, or type the letter/number"
       Print #fnum3, "                           " + "of the player you would like to send a message to."
       Print #fnum3, "                           " + "After you type A, T, or the player letter/number, "
       Print #fnum3, "                           " + "type your message and press enter."
       Print #fnum3, "Help                 -  " + "h"
       Print #fnum3, Chr(13)
       Print #fnum3, "Torpedoes            -  " + Text1(0)
       Print #fnum3, "Phasers              -  " + Text1(1)
       Print #fnum3, "Plasma               -  " + Text1(18)
       Print #fnum3, "Shield               -  " + Text1(2)
       Print #fnum3, "Bomb                 -  " + Text1(3)
       Print #fnum3, "Cloak                -  " + Text1(4)
       Print #fnum3, Chr(13)
       Print #fnum3, "Drop Armies          -  " + Text1(5)
       Print #fnum3, "Pickup Armies        -  " + Text1(6)
       Print #fnum3, Chr(13)
       Print #fnum3, "Detonate Own Torps   -  " + Text1(16)
       If Text1(17) = " " Then
          Print #fnum3, "Detonate Enemy Torps -  <Space Bar>"
       Else
          Print #fnum3, "Detonate Enemy Torps -  " + Text1(17)
       End If
       Print #fnum3, Chr(13)
       Print #fnum3, "Tractor On           -  " + Text1(8)
       Print #fnum3, "Pressor On           -  " + Text1(7)
       Print #fnum3, "Tractor/Pressor Off  -  " + Text1(24)
       Print #fnum3, Chr(13)
       Print #fnum3, "Repair               -  " + Text1(11)
       Print #fnum3, "Refit Ship Type      -  " + Text1(12)
       Print #fnum3, "Lock on Target       -  " + Text1(13)
       Print #fnum3, "Target Info          -  " + Text1(14)
       Print #fnum3, Chr(13)
       Print #fnum3, "Planet List          -  " + Text1(25)
       Print #fnum3, "Docking Premissions  -  " + Text1(27)
       Print #fnum3, Chr(13)
       Print #fnum3, "Quit                 -  " + Text1(15)
       Print #fnum3, "War Declaration      -  " + Text1(23)
       Print #fnum3, Chr(13)
       Print #fnum3, "Max Warp             -  " + Text1(9)
       Print #fnum3, "Half Speed           -  " + Text1(10)
       Print #fnum3, "Warp 0               -  " + Text1(19)
       Print #fnum3, "Warp 2               -  " + Text1(20)
       Print #fnum3, "Warp 3               -  " + Text1(21)
       Print #fnum3, "Warp 4               -  " + Text1(22)
       Print #fnum3, "Volume Up            -  " + Text1(28)
       Print #fnum3, "Volume Down          -  " + Text1(29)
       Print #fnum3, "Close Special Windows-  " + Text1(30)
       Close #fnum3
       Unload Form3
       MsgBox "YOUR CONFIGURATION HAS BEEN CREATED!  After you click on OK a document with the controls you have chosen will open.  A shortcut to this document has been created in your Netrek Config Utility program group.  If you already had a configuration file in place it has been saved in your Netrek install directory as netrekrc.sav"
       Shell Application & " " & FileName, vbNormalFocus
     Else
     End If
End Sub

Private Sub Command2_Click()
    Unload Form3
End Sub


Private Sub Option2_Click()
 Form1.Show
 Unload Form3
End Sub

Private Sub Text1_Change(Index As Integer)
   If Text1(17) = " " Then
      Text1(17) = "Space Bar"
   Else
      Text1(17) = Text1(17)
   End If
End Sub

