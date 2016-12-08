// DlgCompareCanopy.cpp : implementation file
//

#include "stdafx.h"
#include "Tree Analyser.h"
#include "DlgCompareCanopy.h"
#include ".\dlgcomparecanopy.h"


// CDlgCompareCanopy dialog

IMPLEMENT_DYNAMIC(CDlgCompareCanopy, CDialog)
CDlgCompareCanopy::CDlgCompareCanopy(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCompareCanopy::IDD, pParent)
	, file_canopy1(_T(""))
	, file_canopy2(_T(""))
{
	CurrentPath="";
}

CDlgCompareCanopy::~CDlgCompareCanopy()
{
}

void CDlgCompareCanopy::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, file_canopy1);
	DDX_Text(pDX, IDC_EDIT5, file_canopy2);
	DDX_Control(pDX, IDC_EDIT1, ctrlFileCanopy1);
	DDX_Control(pDX, IDC_EDIT5, ctrlFileCanopy2);
}


BEGIN_MESSAGE_MAP(CDlgCompareCanopy, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BROWSE1, OnBnClickedBrowse1)
	ON_BN_CLICKED(IDC_BROWSE2, OnBnClickedBrowse2)
END_MESSAGE_MAP()


// CDlgCompareCanopy message handlers

void CDlgCompareCanopy::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	this->UpdateData(TRUE);
	if(this->file_canopy1==""||this->file_canopy2=="")
	{
		MessageBox("Error input file name");
	}
	else
	{
	CString ms;
	//ms.Format("Canopy1='%s'  Canopy2='%s'",this->file_canopy1,this->file_canopy2);
	//MessageBox(ms);
	OnOK();
	}
}

void CDlgCompareCanopy::OnBnClickedBrowse1()
{
	CString fname;
try_again:;
	fname=GetCanopyFile();
	if(fname!="")
	{
		//MessageBox(fname);
		if(fname==file_canopy2)
		{
			MessageBox("Selected file is the same as canopy1\nPlease select another.");
			goto try_again;
		}
		else
		{
			this->file_canopy1=fname;
			UpdateData(FALSE);
		}
	}
}//void CDlgCompareCanopy::OnBnClickedBrowse1()

void CDlgCompareCanopy::OnBnClickedBrowse2()
{
	CString fname;
try_again:;
	fname=GetCanopyFile();
	if(fname!="")
	{
		//MessageBox(fname);
		if(fname==file_canopy1)
		{
			MessageBox("Selected file is the same as canopy1\nPlease select another.");
			goto try_again;
		}
		else
		{
			this->file_canopy2=fname;
			UpdateData(FALSE);
		}
	}
}//void CDlgCompareCanopy::OnBnClickedBrowse2()

CString CDlgCompareCanopy::GetCanopyFile()
{
	CString filename; 
     static char BASED_CODE szFilter[] = "Tree Project (*.cpn)|*.cpn||";
     CFileDialog m_ldFile(TRUE, ".cpn", CurrentPath+"*.cpn",
          OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);

	if (m_ldFile.DoModal() == IDOK)
	{
		return m_ldFile.GetPathName();

	}
	else {return "";}
}
