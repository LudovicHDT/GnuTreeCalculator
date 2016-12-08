#include "afxwin.h"
#if !defined(AFX_PROGRAMSETTINGDLG_H__BED04C03_751D_4A93_A586_91AE8D148155__INCLUDED_)
#define AFX_PROGRAMSETTINGDLG_H__BED04C03_751D_4A93_A586_91AE8D148155__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProgramSettingDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CProgramSettingDlg dialog

class CProgramSettingDlg : public CDialog
{
// Construction
public:
	CProgramSettingDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CProgramSettingDlg)
	enum { IDD = IDD_DLG_PROGRAM_SET };
	CString	m_OutputLocation;
	double	m_cellSize_dx;
	double	m_cellSize_dy;
	double	m_cellSize_dz;
	int	m_enlarge;
	int		m_raypx;
	int		m_raypy;
	BOOL	m_ifoutputbox;
	BOOL	m_iflogvolume;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProgramSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	// Generated message map functions
	//{{AFX_MSG(CProgramSettingDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnImage();
	afx_msg void OnBtnDefault();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	bool flagDisableControls;
	int m_LimitEle;

	CButton ctrl_cube_dx;
	CButton ctrl_logfile;
	CButton ctrl_VgsTAR;
	CButton ctrl_btn_default;
	CButton ctrl_btn_ok;

	int m_distribution;
	BOOL check_cube_value;
	CEdit ctrl_dy;
	CEdit ctrl_dx;
	CEdit ctrl_dz;
	CEdit ctrl_zone_type;
	CEdit ctrl_output_location;
	//int lapx;
	//CEdit ctrl_lapy;
	//int lapy;
	//CEdit ctrl_lapx;
	CEdit ctrl_gpx;
	CEdit ctrl_gpy;
	int gpx;
	int gpy;
	int gap_option;
	CStatic ctrl_G_spherical;
	CComboBox ctrl_leaf_distribution;
	int m_leaf_distribution;
	void SetShow();
	int gap_average_option;
	CEdit ctrl_MA;
	CStatic ctrl_deg;
	CButton ctrl_browse;
	CEdit ctrl_dis_file;
	CString CurrentPath;
	double m_mean_leaf_inc;
	CString m_dis_file;
	int ZeroGapManage;
	double minimumGap;
	bool IfSetMaxLAD;
	double MaxLAD;
	int m_leaf_dispersion_option;
	double m_leaf_dispersion;
	double fix_pza;
	double default_pza;

	//Variable added for subdivision of PZA
	BOOL is_divide_pza;
	int division_horizontal;
	int division_vertical;

	afx_msg void OnBnClickedBtnAdvance();
	afx_msg void OnBnClickedBrowse();
	afx_msg void OnBnClickedOk();
	afx_msg void OnCbnSelchangeComboLdistribution();
	afx_msg void OnEnChangeEditCellsizeX();
	afx_msg void OnBnClickedCheck5();

	void DisableAll();
	void DisableBtnOK();

	double m_mean_leaf_area;
	int m_inversion_method;
	CButton ctrl_inversion_method;
	CEdit ctrlLeafArea;
	afx_msg void OnBnClickedRadio1();
	void SetctrlLeafArea();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	BOOL m_ChkLAD;
	afx_msg void OnBnClickedButton1();

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROGRAMSETTINGDLG_H__BED04C03_751D_4A93_A586_91AE8D148155__INCLUDED_)
