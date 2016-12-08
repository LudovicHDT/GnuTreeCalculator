// Tree AnalyserDlg.h : header file
//

#if !defined(AFX_TREEANALYSERDLG_H__6C6FADD2_7DD9_4904_ABE6_07ED5DA92410__INCLUDED_)
#define AFX_TREEANALYSERDLG_H__6C6FADD2_7DD9_4904_ABE6_07ED5DA92410__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "EachImage.h"
#include "utils.h"
#include "VirtualPlant.h"
#include "ImagePool.h"
#include "Direction.h"
#include "cPoint.h"	// Added by ClassView
#include "EquationPool.h"
#include "CameraPool.h"
#include "afxwin.h"



/////////////////////////////////////////////////////////////////////////////
// CTreeAnalyserDlg dialog

/**
* @brief The class CTreeAnalyserDlg this class coltrol the program dialog.\n
* Main class of Tree Analyser.
* 
*/

class CTreeAnalyserDlg : public CDialog
{
// Construction
public:
	UINT keypressed;	
	CString OutputLocation;
	CString mrl[4];
	CTreeAnalyserDlg(CWnd* pParent = NULL);	// standard constructor
	CString GetRootName(CString fname);

// Dialog Data
	//{{AFX_DATA(CTreeAnalyserDlg)
	enum { IDD = IDD_TREEANALYSER_DIALOG };
	CEdit	m_statusBar;
	CEdit	m_EditOutput;
	CEdit	m_PropertiesBox;
	CTreeCtrl	m_Tree1;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTreeAnalyserDlg)
	public:
	//virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	// Generated message map functions
	//{{AFX_MSG(CTreeAnalyserDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnFileExit();
	afx_msg void OnHelpAbout();
	afx_msg void OnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnFileOpenproject();
	afx_msg void OnFileNewproject();
	afx_msg void OnFileSaveproject();
	afx_msg void OnImageSavepixelvalue();
	afx_msg void OnImageEditparameters();
	afx_msg void OnImageDelete();
	afx_msg void OnFile1();
	afx_msg void OnFile2();
	afx_msg void OnFile3();
	afx_msg void OnFile4();
	virtual void OnOK();
	afx_msg void OnProjectEstimatetreedimension();
	afx_msg void OnProjectSetting();
	afx_msg void OnProjectVolume();
	afx_msg void OnProjectAnalyseall();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

//private:
	//private variables
	short int WorkIndex; //0=not work,1=dimension, 2=Volume, 3=LA total, 4=LA intercepted, 5=Do All Calculations, 6=LAD distribution
	CImagePool * pImagePool1;
	CCameraPool * pCameraPool;

	int m_Tree1Width;
	CString ActivePictureName; //this value must be null terminate string (+'\0')
	CBitmap m_bmpBitmap;
	CString ActiveProjectFilename;
	bool opening;
	bool flagChanged; //0=no changed 1=changed
	CMenu *pMenu;
	CRect appRect;
	CRect treeRect;
	CRect rectOutput;
	CRect propertiesRect;
	CRect rectPic;
	std::vector<HTREEITEM> vctTreeItem;
	std::vector<CDirection> vctDirection;
	double percentZeroGap,setZeroGap,percentFullGap;
	CString CurrentPath;
	double CurrentScale;

	//last used parameter
	double last_cam_distance,last_cam_height,last_fc,last_ele;
	int last_selected_direction;
	CString LastCamera;
	double default_pza;

