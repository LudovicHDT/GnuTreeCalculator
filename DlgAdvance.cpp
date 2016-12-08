// DlgAdvance.cpp : implementation file
//

#include "stdafx.h"
#include "Tree Analyser.h"
#include "DlgAdvance.h"
#include ".\dlgadvance.h"


// CDlgAdvance dialog

IMPLEMENT_DYNAMIC(CDlgAdvance, CDialog)
CDlgAdvance::CDlgAdvance(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAdvance::IDD, pParent)
	, m_LimitEle(45)
	, m_enlarge(5)
	, m_raypx(100)
	, m_raypy(100)
	, m_maxLAD(30)
	, RadioGap1(0)
	, minimumGap(0.001)
	, IfSetMaxLAD(FALSE)
	, m_leaf_dispersion_option(0)
	, m_leaf_dispersion(0)
	, m_gpx(0)
	, m_gpy(0)
	, m_gap_average_option(0)
, gap_option(false)
, is_divide_pza(FALSE)
, division_horizontal(1)
, division_vertical(1)
{
	flagDisable=0;
	m_leaf_dispersion_option=0;
	m_leaf_dispersion=1;
	fix_pza=17.0;
	defualt_pza=fix_pza;
}

CDlgAdvance::~CDlgAdvance()
{
}

void CDlgAdvance::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ELE, m_LimitEle);
	DDV_MinMaxInt(pDX, m_LimitEle, 30, 90);
	DDX_Text(pDX, IDC_EDIT_ENLARGER, m_enlarge);
	DDV_MinMaxInt(pDX, m_enlarge, 0, 50);
	DDX_Text(pDX, IDC_EDIT_RAYPX, m_raypx);
	DDV_MinMaxInt(pDX, m_raypx, 1, 300);
	DDX_Text(pDX, IDC_EDIT_RAYPY, m_raypy);
	DDV_MinMaxInt(pDX, m_raypy, 1, 300);
	DDX_Text(pDX, IDC_EDIT_MAXLAD, m_maxLAD);
	DDV_MinMaxDouble(pDX, m_maxLAD, 0, 100);
	DDX_Radio(pDX, IDC_RADIO1, RadioGap1);
	DDX_Text(pDX, IDC_EDIT1, minimumGap);
	DDV_MinMaxDouble(pDX, minimumGap, 0, 0.1);
	DDX_Check(pDX, IDC_CHK_SET_LAD, IfSetMaxLAD);

	DDX_Control(pDX, IDC_EDIT1, ctrl_txtFixedGap);
	DDX_Control(pDX, IDC_RADIO1, ctrl_RadioGap);
	DDX_Control(pDX, IDC_EDIT_MAXLAD, ctrl_txt_maxLAD);
	DDX_Control(pDX, IDC_CHK_SET_LAD, ctrl_checkLAD);

	DDX_Control(pDX, IDOK, ctrl_btn_ok);
	DDX_Control(pDX, IDC_BTN_DEFAULT, ctrl_btn_default);
	DDX_Control(pDX, IDC_EDIT_CLUMP, ctrl_input_clumping);
	DDX_Radio(pDX, IDC_RADIO_NOCLUMP, m_leaf_dispersion_option);
	DDX_Text(pDX, IDC_EDIT_CLUMP, m_leaf_dispersion);
	DDV_MinMaxDouble(pDX, m_leaf_dispersion, 0.001, 100);
	DDX_Text(pDX, IDC_EDIT_GPX, m_gpx);
	DDV_MinMaxInt(pDX, m_gpx, 1, 10000);
	DDX_Text(pDX, IDC_EDIT_GPY, m_gpy);
	DDV_MinMaxInt(pDX, m_gpy, 1, 10000);
	//DDX_Control(pDX, IDC_COMBO3, ctrl_combo3);
	//DDX_CBIndex(pDX, IDC_COMBO3, m_gap_average_option);
	DDX_Radio(pDX, IDC_RADIO5, gap_option);
	DDX_Control(pDX, IDC_EDIT_GPY, ctrlGPY);
	DDX_Control(pDX, IDC_EDIT_GPX, ctrlGPX);
	DDX_Control(pDX, IDC_EDIT_PZA, ctrlPZA);
	DDX_Text(pDX, IDC_EDIT_PZA, fix_pza);
	DDV_MinMaxDouble(pDX, fix_pza, 1.0, 500.0);
	DDX_Check(pDX, IDC_CHECK3, is_divide_pza);
	DDX_Text(pDX, IDC_EDIT4, division_horizontal);
	DDV_MinMaxInt(pDX, division_horizontal, 1, 100);
	DDX_Text(pDX, IDC_EDIT6, division_vertical);
	DDV_MinMaxInt(pDX, division_vertical, 1, 100);
	DDX_Control(pDX, IDC_EDIT4, ctrl_subdivision_horizontal);
	DDX_Control(pDX, IDC_EDIT6, ctrl_subdivision_vertical);
}


