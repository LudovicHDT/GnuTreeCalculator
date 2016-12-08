; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CTreeAnalyserDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Tree Analyser.h"

ClassCount=6
Class1=CTreeAnalyserApp
Class2=CTreeAnalyserDlg
Class3=CAboutDlg

ResourceCount=9
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_TREEANALYSER_DIALOG
Resource4=IDD_TREEANALYSER_DIALOG (English (U.S.))
Resource5=IDD_ABOUTBOX (English (U.S.))
Resource6=IDD_DLG_ADD_IMAGE
Class4=CDlgAdd
Resource7=IDD_DLG_PROGRAM_SET
Class5=CProgramSettingDlg
Resource8=IDD_PICTURE_COMBINATION
Class6=CTestDlg
Resource9=IDR_MENU1

[CLS:CTreeAnalyserApp]
Type=0
HeaderFile=Tree Analyser.h
ImplementationFile=Tree Analyser.cpp
Filter=N
LastObject=ID_FILE_OPENPROJECT
BaseClass=CWinApp
VirtualFilter=AC

[CLS:CTreeAnalyserDlg]
Type=0
HeaderFile=Tree AnalyserDlg.h
ImplementationFile=Tree AnalyserDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=ID_IMAGE_SAVEPIXELVALUE

[CLS:CAboutDlg]
Type=0
HeaderFile=Tree AnalyserDlg.h
ImplementationFile=Tree AnalyserDlg.cpp
Filter=D
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Class=CAboutDlg


[DLG:IDD_TREEANALYSER_DIALOG]
Type=1
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Class=CTreeAnalyserDlg

[DLG:IDD_TREEANALYSER_DIALOG (English (U.S.))]
Type=1
Class=CTreeAnalyserDlg
ControlCount=4
Control1=IDC_STATUS_BAR,edit,1342179456
Control2=IDC_TREE1,SysTreeView32,1350631680
Control3=IDC_EDIT_OUTPUT,edit,1353711748
Control4=IDC_IMAGE_PROPERTIES,edit,1484849156

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=CAboutDlg
ControlCount=8
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342177294
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342177294

[MNU:IDR_MENU1]
Type=1
Class=CTreeAnalyserDlg
Command1=ID_FILE_NEWPROJECT
Command2=ID_FILE_OPENPROJECT
Command3=ID_FILE_SAVEPROJECT
Command4=ID_FILE_SAVEAS
Command5=ID_FILE_1
Command6=ID_FILE_2
Command7=ID_FILE_3
Command8=ID_FILE_4
Command9=ID_FILE_EXIT
Command10=ID_PROJECT_ADDIMAGE
Command11=ID_PROJECT_ESTIMATETREEDIMENSION
Command12=ID_PROJECT_VOLUME
Command13=ID_PROJECT_LAD
Command14=ID_PROJECT_RATP
Command15=ID_PROJECT_EXPORTDATA
Command16=ID_PROJECT_ANALYSEALL
Command17=ID_PROJECT_SETTING
Command18=ID_PROJECT_CLEAROUTPUT
Command19=ID_IMAGE_REGISTERIMAGE
Command20=ID_IMAGE_SAVEPIXELVALUE
Command21=ID_IMAGE_ADDGRID
Command22=ID_IMAGE_CALCUALTEGAPFRACTION
Command23=ID_IMAGE_DELETE
Command24=ID_IMAGE_EDITPARAMETERS
Command25=ID_HELP_ABOUT
Command26=ID_TEST_PICTURECOMBINAMTION
CommandCount=26

