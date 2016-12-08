// ProgramSettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Tree Analyser.h"
#include "ProgramSettingDlg.h"
#include ".\programsettingdlg.h"
#include "DlgAdvance.h"
#include "FolderDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProgramSettingDlg dialog


CProgramSettingDlg::CProgramSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProgramSettingDlg::IDD, pParent)
	, m_LimitEle(0)
	, m_distribution(0)
	, check_cube_value(FALSE)
//	, lapx(3)
//	, lapy(3)
	, gpx(100)
	, gpy(100)
	, m_leaf_distribution(0)
	, gap_average_option(0)
	, m_mean_leaf_inc(0)
	, m_dis_file(_T(""))
	, m_mean_leaf_area(0)
	, m_inversion_method(0)
	, m_ChkLAD(FALSE)
{
	//{{AFX_DATA_INIT(CProgramSettingDlg)
	m_OutputLocation = _T("");
	m_cellSize_dx = 0.25;
	m_cellSize_dy = 0.25;
	m_cellSize_dz = 0.25;
	m_enlarge = 5;
	m_LimitEle=45;
	m_raypx = 1;
	m_raypy = 1;
	m_ifoutputbox = TRUE;
	m_iflogvolume = FALSE;
	m_distribution=2;
	check_cube_value=1;
	ZeroGapManage=0;
	minimumGap=0.0001;
	IfSetMaxLAD=0;
	MaxLAD=30;
	m_leaf_dispersion_option=0;
	m_leaf_dispersion=1;
	gap_option=0;
	m_mean_leaf_area=0;

	//}}AFX_DATA_INIT
	flagDisableControls=0;
	fix_pza=17;
	default_pza=17;

	//Variable added for subdivision of PZA
	is_divide_pza=false;
	division_horizontal=1;
	division_vertical=1;


}


void CProgramSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProgramSettingDlg)
	DDX_Control(pDX, IDC_EDIT_OUTPUT, ctrl_output_location);
	DDX_Text(pDX, IDC_EDIT_OUTPUT, m_OutputLocation);
	DDX_Text(pDX, IDC_EDIT_CELLSIZE_X, m_cellSize_dx);
	DDV_MinMaxDouble(pDX, m_cellSize_dx, 0.1, 1000.0);
	DDX_Text(pDX, IDC_EDIT_CELLSIZE_Y, m_cellSize_dy);
	DDV_MinMaxDouble(pDX, m_cellSize_dy, 0.1, 1000.);
	DDX_Text(pDX, IDC_EDIT_CELLSIZE_Z, m_cellSize_dz);
	DDV_MinMaxDouble(pDX, m_cellSize_dz, 0.1, 1000.);
	DDX_Check(pDX, IDC_CHECK_BOX, m_ifoutputbox);
	DDX_Check(pDX, IDC_CHECK_VOLUMELOG, m_iflogvolume);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_CHECK5, ctrl_cube_dx);

	DDX_Control(pDX, IDC_CHECK_VOLUMELOG, ctrl_logfile);
	DDX_Control(pDX, IDC_CHECK_BOX, ctrl_VgsTAR);
	DDX_Control(pDX, IDC_BTN_DEFAULT, ctrl_btn_default);
	DDX_Control(pDX, IDOK, ctrl_btn_ok);

	DDX_Check(pDX, IDC_CHECK5, check_cube_value);

	DDX_Control(pDX, IDC_EDIT_CELLSIZE_Y, ctrl_dy);
	DDX_Control(pDX, IDC_EDIT_CELLSIZE_X, ctrl_dx);
	DDX_Control(pDX, IDC_EDIT_CELLSIZE_Z, ctrl_dz);
	DDX_Control(pDX, IDC_G_SPHERICAL, ctrl_G_spherical);
	DDX_Control(pDX, IDC_COMBO_LDISTRIBUTION, ctrl_leaf_distribution);
	DDX_CBIndex(pDX, IDC_COMBO_LDISTRIBUTION, m_leaf_distribution);
	DDX_Control(pDX, IDC_MA, ctrl_MA);
	DDX_Control(pDX, IDC_DEG, ctrl_deg);
	DDX_Control(pDX, IDC_BROWSE, ctrl_browse);
	DDX_Control(pDX, IDC_DISTRIBUTION_FILE, ctrl_dis_file);
	DDX_Text(pDX, IDC_MA, m_mean_leaf_inc);
	DDV_MinMaxDouble(pDX, m_mean_leaf_inc, 0, 90);
	DDX_Text(pDX, IDC_DISTRIBUTION_FILE, m_dis_file);
	DDX_Text(pDX, IDC_EDIT3, m_mean_leaf_area);
	DDV_MinMaxDouble(pDX, m_mean_leaf_area, 0, 10000);
	DDX_Control(pDX, IDC_RADIO1, ctrl_inversion_method);
	DDX_Radio(pDX, IDC_RADIO1, m_inversion_method);
	DDX_Control(pDX, IDC_EDIT3, ctrlLeafArea);
	DDX_Check(pDX, IDC_CHK_LAD, m_ChkLAD);
}


