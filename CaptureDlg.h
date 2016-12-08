#pragma once
#include "afxwin.h"


// CCaptureDlg dialog

class CCaptureDlg : public CDialog
{
	DECLARE_DYNAMIC(CCaptureDlg)

public:
	CCaptureDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCaptureDlg();

// Dialog Data
	enum { IDD = IDD_CAPTRUE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCapturebtn();
	CStatic m_frame;
	bool b_button1_on;
	int posx, posy, sizex, sizey;
	CButton ctrlBtnCapture;
	bool capture_status;
};
