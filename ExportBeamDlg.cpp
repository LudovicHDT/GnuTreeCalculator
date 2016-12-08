// ExportBeamDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Tree Analyser.h"
#include "ExportBeamDlg.h"
#include ".\exportbeamdlg.h"
#include "FolderDialog.h"


// CExportBeamDlg dialog

IMPLEMENT_DYNAMIC(CExportBeamDlg, CDialog)
CExportBeamDlg::CExportBeamDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExportBeamDlg::IDD, pParent)
	, txt_filename(_T(""))
	, txt_directory(_T(""))
	, m_model(FALSE)
	, m_separate_file(FALSE)
	, m_output_python_code(FALSE)
	, m_python_version(1)
{
}

CExportBeamDlg::~CExportBeamDlg()
{
}

void CExportBeamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_FILENAME, txt_filename);
	DDX_Text(pDX, IDC_EDIT_OUTPUT_DIRECTORY, txt_directory);
	DDX_Radio(pDX, IDC_RADIO1, m_model);
	DDX_Control(pDX, IDC_EDIT_FILENAME, ctrl_filename);
	DDX_Check(pDX, IDC_ONE_FILE, m_separate_file);
	DDX_Check(pDX, IDC_CHECK_PYTHON_OUT, m_output_python_code);
	DDX_Radio(pDX, IDC_PY_VERSION, m_python_version);
}

BEGIN_MESSAGE_MAP(CExportBeamDlg, CDialog)
	//ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDBROWSE, OnBnClickedBrowse)
END_MESSAGE_MAP()

void CExportBeamDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	//CString ms;
	//ms.Format("File name=%s  Directory=%s  Model=%i",this->txt_filename,this->txt_directory,this->m_model);
	//MessageBox(ms);
	OnOK();
}

BOOL CExportBeamDlg::OnInitDialog() 
{
	CDialog::OnInitDialog(); //call to prevent assertion failed
	this->m_model=0;
	this->m_separate_file=0;
	this->m_output_python_code=1;
	UpdateData(false);
	return true;
}


void CExportBeamDlg::OnBnClickedBrowse()
{
	CFolderDialog dlg(txt_directory);
	if(dlg.DoModal()==IDOK)
	{
		txt_directory=dlg.GetPathName()+"\\";
		UpdateData(false);
	}

}