[DLG:IDD_DLG_ADD_IMAGE]
Type=1
Class=CDlgAdd
ControlCount=39
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDIT_FNAME,edit,1484849280
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,button,1342177287
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_EDIT_DISTANCE,edit,1350631552
Control11=IDC_EDIT_HEIGHT,edit,1350631552
Control12=IDC_EDIT_DIRECTION,edit,1484849280
Control13=IDC_STATIC,button,1342177287
Control14=IDC_STATIC,static,1342308352
Control15=IDC_STATIC,static,1342308352
Control16=IDC_EDIT_INC,edit,1350631552
Control17=IDC_EDIT_AZ,edit,1350631552
Control18=IDC_EDIT_ROT,edit,1350631552
Control19=IDC_STATIC,button,1342177287
Control20=IDC_COMBO_CAMERA,combobox,1344471043
Control21=IDC_STATIC,button,1342177287
Control22=IDC_STATIC,static,1342308352
Control23=IDC_EDIT_FOCAL,edit,1350631552
Control24=IDC_EDIT_PICH,edit,1484849280
Control25=IDC_STATIC,static,1342308352
Control26=IDC_STATIC,static,1342308352
Control27=IDC_EDIT_PICW,edit,1484849280
Control28=IDC_STATIC,static,1342308352
Control29=IDC_EDIT_CCDSIZE,edit,1484849280
Control30=IDC_STATIC,static,1342308352
Control31=IDC_EDIT_CURVE,edit,1484849280
Control32=IDC_PICTYPE,button,1342308361
Control33=IDC_RADIO2,button,1342177289
Control34=IDC_STATIC,static,1342308352
Control35=IDC_EDIT_PICNAME,edit,1350631552
Control36=IDC_btnBrowse,button,1342242816
Control37=IDC_STATIC,static,1342308352
Control38=IDC_EDIT_VIEWANGLE,edit,1484849280
Control39=IDC_COMBO_DIRECTION,combobox,1344471043

[CLS:CDlgAdd]
Type=0
HeaderFile=DlgAdd.h
ImplementationFile=DlgAdd.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CDlgAdd

[DLG:IDD_DLG_PROGRAM_SET]
Type=1
Class=CProgramSettingDlg
ControlCount=45
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_EDIT_IMAGE,edit,1350631552
Control6=IDC_EDIT_OUTPUT,edit,1350631552
Control7=IDC_STATIC,static,1342308352
Control8=IDC_EDIT_CELLSIZE_X,edit,1350639744
Control9=IDC_STATIC,static,1342308352
Control10=IDC_EDIT_CELLSIZE_Y,edit,1350639744
Control11=IDC_EDIT_CELLSIZE_Z,edit,1350639744
Control12=IDC_STATIC,static,1342308352
Control13=IDC_STATIC,static,1342308352
Control14=IDC_STATIC,static,1342308352
Control15=IDC_EDIT_ENLARGER,edit,1350639744
Control16=IDC_STATIC,button,1342177287
Control17=IDC_STATIC,static,1342308352
Control18=IDC_EDIT_RAYPX,edit,1350639744
Control19=IDC_EDIT_RAYPY,edit,1350639744
Control20=IDC_STATIC,static,1342308352
Control21=IDC_STATIC,static,1342308352
Control22=IDC_STATIC,button,1342177287
Control23=IDC_CHECK_BOX,button,1342242819
Control24=IDC_CHECK_VOLUMELOG,button,1342242819
Control25=IDC_STATIC,button,1342177287
Control26=IDC_BTN_DEFAULT,button,1342242816
Control27=IDC_STATIC,button,1476395015
Control28=IDC_RADIO1,button,1476395017
Control29=IDC_RADIO3,button,1476395017
Control30=IDC_RADIO4,button,1476395017
Control31=IDC_STATIC,button,1476395015
Control32=IDC_EDIT1,edit,1484849280
Control33=IDC_STATIC,static,1476526080
Control34=IDC_STATIC,static,1476526080
Control35=IDC_EDIT_RAYPX2,edit,1484857472
Control36=IDC_EDIT_RAYPY2,edit,1484857472
Control37=IDC_STATIC,static,1476526080
Control38=IDC_STATIC,static,1476526080
Control39=IDC_STATIC,button,1476395015
Control40=IDC_CHECK1,button,1476460547
Control41=IDC_CHECK2,button,1476460547
Control42=IDC_CHECK3,button,1476460547
Control43=IDC_STATIC,static,1476526080
Control44=IDC_EDIT2,edit,1484849280
Control45=IDC_CHECK4,button,1476460547

[CLS:CProgramSettingDlg]
Type=0
HeaderFile=ProgramSettingDlg.h
ImplementationFile=ProgramSettingDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CProgramSettingDlg
VirtualFilter=dWC

[DLG:IDD_PICTURE_COMBINATION]
Type=1
Class=CTestDlg
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_EDIT_NUM,edit,1350631552
Control4=IDC_STATIC,static,1342308352

[CLS:CTestDlg]
Type=0
HeaderFile=TestDlg.h
ImplementationFile=TestDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_EDIT_NUM
VirtualFilter=dWC

