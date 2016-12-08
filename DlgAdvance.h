#pragma once
#include "afxwin.h"
#include "InputDlg.h"

// CDlgAdvance dialog

class CDlgAdvance : public CDialog
{
	DECLARE_DYNAMIC(CDlgAdvance)

public:
	CDlgAdvance(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgAdvance();

// Dialog Data
	enum { IDD = IDD_DLG_ADVANCE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void OnOK();

	DECLARE_MESSAGE_MAP()
public:
	int m_LimitEle;
	int m_enlarge;
	int m_raypx;
	int m_raypy;
	double m_maxLAD;
	int RadioGap1;
	double minimumGap;
	BOOL IfSetMaxLAD;
	bool flagDisable;

	CEdit ctrl_txt_maxLAD;
	CButton chk_SetMaxLAD;
	CEdit ctrl_txtFixedGap;
	CButton ctrl_checkLAD;
	CButton ctrl_RadioGap;

	afx_msg void OnBnClickedBtnDefault();
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedChkSetLad();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio1();

	void DisableControls();
	void SetCtrlInputClumping();

	CButton ctrl_btn_ok;
	CButton ctrl_btn_default;
	CEdit ctrl_input_clumping;
	afx_msg void OnBnClickedRadioCustomclump();
	afx_msg void OnBnClickedRadioAutoclump();
	afx_msg void OnBnClickedRadioNoclump();
	int m_leaf_dispersion_option;
	double m_leaf_dispersion;
	int m_gpx;
	int m_gpy;
	CComboBox ctrl_combo3;
	int m_gap_average_option;
	int gap_option;
	CEdit ctrlGPY;
	afx_msg void OnEnChangeEditGpx();
	CEdit ctrlGPX;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedRadio6();
	afx_msg void OnBnClickedRadio5();
	afx_msg void OnBnClickedRadio7();
	CEdit ctrlPZA;
	double fix_pza;
	BOOL is_divide_pza;
	int division_horizontal;
	int division_vertical;
	CEdit ctrl_subdivision_horizontal;
	CEdit ctrl_subdivision_vertical;
	afx_msg void OnBnClickedCheck3();
	afx_msg void OnBnClickedBtnSetpza();
	double defualt_pza;
};