BEGIN_MESSAGE_MAP(CProgramSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CProgramSettingDlg)
	//ON_BN_CLICKED(IDC_BTN_IMAGE, OnBtnImage)
	ON_BN_CLICKED(IDC_BTN_DEFAULT, OnBtnDefault)
	//}}AFX_MSG_MAP
ON_EN_CHANGE(IDC_EDIT_CELLSIZE_X, OnEnChangeEditCellsizeX)
ON_BN_CLICKED(IDC_CHECK5, OnBnClickedCheck5)
ON_CBN_SELCHANGE(IDC_COMBO_LDISTRIBUTION, OnCbnSelchangeComboLdistribution)
ON_BN_CLICKED(IDC_BROWSE, OnBnClickedBrowse)
ON_BN_CLICKED(IDOK, OnBnClickedOk)
ON_BN_CLICKED(IDC_BUTTON_ADVANCE, OnBnClickedBtnAdvance)

ON_BN_CLICKED(IDC_RADIO1, OnBnClickedRadio1)
ON_BN_CLICKED(IDC_RADIO2, OnBnClickedRadio2)
ON_BN_CLICKED(IDC_RADIO3, OnBnClickedRadio3)
ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProgramSettingDlg message handlers

void CProgramSettingDlg::OnBtnImage() 
{
	/*
     CFileDialog m_ldFile(TRUE, ".tre", CurrentPath+"*.tre",
          OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);

	if (m_ldFile.DoModal() == IDOK)*/
}


void CProgramSettingDlg::OnBtnDefault() 
{
	m_cellSize_dx = 20;
	m_cellSize_dy = 20;
	m_cellSize_dz = 20;
	m_ifoutputbox = FALSE;
	m_iflogvolume = TRUE;
	check_cube_value=1;
	m_distribution=5;
	m_inversion_method=0;
	m_ChkLAD=1,
	UpdateData(FALSE);
	OnBnClickedCheck5();
	SetctrlLeafArea();	
	OnCbnSelchangeComboLdistribution();
}



void CProgramSettingDlg::OnEnChangeEditCellsizeX()
{
	if(check_cube_value==1){
		CString ns;
		ctrl_dx.GetWindowText(ns);
		ctrl_dy.SetWindowText(ns);
		ctrl_dz.SetWindowText(ns);
	}
	
}

//! A function to check if user prees specified key
/*!
*	Created: 21-Sep-04
*	\n Last modification: 21-Sep-04.\n
*	Called when user press key
*/
BOOL CProgramSettingDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN)// ถ้ามีการกดคีย์
	{
		if(pMsg->wParam == VK_RETURN)
		{
			OnOK();
		}
		if(pMsg->wParam == VK_ESCAPE)
		{
			OnCancel();
		}
	}
	return 0; // ให้ออกจากฟังก์ชั่นนี้ //return CDialog::PreTranslateMessage(pMsg);
}




void CProgramSettingDlg::OnBnClickedCheck5()
{
		int a1=ctrl_cube_dx.GetCheck();
		check_cube_value=a1;
	if(a1==1)//cube was set
	{
		ctrl_dy.EnableWindow(0);
		ctrl_dz.EnableWindow(0);
		CString ms;
		ctrl_dx.GetWindowText(ms);
		ctrl_dy.SetWindowText(ms);
		ctrl_dz.SetWindowText(ms);
	}
	else
	{
		ctrl_dy.EnableWindow(1);
		ctrl_dz.EnableWindow(1);
	}
}