//private:
	//Project menu
	void EstimateTreeDimension();
	void VolumeReconstruction(void);
	void AnalyseAll();
	void ComputeLADdistribution();
	
	//Image menu
	void Fractalanalysis();
	void Colorcounting();
	void Savepixelvalue();

	//Canopy menu
	void DisplayImageProperties(bool option);
	void DisplaySettingProperties();

	//internal functions
	//int CountFractal(CEachImage *image,int gx,int gy);
	bool CountFractal(CEachImage *image,int gx,int gy,CString output_fname);
	void UpdateMenu();
	void UpdatePicture();
	void UpdateMRL(CString *fname);
	bool IniRead();
	void IniWrite();
	bool CreateVirtualPlant(int option);
	bool fShowPicture(CString fname, double *scale);
	int GetTreeID(HTREEITEM *item);
	void UpdateCheckItem();
	void AddDisplayText(CString *txt);
	void AddDisplayText(CString txt);
	void TestPictureCombination(int picnum);
	void DisplayStatus();
	void DisplayStatus(CString txt);

	void CalculDirections(CEachImage *image); //call by volume reconstruction
	void CalculDirections4(CEachImage *image,int gx,int gy,BOOL is_division, int sx, int sy);
	int CalculDirections6(CEachImage *image,int gx,int gy,CString *fname,bool model_option);//for exporting data for Christophe, return number of exported line
	void ExportVirtualPlant(int option,CString filename,int celltype);
	void EstimateLeafArea();
	int CalculDirections8(CEachImage *image,int gx,int gy,CString *fname);//Compute P0 Beer from virtual canopy
	double GetGapFraction(CImage *image,double px, double py, int gx, int gy);

	//Functions to compare canopy
	void OpenCanopy(CString fname,cVirtualPlant *vp1);
	void SaveCanopy(CString filename,cVirtualPlant *vp1);
	void CompareCanopy(CString fname1,CString fname2);




	//thread functions
	CWinThread *pWorkThread;
	static UINT ThreadWork(LPVOID pParam);
	int AskForStopThread(void);
	void DisplayTerminateMessage(void);
	void SetWorkingMenu(void);
	void DisableAllMenus(void);


public:
	// File menu
	afx_msg void OnFileSaveAs();

	//menu View
	afx_msg void OnViewClearoutput();

	//menu Image
	afx_msg void OnImageMarkall();
	afx_msg void OnImageUnmarkall();
	afx_msg void OnImageToglemark();
	afx_msg void OnImageCalcualtegapfraction();
	afx_msg void OnImageFractalanalysis();
	afx_msg void OnImageAddimagetoproject();
	afx_msg void OnImageColorcounting();

	//menu Calculation
	afx_msg void OnProjectLaTotalDirection();
	afx_msg void OnProjectLaInterceptedDirection();

	
	//menu Canopy
	afx_msg void OnCanopyDisplayproperties();
	afx_msg void OnCanopyComparecanopy();
	afx_msg void OnCanopyExportvegestarcanopy();
	afx_msg void OnCanopyVerticalprofileofleafarea();
	afx_msg void OnCanopyVerticalprofileofvolume();
	afx_msg void OnCanopySavecanopy();
	afx_msg void OnCanopyOpencanopy();
	afx_msg void OnCanopyRandomleafinvolume();
	afx_msg void OnCanopyExportratpvoxels();
	afx_msg void OnCanopyViewbyplantgl3dviewer();
	afx_msg void OnCanopyExportplantglfile();


	//Other message
	afx_msg void OnNMDblclkTree1(NMHDR *pNMHDR, LRESULT *pResult);


	void Openproject();
	void Newproject();
	void Openproject(CString *fname);
	void DrawZone(int px, int py, int gx, int gy);


	CString GetDirectoryName(CString fullPathName);
	CString GetFileName(CString fullPathName,bool include_ext);
	CString GetLogFilename(void);


	afx_msg void OnViewCentermark();
	bool center_mark;
	bool margin_mark;
	afx_msg void OnViewMarginmark();

	CPen penBlue; //(PS_SOLID, 1, RGB(0, 0, 255));
	CPen penRed;//(PS_SOLID, 1, RGB(255, 0, 0));
	CPen penBlueSky;//(PS_SOLID, 1, RGB(0, 255, 255));

	afx_msg void OnHelpTreeanalyserusermanual();
	//void Exportcsvfileofp0binom(); //Export beam equations as csv file for Python for model binomial
	//void ExportP0Beer(CString ExportName, BOOL separate);
	//void ExportP0Binom(CString ExportName, BOOL separate);
	void Exportbeamequations();
	void ExportBeamToFile(CString ExportName, BOOL separate, bool model); //model 0 is Beer, 1 is binomail
	void WritePythonCode(CString output_directory, CString filename,int python_version);
	afx_msg void OnCalculationExportequationsforpython();
	CEdit ctrl_setting_properties;
	afx_msg void OnHelpVersionhistory();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TREEANALYSERDLG_H__6C6FADD2_7DD9_4904_ABE6_07ED5DA92410__INCLUDED_)
