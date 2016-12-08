// InputDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Tree Analyser.h"
#include "InputDlg.h"
#include ".\inputdlg.h"


// CInputDlg dialog

IMPLEMENT_DYNAMIC(CInputDlg, CDialog)
CInputDlg::CInputDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInputDlg::IDD, pParent)
	, m_input(100)
{
	UnitText="";
	DlgTitle="";
	InputLabelText="";
}

CInputDlg::~CInputDlg()
{
}

void CInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_INPUT, m_input);
	//DDV_MinMaxInt(pDX, m_input, 1, 100000);
	DDX_Control(pDX, IDC_Txt_Instruction, InputLabel);
	DDX_Control(pDX, IDC_Txt_Unit, ctrlUnit);
}


BEGIN_MESSAGE_MAP(CInputDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CInputDlg message handlers


void CInputDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	//CString ms;
	//ms.Format("Input=%i",m_input);
	//MessageBox(ms);
	OnOK();
}

BOOL CInputDlg::OnInitDialog() 
{
	CDialog::OnInitDialog(); //call to prevent assertion failed

	this->SetWindowText(DlgTitle);
	InputLabel.SetWindowText(InputLabelText);
	this->ctrlUnit.SetWindowText(UnitText);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


//! A function to check if user prees specified key
/*!
*	Created: 21-Sep-04
*	\n Last modification: 21-Sep-04.\n
*	Called when user press key
*/
BOOL CInputDlg::PreTranslateMessage(MSG* pMsg) 
{
if(pMsg->message == WM_KEYDOWN) //{AfxMessageBox("Key press");} // ถ้ามีการกดคีย์
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
	return 0; // ให้ออกจากฟังก์ชั่นนี้b //return CDialog::PreTranslateMessage(pMsg);
} 
