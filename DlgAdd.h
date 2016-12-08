#if !defined(AFX_DLGADD_H__2161D8C6_E108_4FE2_B52F_400CF0EED279__INCLUDED_)
#define AFX_DLGADD_H__2161D8C6_E108_4FE2_B52F_400CF0EED279__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAdd.h : header file
//
//#include "VideoCapture.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgAdd dialog
#include "CameraPool.h"

class CEachImage;
class CImagePool;

class CDlgAdd : public CDialog
{
private:
	bool ValidateImage(bool option,CString *picname);
	void DisableAll();
	void EnabledAll();

	//private variables
	int m_pixel_value,m_nR,m_nG,m_nB;

public:
	CDlgAdd(CWnd* pParent = NULL);   // standard constructor
	CString CurrentPath;
	CBitmap m_bmpBitmap;
	CImagePool * tempImagePool;
	CString dlgTitle;
	bool callType; //0=add 1=edit
	CCameraPool * pCameraPool;
	void AddCameraToList();

// Dialog Data
	//{{AFX_DATA(CDlgAdd)
	enum { IDD = IDD_DLG_ADD_IMAGE };
	CEdit	m_editDirection;
	CComboBox	m_combodirection;
	CEdit	m_focalCtrl;
	CEdit	m_viewangle;
	CEdit	m_editCCDSize;
	CComboBox	m_combocamera;
	CString	m_fname;
	double	m_distance;
	double	m_direction;
	double	m_height;
	double	m_bending;
	double	m_rot;
	double	m_inc;
	int		m_pictureWidth;
	int		m_pictureHeight;
	CString CameraName;
	double	camera_cal;
	int		m_picType;
	CString	m_picName;
	double	m_focalLength;
	int		m_selectedDirection;
	//}}AFX_DATA

	int		m_pictureBit;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAdd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgAdd)
	afx_msg void OnbtnBrowse();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelchangeComboCamera();
	afx_msg void OnChangeEditFocal();
	afx_msg void OnChangeEditCcdsize();
	afx_msg void OnSelchangeComboDirection();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedbtncapture();
	afx_msg void OnBnClickedAddCamera();
	afx_msg void OnBnClickedViewCameraInfo();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGADD_H__2161D8C6_E108_4FE2_B52F_400CF0EED279__INCLUDED_)