void CProgramSettingDlg::OnCbnSelchangeComboLdistribution()
{
	int curr_la_dis=this->ctrl_leaf_distribution.GetCurSel();
	//CString ss;
	//ctrl_leaf_distribution.GetLBText(curr_la_dis,ss);
	//CString ms;
	//ms.Format("Selection=%i",curr_la_dis);
	//MessageBox(ms);
	switch(curr_la_dis)
	{
	case 0:
		ctrl_G_spherical.SetWindowText("Mean leaf angle:");
		ctrl_MA.ShowWindow(1);
		ctrl_deg.ShowWindow(1);
		ctrl_browse.ShowWindow(SW_HIDE);
		ctrl_dis_file.ShowWindow(0);
		break;
	case 2: //Erectophile
		ctrl_G_spherical.SetWindowText("g(A)=2/pi(1-cos(A))  MA=63");
		ctrl_MA.ShowWindow(0);
		ctrl_deg.ShowWindow(0);
		ctrl_browse.ShowWindow(SW_HIDE);
		ctrl_dis_file.ShowWindow(0);
		break;
	case 3: //Extremophile
		ctrl_G_spherical.SetWindowText("g(a)=2/pi(1+cos(4a))  MA=45");
		ctrl_MA.ShowWindow(0);
		ctrl_deg.ShowWindow(0);
		ctrl_browse.ShowWindow(SW_HIDE);
		ctrl_dis_file.ShowWindow(0);
		break;
	case 4: //Plagiophile
		ctrl_G_spherical.SetWindowText("g(a)=2/pi(1-cos(4a))  MA=45");
		ctrl_MA.ShowWindow(0);
		ctrl_deg.ShowWindow(0);
		ctrl_browse.ShowWindow(SW_HIDE);
		ctrl_dis_file.ShowWindow(0);
		break;
	case 5: //Planophile
		ctrl_G_spherical.SetWindowText("g(a)=2/pi(1+cos(2a))  MA=27");
		ctrl_MA.ShowWindow(0);
		ctrl_deg.ShowWindow(0);
		ctrl_browse.ShowWindow(SW_HIDE);
		ctrl_dis_file.ShowWindow(0);
		break;
	case 6:
        
		ctrl_G_spherical.SetWindowText("G = 0.5\ng = sin x");
		ctrl_MA.ShowWindow(0);
		ctrl_deg.ShowWindow(0);
		ctrl_browse.ShowWindow(SW_HIDE);
		ctrl_dis_file.ShowWindow(0);
		break;
	case 7:
		ctrl_MA.ShowWindow(0);
		ctrl_deg.ShowWindow(0);
		if(!ctrl_browse.IsWindowVisible()) {ctrl_browse.ShowWindow(SW_SHOW);}
		else {ctrl_browse.ShowWindow(SW_HIDE);ctrl_browse.ShowWindow(SW_SHOW);}
		ctrl_dis_file.ShowWindow(1);
		ctrl_G_spherical.SetWindowText("");
		break;
	default:
		//ctrl_G_spherical.ShowWindow(1);
		ctrl_G_spherical.SetWindowText("Not Available");
		ctrl_MA.ShowWindow(0);
		ctrl_deg.ShowWindow(0);
		ctrl_browse.ShowWindow(SW_HIDE);
		ctrl_dis_file.ShowWindow(0);
	}
}

void CProgramSettingDlg::SetShow()
{
	if(m_leaf_distribution==6)
	{
		ctrl_G_spherical.ShowWindow(1);
	}
	else
	{
		ctrl_G_spherical.ShowWindow(0);
	}
}
void CProgramSettingDlg::OnBnClickedBrowse()
{
      static char BASED_CODE szFilter[] = "Leaf distribution files (*.dis)|*.dis||";
      // Create the File Open dialog
	  CString dis_filename;
      CFileDialog m_ldFile(TRUE, ".bmp",CurrentPath+"*.dis", 
          OFN_HIDEREADONLY |OFN_OVERWRITEPROMPT, szFilter);
	
     // Show the File Open dialog and capture the result
     if (m_ldFile.DoModal() == IDOK)
     {
		// Get the filename selected
         dis_filename = m_ldFile.GetPathName();
		 ctrl_dis_file.SetWindowText(dis_filename);
	 }
}