BEGIN_MESSAGE_MAP(CDlgAdvance, CDialog)
	ON_BN_CLICKED(IDC_BTN_DEFAULT, OnBnClickedBtnDefault)
	ON_BN_CLICKED(IDC_CHK_SET_LAD, OnBnClickedChkSetLad)
	ON_BN_CLICKED(IDC_RADIO2, OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO1, OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO_CUSTOMCLUMP, OnBnClickedRadioCustomclump)
	ON_BN_CLICKED(IDC_RADIO_AUTOCLUMP, OnBnClickedRadioAutoclump)
	ON_BN_CLICKED(IDC_RADIO_NOCLUMP, OnBnClickedRadioNoclump)
	ON_EN_CHANGE(IDC_EDIT_GPX, OnEnChangeEditGpx)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO6, OnBnClickedRadio6)
	ON_BN_CLICKED(IDC_RADIO5, OnBnClickedRadio5)
	ON_BN_CLICKED(IDC_RADIO_PZA, OnBnClickedRadio7)
	ON_BN_CLICKED(IDC_CHECK3, OnBnClickedCheck3)
	ON_BN_CLICKED(IDC_Btn_SetPZA, OnBnClickedBtnSetpza)
END_MESSAGE_MAP()


// CDlgAdvance message handlers

void CDlgAdvance::DisableControls()
{
	this->ctrl_btn_ok.EnableWindow(0);
	this->ctrl_btn_default.EnableWindow(0);

	/*
	this->ctrl_checkLAD.EnableWindow(0);
	this->ctrl_RadioGap.EnableWindow(0);
	this->ctrl_txt_maxLAD.EnableWindow(0);
	this->ctrl_txtFixedGap.EnableWindow(0);
	*/
}

