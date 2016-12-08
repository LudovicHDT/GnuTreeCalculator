#pragma once
#include "afxwin.h"


// CAddCameraDlg dialog

class CAddCameraDlg : public CDialog
{
	DECLARE_DYNAMIC(CAddCameraDlg)

public:
	CAddCameraDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAddCameraDlg();

// Dialog Data
	enum { IDD = IDD_DLG_ADD_CAMERA };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString dlg_title;
	virtual BOOL OnInitDialog();
	CString m_camera_name;
	double m_calibration_value;
	double m_min_fc;
	double m_max_fc;
	CString m_calibration_date;
	CString m_owner;
	CString m_note;
	CButton ctrlBtnOK;
	bool show_option; //0=add new, 1=view information
	bool edit_option;
protected:
	virtual void OnOK();
public:
	void DisableAll(void);
	void EnabledAll(void);
	CEdit ctrlEditModel;
	CEdit ctrlEditCalibrationValue;
	CEdit ctrlEditMinFC;
	CEdit ctrlEditMaxFC;
	CEdit ctrlEditDate;
	CEdit ctrlEditOwner;
	CEdit ctrlEditNote;
};