BOOL CProgramSettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog(); //call to prevent assertion failed

	int a1=this->check_cube_value;
	if(a1==1)//cube was set
	{
		ctrl_dy.EnableWindow(0);
		ctrl_dz.EnableWindow(0);
	}
	else
	{
		ctrl_dy.EnableWindow(1);
		ctrl_dz.EnableWindow(1);
	}

	switch(m_leaf_distribution)
	{
	case 0:
		ctrl_G_spherical.SetWindowText("Mean leaf angle:");
		ctrl_MA.ShowWindow(1);
		ctrl_deg.ShowWindow(1);
		ctrl_browse.ShowWindow(0);
		ctrl_dis_file.ShowWindow(0);
		break;
	case 2: //Erectophile
		ctrl_G_spherical.SetWindowText("g(a)=2/pi(1-cos(2a))  MA=63");
		ctrl_MA.ShowWindow(0);
		ctrl_deg.ShowWindow(0);
		ctrl_browse.ShowWindow(SW_HIDE);
		ctrl_dis_file.ShowWindow(0);
		break;
	case 3: //Extremophile
		ctrl_G_spherical.SetWindowText("g(a)=2/pi(1+cos(4a))  MA=45");
		ctrl_MA.ShowWindow(0);
		ctrl_deg.ShowWindow(0);
		ctrl_browse.ShowWindow(SW_HIDE);
		ctrl_dis_file.ShowWindow(0);
		break;
	case 4: //Plagiophile
		ctrl_G_spherical.SetWindowText("g(a)=2/pi(1-cos(4a))  MA=45");
		ctrl_MA.ShowWindow(0);
		ctrl_deg.ShowWindow(0);
		ctrl_browse.ShowWindow(SW_HIDE);
		ctrl_dis_file.ShowWindow(0);
		break;
	case 5: //Planophile
		ctrl_G_spherical.SetWindowText("g(a)=2/pi(1+cos(2a))  MA=27");
		ctrl_MA.ShowWindow(0);
		ctrl_deg.ShowWindow(0);
		ctrl_browse.ShowWindow(SW_HIDE);
		ctrl_dis_file.ShowWindow(0);
		break;
	case 6:
        
		ctrl_G_spherical.SetWindowText("G = 0.5\ng = sin x");
		ctrl_MA.ShowWindow(0);
		ctrl_deg.ShowWindow(0);
		ctrl_browse.ShowWindow(0);
		ctrl_dis_file.ShowWindow(0);
		break;
	case 7:
		ctrl_MA.ShowWindow(0);
		ctrl_deg.ShowWindow(0);
		ctrl_browse.ShowWindow(1);
		ctrl_dis_file.ShowWindow(1);
		ctrl_G_spherical.SetWindowText("");
		break;
	default:
		//ctrl_G_spherical.ShowWindow(1);
		ctrl_G_spherical.SetWindowText("Not Available");
		ctrl_MA.ShowWindow(0);
		ctrl_deg.ShowWindow(0);
		ctrl_browse.ShowWindow(0);
		ctrl_dis_file.ShowWindow(0);
	}
	SetctrlLeafArea();
	if(this->flagDisableControls==1){DisableBtnOK();}
	return TRUE;
}

void CProgramSettingDlg::DisableBtnOK()
{
	ctrl_btn_ok.EnableWindow(0);
}
void CProgramSettingDlg::DisableAll()
{
	
	ctrl_dx.EnableWindow(0);
	ctrl_dy.EnableWindow(0);
	ctrl_dz.EnableWindow(0);
	ctrl_browse.EnableWindow(0);
	ctrl_MA.EnableWindow(0);	
	ctrl_deg.EnableWindow(0);
	ctrl_dis_file.EnableWindow(0);

	ctrl_gpx.EnableWindow(0);
	ctrl_gpy.EnableWindow(0);
	//ctrl_lapx.EnableWindow(0);
	//ctrl_lapy.EnableWindow(0);
	ctrl_G_spherical.EnableWindow(0);
	
	ctrl_leaf_distribution.EnableWindow(0);
	ctrl_zone_type.EnableWindow(0);
	ctrl_output_location.EnableWindow(0);
	ctrl_cube_dx.EnableWindow(0);
	ctrl_logfile.EnableWindow(0);
	ctrl_VgsTAR.EnableWindow(0);

	ctrl_btn_default.EnableWindow(0);
	ctrl_btn_ok.EnableWindow(0);

}