BOOL CDlgAdvance::OnInitDialog() 
{
	UpdateData(false);
	if(RadioGap1==1){ctrl_txtFixedGap.EnableWindow(true);}
	else{ctrl_txtFixedGap.EnableWindow(false);}
	OnBnClickedChkSetLad();

	if(this->flagDisable==1){DisableControls();}
	SetCtrlInputClumping();
//	ctrl_combo3.AddString("Fixed zone");
//	ctrl_combo3.AddString("Gliding zone");
	UpdateData(false);

	if(gap_option==0)
	{
		ctrlGPX.EnableWindow(0);
		ctrlGPY.EnableWindow(0);
		//ctrl_combo3.EnableWindow(0);
	}
	else
	{
		ctrlGPX.EnableWindow(1);
		ctrlGPY.EnableWindow(1);
		//ctrl_combo3.EnableWindow(1);
	}

	if(this->gap_option==2)
	{
		ctrlGPX.EnableWindow(0);
		ctrlGPY.EnableWindow(0);
		ctrlPZA.EnableWindow(1);
	}

	OnBnClickedCheck3();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CDlgAdvance::OnBnClickedBtnDefault()
{
	m_LimitEle=45;
	m_enlarge = 5;
	m_maxLAD=30;
	RadioGap1=0;
	IfSetMaxLAD=1;
	m_leaf_dispersion_option=0;
	m_leaf_dispersion=1;
	this->gap_option=0;

	this->minimumGap=0.001;
	this->m_raypx=3;
	this->m_raypy=3;

	this->is_divide_pza=TRUE;
	this->division_horizontal=2;
	this->division_vertical=2;

	if(RadioGap1==1){ctrl_txtFixedGap.EnableWindow(1);}
	else{ctrl_txtFixedGap.EnableWindow(0);}
	OnBnClickedChkSetLad();
	SetCtrlInputClumping();
	this->defualt_pza=17;
	this->fix_pza=17;
	UpdateData(false);

}

void CDlgAdvance::OnBnClickedChkSetLad()
{
	int if_check=ctrl_checkLAD.GetCheck();
	if(if_check==1){ this->ctrl_txt_maxLAD.EnableWindow(1);}
	else { this->ctrl_txt_maxLAD.EnableWindow(0);}
}

void CDlgAdvance::OnBnClickedRadio2()
{
	this->ctrl_txtFixedGap.EnableWindow(1);
}

void CDlgAdvance::OnBnClickedRadio1()
{
	this->ctrl_txtFixedGap.EnableWindow(0);
}

void CDlgAdvance::OnBnClickedRadioCustomclump()
{
	this->ctrl_input_clumping.EnableWindow(1);
}

void CDlgAdvance::OnBnClickedRadioAutoclump()
{
	this->ctrl_input_clumping.EnableWindow(0);
}

void CDlgAdvance::OnBnClickedRadioNoclump()
{
	this->ctrl_input_clumping.EnableWindow(0);
}


void CDlgAdvance::SetCtrlInputClumping()
{
	if(this->m_leaf_dispersion_option==2){	this->ctrl_input_clumping.EnableWindow(1);}
	else{this->ctrl_input_clumping.EnableWindow(0);}
}


void CDlgAdvance::OnEnChangeEditGpx()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	//if(check_cube_value==1){
		CString ms;
		ctrlGPX.GetWindowText(ms);
		ctrlGPY.SetWindowText(ms);
	//}

	// TODO:  Add your control notification handler code here
}

void CDlgAdvance::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialog::OnOK();
}

//! A function to check if user prees specified key
/*!
*	Created: 21-Sep-04
*	\n Last modification: 21-Sep-04.\n
*	Called when user press key
*/
BOOL CDlgAdvance::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN)// ถ้ามีการกดคีย์
	{
		if(pMsg->wParam == VK_RETURN)
		{
			OnOK();
			pMsg->wParam=0; //prevent exit from dialog setting
		}
		if(pMsg->wParam == VK_ESCAPE)
		{
			OnCancel();
			pMsg->wParam=0; //prevent exit from dialog setting
		}
	}
	return 0; // ให้ออกจากฟังก์ชั่นนี้b //return CDialog::PreTranslateMessage(pMsg);
} 
void CDlgAdvance::OnBnClickedRadio6()
{
	ctrlGPX.EnableWindow(1);
	ctrlGPY.EnableWindow(1);
	ctrlPZA.EnableWindow(0);
}

void CDlgAdvance::OnBnClickedRadio5()
{
	ctrlGPX.EnableWindow(0);
	ctrlGPY.EnableWindow(0);
	ctrlPZA.EnableWindow(0);
}

void CDlgAdvance::OnBnClickedRadio7()
{
	ctrlGPX.EnableWindow(0);
	ctrlGPY.EnableWindow(0);
	ctrlPZA.EnableWindow(1);
}

void CDlgAdvance::OnBnClickedCheck3()
{
	UpdateData(TRUE);
	if(this->is_divide_pza==TRUE)
	{
		this->ctrl_subdivision_horizontal.EnableWindow(1);
		this->ctrl_subdivision_vertical.EnableWindow(1);
	}
	else
	{
		this->ctrl_subdivision_horizontal.EnableWindow(0);
		this->ctrl_subdivision_vertical.EnableWindow(0);
	}
}


void CDlgAdvance::OnBnClickedBtnSetpza()
{
		CInputDlg dlg;
		dlg.DlgTitle="Defualt vaule of PZA";
		dlg.InputLabelText="Enter defualt value of PZA:";
		dlg.m_input=defualt_pza;

		if (dlg.DoModal()==IDOK)
		{
			defualt_pza=dlg.m_input;
			fix_pza=defualt_pza;
			this->UpdateData(false);
		}
}
void CDlgAdvance::OnOK()
{
}
