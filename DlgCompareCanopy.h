#pragma once
#include "afxwin.h"


// CDlgCompareCanopy dialog

class CDlgCompareCanopy : public CDialog
{
	DECLARE_DYNAMIC(CDlgCompareCanopy)

public:
	CDlgCompareCanopy(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgCompareCanopy();

// Dialog Data
	enum { IDD = IDD_DLG_COMPARE_CANOPY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString file_canopy1;
	CString file_canopy2;
	afx_msg void OnBnClickedOk();
	CEdit ctrlFileCanopy1;
	CEdit ctrlFileCanopy2;
	afx_msg void OnBnClickedBrowse1();
	CString GetCanopyFile();
	CString CurrentPath;
	afx_msg void OnBnClickedBrowse2();
};