void CProgramSettingDlg::OnBnClickedOk()
{
	this->UpdateData(TRUE);
	if(m_leaf_distribution==1)
	{
		MessageBox("Ellipsoidal leaf angle distribution is not available now.\nPlease select the others.");
	}
	else {OnOK();}
}

void CProgramSettingDlg::OnBnClickedBtnAdvance()
{
	CDlgAdvance dlg;
	dlg.m_LimitEle=m_LimitEle;
	dlg.m_enlarge=m_enlarge;
	dlg.m_raypx=m_raypx;
	dlg.m_raypy=m_raypy;
	dlg.RadioGap1=ZeroGapManage;
	dlg.minimumGap=minimumGap;
	dlg.IfSetMaxLAD=IfSetMaxLAD;
	dlg.m_maxLAD=MaxLAD;
	dlg.m_leaf_dispersion=m_leaf_dispersion;
	dlg.m_leaf_dispersion_option=m_leaf_dispersion_option;

	//move from setting
	dlg.m_gpx=this->gpx;
	dlg.m_gpy=this->gpy;
	dlg.m_gap_average_option=this->gap_average_option;

	dlg.gap_option=this->gap_option;
	dlg.fix_pza=this->fix_pza;
	dlg.defualt_pza=this->default_pza;

	dlg.is_divide_pza=this->is_divide_pza;
	dlg.division_horizontal=this->division_horizontal;
	dlg.division_vertical=this->division_vertical;


	if(this->flagDisableControls==1){dlg.flagDisable=1;}

	if(dlg.DoModal()==IDOK)
	{
		m_LimitEle=dlg.m_LimitEle;
		m_enlarge=dlg.m_enlarge;
		m_raypx=dlg.m_raypx;
		m_raypy=dlg.m_raypy;
		ZeroGapManage=dlg.RadioGap1;
		minimumGap=dlg.minimumGap;
		if(dlg.IfSetMaxLAD==TRUE){IfSetMaxLAD=1;} else{IfSetMaxLAD=0;}
		MaxLAD=dlg.m_maxLAD;
		m_leaf_dispersion=dlg.m_leaf_dispersion;
		m_leaf_dispersion_option=dlg.m_leaf_dispersion_option;
		this->gpx=dlg.m_gpx;
		this->gpy=dlg.m_gpy;
		this->gap_average_option=dlg.m_gap_average_option;
		this->gap_option=dlg.gap_option;
		this->fix_pza=dlg.fix_pza;
		this->is_divide_pza=dlg.is_divide_pza;
		this->division_horizontal=dlg.division_horizontal;
		this->division_vertical=dlg.division_vertical;
		this->default_pza=dlg.defualt_pza;

	}
}

void CProgramSettingDlg::SetctrlLeafArea()
{
	
	this->UpdateData(TRUE);
	switch (this->m_inversion_method)
	{
	case 0:
		ctrlLeafArea.EnableWindow(1);
		break;
	case 1:
		ctrlLeafArea.EnableWindow(1);
		break;
	case 2:
		ctrlLeafArea.EnableWindow(0);
		break;
	}

}

void CProgramSettingDlg::OnBnClickedRadio1()
{
	//SetctrlLeafArea();
}


void CProgramSettingDlg::OnBnClickedRadio2()
{
	//SetctrlLeafArea();
}

void CProgramSettingDlg::OnBnClickedRadio3()
{
	//SetctrlLeafArea();
}

void CProgramSettingDlg::OnBnClickedButton1()
{
	CFolderDialog dlg;
	if(dlg.DoModal()==IDOK)
	{
		//CString ms=dlg.GetPathName();
		//MessageBox(ms);
		this->m_OutputLocation=dlg.GetPathName()+"\\";
		UpdateData(false);
	}


}
