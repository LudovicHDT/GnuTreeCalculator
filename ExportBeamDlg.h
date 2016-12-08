#pragma once
#include "afxwin.h"


// CExportBeamDlg dialog

class CExportBeamDlg : public CDialog
{
	DECLARE_DYNAMIC(CExportBeamDlg)

public:
	CExportBeamDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CExportBeamDlg();

// Dialog Data
	enum { IDD = IDD_DLG_EXPORT_BEAMS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CString txt_filename;
	CString txt_directory;
	BOOL m_model;
	afx_msg void OnBnClickedOk();
	CEdit ctrl_filename;


	afx_msg void OnBnClickedBrowse();
	BOOL m_separate_file;
	BOOL m_output_python_code;
	int m_python_version;
};
