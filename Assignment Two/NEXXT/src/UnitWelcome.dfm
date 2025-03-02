object FormWelcome: TFormWelcome
  Left = 0
  Top = 0
  BorderStyle = bsToolWindow
  Caption = 'Welcome to NEXXT studio 3.3.2 !'
  ClientHeight = 462
  ClientWidth = 390
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  OnClose = FormClose
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object Label2: TLabel
    Left = 8
    Top = 440
    Width = 250
    Height = 13
    Caption = 'Your choice is stored in the presets && configs folder.'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = [fsItalic]
    ParentFont = False
  end
  object GroupBox1: TGroupBox
    Left = 8
    Top = 252
    Width = 377
    Height = 100
    Caption = 'A few words from me'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 0
    object StaticText1: TStaticText
      Left = 11
      Top = 16
      Width = 360
      Height = 80
      AutoSize = False
      Caption = 'StaticText1'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
    end
  end
  object RadioButton1: TRadioButton
    Left = 8
    Top = 404
    Width = 404
    Height = 17
    Caption = 'Don'#39't show again'
    Checked = True
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 1
    TabStop = True
  end
  object RadioButton2: TRadioButton
    Left = 8
    Top = 420
    Width = 201
    Height = 17
    Caption = 'Remind me some other time'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 2
  end
  object GroupBox2: TGroupBox
    Left = 8
    Top = 8
    Width = 377
    Height = 120
    Caption = 'What'#39's new since 3.3.1'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 3
    object StaticText2: TStaticText
      Left = 11
      Top = 16
      Width = 301
      Height = 101
      AutoSize = False
      Caption = 'StaticText2'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
    end
  end
  object GroupBox3: TGroupBox
    Left = 8
    Top = 130
    Width = 377
    Height = 120
    Caption = 'What'#39's likely coming in the next few updates'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 4
    object StaticText3: TStaticText
      Left = 11
      Top = 16
      Width = 301
      Height = 101
      AutoSize = False
      Caption = 'StaticText3'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
    end
  end
  object Button1: TButton
    Left = 7
    Top = 358
    Width = 124
    Height = 25
    Caption = 'Donate on Ko-Fi'
    TabOrder = 5
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 137
    Top = 358
    Width = 120
    Height = 25
    Caption = 'Visit my Patreon'
    TabOrder = 6
    OnClick = Button2Click
  end
  object Button3: TButton
    Left = 269
    Top = 430
    Width = 113
    Height = 25
    Caption = 'OK'
    Default = True
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ModalResult = 1
    ParentFont = False
    TabOrder = 7
  end
  object StaticText4: TStaticText
    Left = 269
    Top = 358
    Width = 112
    Height = 64
    AutoSize = False
    Caption = 'StaticText4'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 8
  end
end
