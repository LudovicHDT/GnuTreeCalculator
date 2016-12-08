// AddCameraDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Tree Analyser.h"
#include "AddCameraDlg.h"
#include "EachCamera.h"
#include ".\addcameradlg.h"


// CAddCameraDlg dialog

IMPLEMENT_DYNAMIC(CAddCameraDlg, CDialog)
CAddCameraDlg::CAddCameraDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAddCameraDlg::IDD, pParent)
	, m_camera_name(_T("New camera"))
	, m_calibration_value(0)
	, m_min_fc(1)
	, m_max_fc(100)
	, m_calibration_date(_T("Not defined"))
	, m_owner(_T("Not defined"))
	, m_note(_T("None"))
{
	dlg_title="";
	show_option=0;
}

CAddCameraDlg::~CAddCameraDlg()
{
}


void CAddCameraDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_MODEL, m_camera_name);
	DDV_MaxChars(pDX, m_camera_name, 50);
	DDX_Text(pDX, IDC_CALIBRATION_VALUE, m_calibration_value);
	DDV_MinMaxDouble(pDX, m_calibration_value, 1, 100);
	DDX_Text(pDX, IDC_MIN_FC, m_min_fc);
	DDV_MinMaxDouble(pDX, m_min_fc, 1, 100);
	DDX_Text(pDX, IDC_MAX_FC, m_max_fc);
	DDV_MinMaxDouble(pDX, m_max_fc, 1, 100);
	DDX_Text(pDX, IDC_DATE, m_calibration_date);
	DDX_Text(pDX, IDC_OWNER, m_owner);
	DDX_Text(pDX, IDC_NOTE, m_note);
	DDX_Control(pDX, IDOK, ctrlBtnOK);
	DDX_Control(pDX, IDC_MODEL, ctrlEditModel);
	DDX_Control(pDX, IDC_CALIBRATION_VALUE, ctrlEditCalibrationValue);
	DDX_Control(pDX, IDC_MIN_FC, ctrlEditMinFC);
	DDX_Control(pDX, IDC_MAX_FC, ctrlEditMaxFC);
	DDX_Control(pDX, IDC_DATE, ctrlEditDate);
	DDX_Control(pDX, IDC_OWNER, ctrlEditOwner);
	DDX_Control(pDX, IDC_NOTE, ctrlEditNote);
}


BEGIN_MESSAGE_MAP(CAddCameraDlg, CDialog)
END_MESSAGE_MAP()


// CAddCameraDlg message handlers

BOOL CAddCameraDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	this->SetWindowText(dlg_title);
	if(show_option)
	{//if view information
		ctrlBtnOK.SetWindowText("Edit");
		this->DisableAll();
		edit_option=0;
	}
	else
	{//if add new camera
		ctrlBtnOK.SetWindowText("OK");
		edit_option=1;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CAddCameraDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	if(edit_option)
	{//if editing mode mean that user click OK after add new camera or edit
		CDialog::OnOK();
	}
	else
	{//user click to edit
		edit_option=1;
		ctrlBtnOK.SetWindowText("OK");
		EnabledAll();
	}
}

void CAddCameraDlg::DisableAll(void)
{
	ctrlEditModel.EnableWindow(0);
	ctrlEditCalibrationValue.EnableWindow(0);
	ctrlEditMinFC.EnableWindow(0);
	ctrlEditMaxFC.EnableWindow(0);
	ctrlEditDate.EnableWindow(0);
	ctrlEditOwner.EnableWindow(0);
	ctrlEditNote.EnableWindow(0);}

void CAddCameraDlg::EnabledAll(void)
{
	ctrlEditModel.EnableWindow(1);
	ctrlEditCalibrationValue.EnableWindow(1);
	ctrlEditMinFC.EnableWindow(1);
	ctrlEditMaxFC.EnableWindow(1);
	ctrlEditDate.EnableWindow(1);
	ctrlEditOwner.EnableWindow(1);
	ctrlEditNote.EnableWindow(1);
}
