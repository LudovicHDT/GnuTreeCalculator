// Tree AnalyserDlg.cpp : implementation file

#include "stdafx.h"
#include "Tree Analyser.h"
#include "Tree AnalyserDlg.h"
#include "DlgAdd.h"
#include "InputDlg.h"
#include "DlgCompareCanopy.h"
//#include "vardiagram.h"
#include "HyperLink.h"
#include "ExportBeamDlg.h"



#include <algorithm> //add for random_shuffle
#include <functional>  //add for random_shuffle

#include "ProgramSettingDlg.h"
#include "MyFunctions.h"
#include <atlstr.h>
#include ".\tree analyserdlg.h"
#include "afxwin.h"
#include "TA_ini.h" //class for ini file

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About


class ColorCount
{
public:
	ColorCount();
	ColorCount(int incolor,int inpixel);
	int ColorIndex;
	int SumPixel;
};

ColorCount::ColorCount()
{
	ColorIndex=-1;
	SumPixel=0;
}
ColorCount::ColorCount(int incolor,int inpixel)
{
	ColorIndex=incolor;
	SumPixel=inpixel;
}

class MatchedVoxel
{
public:
	MatchedVoxel();
	int cell1i;
	int cell2i;
};

MatchedVoxel::MatchedVoxel()
{
	cell1i=-1;
	cell2i=-1;
}

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CStatic ctrlTxtUpdate;
	CString m_txtUpdate;
	CStatic ctrlTxtVersion;
	CString m_txtVersion;
	CStatic ctrlCopyright;
	CString m_txtCopyright;
	CHyperLink m_inra_web;
	CHyperLink m_ku_web;
	//afx_msg void OnStnClickedInraWeb();
	CHyperLink m_mail_to_jessada;
	CHyperLink m_mail_to_sinoquet;
	CHyperLink m_mail_to_TreeAnalyser;
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
, m_txtUpdate(_T("Last update:"))
, m_txtVersion(_T("Tree Anslyser"))
, m_txtCopyright(_T(""))
{

	m_txtUpdate.LoadString(IDS_LAST_UPDATE);
	m_txtVersion.LoadString(IDS_VERSION);
	m_txtCopyright.LoadString(IDS_COPYRIGHT);
	m_inra_web.SetURL(_T("http://www.clermont.inra.fr/piaf/index_eng.php"));
	m_ku_web.SetURL(_T("http://www.ku.ac.th/english2003"));
	m_mail_to_jessada.SetURL(_T("mailto:agrjes@yahoo.com"));
	m_mail_to_sinoquet.SetURL(_T("mailto:sinoquet@clermont.inra.fr"));
	m_mail_to_TreeAnalyser.SetURL(_T("mailto:agrjes@yahoo.com"));
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_TXT_UPDATE, ctrlTxtUpdate);
	DDX_Text(pDX, IDC_TXT_UPDATE, m_txtUpdate);
	DDX_Control(pDX, IDC_TXT_VERSION, ctrlTxtVersion);
	DDX_Text(pDX, IDC_TXT_VERSION, m_txtVersion);
	DDX_Control(pDX, IDC_TXT_COPYRIGHT, ctrlCopyright);
	DDX_Text(pDX, IDC_TXT_COPYRIGHT, m_txtCopyright);
	DDX_Control(pDX, IDC_INRA_WEB, m_inra_web);
	DDX_Control(pDX, IDC_KU_WEB, m_ku_web);
	DDX_Control(pDX, IDC_MAIL_TO_JESSADA, m_mail_to_jessada);
	DDX_Control(pDX, IDC_MAIL_TO_SINOQUET, m_mail_to_sinoquet);
	DDX_Control(pDX, IDC_MAIL_TO_TREEANALYSER, m_mail_to_TreeAnalyser);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
//	ON_STN_CLICKED(IDC_INRA_WEB, OnStnClickedInraWeb)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTreeAnalyserDlg dialog

//------------------------------------------------------------------
	CEachImage *pWorkImage=new CEachImage;
	cVirtualPlant *vp1=new cVirtualPlant();
	CString ms;
	CString ProgramPath;
//------------------------------------------------------------------



CTreeAnalyserDlg::CTreeAnalyserDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTreeAnalyserDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTreeAnalyserDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	penBlue.CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
	penRed.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	penBlueSky.CreatePen(PS_SOLID, 1, RGB(0, 255, 255));
}

void CTreeAnalyserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTreeAnalyserDlg)
	DDX_Control(pDX, IDC_STATUS_BAR, m_statusBar);
	DDX_Control(pDX, IDC_EDIT_OUTPUT, m_EditOutput);
	DDX_Control(pDX, IDC_IMAGE_PROPERTIES, m_PropertiesBox);
	DDX_Control(pDX, IDC_TREE1, m_Tree1);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_SETTING_PROPERTIES, ctrl_setting_properties);
}

BEGIN_MESSAGE_MAP(CTreeAnalyserDlg, CDialog)
	//{{AFX_MSG_MAP(CTreeAnalyserDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

	ON_COMMAND(ID_FILE_EXIT, OnFileExit)
	ON_COMMAND(ID_HELP_ABOUT, OnHelpAbout)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, OnSelchangedTree1)

	ON_COMMAND(ID_FILE_OPENPROJECT, OnFileOpenproject)
	ON_COMMAND(ID_FILE_SAVEPROJECT, OnFileSaveproject)
	ON_COMMAND(ID_FILE_1, OnFile1)
	ON_COMMAND(ID_FILE_2, OnFile2)
	ON_COMMAND(ID_FILE_3, OnFile3)
	ON_COMMAND(ID_FILE_4, OnFile4)

	ON_COMMAND(ID_IMAGE_SAVEPIXELVALUE, OnImageSavepixelvalue)
	ON_COMMAND(ID_IMAGE_EDITPARAMETERS, OnImageEditparameters)
	ON_COMMAND(ID_IMAGE_DELETE, OnImageDelete)

	ON_COMMAND(ID_PROJECT_ESTIMATETREEDIMENSION, OnProjectEstimatetreedimension)
	ON_COMMAND(ID_PROJECT_SETTING, OnProjectSetting)
	ON_COMMAND(ID_PROJECT_VOLUME, OnProjectVolume)
	ON_COMMAND(ID_PROJECT_ANALYSEALL, OnProjectAnalyseall)

	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_DBLCLK, IDC_TREE1, OnNMDblclkTree1)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_COMMAND(ID_FILE_NEWPROJECT, OnFileNewproject)
	ON_COMMAND(ID_PROJECT_LA_TOTAL, OnProjectLaTotalDirection)
	ON_COMMAND(ID_PROJECT_LA_INTERCEPTED,OnProjectLaInterceptedDirection)

	ON_COMMAND(ID_IMAGE_COLORCOUNTING, OnImageColorcounting)
	ON_COMMAND(ID_IMAGE_MARKALL, OnImageMarkall)
	ON_COMMAND(ID_IMAGE_UNMARKALL, OnImageUnmarkall)
	ON_COMMAND(ID_IMAGE_TOGLEMARK, OnImageToglemark)
	ON_COMMAND(ID_IMAGE_FRACTALANALYSIS, OnImageFractalanalysis)

	ON_COMMAND(ID_CANOPY_DISPLAYPROPERTIES, OnCanopyDisplayproperties)
	ON_COMMAND(ID_CANOPY_SAVECANOPY, OnCanopySavecanopy)
	ON_COMMAND(ID_CANOPY_OPENCANOPY, OnCanopyOpencanopy)
	ON_COMMAND(ID_CANOPY_RANDOMLEAFINVOLUME, OnCanopyRandomleafinvolume)

	ON_COMMAND(ID_CANOPY_COMPARECANOPY, OnCanopyComparecanopy)
	ON_COMMAND(ID_CANOPY_EXPORTVEGESTARCANOPY, OnCanopyExportvegestarcanopy)
	ON_COMMAND(ID_CANOPY_VERTICALPROFILEOFLEAFAREA, OnCanopyVerticalprofileofleafarea)
	ON_COMMAND(ID_CANOPY_VERTICALPROFILEOFVOLUME, OnCanopyVerticalprofileofvolume)
	ON_COMMAND(ID_CANOPY_EXPORTRATPVOXELS, OnCanopyExportratpvoxels)
	ON_COMMAND(ID_IMAGE_CALCUALTEGAPFRACTION, OnImageCalcualtegapfraction)
	ON_COMMAND(ID_CANOPY_VIEWBYPLANTGL3DVIEWER, OnCanopyViewbyplantgl3dviewer)
	ON_COMMAND(ID_CANOPY_EXPORTPLANTGLFILE, OnCanopyExportplantglfile)
	ON_COMMAND(ID_IMAGE_ADDIMAGETOPROJECT, OnImageAddimagetoproject)
	ON_COMMAND(ID_VIEW_CLEAROUTPUT, OnViewClearoutput)
	ON_COMMAND(ID_VIEW_CENTERMARK, OnViewCentermark)
	ON_COMMAND(ID_VIEW_MARGINMARK, OnViewMarginmark)
	ON_COMMAND(ID_HELP_TREEANALYSERUSERMANUAL, OnHelpTreeanalyserusermanual)
	ON_COMMAND(ID_CALCULATION_EXPORTEQUATIONSFORPYTHON, OnCalculationExportequationsforpython)
	ON_COMMAND(ID_HELP_VERSIONHISTORY, OnHelpVersionhistory)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTreeAnalyserDlg message handlers


BOOL CTreeAnalyserDlg::DestroyWindow() 
{
	IniWrite();
	delete pImagePool1;
	delete vp1;

	return CDialog::DestroyWindow();
}


BOOL CTreeAnalyserDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	//MessageBox("OK1");
//--------- Start  variables setup ------------
	opening=0;
	flagChanged=0;
	//flagStop=0;
	ActiveProjectFilename="";
	m_Tree1Width=220;
	vctTreeItem.clear();
	LastCamera="Fuji FinePix1400";
	pImagePool1=new CImagePool;
	pCameraPool=new CCameraPool;
	vp1=new cVirtualPlant(&m_statusBar,&m_EditOutput);
	percentZeroGap=0;setZeroGap=1;percentFullGap=0;
	pWorkThread=NULL;
	WorkIndex=0;
	m_EditOutput.SetLimitText(0);
	last_cam_distance=10;
	last_cam_height=1;
	last_fc=12;
	last_ele=0;
	last_selected_direction=0;
	CurrentScale=1;
	center_mark=true;
	margin_mark=true;
	default_pza=17;
	

//--------- End variables setup ------------

//---------Start seting size  of components------------
	//set windows size
	MoveWindow(0,0,800,600,TRUE);
	// Get the display area available
	GetClientRect(appRect);
	appRect.NormalizeRect();
	//Get Tree1 area
	m_Tree1.SetWindowPos(NULL,5,5,m_Tree1Width,appRect.Height()-350,SWP_DRAWFRAME);
	m_Tree1.GetClientRect(treeRect);
	m_PropertiesBox.SetWindowPos(NULL,5,treeRect.Height()+15,treeRect.Width()+5,appRect.Height()-treeRect.Height()-180,SWP_DRAWFRAME);
	m_PropertiesBox.GetClientRect(propertiesRect);

	ctrl_setting_properties.SetWindowPos(NULL,5,treeRect.Height()+193,treeRect.Width()+5,140,SWP_DRAWFRAME);

	m_EditOutput.SetWindowPos(NULL,m_Tree1Width+10,387,appRect.Width()-m_Tree1Width-15,140,SWP_DRAWFRAME);


	m_statusBar.SetWindowPos(NULL,5,appRect.Height()-18,appRect.Width()-10,15,SWP_DRAWFRAME);

	//set display area
	rectPic.top=5;
	rectPic.left=m_Tree1Width+5+7;
	rectPic.bottom=382;
	rectPic.right=appRect.right-5;


//---------Start seting size  of components-----------

	
//------- Find the current path of the program ------------------------	
	//MessageBox(_pgmptr);
	ms=_pgmptr;
	int i=strlen(_pgmptr),j=i-1,k=0;
	while(j>0)
	{
		
		if(ms.GetAt(j)=='\\'){k=j+1;j=0;}
		j--;
	}
	ProgramPath=ms.Left(k);
//------- end find the current path of the program ------------------------	

	//CTA_ini TAini(ProgramPath);
	//TAini.ReadIni();
	//MessageBox("OK2");
	
	if(!IniRead())
	{
		//MessageBox("Error reading INI file!\nDefault value was set.");
		CurrentPath=ProgramPath;
		OutputLocation=ProgramPath;
		IniWrite();
		IniRead();
	}

	pImagePool1->fix_pza=default_pza;
	this->SetWindowText("Tree Analyser: untitle");
	this->SetWorkingMenu();
	
	CString ms2;
	ms2.LoadString(IDS_COPYRIGHT);
	ms.LoadString(IDS_VERSION);
	ms=ms+" ";
	ms=ms+ms2;
	AddDisplayText(ms);
	AddDisplayText("J. PHATTARALERPHONG and H. SINOQUET.");
	AddDisplayText("U.M.R. PIAF. Institut National de la Recherche Agronomique (INRA), France.");
	AddDisplayText("Plant Architecture Research Unit (PARU), Kasetsart University, Bangkok, Thailand.");
	//AddDisplayText("agrjes@yahoo.com");
	ms.LoadString(IDS_LAST_UPDATE);
	AddDisplayText(ms);

	CString camera_file;
	pCameraPool->filename=ProgramPath+"cameras.dat";
	if(!pCameraPool->Cameras_Load())
	{
		MessageBox("Error loading camera!\nDefault cameras data will be written.");
		pCameraPool->LoadDefault();
	}
	
	DisplayStatus();
	DisplaySettingProperties();
	//ms.Format("Number of argument=%i  First='%s'  Second='%s'",::__argc,::__argv[0],::__argv[1]);
	//MessageBox(ms);

	//two line below are necessary to allow the check box work when open with argurement
	m_Tree1.ModifyStyle( TVS_CHECKBOXES, 0 );
	m_Tree1.ModifyStyle( 0, TVS_CHECKBOXES );

	if(::__argc==2)
	{
		ms=::__argv[1];
		this->Openproject(&ms);
	}
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTreeAnalyserDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		//CAboutDlg dlgAbout;
		//dlgAbout.DoModal();
		OnHelpAbout();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTreeAnalyserDlg::OnPaint() 
{

	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		UpdatePicture();
		CDialog::OnPaint();
	}

}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.


HCURSOR CTreeAnalyserDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


void CTreeAnalyserDlg::OnFileExit() 
{

	//check if thread is working
	DWORD dwExitCode;
	if (pWorkThread == NULL){goto nextstep;}
	else
	{
		::GetExitCodeThread(pWorkThread->m_hThread,&dwExitCode);
		if(dwExitCode!=STILL_ACTIVE){goto nextstep;}
		else //if thread is working
		{
			pWorkThread->SuspendThread();
			if(this->AskForStopThread()==IDNO){pWorkThread->ResumeThread();goto endexit;}
			else
			{
				::WaitForSingleObject(pWorkThread->m_hThread,0);
				delete pWorkThread;
				pWorkThread=NULL;
				this->DisplayTerminateMessage();
				WorkIndex=0;
				this->SetWorkingMenu();
				DisplayStatus();
			}
		}
	}

nextstep:;

	UpdateCheckItem();
	if(flagChanged==1 && pImagePool1->GetImageCount()>0)
	{
		int ans=MessageBox("Save change of the current project?","Tree Analyser",MB_YESNOCANCEL);
		if (ans==IDYES)
		{
			if (ActiveProjectFilename==""){OnFileSaveAs();}
			else 
			{
				if (pImagePool1->SaveNewFormat(&ActiveProjectFilename)){flagChanged=0;}
				else {MessageBox("Unable to save file!");}
			}
			CDialog::OnOK();
		}
		if (ans==IDNO)
		{
			CDialog::OnOK();
		}
		else {}
	}
	else{CDialog::OnOK();}
endexit:;
}



//BOOL CTreeAnalyserDlg::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
//{
//	return CDialog::Create(IDD, pParentWnd);
//}

void CTreeAnalyserDlg::OnHelpAbout() 
{
	CAboutDlg dlg;
	/*
	ms.LoadString(IDS_LAST_UPDATE);
	dlg.m_txtUpdate=ms;
	ms.LoadString(IDS_VERSION);
	dlg.m_txtVersion=ms;
	ms.LoadString(IDS_COPYRIGHT);
	dlg.m_txtCopyright=ms;
	*/
	dlg.DoModal();
}


void CTreeAnalyserDlg::OnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	//NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if (opening==0) //disable this process while openig project
	{
		HTREEITEM aa=m_Tree1.GetSelectedItem();
		ActivePictureName=m_Tree1.GetItemText(aa);
//		ms.Format("Item count=%i. Current selection=%s check=%i",m_Tree1.GetCount(), m_Tree1.GetItemText(aa),m_Tree1.GetCheck(aa));
//		MessageBox(ms);
		UpdatePicture();
		DisplayImageProperties(1);
	}
	*pResult = 0;
}


void CTreeAnalyserDlg::OnFileOpenproject() 
{
	UpdateCheckItem();
	if(flagChanged==1 && pImagePool1->GetImageCount()>0)
	{
		int ans=MessageBox("Save change of the current project?","Tree Analyser",MB_YESNOCANCEL);
		if (ans==IDYES)
		{
			if (ActiveProjectFilename==""){OnFileSaveAs();}
			else 
			{
				if (pImagePool1->SaveNewFormat(&ActiveProjectFilename)){flagChanged=0;}
				else {MessageBox("Unable to save file!");}
			}
			Openproject();
		}
		if (ans==IDNO)
		{
			Openproject();
		}
		else {}
	}
	else{Openproject();}
}

void CTreeAnalyserDlg::Openproject()
{ //open with dialog box
	opening=1; //for close update OnSelchangedTree1
     static char BASED_CODE szFilter[] = "Tree Analyser Project (*.tre)|*.tre||";
     CFileDialog m_ldFile(TRUE, ".tre", CurrentPath+"*.tre",
          OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);

	if (m_ldFile.DoModal() == IDOK)
	{
		CString filename; 
		filename = m_ldFile.GetPathName();
		Openproject(&filename);
/*
		double chk=0;
		chk=pImagePool1->CheckFile(filename);
		int a=0;
		if(chk==0) //if file not found
		{
			//ms.Format("The file '%s' does not exist.",filename);
			//AfxMessageBox(ms);
			return;
		}
		
		if(chk>0)//if file is OK
		{
			DisplayStatus("Opening file.......");
			UpdateMenu();
			delete pImagePool1;
			pImagePool1=new CImagePool;

			opening=1;//for open update OnSelchangedTree1

			m_Tree1.DeleteAllItems();
			pImagePool1->ClearData();
			this->SetWindowText("Tree Analyser: untitle");

			pImagePool1->ActivePath=this->GetDirectoryName(filename);

			UpdateMRL(&filename); //update MRL list

			ActiveProjectFilename=m_ldFile.GetPathName();
			CurrentPath=filename.Left(filename.GetLength()-strlen(m_ldFile.GetFileName())); //recalculate current path
			int chk2=0;
			if(chk<1.18){chk2=pImagePool1->OpenTextData(&filename);}
			else{}
			if(chk2)
			{
				vctTreeItem.clear(); //clear old item list
				for (int i=0;i<pImagePool1->GetImageCount();i++)
				{		
					ms.Format("Reading image %i of %i",i,pImagePool1->GetImageCount());
					DisplayStatus(ms);

					pWorkImage=pImagePool1->GetImage(i);
					ms=pWorkImage->GetPictureName();


					LPTSTR ff = new TCHAR[ms.GetLength()+1];
					_tcscpy(ff, ms);

					ActivePictureName=pWorkImage->GetPictureName();

					TV_INSERTSTRUCT TreeCtrlItem;
					TreeCtrlItem.hParent = TVI_ROOT;
					TreeCtrlItem.hInsertAfter = TVI_LAST;
					TreeCtrlItem.item.mask = TVIF_TEXT | TVIF_PARAM;
					TreeCtrlItem.item.pszText = ff;
					TreeCtrlItem.item.lParam = 1;
					HTREEITEM hTreeItem1 = m_Tree1.InsertItem(&TreeCtrlItem);
					m_Tree1.SetCheck(hTreeItem1,pWorkImage->GetCheck());
					m_Tree1.SelectItem(hTreeItem1);
					vctTreeItem.push_back(hTreeItem1);
				}

				ms="Tree Analyser: " + filename;
				this->SetWindowText(ms);
				this->SetWorkingMenu();

				flagChanged=0;
				//ms.Format("Loading project retrieved %i pictre",pImagePool1->GetImageCount());
				//MessageBox(ms);
			}//if(pImagePool1->OpenTextData(&filename))
			opening=0;//for open update OnSelchangedTree1
			if (pImagePool1->GetImageCount()>0)
			{
				//ms.Format("%s",pImagePool1->GetPictureFileName(&ActivePictureName));
				//MessageBox(ms);
				pMenu->EnableMenuItem(ID_IMAGE_SAVEPIXELVALUE,MF_BYCOMMAND);
				double scale=1;
				fShowPicture(pImagePool1->GetPictureFileName(ActivePictureName),&scale);
				DisplayImageProperties(1);
			}
			this->UpdatePicture();
			DisplayStatus();

			//clear virtual canopy
			vp1->acell.clear();
			this->SetWorkingMenu();
			DisplaySettingProperties();

			return;
		}//if chk==1

		if(chk==2)//if file was found but is not the TA format
		{
			ms.Format("The selected file '%s' is not Tree Analyser file or very old format!",filename);
			AfxMessageBox(ms);
		}//if(chk==2)
*/
	} //if (m_ldFile.DoModal() == IDOK)

	opening=0;//for open update OnSelchangedTree1

}//void CTreeAnalyserDlg::OnFileOpenproject()


//! A function to open Tree Analyser file with a given file name.
/*!
*	Last modification: 10-Sep-04.\n
*	Called by:
*	-# CTreeAnalyserDlg::OnInitDialog()
*	-# CTreeAnalyserDlg::OnFile1() 
*	-# CTreeAnalyserDlg::OnFile2() 
*	-# CTreeAnalyserDlg::OnFile3() 
*	-# CTreeAnalyserDlg::OnFile4() 
*/
void CTreeAnalyserDlg::Openproject(CString *fname)
{
	CString filename=*fname;
	double chk=0;
	chk=pImagePool1->CheckFile(filename);
	if(chk==0)//if file not found
	{
		ms.Format("The file '%s' does not exist.",filename);
		AfxMessageBox(ms);
		return;
	}
	if(chk==-1)//if file was found but is not the TA format
	{
		ms.Format("The selected file '%s' is not Tree Analyser file or very old format!",filename);
		AfxMessageBox(ms);
		return;
	}
	if(chk>0)//if the file is TA file
	{
		DisplayStatus("Opening file.......");
		delete pImagePool1;
		pImagePool1=new CImagePool;
		UpdateMenu();

		CString temp=*fname;
		opening=1;//for open update OnSelchangedTree1
		m_Tree1.DeleteAllItems();
		pImagePool1->ClearData();
		this->SetWindowText("Tree Analyser: untitle");
		if(chk<1.18){pImagePool1->OpenTextData(fname);}
		else{pImagePool1->OpenNewFormat(fname);}
		CurrentPath=this->GetDirectoryName(temp);
		UpdateMRL(fname); //update MRL list
		ActiveProjectFilename=*fname;
		ActivePictureName="";
		pImagePool1->ActivePath=this->GetDirectoryName(*fname);

		vctTreeItem.clear(); //clear old item list
		for (int i=0;i<pImagePool1->GetImageCount();i++)
		{
			ms.Format("Reading image %i of %i",i,pImagePool1->GetImageCount());
			DisplayStatus(ms);
			pWorkImage=pImagePool1->GetImage(i);
			ms=pWorkImage->GetPictureName();

			LPTSTR ff = new TCHAR[ms.GetLength()+1];
			_tcscpy(ff, ms);
			ActivePictureName=ms;

			TV_INSERTSTRUCT TreeCtrlItem;
			TreeCtrlItem.hParent = TVI_ROOT;
			TreeCtrlItem.hInsertAfter = TVI_LAST;
			TreeCtrlItem.item.mask = TVIF_TEXT | TVIF_PARAM;
			TreeCtrlItem.item.pszText = ff;
			TreeCtrlItem.item.lParam = 1;
			
			HTREEITEM hTreeItem1 = m_Tree1.InsertItem(&TreeCtrlItem);
			m_Tree1.SelectItem(hTreeItem1);
			m_Tree1.SetCheck(hTreeItem1,pWorkImage->GetCheck());
			vctTreeItem.push_back(hTreeItem1);


		}
		ms="Tree Analyser: " + temp;
		this->SetWindowText(ms);
		this->SetWorkingMenu();

		flagChanged=0;
		opening=0;//for open update OnSelchangedTree1
		if (pImagePool1->GetImageCount()>0)
		{
			//ms.Format("%s",pImagePool1->GetPictureFileName(&ActivePictureName));
			//MessageBox(ms);
			pMenu->EnableMenuItem(ID_IMAGE_SAVEPIXELVALUE,MF_BYCOMMAND);
			double scale=1;
			fShowPicture(pImagePool1->GetPictureFileName(ActivePictureName),&scale);
			DisplayImageProperties(1);
		}
		this->UpdatePicture();

		//clear virtual canopy
		vp1->acell.clear();
		this->SetWorkingMenu();

		DisplayStatus();
		DisplaySettingProperties();
	}//if(chk==1)//if file OK
}//void CTreeAnalyserDlg::OnFileOpenproject()



void CTreeAnalyserDlg::OnFileNewproject() 
{
	UpdateCheckItem();
	if(flagChanged==1 && pImagePool1->GetImageCount()>0)
	{
		int ans=MessageBox("Save change of the current project?","Tree Analyser",MB_YESNOCANCEL);
		if (ans==IDYES)
		{
			if (ActiveProjectFilename==""){OnFileSaveAs();}
			else 
			{
				if (pImagePool1->SaveNewFormat(&ActiveProjectFilename)){flagChanged=0;}
				else {MessageBox("Unable to save file!");}
			}
			Newproject();
			UpdateMenu();
		}
		if (ans==IDNO)
		{
			Newproject();
			UpdateMenu();
		}
		else {}
	}
	else{Newproject();UpdateMenu();}
}

void CTreeAnalyserDlg::Newproject() 	
{
	delete pImagePool1;
	pImagePool1=new CImagePool;
	vp1->acell.clear();
	this->ActiveProjectFilename="";
	opening=1; //for close update OnSelchangedTree1
	m_Tree1.SetFocus();
	m_Tree1.DeleteAllItems();
	vctTreeItem.clear();
	pImagePool1->ClearData();
	pImagePool1->fix_pza=default_pza;
	Invalidate();
	this->SetWindowText("Tree Analyser: untitle");
	this->SetWorkingMenu();

	DisplayImageProperties(0);	
	flagChanged=0;
	opening=0; //for open update OnSelchangedTree1
	DisplaySettingProperties();
}//void CTreeAnalyserDlg::Newproject()  


void CTreeAnalyserDlg::OnFileSaveproject() 
{
	UpdateCheckItem();
	//	MessageBox(ActiveProjectFilename);
	this->DisplayStatus("Saving file.......");
	if (pImagePool1->SaveNewFormat(&ActiveProjectFilename))
	{
		flagChanged=0;
	}
	else {MessageBox("Unable to save file!");}
	DisplayStatus();
}


void CTreeAnalyserDlg::OnImageSavepixelvalue() 
{
	CTreeAnalyserDlg * pNewObject = this; //create pointer to this
	DWORD dwExitCode;
	if (pWorkThread == NULL) //if thread have not been created
	{
		WorkIndex=9;
		pWorkThread=AfxBeginThread(ThreadWork, pNewObject);
	}
	else
	{
		::GetExitCodeThread(pWorkThread->m_hThread,&dwExitCode);
		if(dwExitCode!=STILL_ACTIVE) //if thread is finish
		{
			WorkIndex=9;
			pWorkThread=AfxBeginThread(ThreadWork, pNewObject);
		}
		else //if thread is working
		{
			pWorkThread->SuspendThread();
			if(this->AskForStopThread()==IDNO){pWorkThread->ResumeThread();}
			else
			{
				::WaitForSingleObject(pWorkThread->m_hThread,0); //stop the thread suddenly
				delete pWorkThread;
				pWorkThread=NULL;
				this->DisplayTerminateMessage();
				WorkIndex=0;
				this->SetWorkingMenu();
				DisplayStatus();
			}
		}
	}
}//void CTreeAnalyserDlg::OnImageSavepixelvalue()

void CTreeAnalyserDlg::Savepixelvalue() 
{
	CString output_filename=OutputLocation + ActivePictureName + "_pixel";

     static char BASED_CODE szFilter[] = "Tree Project (*.txt)|*.txt||";
     CFileDialog m_ldFile(FALSE, ".txt", output_filename,
          OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	int ans=m_ldFile.DoModal();
	if (ans == IDOK)
	{
		output_filename = m_ldFile.GetPathName();

		// Get the filename selected
		CString bmp_filename;
         bmp_filename = pImagePool1->GetPictureFileName(ActivePictureName);
		 //MessageBox(pImagePool1->GetPictureFileName());
		CImage ctrlImage;
		HRESULT loadresule= ctrlImage.Load(bmp_filename);
		if (FAILED(loadresule)){MessageBox("Error reading file!\nSelected file may not existed or not be the bitmap file.");}
		else //if load image OK
		{
				COLORREF m_pixel_value; // Current Pixel
				int m_nR; // Red
				int m_nG; // Green
				int m_nB; // Blue

				//------Start write value for each pixel -------
				FILE * pFile;
				pFile=fopen(output_filename,"wt");

				//print header
				fprintf(pFile,"Picture name=%s\n",ActivePictureName);
				fprintf(pFile,"Bitmap source=%s\n",bmp_filename);
				fprintf(pFile,"Image height=%i  width=%i  bit per pixel=%i pitch=%i max_color=%i\n",
					ctrlImage.GetHeight(),ctrlImage.GetWidth(),ctrlImage.GetBPP(),
					ctrlImage.GetPitch(),ctrlImage.GetMaxColorTableEntries());

					
				fprintf(pFile,"px	py	RGB	R	G	B\n");
				//end print header

				int px=0,py=0;//,percent=0;
				while (px<ctrlImage.GetWidth()) //bm.bmWidth)
				{
					ms.Format("Saving %s %3.0f%%",output_filename,double(px)/ctrlImage.GetWidth()*100);
					DisplayStatus(ms);
					py=0;
					while (py<ctrlImage.GetHeight()) //bm.bmHeight)
					{
						m_pixel_value = ctrlImage.GetPixel(px, py);
						m_nR = GetRValue(m_pixel_value);
						m_nG = GetGValue(m_pixel_value);
						m_nB = GetBValue(m_pixel_value);

						fprintf(pFile,"%i	%i	%i	%i	%i	%i\n",
							px,py,m_pixel_value,m_nR,m_nG,m_nB);

						py++;
					}
					px++;
				}
				fclose(pFile);			
				//------End write value for each pixel -------
         } //if load image OK
		ctrlImage.Detach();
	}//if (m_ldFile.DoModal() == IDOK)
	DisplayStatus();
} //void CTreeAnalyserDlg::Savepixelvalue() 


//! Update the picture display on screen.
/*!
*	Created: 15-Jun-03
*	\n Last modification: 4-Oct-04.\n
*	Called by:
*	-# CTreeAnalyserDlg::OnImageDelete()
*	-# CTreeAnalyserDlg::OnFileSaveAs()
*	-# CTreeAnalyserDlg::Colorcounting()
*	-# CTreeAnalyserDlg::OnNMDblclkTree1(NMHDR *pNMHDR, LRESULT *pResult)
*	-# CTreeAnalyserDlg::OnPaint()
*	-# CTreeAnalyserDlg::OnProjectAddimage()
*	-# CTreeAnalyserDlg::OnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult)
*/
void CTreeAnalyserDlg::UpdatePicture()
{
	if(pImagePool1->GetImageCount()) //if there are some images
	{
		CString bmp_filename;
         bmp_filename = pImagePool1->GetPictureFileName(ActivePictureName);
		double scale=1;
		 if (!fShowPicture(bmp_filename,&scale))
		 {
			 OnImageDelete();
			 ms="Error open image =" + bmp_filename;
			 ms=ms+ "\nThis image will be removed from the list.";
			 MessageBox(ms); return;
		 }
		CurrentScale=scale;
		CEachImage *tempImage=pImagePool1->GetImage(&ActivePictureName);

		CDC *pDC=this->GetDC();

		//Draw with a red pen.
		pDC->SelectObject(&penBlueSky);
		pDC->SetBkMode(TRANSPARENT);

		if(center_mark)
		{
			pDC->Arc( //mid circle
				int(tempImage->GetPictureWidth()/2*scale)-11+rectPic.left,int(tempImage->GetPictureHeight()/2*scale)-11+rectPic.top, //upper left
				int(tempImage->GetPictureWidth()/2*scale)+11+rectPic.left,int(tempImage->GetPictureHeight()/2*scale)+11+rectPic.top, //lower right
				int(tempImage->GetPictureWidth()/2*scale)-11+rectPic.left,int(tempImage->GetPictureHeight()/2*scale)-11+rectPic.top, //start point
				int(tempImage->GetPictureWidth()/2*scale)-11+rectPic.left,int(tempImage->GetPictureHeight()/2*scale)-11+rectPic.top //end=start
				);
			//horizontal line
			pDC->MoveTo(int(tempImage->GetPictureWidth()/2*scale)-16+rectPic.left,
				int(tempImage->GetPictureHeight()/2*scale)+rectPic.top);
			pDC->LineTo(int(tempImage->GetPictureWidth()/2*scale)+16+rectPic.left,
				int(tempImage->GetPictureHeight()/2*scale)+rectPic.top);
			//vertical line
			pDC->MoveTo(int(tempImage->GetPictureWidth()/2*scale)+rectPic.left,
				int(tempImage->GetPictureHeight()/2*scale)-16+rectPic.top);
			pDC->LineTo(int(tempImage->GetPictureWidth()/2*scale)+rectPic.left,
				int(tempImage->GetPictureHeight()/2*scale)+16+rectPic.top);
		}
		/*
		//test draw rectangle
		int gx=32,gy=32;
		int px=int(tempImage->GetPictureWidth()/2*scale);
		int py=int(tempImage->GetPictureHeight()/2*scale);
		//left
		pDC->MoveTo(px-gx/2+rectPic.left,py-gy/2+rectPic.top); //top left
		pDC->LineTo(px-gx/2+rectPic.left,py+gy/2+rectPic.top);
		//right
		pDC->MoveTo(px+gx/2+rectPic.left,py-gy/2+rectPic.top); //top right
		pDC->LineTo(px+gx/2+rectPic.left,py+gy/2+rectPic.top);
		//top
		pDC->MoveTo(px-gx/2+rectPic.left,py-gy/2+rectPic.top);//top left
		pDC->LineTo(px+gx/2+rectPic.left,py-gy/2+rectPic.top);
		//bottom
		pDC->MoveTo(px-gx/2+rectPic.left,py+gy/2+rectPic.top);//bottom left
		pDC->LineTo(px+gx/2+rectPic.left,py+gy/2+rectPic.top);
*/
		if(tempImage->ifMarkedPoint && margin_mark)
		{
			pDC->SelectObject(&penRed);
			int radious=5;
			//draw circle around the point
			pDC->Arc( //left circle
				int(tempImage->pixLeft.x*scale)-radious+rectPic.left,int(tempImage->pixLeft.y*scale)-radious+rectPic.top, //upper left
                int(tempImage->pixLeft.x*scale)+radious+rectPic.left,int(tempImage->pixLeft.y*scale)+radious+rectPic.top, //lower right
				int(tempImage->pixLeft.x*scale)-radious+rectPic.left,int(tempImage->pixLeft.y*scale)-radious+rectPic.top, //start point
				int(tempImage->pixLeft.x*scale)-radious+rectPic.left,int(tempImage->pixLeft.y*scale)-radious+rectPic.top //end=start
				);

			pDC->Arc(//right circle
				int(tempImage->pixRight.x*scale)-radious+rectPic.left,int(tempImage->pixRight.y*scale)-radious+rectPic.top,//upper left
				int(tempImage->pixRight.x*scale)+radious+rectPic.left,int(tempImage->pixRight.y*scale)+radious+rectPic.top,//lower right
				int(tempImage->pixRight.x*scale)-radious+rectPic.left,int(tempImage->pixRight.y*scale)-radious+rectPic.top,//start point
				int(tempImage->pixRight.x*scale)-radious+rectPic.left,int(tempImage->pixRight.y*scale)-radious+rectPic.top//end=start
				);
  

			pDC->Arc(//top circle
				int(tempImage->pixTop.x*scale)-radious+rectPic.left,int(tempImage->pixTop.y*scale)-radious+rectPic.top,//upper left
				int(tempImage->pixTop.x*scale)+radious+rectPic.left,int(tempImage->pixTop.y*scale)+radious+rectPic.top,//lower right
				int(tempImage->pixTop.x*scale)-radious+rectPic.left,int(tempImage->pixTop.y*scale)-radious+rectPic.top,//start point
				int(tempImage->pixTop.x*scale)-radious+rectPic.left,int(tempImage->pixTop.y*scale)-radious+rectPic.top//end=start
				);

			pDC->Arc(//bottom circle
				int(tempImage->pixBottom.x*scale)-radious+rectPic.left,int(tempImage->pixBottom.y*scale)-radious+rectPic.top,//upper left
                int(tempImage->pixBottom.x*scale)+radious+rectPic.left,int(tempImage->pixBottom.y*scale)+radious+rectPic.top,//lower right
				int(tempImage->pixBottom.x*scale)-radious+rectPic.left,int(tempImage->pixBottom.y*scale)-radious+rectPic.top,//start point
				int(tempImage->pixBottom.x*scale)-radious+rectPic.left,int(tempImage->pixBottom.y*scale)-radious+rectPic.top//end=start
				);
		}//if(tempImage->ifMarkedPoint)
	} //if(pImagePool1->GetImageCount())
} //void CTreeAnalyserDlg::UpdatePicture()


bool CTreeAnalyserDlg::fShowPicture(CString fname, double *scale_return)
{
		bool round=0;
try_again:;

		CImage ctrlImage; //create control for the image
		HRESULT loadresule= ctrlImage.Load(fname); // Load the selected bitmap file

         if (!FAILED(loadresule))
         {
			//calculate area for picture
			CRect rectOutput;
			m_EditOutput.GetClientRect(rectOutput);

			CRect clear_area;
			clear_area.top=rectPic.top-5;
			clear_area.bottom=rectPic.bottom+5;
			clear_area.left=rectPic.left-5;
			clear_area.right=rectPic.right+5;
			InvalidateRect(clear_area); //clear area

			//calculate scale for picture
			double scaleHorizontal=1,scaleVertical=1,scale=1;
			if(ctrlImage.GetWidth()<rectPic.Width() && ctrlImage.GetHeight()<rectPic.Height())
			{//picture is smaller than display area
				scale=1;
			}
			else if(ctrlImage.GetWidth()>rectPic.Width() && ctrlImage.GetHeight()>rectPic.Height())
			{//both vertical and horizantal are bigger than display area
				if(ctrlImage.GetWidth()>ctrlImage.GetHeight()) //horizontal image
				{
					scaleHorizontal=double(rectPic.Width())/double (ctrlImage.GetWidth());
					scaleVertical=double(rectPic.Height())/double(ctrlImage.GetHeight());
					if (scaleHorizontal<scaleVertical){scale=scaleHorizontal;}
					else {scale=scaleVertical;}
				}
				else //vertical image
				{
					scaleHorizontal=double(rectPic.Width())/double (ctrlImage.GetWidth());
					scaleVertical=double(rectPic.Height())/double(ctrlImage.GetHeight());
					if (scaleHorizontal<scaleVertical){scale=scaleHorizontal;}
					else {scale=scaleVertical;}
				}	
			}
			else if(ctrlImage.GetWidth()>rectPic.Width())
			{//width of the image is larger than display width
				scale=double(rectPic.Width())/ctrlImage.GetWidth();
			}
			else if(ctrlImage.GetHeight()>rectPic.Height())
			{//height of the image is larger than display height
				scale=double(rectPic.Height())/ctrlImage.GetHeight();
			}

			CPaintDC dc(this);
			ctrlImage.StretchBlt(dc,
				rectPic.left, rectPic.top ,	//start drawing point
				int(ctrlImage.GetWidth()*scale),int(ctrlImage.GetHeight()*scale)
				,SRCCOPY);

			*scale_return=scale;
			return 1;
		 }	
		else //if load image failed then try again in current directory
		{
			if (round==0)
			{
				CString oname=fname;
				fname=this->CurrentPath+this->GetFileName(fname,0);
				pImagePool1->ChangeFilename(oname,fname);
				this->flagChanged=1;
				round=1;
				goto try_again;
			}
			else {return 0;}
		}
}

void CTreeAnalyserDlg::DisplayImageProperties(bool option)
{
	if(option)
	{
		CEachImage *temp=pImagePool1->GetImage(&ActivePictureName);

		CString l1;
		ms="";
		l1.Format("Dimension:%ix%i pixels\r\n",temp->GetPictureWidth(),temp->GetPictureHeight());
		ms+=l1;
		l1.Format("Camera distance=:%6.2f m.\r\n",temp->GetCameraDistance());
		ms+=l1;
		l1.Format("Camera height=:%6.2f m.\r\n",temp->GetCameraHeight());
		ms+=l1;
		l1.Format("Camera direction=:%6.2f deg.\r\n",temp->GetCameraDirection());
		ms+=l1;
		l1.Format("Camera elevation=:%6.2f deg.\r\n",temp->GetCameraInc());
		ms+=l1;
		l1.Format("Camera azimuth=:%6.2f deg.\r\n",temp->GetCameraBending());
		ms+=l1;
		l1.Format("Camera rotation=:%6.2f deg.\r\n",temp->GetCameraRot());
		ms+=l1;
		l1.Format("Focal length=:%6.2f mm.\r\n",temp->GetCameraFocalLength());
		ms+=l1;
		//l1.Format("Picture type: %s\r\n",(temp->GetPictureType()==0)?"Perspective":"Hemispheric");
		//ms+=l1;
		l1.Format("%s  (%6.4f mm)\r\n",temp->camera_name,temp->GetCameraCCDSize());
		ms+=l1;

		l1.Format("View angle: %6.2f\r\n",atan(temp->GetCameraCCDSize()/(temp->GetCameraFocalLength()*2))*2*180/acos(-1.0));
		ms+=l1;

		l1.Format("Optimal zone size= %ix%i  pixels\r\n",pImagePool1->CalculOptimalZoneSize(temp),pImagePool1->CalculOptimalZoneSize(temp));
		ms+=l1;

		double a=temp->Get_cam_ele_deg();
		l1.Format("G (camera direction) = %1.4f\r\n",pImagePool1->CalculG(temp->Get_cam_ele_deg()));
		ms+=l1;

		//double a=temp->CalculSectionArea(int(pImagePool1->CalculMinimumZoneSize(temp)),temp->GetPictureWidth()/2.0,temp->GetPictureHeight()/2.0)*10000;
		//double b=pImagePool1->CalculMinimumZoneSize(temp);
		//l1.Format("Optimal sampling= %6.4f cm2",temp->CalculSectionArea(int(pImagePool1->CalculMinimumZoneSize(temp)*4),temp->GetPictureWidth()/2.0,temp->GetPictureHeight()/2.0)*10000);
		//ms+=l1;

		m_PropertiesBox.SetWindowText(ms);
	}
	else{m_PropertiesBox.SetWindowText("");}
}

void CTreeAnalyserDlg::DisplaySettingProperties()
{
	CString l1;
	ms="";
	l1.Format("Voxel size:%3.1fx%3.1fx%3.1f cm\r\n",pImagePool1->cellSize_dx*100,pImagePool1->cellSize_dy*100,pImagePool1->cellSize_dz*100);
	ms+=l1;

	if(pImagePool1->m_inversion_method==0){l1.Format("Model: Binomial\r\n");} 
	if(pImagePool1->m_inversion_method==1){l1.Format("Model: Beer's\r\n");} 
	ms+=l1;

	l1.Format("Mean leaf area: %3.2f cm2\r\n",pImagePool1->m_mean_leaf_area);
	ms+=l1;

		if(pImagePool1->GetLeafDistributionName().Left(6)=="Custom")
	{
		l1="Leaf distribution: Custom\r\n";
	}
	else{l1.Format("Leaf distribution: %s\r\n",pImagePool1->GetLeafDistributionName());}
	ms+=l1;

	l1="-------------------------------------------------------------------\r\n";
	ms+=l1;

	l1.Format("Canopy height: %3.4f m\r\n",pImagePool1->averageTreeHeight );
	ms+=l1;

	l1.Format("Canopy diameter: %3.4f m\r\n",pImagePool1->averageDiameter);
	ms+=l1;

	l1.Format("Total voxels: %i\r\n",vp1->GetTotal_cells());
	ms+=l1;

	l1.Format("Volume: %6.4f m3\r\n",vp1->GetCanopyVolume());
	ms+=l1;
	
	l1.Format("Total Leaf area: %6.4f m2\r\n",pImagePool1->EsimatedLeafArea);
	ms+=l1;


	/*
	l1.Format("Camera distance=:%6.2f m.\r\n",temp->GetCameraDistance());
	ms+=l1;
	l1.Format("Camera height=:%6.2f m.\r\n",temp->GetCameraHeight());
	ms+=l1;
*/
	this->ctrl_setting_properties.SetWindowText(ms);
}//void CTreeAnalyserDlg::DisplaySettingProperties()

void CTreeAnalyserDlg::DisplayStatus(CString inputText)
{
	m_statusBar.SetWindowText(inputText);
	m_statusBar.UpdateWindow();
}

void CTreeAnalyserDlg::DisplayStatus()
{
	m_statusBar.SetWindowText("Ready");
	m_statusBar.UpdateWindow();
}

void CTreeAnalyserDlg::AddDisplayText(CString *txt)
{
	*txt+="\r\n";
	m_EditOutput.SetSel(m_EditOutput.GetWindowTextLength(),m_EditOutput.GetWindowTextLength(), FALSE);
	m_EditOutput.ReplaceSel(*txt);

}

void CTreeAnalyserDlg::AddDisplayText(CString txt)
{
	txt+="\r\n";

	m_EditOutput.SetSel(m_EditOutput.GetWindowTextLength(),m_EditOutput.GetWindowTextLength(), FALSE);
	m_EditOutput.ReplaceSel(txt);
}



void CTreeAnalyserDlg::OnImageEditparameters() 
{
	
	CEachImage *tImage;
	tImage=pImagePool1->GetImage(&ActivePictureName); //problem in this line
	CDlgAdd editDlg;

	editDlg.pCameraPool=pCameraPool;
	editDlg.CurrentPath=CurrentPath;
	editDlg.dlgTitle="Edit Image Properties";
	editDlg.callType=1;

	//data
	editDlg.m_bending=tImage->GetCameraBending();
	editDlg.CameraName=tImage->camera_name;
	editDlg.camera_cal=tImage->GetCameraCCDSize();
	editDlg.m_direction=tImage->GetCameraDirection();
	editDlg.m_selectedDirection=tImage->GetSelectedDirection();
	editDlg.m_distance=tImage->GetCameraDistance();
	editDlg.m_focalLength=tImage->GetCameraFocalLength();
	editDlg.m_height=tImage->GetCameraHeight();
	editDlg.m_inc=tImage->GetCameraInc();
	editDlg.m_fname=tImage->GetFileName();
	editDlg.m_pictureBit=tImage->GetPictureBit();
	editDlg.m_pictureHeight=tImage->GetPictureHeight();
	editDlg.m_picName=tImage->GetPictureName();
	editDlg.m_rot=tImage->GetCameraRot();
	editDlg.m_pictureWidth=tImage->GetPictureWidth();

	if (editDlg.DoModal()==IDOK) //user click OK
	{
		LastCamera=editDlg.CameraName;

		tImage->PutCameraBending(editDlg.m_bending);
		tImage->camera_name=editDlg.CameraName;
		tImage->PutCameraCCDSize(editDlg.camera_cal);
		tImage->PutCameraDirection(editDlg.m_direction);
		tImage->PutCameraDistance(editDlg.m_distance);
		tImage->PutCameraFocalLength(editDlg.m_focalLength);
		tImage->PutCameraHeight(editDlg.m_height);
		tImage->PutCameraInc(editDlg.m_inc);
		tImage->PutFileName(editDlg.m_fname);
		tImage->PutPictureBit(editDlg.m_pictureBit);
		tImage->PutPictureHeight(editDlg.m_pictureHeight);
		tImage->PutPictureName(editDlg.m_picName);
		tImage->PutCameraRot(editDlg.m_rot);
		tImage->PutPictureWidth(editDlg.m_pictureWidth);
		tImage->PutSelectedDirection(editDlg.m_selectedDirection);

		//convert entered picture name to null terminate string before use with tree item
		ms=editDlg.m_picName;

		LPTSTR ff = new TCHAR[ms.GetLength()+1];
		_tcscpy(ff, ms);

		ActivePictureName=ms; //update active picture name
		

		//reset selected item
		HTREEITEM aa=m_Tree1.GetSelectedItem();	
		int currentID=GetTreeID(&aa);
		m_Tree1.SetItemText(aa,ff);
		vctTreeItem[currentID]=aa;
		flagChanged=1;
		DisplayImageProperties(1);

	}
	else{}

//	UpdatePicture(); //this line cause problem if forget to update ActivePictureName

	
}

void CTreeAnalyserDlg::OnImageDelete() 
{
	//delete data from imagepool
	HTREEITEM aa=m_Tree1.GetSelectedItem();
	ActivePictureName=m_Tree1.GetItemText(aa);
	ms.Format("Would you like to remove '%s' from the list?",ActivePictureName);
	int ans=MessageBox(ms,"Confirm Delete Picture",MB_YESNO);
	if (ans==IDYES)
	{
		//ms.Format("%i",pImagePool1->GetImageID(&ActivePictureName));
		//MessageBox(ms);
		pImagePool1->RemoveImage(pImagePool1->GetImageID(&ActivePictureName));
		//ms.Format("count=%i",pImagePool1->GetImageCount());
		//MessageBox(ms);

		//remove from tree control
		m_Tree1.DeleteItem(aa);
		ms.Format("Test=%i",GetTreeID(&aa));
		vctTreeItem.erase(&vctTreeItem[GetTreeID(&aa)]);
		flagChanged=1;

		//update new picture and properties box if it is the last picture
		if(pImagePool1->GetImageCount()==0)
		{
			UpdatePicture();
			DisplayImageProperties(0);	
		}
	}
}


void CTreeAnalyserDlg::IniWrite()
{
	CString filename;
	filename=ProgramPath+"TreeAnalyser.ini";
	CFile theFile;
	CFileException e;
	if( !theFile.Open( filename.GetString(), CFile::modeCreate | CFile::modeWrite, &e ) )
	{
		#ifdef _DEBUG
		afxDump << "Initial file could not be opened " << e.m_cause << "\n";
		#endif
		//return false;
	}
	CArchive ar(&theFile, CArchive::store);

	//write data
	ms="Tree Analyser initial file";
	ar<<ms;
	double version=1.2;
	ar<<version;
	ar<<CurrentPath;
	ar<<OutputLocation;
	ar<<mrl[0];
	ar<<mrl[1];
	ar<<mrl[2];
	ar<<mrl[3];

	ar<<LastCamera;
	ar<<last_cam_distance;
	ar<<last_cam_height;
	ar<<last_fc;
	ar<<last_ele;
	ar<<last_selected_direction;
	ar<<default_pza;

	ar.Close();
	theFile.Close();
//	return true;
}



bool CTreeAnalyserDlg::IniRead()
{
	CString filename;
	filename=ProgramPath+"TreeAnalyser.ini";
	CFile theFile;
	CFileException e;
	if( !theFile.Open( filename.GetString(), CFile::modeRead, &e ) )
	{
		#ifdef _DEBUG
		afxDump << "Initial file could not be opened " << e.m_cause << "\n";
		#endif
		return false;
	}

	CArchive ar(&theFile, CArchive::load);

	//begin read data
	ar>>ms;
	if(ms!="Tree Analyser initial file")
	{
		ar.Close();
		theFile.Close();
		return 0;
	}
	else//if is TA ini file
	{
		double version=0;
		ar>>version;
		if(version<=1.2)
		{
			ar>>CurrentPath;
			ar>>OutputLocation;
			ar>>mrl[0];
			ar>>mrl[1];
			ar>>mrl[2];
			ar>>mrl[3];

			ar>>LastCamera;
			ar>>last_cam_distance;
			ar>>last_cam_height;
			ar>>last_fc;
			ar>>last_ele;
			ar>>last_selected_direction;
			if(version==1.20)
			{
				ar>>this->default_pza;
			}
		}//if(version=1.18)

		ar.Close();
		theFile.Close();
		return 1;
	}//else//if is TA ini file
}//bool CTreeAnalyserDlg::IniRead()



void CTreeAnalyserDlg::UpdateMRL(CString *fname)
{
	int i=-1;
	if (*fname==mrl[0]){i=0;}
	else 
	{	
		if (*fname==mrl[1]){i=1;}
		else
		{
			if (*fname==mrl[2]){i=2;}
			else 
			{
				if (*fname==mrl[3]){i=3;}
			}
		}
	}
//	ms.Format("i=%i",i);
//	MessageBox(ms);
	//if open file on the list
	if (i>=0)
	{
		CString temp=mrl[i];
		for (int j=i;j>0;j--)
		{
			mrl[j]=mrl[j-1];
		}
		mrl[0]=temp;
	}
	else
	{
		if (mrl[2]!="3.") {mrl[3]=mrl[2];}
		if (mrl[1]!="2.") {mrl[2]=mrl[1];}
		if (mrl[0]!="1.") {mrl[1]=mrl[0];}
		mrl[0]=*fname;
	}

}

void CTreeAnalyserDlg::UpdateMenu()
{
	this->SetWindowText("Tree Analyser: untitle");
	this->SetWorkingMenu();
}


void CTreeAnalyserDlg::OnFile1() 
{
	pMenu = this->GetMenu();
	pMenu->GetMenuString(ID_FILE_1,ms,0);
	if (ms!="1.")
	{
		UpdateCheckItem();
		if(flagChanged==1 && pImagePool1->GetImageCount()>0)
		{
			int ans=MessageBox("Save change of the current project?","Tree Analyser",MB_YESNOCANCEL);
			if (ans==IDYES)
			{
				if (ActiveProjectFilename==""){OnFileSaveAs();}
				else 
				{
					if (pImagePool1->SaveNewFormat(&ActiveProjectFilename)){flagChanged=0;}
					else {MessageBox("Unable to save file!");}
				}
				Openproject(&ms);
			}
			if (ans==IDNO)
			{
				Openproject(&ms);
			}
			else {}
		}
		else{Openproject(&ms);}
		//Openproject(&ms);		
	}
}


void CTreeAnalyserDlg::OnFile2() 
{
	pMenu = this->GetMenu();
	pMenu->GetMenuString(ID_FILE_2,ms,0);
	if (ms!="2.")
	{
		UpdateCheckItem();
		if(flagChanged==1 && pImagePool1->GetImageCount()>0)
		{
			int ans=MessageBox("Save change of the current project?","Tree Analyser",MB_YESNOCANCEL);
			if (ans==IDYES)
			{
				if (ActiveProjectFilename==""){OnFileSaveAs();}
				else 
				{
					if (pImagePool1->SaveNewFormat(&ActiveProjectFilename)){flagChanged=0;}
					else {MessageBox("Unable to save file!");}
				}
				Openproject(&ms);
			}
			if (ans==IDNO)
			{
				Openproject(&ms);
			}
			else {}
		}
		else{Openproject(&ms);}
		//Openproject(&ms);		
	}	
}


void CTreeAnalyserDlg::OnFile3() 
{
	pMenu = this->GetMenu();
	pMenu->GetMenuString(ID_FILE_3,ms,0);
	if (ms!="3.")
	{
		UpdateCheckItem();
		if(flagChanged==1 && pImagePool1->GetImageCount()>0)
		{
			int ans=MessageBox("Save change of the current project?","Tree Analyser",MB_YESNOCANCEL);
			if (ans==IDYES)
			{
				if (ActiveProjectFilename==""){OnFileSaveAs();}
				else 
				{
					if (pImagePool1->SaveNewFormat(&ActiveProjectFilename)){flagChanged=0;}
					else {MessageBox("Unable to save file!");}
				}
				Openproject(&ms);
			}
			if (ans==IDNO)
			{
				Openproject(&ms);
			}
			else {}
		}
		else{Openproject(&ms);}

		//	Openproject(&ms);		
	}
}


void CTreeAnalyserDlg::OnFile4() 
{
	pMenu = this->GetMenu();
	pMenu->GetMenuString(ID_FILE_4,ms,0);
	if (ms!="4.")
	{
		UpdateCheckItem();
		if(flagChanged==1 && pImagePool1->GetImageCount()>0)
		{
			int ans=MessageBox("Save change of the current project?","Tree Analyser",MB_YESNOCANCEL);
			if (ans==IDYES)
			{
				if (ActiveProjectFilename==""){OnFileSaveAs();}
				else 
				{
					if (pImagePool1->SaveNewFormat(&ActiveProjectFilename)){flagChanged=0;}
					else {MessageBox("Unable to save file!");}
				}
				Openproject(&ms);
			}
			if (ans==IDNO)
			{
				Openproject(&ms);
			}
			else {}
		}
		else{Openproject(&ms);}

		//Openproject(&ms);		
	}
}



void CTreeAnalyserDlg::UpdateCheckItem()
{
	CEachImage *pImage;

	for(unsigned int i=0;i<vctTreeItem.size();i++)
	{
		pImage=pImagePool1->GetImage(i);
		bool tempbool;
		if(m_Tree1.GetCheck(vctTreeItem[i])==0){tempbool=0;} else {tempbool=1;}
		if(pImage->GetCheck()!=tempbool){this->flagChanged=1;}
		pImage->PutCheck(tempbool);
	}

}


int CTreeAnalyserDlg::GetTreeID(HTREEITEM *item)
{

	unsigned int i=0;
	while(i<vctTreeItem.size())
	{
		if(vctTreeItem[i]==*item)
		{
			break;}
		i++;
	}
	return i;
}



void CTreeAnalyserDlg::OnOK() 
{
	// TODO: Add extra validation here
//	TRACE("CTreeAnalyserDlg::OnOK\n");
//	MessageBox("Exit");
//	CDialog::OnOK();
}

void CTreeAnalyserDlg::OnProjectEstimatetreedimension() 
{
	CTreeAnalyserDlg * pNewObject = this; //create pointer to this
	DWORD dwExitCode;
	if (pWorkThread == NULL) //if thread have not been created
	{
		WorkIndex=1;
		pWorkThread=AfxBeginThread(ThreadWork, pNewObject);
	}
	else
	{
		::GetExitCodeThread(pWorkThread->m_hThread,&dwExitCode);
		if(dwExitCode!=STILL_ACTIVE) //if thread is finish
		{
			WorkIndex=1;
			pWorkThread=AfxBeginThread(ThreadWork, pNewObject);
		}
		else //if thread is working
		{
			pWorkThread->SuspendThread();
			if(this->AskForStopThread()==IDNO){pWorkThread->ResumeThread();}
			else
			{
				::WaitForSingleObject(pWorkThread->m_hThread,0); //stop the thread suddenly
				delete pWorkThread;
				pWorkThread=NULL;
				this->DisplayTerminateMessage();
				WorkIndex=0;
				this->SetWorkingMenu();
			}
		}
	}
	DisplaySettingProperties();
}//void CTreeAnalyserDlg::OnProjectEstimatetreedimension() 



void CTreeAnalyserDlg::OnClose() 
{
	//check if thread is working
	DWORD dwExitCode;
	if (pWorkThread == NULL){goto nextstep;}
	else
	{
		::GetExitCodeThread(pWorkThread->m_hThread,&dwExitCode);
		if(dwExitCode!=STILL_ACTIVE){goto nextstep;}
		else //if thread is working
		{
			pWorkThread->SuspendThread();
			if(this->AskForStopThread()==IDNO){pWorkThread->ResumeThread();}
			else
			{
				::WaitForSingleObject(pWorkThread->m_hThread,0);
				delete pWorkThread;
				pWorkThread=NULL;
				this->DisplayTerminateMessage();
				WorkIndex=0;
				this->SetWorkingMenu();
				DisplayStatus();
			}
		}
	}

nextstep:;

	UpdateCheckItem();
	if(flagChanged)
	{	
		int ans;
		ans=MessageBox("Project has been changed. \nSave change before exit?","Tree Analyser",MB_YESNOCANCEL);
		if (ans==IDYES)
		{
			if (ActiveProjectFilename==""){OnFileSaveAs();}
			else 
			{
				if (pImagePool1->SaveNewFormat(&ActiveProjectFilename)){flagChanged=0;}
				else {MessageBox("Unable to save file!");}
			}
			CDialog::OnClose();
		}
		if (ans==IDNO)
		{
			CDialog::OnClose();
		}
	}
	else {CDialog::OnClose();}
}

void CTreeAnalyserDlg::OnProjectSetting() 
{
	// TODO: Add your command handler code here
	CProgramSettingDlg dlg;
	
	dlg.m_OutputLocation=OutputLocation;
	dlg.m_cellSize_dx=pImagePool1->cellSize_dx*100;
	dlg.m_cellSize_dy=pImagePool1->cellSize_dy*100;
	dlg.m_cellSize_dz=pImagePool1->cellSize_dz*100;
	dlg.m_enlarge =int(pImagePool1->m_enlarge*100);
	dlg.m_raypx=pImagePool1->raypx;
	dlg.m_raypy=pImagePool1->raypy;
	dlg.m_ifoutputbox=pImagePool1->m_ifoutputbox ;
	dlg.m_iflogvolume=pImagePool1->m_iflogvolume;
	dlg.m_LimitEle=pImagePool1->m_LimitEle;

	//add for leaf area options
	dlg.m_leaf_distribution=pImagePool1->m_leaf_distribution;
	dlg.gpx=pImagePool1->gpx;
	dlg.gpy=pImagePool1->gpy;
	dlg.gap_average_option=pImagePool1->gap_average_option;
	dlg.CurrentPath=this->CurrentPath;
	dlg.m_mean_leaf_inc=pImagePool1->m_mean_leaf_inc;
	dlg.m_dis_file=pImagePool1->LeafDistributionFile;
	dlg.minimumGap=pImagePool1->minimumGap;
	dlg.ZeroGapManage=pImagePool1->ZeroGapManage;
	dlg.IfSetMaxLAD=pImagePool1->IfSetMaxLAD;
	dlg.MaxLAD=pImagePool1->MaxLAD;
	dlg.m_leaf_dispersion=pImagePool1->m_leaf_dispersion;
	dlg.m_leaf_dispersion_option=pImagePool1->m_leaf_dispersion_option;
	dlg.gap_option=pImagePool1->gap_option; //zoning size option 0=auto 1=custom 2=fixed pza
	dlg.m_mean_leaf_area=pImagePool1->m_mean_leaf_area;
	dlg.m_inversion_method=pImagePool1->m_inversion_method;
	dlg.fix_pza=pImagePool1->fix_pza;
	dlg.default_pza=this->default_pza;

	dlg.is_divide_pza=pImagePool1->is_divide_pza;
	dlg.division_horizontal=pImagePool1->division_horizontal;
	dlg.division_vertical=pImagePool1->division_vertical;

	//ms.Format("inversion=%i",dlg.m_inversion_method);
	//MessageBox(ms);
	//Add for LAD options
	dlg.m_ChkLAD=pImagePool1->ifCalculVprofile;


	//CString OImageLocation=ImageLocation;
	CString OOutputLocation=OutputLocation;
	double OcellSize_dx=pImagePool1->cellSize_dx*100;
	double OcellSize_dy=pImagePool1->cellSize_dy*100;
	double OcellSize_dz=pImagePool1->cellSize_dz*100;
	double Oenlarge =pImagePool1->m_enlarge*100;
	int Oraypx=pImagePool1->raypx;
	int Oraypy=pImagePool1->raypy;
	int OLimitEle=pImagePool1->m_LimitEle;
	BOOL Oifoutputbox=pImagePool1->m_ifoutputbox ;
	BOOL Oifoutputskeleton=pImagePool1->m_iflogvolume;
	int O_leaf_distribution=pImagePool1->m_leaf_distribution;
	int Ogpx=pImagePool1->gpx;
	int Ogpy=pImagePool1->gpy;
	int Ogap_average_option=pImagePool1->gap_average_option;
	double Omean_inc=pImagePool1->m_mean_leaf_inc;
	CString Oleafdisfile=pImagePool1->LeafDistributionFile;
	double OminimumGap=pImagePool1->minimumGap;
	int OZeroGapManage=pImagePool1->ZeroGapManage;
	bool OIfSetMaxLAD=pImagePool1->IfSetMaxLAD;
	double OMaxLAD=pImagePool1->MaxLAD;
	double Om_leaf_dispersion=pImagePool1->m_leaf_dispersion;
	int Om_leaf_dispersion_option=pImagePool1->m_leaf_dispersion_option;
	int Ogap_option=pImagePool1->gap_option;
	double Ofix_pza=pImagePool1->fix_pza;
	double Om_mean_leaf_area=pImagePool1->m_mean_leaf_area;
	double Odefault_pza=this->default_pza;

	BOOL Ois_divide_pza=pImagePool1->is_divide_pza;
	int Odivision_horizontal=pImagePool1->division_horizontal;
	int Odivision_vertical=pImagePool1->division_vertical;

	int Om_inversion_method=pImagePool1->m_inversion_method;
	BOOL Om_ChkLAD=pImagePool1->ifCalculVprofile;


	if(WorkIndex>0){dlg.flagDisableControls=1;} //set flag for disable if it is working

	if(dlg.DoModal()==IDOK)
	{	
		CString tempstring;
		OutputLocation=dlg.m_OutputLocation;
		if(OutputLocation!=OOutputLocation){flagChanged=1;}
		tempstring=OutputLocation;
		if(tempstring.GetAt(tempstring.GetLength()-1)!='\\')
		{
			OutputLocation=OutputLocation+"\\";
		}

		pImagePool1->m_LimitEle=dlg.m_LimitEle;
		if(pImagePool1->m_LimitEle!=OLimitEle){flagChanged=1;}

		//ms.Format("limited=%i",pImagePool1->m_LimitEle);
		//MessageBox(ms);

		pImagePool1->cellSize_dx=dlg.m_cellSize_dx/100;
		if(pImagePool1->cellSize_dx!=OcellSize_dx){flagChanged=1;}

		pImagePool1->cellSize_dy=dlg.m_cellSize_dy/100;
		if(pImagePool1->cellSize_dy!=OcellSize_dy){flagChanged=1;}

		pImagePool1->cellSize_dz=dlg.m_cellSize_dz/100;
		if(pImagePool1->cellSize_dz!=OcellSize_dz){flagChanged=1;}

		pImagePool1->m_enlarge =double(dlg.m_enlarge)/100;
		if(pImagePool1->m_enlarge!=Oenlarge){flagChanged=1;}

		pImagePool1->raypx=dlg.m_raypx;
		if(pImagePool1->raypx!=Oraypx){flagChanged=1;}

		pImagePool1->raypy=dlg.m_raypy;
		if(pImagePool1->raypy!=Oraypy){flagChanged=1;}

		pImagePool1->m_ifoutputbox=dlg.m_ifoutputbox ;
		if(pImagePool1->m_ifoutputbox!=Oifoutputbox){flagChanged=1;}

		pImagePool1->m_iflogvolume=dlg.m_iflogvolume;
		if(pImagePool1->m_iflogvolume!=Oifoutputskeleton){flagChanged=1;}

		pImagePool1->m_leaf_distribution=dlg.m_leaf_distribution;
		if(pImagePool1->m_leaf_distribution!=O_leaf_distribution){flagChanged=1;}

		pImagePool1->gpy=dlg.gpy;
		if(pImagePool1->gpy!=Ogpy){flagChanged=1;}
		pImagePool1->gpx=dlg.gpx;
		if(pImagePool1->gpx!=Ogpx){flagChanged=1;}

		pImagePool1->gap_option=dlg.gap_option;
		if(pImagePool1->gap_option!=Ogap_option){flagChanged=1;}

		pImagePool1->fix_pza=dlg.fix_pza;
		if(pImagePool1->fix_pza!=Ofix_pza){flagChanged=1;}

		pImagePool1->gap_average_option=dlg.gap_average_option;
		if(pImagePool1->gap_average_option!=Ogap_average_option){flagChanged=1;}
		pImagePool1->m_mean_leaf_inc=dlg.m_mean_leaf_inc;
		if(pImagePool1->m_mean_leaf_inc!=Omean_inc){flagChanged=1;}
		 pImagePool1->LeafDistributionFile=dlg.m_dis_file;
		if(pImagePool1->LeafDistributionFile!=Oleafdisfile){flagChanged=1;}

		pImagePool1->ZeroGapManage=dlg.ZeroGapManage;
		if(pImagePool1->ZeroGapManage!=OZeroGapManage){flagChanged=1;}

		pImagePool1->minimumGap=dlg.minimumGap;
		if(pImagePool1->minimumGap!=OminimumGap){flagChanged=1;}

		pImagePool1->IfSetMaxLAD=dlg.IfSetMaxLAD;
		if(pImagePool1->IfSetMaxLAD!=OIfSetMaxLAD){flagChanged=1;}

		
		pImagePool1->MaxLAD=dlg.MaxLAD;
		if(pImagePool1->MaxLAD!=OMaxLAD){flagChanged=1;}

		pImagePool1->m_leaf_dispersion=dlg.m_leaf_dispersion;
		if(pImagePool1->m_leaf_dispersion!=Om_leaf_dispersion){flagChanged=1;}

		pImagePool1->m_leaf_dispersion_option=dlg.m_leaf_dispersion_option;
		if(pImagePool1->m_leaf_dispersion_option!=Om_leaf_dispersion_option){flagChanged=1;}

		pImagePool1->m_mean_leaf_area=dlg.m_mean_leaf_area;
		if(pImagePool1->m_mean_leaf_area!=Om_mean_leaf_area){flagChanged=1;}

		pImagePool1->m_inversion_method=dlg.m_inversion_method;
		if(pImagePool1->m_inversion_method!=Om_inversion_method){flagChanged=1;}

		pImagePool1->ifCalculVprofile=dlg.m_ChkLAD;
		if(pImagePool1->ifCalculVprofile!=Om_ChkLAD){flagChanged=1;}

		pImagePool1->is_divide_pza=dlg.is_divide_pza;
		if(pImagePool1->is_divide_pza!=Ois_divide_pza){flagChanged=1;}

		pImagePool1->division_horizontal=dlg.division_horizontal;
		if(pImagePool1->division_horizontal!=Odivision_horizontal){flagChanged=1;}

		pImagePool1->division_vertical=dlg.division_vertical;
		if(pImagePool1->division_vertical!=Odivision_vertical){flagChanged=1;}

		this->default_pza=dlg.default_pza;
		if(this->default_pza!=Odefault_pza){this->IniWrite();}

		//ms.Format("inversion=%i",dlg.m_inversion_method);
		//MessageBox(ms);
		DisplaySettingProperties();
	}	
}

void CTreeAnalyserDlg::OnProjectVolume() 
{
	CTreeAnalyserDlg * pNewObject = this; //create pointer to this
	DWORD dwExitCode;
	if (pWorkThread == NULL) //if thread have not been created
	{
		WorkIndex=2;
		pWorkThread=AfxBeginThread(ThreadWork, pNewObject);
	}
	else
	{
		::GetExitCodeThread(pWorkThread->m_hThread,&dwExitCode);
		if(dwExitCode!=STILL_ACTIVE) //if thread is finish
		{
			WorkIndex=2;
			pWorkThread=AfxBeginThread(ThreadWork, pNewObject);
		}
		else //if thread is working
		{
			pWorkThread->SuspendThread();
			if(this->AskForStopThread()==IDNO){pWorkThread->ResumeThread();}
			else
			{
				::WaitForSingleObject(pWorkThread->m_hThread,0);
				delete pWorkThread;
				pWorkThread=NULL;
				this->DisplayTerminateMessage();
				WorkIndex=0;
				vp1->Reset();
				this->SetWorkingMenu();
				DisplayStatus();
			}
		}
	}

} //void CTreeAnalyserDlg::OnProjectVolume() 


bool CTreeAnalyserDlg::CreateVirtualPlant(int option)
{ //option 0=[0] option 1=]0[

	//initial variables
	//int i=0, j=0,k=0;
	double xmin=0,ymin=0,zmin=0;
	double dx=pImagePool1->cellSize_dx;
	double dy=pImagePool1->cellSize_dy;
	double dz=pImagePool1->cellSize_dz;
	unsigned int total_nx=0,total_ny=0,total_nz=0; //,total_cells=0;

	//display construction information
	ms.Format("Construction of virtual plant was build from ground level.");
	AddDisplayText(ms);
	ms.Format("	Voxel size = %6.1fx%6.1fx%6.1f cm",
		pImagePool1->cellSize_dx*100,pImagePool1->cellSize_dy*100,pImagePool1->cellSize_dz*100);
	AddDisplayText(ms);

	pImagePool1->bounding_x=pImagePool1->maxDiameter*(1+pImagePool1->m_enlarge);
	pImagePool1->bounding_y=pImagePool1->maxDiameter*(1+pImagePool1->m_enlarge);
	pImagePool1->bounding_z=pImagePool1->maxTreeHeight*(1+pImagePool1->m_enlarge);

	ms.Format("	Bounding box size (from ground, enlarge=%5.2f%%) = %6.2fx%6.2fx%6.2f m.",
		pImagePool1->m_enlarge*100,pImagePool1->bounding_x,pImagePool1->bounding_y,pImagePool1->bounding_z);
	AddDisplayText(ms);



	//calculate box variables
	switch (option) //type of division cell option 0=[0] option 1=]0[
	{
	case 0:
		//find nx
		if(mod(((pImagePool1->bounding_x/2)-(pImagePool1->cellSize_dx/2)),pImagePool1->cellSize_dx)==0)
		{
			total_nx=int(((pImagePool1->bounding_x/2)-(pImagePool1->cellSize_dx/2))/pImagePool1->cellSize_dx)*2+1;
		}
		else{total_nx=int((((pImagePool1->bounding_x/2)-(pImagePool1->cellSize_dx/2))/pImagePool1->cellSize_dx)+1)*2+1;}
		xmin=total_nx*dx/-2;

		//find ny
		if(mod(((pImagePool1->bounding_y/2)-(pImagePool1->cellSize_dy/2)),pImagePool1->cellSize_dy)==0)
		{
			total_ny=int(((pImagePool1->bounding_y/2)-(pImagePool1->cellSize_dy/2))/pImagePool1->cellSize_dy)*2+1;
		}
		else{total_ny=int((((pImagePool1->bounding_y/2)-(pImagePool1->cellSize_dy/2))/pImagePool1->cellSize_dy)+1)*2+1;}
		ymin=total_ny*dy/-2;

		//find nz
		if(mod(pImagePool1->bounding_z,pImagePool1->cellSize_dz)==0)
		{
			total_nz=int(pImagePool1->bounding_z/pImagePool1->cellSize_dz);
		}
		else {total_nz=int(pImagePool1->bounding_z/pImagePool1->cellSize_dz)+1;}

		break; //case 0

	case 1:
		//find nx
		if(mod(pImagePool1->bounding_x/2,pImagePool1->cellSize_dx)==0)
		{
			total_nx=int(pImagePool1->bounding_x/2/pImagePool1->cellSize_dx)*2;
		}
		else{total_nx=(int(pImagePool1->bounding_x/2/pImagePool1->cellSize_dx)+1)*2;}
		xmin=double(total_nx/2)*dx*-1;

		//find ny
		if(mod(pImagePool1->bounding_y/2,pImagePool1->cellSize_dy)==0)
		{
			total_ny=int(pImagePool1->bounding_y/2/pImagePool1->cellSize_dy)*2;
		}
		else{total_ny=(int(pImagePool1->bounding_y/2/pImagePool1->cellSize_dy)+1)*2;}
		ymin=double(total_ny/2)*dy*-1;

		//find nz
		if(mod(pImagePool1->bounding_z,pImagePool1->cellSize_dz)==0)
		{
			total_nz=int(pImagePool1->bounding_z/pImagePool1->cellSize_dz);
		}
		else {total_nz=int(pImagePool1->bounding_z/pImagePool1->cellSize_dz)+1;}
		break; //case 1
	} //end switch (option) //type of division cell option 0=[0] option 1=]0[


	cPoint vpoint(xmin,ymin,zmin);
	if(total_nx*total_ny*total_nz>15000000)
	{
		ms.Format("\r\nError reconstruction of volume!!\r\nThe Number of voxel = %i is over the limit of 15000000.",total_nx*total_ny*total_nz);
		AddDisplayText(ms);
		return 0;
	}
	else
	{
		ms.Format("Reconstructing tree volume with %i voxels.....",total_nx*total_ny*total_nz);
		DisplayStatus(ms);
		AddDisplayText(" ");
		int ans=vp1->Reconstruct(total_nx,total_ny,total_nz,dx,dy,dz,vpoint);

		ms.Format("	Construction succeeded with canopy size %6.2fx%6.2fx%6.2f m.",
			total_nx*dx,total_ny*dy,total_nz*dz);
		AddDisplayText(ms);
		ms.Format("	Total voxels=%i nx=%i ny=%i nz=%i",ans,total_nx,total_ny,total_nz);
		AddDisplayText(ms);
		return 1;
	}
}//void CTreeAnalyserDlg::CreateVirtualPlant()




void CTreeAnalyserDlg::ExportVirtualPlant(int option, CString filename,int celltype)
{//option 1=skeleton 2=box
	vp1->ExportCellVGS(filename,option,celltype); 
}



void CTreeAnalyserDlg::EstimateTreeDimension()
{
//-------- initializing time used ---------------
   time_t   start, finish;
   double   elapsed_time;   
   time( &start );
//-------- end initializing time used ---------------

	UpdateCheckItem();
	DisplayStatus("Estimate Tree Dimension");
	AddDisplayText("\r\n******************** Estimate tree dimension (height and diameter) **********************");

	int workedPic=0;
	double sumHeight=0,sumDiameter=0,sumVHeight=0; //,sumVolume=0;
	double sumHeight2=0,sumDiameter2=0,sumVHeight2=0;
	double maxTreeHeight=0, minTreeHeight=999999;
	double maxVHeight=0, minVHeight=999999;
	double maxDiameter=0, minDiameter=999999;
	double pii=acos(double(-1));
	CImage ctrlImage;

	//write header of log file
	bool error_open_log=0;
	if(pImagePool1->m_iflogvolume==TRUE)
	{
		FILE * LogFileStream;
		if((LogFileStream=fopen(this->GetLogFilename(),"at"))==NULL)
		{
			ms.Format("Error open log file '%s'. \nThis file maybe already opened.",this->GetLogFilename());
			MessageBox(ms);
			AddDisplayText("Error! Can not write to log file");
			error_open_log=1;
		}
		else
		{
			fprintf(LogFileStream,"ImageNo.	ImageName	Az	Ele	View_angle	Tree_height	Vegetation_height	Diameter\n");
			fclose(LogFileStream);
		}
	}

	for(int pic=0;pic<pImagePool1->GetImageCount();pic++)
	{

		ms.Format("Estimate tree dimension image %i of %i",pic+1,pImagePool1->GetImageCount());
		DisplayStatus(ms);
		CEachImage *tempImage=pImagePool1->GetImage(pic);


		AddDisplayText("");
		ms.Format("Image name = %s",tempImage->GetPictureName());
		AddDisplayText(ms);


		if (tempImage->GetFileName()!="empty" && tempImage->GetCheck()==1)
		{

			m_Tree1.SelectItem(vctTreeItem[pic]);


			if(fabs(tempImage->GetCameraInc())>pImagePool1->m_LimitEle && tempImage->GetFileName()!="empty" && tempImage->GetCheck()==1)
			{

				AddDisplayText("	Estimation was skipped due to limit of elevation.");
				goto skipimage;
			}


			 // Load the selected bitmap file
			HRESULT loadresule= ctrlImage.Load(tempImage->GetFileName());

			if (FAILED(loadresule))
			{
				ms.Format("Error reading file '%s'\r\nSelected file may not existed or not be the bitmap file.",tempImage->GetFileName());
				AddDisplayText(ms);
				goto skipimage;
			}
			else //if load image OK
			{
				ms.Format("	view angle=%6.2f degs",tempImage->GetViewAngle()*180/pii);
				AddDisplayText(ms);
				int i=0,j=1,k=1,m_pixel_value;
				//CString ms;
				ms.Format("Estimate tree dimension image %i of %i.  Searching left pixel.",pic+1,pImagePool1->GetImageCount());
				DisplayStatus(ms);

				//search for left pixel
				int progress_int=0;
				double unit=ctrlImage.GetWidth()/50;
				while(i<ctrlImage.GetWidth())//bm.bmWidth)
				{
					if((i/unit)>progress_int)
					{
						progress_int++;
						ms+=".";
						DisplayStatus(ms);
					}
					

					j=0;
					while(j<ctrlImage.GetHeight()) //bm.bmHeight)
					{
						m_pixel_value = ctrlImage.GetPixel(i,j); //m_bmpCDC.GetPixel(i,j );
						if(m_pixel_value==CLR_INVALID){ms.Format("Error reading left pixel i=%i j=%i",i,j);MessageBox(ms);}
						if (m_pixel_value==0)
						{
							//ms.Format("	Left pixel x=%i y=%i",i,j);
							tempImage->pixLeft.x=i;tempImage->pixLeft.y=j;
							//AddDisplayText(ms);
							k=0;
							break;
						}
						if(k==0){break;}
						j++;
					}
					if(k==0){break;}
					i++;
				}
				cPoint LeftPoint;
				tempImage->GetIntersectRayPlane(tempImage->pixLeft.x,tempImage->pixLeft.y,&LeftPoint,1);
				//ms.Format("Left point =(%6.4f,%6.4f,%6.4f",LeftPoint.GetX(),LeftPoint.GetY(),LeftPoint.GetZ());
				//AddDisplayText(ms);

				//search for Right pixel
				ms.Format("Estimate tree dimension image %i of %i.  Searching right pixel.",pic+1,pImagePool1->GetImageCount());
				DisplayStatus(ms);
				i=ctrlImage.GetWidth()-1; k=1;
				progress_int=0;
				while(i>=0)
				{
					if(((ctrlImage.GetWidth()-i)/unit)>progress_int)
					{
						progress_int++;
						ms+=".";
						DisplayStatus(ms);
					}
					
					j=0;
					while(j<ctrlImage.GetHeight()) //bm.bmHeight)
					{
						m_pixel_value = ctrlImage.GetPixel(i,j); //m_bmpCDC.GetPixel(i, j);
						if(m_pixel_value==CLR_INVALID){ms.Format("Error reading right pixel i=%i j=%i",i,j);MessageBox(ms);}
						if (m_pixel_value==0)
						{
							//ms.Format("	Right pixel x=%i y=%i",i,j);
							tempImage->pixRight.x=i;tempImage->pixRight.y=j; 
							//AddDisplayText(ms);
							k=0;
							break;
						}
						if(k==0){break;}
						j++;
					}
					if(k==0){break;}
					i--;
				}
				ms.Format("	Vegetation Diameter=%i pixels	Left=(%i,%i)  Right=(%i,%i)",
					tempImage->pixRight.x-tempImage->pixLeft.x,
					tempImage->pixLeft.x,tempImage->pixLeft.y,
					tempImage->pixRight.x,tempImage->pixRight.y);
				AddDisplayText(ms);

				cPoint RightPoint;
				tempImage->GetIntersectRayPlane(tempImage->pixRight.x,tempImage->pixRight.y,&RightPoint,1);
				//ms.Format("Right point =(%6.4f,%6.4f,%6.4f",RightPoint.GetX(),RightPoint.GetY(),RightPoint.GetZ());
				//AddDisplayText(ms);



				//search for top pixel
				ms.Format("Estimate tree dimension image %i of %i.  Searching top pixel.",pic+1,pImagePool1->GetImageCount());
				DisplayStatus(ms);
				i=0;k=1;progress_int=0;
				while(i<ctrlImage.GetHeight()) //bm.bmHeight)
				{
					if((i/unit)>progress_int)
					{
						progress_int++;
						ms+=".";
						DisplayStatus(ms);
					}
					
					j=0;
					while(j<ctrlImage.GetWidth()) //bm.bmWidth)
					{
						m_pixel_value = ctrlImage.GetPixel(j,i); //m_bmpCDC.GetPixel(j, i);
						if(m_pixel_value==CLR_INVALID){ms.Format("Error reading top pixel i=%i j=%i",i,j);MessageBox(ms);}
						if (m_pixel_value==0)
						{
							//ms.Format("	Top pixel x=%i y=%i",j,i);
							tempImage->pixTop.x=j;tempImage->pixTop.y=i;
							//AddDisplayText(ms);
							k=0;
							break;
						}
						if(k==0){break;}
						j++;
					}
					if(k==0){break;}
					i++;
				}
				cPoint TopPoint;
				tempImage->GetIntersectRayPlane(tempImage->pixTop.x,tempImage->pixTop.y,&TopPoint,0);
				//ms.Format("Right point =(%6.4f,%6.4f,%6.4f",TopPoint.GetX(),TopPoint.GetY(),TopPoint.GetZ());
				//AddDisplayText(ms);


				//search for bottom pixel
				ms.Format("Estimate tree dimension image %i of %i.  Searching bottom pixel.",pic+1,pImagePool1->GetImageCount());
				DisplayStatus(ms);
				i=ctrlImage.GetHeight()-1;k=1;
				progress_int=0;
				while(i>=0)
				{
					if(((ctrlImage.GetHeight()-i)/unit)>progress_int)
					{
						progress_int++;
						ms+=".";
						DisplayStatus(ms);
					}
					
					j=0;
					while(j<ctrlImage.GetWidth()) //bm.bmWidth)
					{
						m_pixel_value = ctrlImage.GetPixel(j,i); //m_bmpCDC.GetPixel(j, i);
						if(m_pixel_value==CLR_INVALID){ms.Format("Error reading bottom pixel i=%i j=%i",i,j);MessageBox(ms);}
						if (m_pixel_value==0)
						{
							//ms.Format("	Bottom pixel x=%i y=%i",j,i);
							tempImage->pixBottom.x=j;tempImage->pixBottom.y=i;
							//AddDisplayText(ms);
							k=0;
							break;
						}
						if(k==0){break;}
						j++;
					}
					if(k==0){break;}
					i--;
				}
				cPoint BottomPoint;
				tempImage->GetIntersectRayPlane(tempImage->pixBottom.x,tempImage->pixBottom.y,&BottomPoint,0);
				tempImage->ifMarkedPoint=1; //tell the program that this image already computed dimension
				ctrlImage.Destroy(); //detach and destroy bitmap
				
				double TreeHeight=TopPoint.GetZ();
				double VHeight=TopPoint.GetZ()-BottomPoint.GetZ();

				//first calculate distance between left and right pixel
				double Diameter=sqrt(pow((LeftPoint.GetX()-RightPoint.GetX()),2)+pow((LeftPoint.GetY()-RightPoint.GetY()),2));
				//double DistanceLetRight=sqrt(pow((LeftPoint.GetX()-RightPoint.GetX()),2)+pow((LeftPoint.GetY()-RightPoint.GetY()),2));
				//double DifferHeight=abs(LeftPoint.GetZ()-RightPoint.GetZ());
				//double Diameter=sqrt(pow(DistanceLetRight,2)-pow(DifferHeight,2)) ;

				ms.Format("Estimate tree dimension image %i of %i.  Calculating tree dimension.",pic+1,pImagePool1->GetImageCount());
				DisplayStatus(ms);

				ms.Format("	Vegetation Height=%i pixels		Top=(%i,%i)  Bottom=(%i,%i)",
					tempImage->pixBottom.y-tempImage->pixTop.y,
					tempImage->pixTop.x,tempImage->pixTop.y,
					tempImage->pixBottom.x,tempImage->pixBottom.y);
				AddDisplayText(ms);

				ms.Format("	Estimated	  Tree Height=%6.2f m. Diameter=%6.2f m.",
					TreeHeight,Diameter);
				AddDisplayText(ms);

				ms.Format("	Vegetation Height=%6.2f m.  Volume(cylinder)=%6.2f m3  Volume(box)=%6.2f m3.",
					VHeight,pii*pow((Diameter/2),2)*VHeight,VHeight*Diameter*Diameter); //pii*r2*h
				AddDisplayText(ms);

				//writing output
				if(pImagePool1->m_iflogvolume==TRUE && !error_open_log)
				{
					FILE * LogFileStream;
					LogFileStream=fopen(this->GetLogFilename(),"at");
					fprintf(LogFileStream,"%i\t%s\t",pic+1,tempImage->GetPictureName());
					fprintf(LogFileStream,"%6.2f\t",tempImage->GetCameraDirection());
					fprintf(LogFileStream,"%6.2f\t",tempImage->GetCameraInc());
					fprintf(LogFileStream,"%6.2f\t",tempImage->GetViewAngle()*180/pii);
					fprintf(LogFileStream,"%6.4f\t%6.4f\t%6.4f\n",TreeHeight,VHeight,Diameter);
					fclose(LogFileStream);
				}

				if (maxTreeHeight<TreeHeight){maxTreeHeight=TreeHeight;}
				if (minTreeHeight>TreeHeight){minTreeHeight=TreeHeight;}
				if (maxVHeight<VHeight){maxVHeight=VHeight;}
				if (minVHeight>VHeight){minVHeight=VHeight;}
				if (maxDiameter<Diameter){maxDiameter=Diameter;}
				if (minDiameter>Diameter){minDiameter=Diameter;}

				sumHeight+=TreeHeight;
				sumDiameter+=Diameter;
				sumVHeight+=VHeight;

				sumHeight2+=pow(TreeHeight,2);
				sumDiameter2+=pow(Diameter,2);
				sumVHeight2+=pow(VHeight,2);
				workedPic++;
			} //else //if load image OK
		}//if (tempImage->GetFileName()!="empty" && tempImage->GetCheck()==1)
		else {AddDisplayText("	Image skipped.");}
skipimage:;
	} //for(int pic=0;pic<pImagePool1->GetImageCount();pic++)

	m_Tree1.SelectItem(vctTreeItem[0]); //select the first picture

	//put information to image pool
	pImagePool1->maxDiameter=maxDiameter;
	pImagePool1->maxTreeHeight=maxTreeHeight;
	pImagePool1->maxVHeight=maxVHeight;
	pImagePool1->minDiameter=minDiameter;
	pImagePool1->minTreeHeight=minTreeHeight;
	pImagePool1->minVHeight=minVHeight;
	pImagePool1->averageDiameter=sumDiameter/workedPic;
	pImagePool1->averageTreeHeight=sumHeight/workedPic;
	pImagePool1->averageVHeight=sumVHeight/workedPic;


	double sdTreeHeight=sqrt((sumHeight2-pow(sumHeight,2)/workedPic)/(workedPic-1));
	double sdDiameter=sqrt((sumDiameter2-pow(sumDiameter,2)/workedPic)/(workedPic-1));
	double sdsumVHeight=sqrt((sumVHeight2-pow(sumVHeight,2)/workedPic)/(workedPic-1));


	//Display Output
	AddDisplayText(" ");
	ms.Format("Average	Tree Height=%6.2f m.	SD=%6.4f\tMax=%6.2f  Min=%6.2f",pImagePool1->averageTreeHeight,sdTreeHeight,pImagePool1->maxTreeHeight,pImagePool1->minTreeHeight );
	AddDisplayText(ms);
	ms.Format("	Tree Diameter=%6.2f m.	SD=%6.4f\tMax=%6.2f  Min=%6.2f",pImagePool1->averageDiameter,sdDiameter,pImagePool1->maxDiameter,pImagePool1->minDiameter);
	AddDisplayText(ms);
	ms.Format("	Vegetation Height=%6.2f m.	SD=%6.4f\tMax=%6.2f  Min=%6.2f",pImagePool1->averageVHeight,sdsumVHeight,pImagePool1->maxVHeight,pImagePool1->minVHeight);
	AddDisplayText(ms);
	ms.Format("	Vegetation Volume(cylinder)=%6.2f m3  Volume(box)=%6.2f m3",
		pii*(pImagePool1->averageVHeight)*pow((pImagePool1->averageDiameter/2),2),
		pImagePool1->averageVHeight*pow(pImagePool1->averageDiameter,2));
	AddDisplayText(ms);

	DisplayStatus();
	DisplaySettingProperties();

//----------- calculate time used ---------------------------
   time( &finish );
   elapsed_time = difftime( finish, start );
   ms.Format( "Estimate tree dimension using = %6.0f seconds.", elapsed_time );
	AddDisplayText(ms);

	char datebuf[128];
	struct tm *today;__time64_t ltime;
	_time64( &ltime );
	today = _localtime64( &ltime );
	strftime( datebuf, 128,"%d-%b-%y %H:%M:%S", today );

	//writing output
	if(pImagePool1->m_iflogvolume==TRUE && !error_open_log)
	{
		CString ms2;
		ms2.LoadString(IDS_VERSION);
		FILE * LogFileStream;
		LogFileStream=fopen(this->GetLogFilename(),"at");
		fprintf(LogFileStream,"Estimation of tree dimension was done by %s on %s, using time = %i seconds.\n\n",ms2,datebuf, int(elapsed_time));
		fclose(LogFileStream);
		ms.Format("Log output to => %s",this->GetLogFilename());
		AddDisplayText(ms);
	}
	if(error_open_log){AddDisplayText("!Unable to write the log file!");}

//----------- calculate time used ---------------------------

}

//call by volume reconstruction
void CTreeAnalyserDlg::CalculDirections(CEachImage *image)
{///Calculate the directions of the beams pass through the canopy using given image.
	
	vctDirection.resize(0);
	ms.Format("Calcualting intercepted directions of '%s'",image->GetPictureName());
	DisplayStatus(ms);
	ms.Format("Reading image '%s'",image->GetPictureName());
	DisplayStatus(ms);

	CImage ctrlImage;
	// Load the selected bitmap file
	DisplayStatus("Loading Image....");
	HRESULT loadresule= ctrlImage.Load(image->GetFileName());

	if (FAILED(loadresule))
	{
		ms.Format("Error reading file '%s'\r\nSelected file may not existed or not be the bitmap file.",image->GetFileName());
		AddDisplayText(ms);
	}
    // Do we have a valid handle for the loaded image?
	else //if load image OK
    {
	

		//calculate number of direction
		int nx=0,ny=0;
		if (mod(ctrlImage.GetWidth(),pImagePool1->raypx)==0)
		{
			nx=ctrlImage.GetWidth()/pImagePool1->raypx;
		}
		else {nx=(ctrlImage.GetWidth()/pImagePool1->raypx)+1;}

		if (mod(ctrlImage.GetHeight(),pImagePool1->raypy)==0)
		{
			ny=ctrlImage.GetHeight()/pImagePool1->raypy;
		}
		else {ny=(ctrlImage.GetHeight()/pImagePool1->raypy)+1;}



	// ------------ Start read pixel and calculate each direction-----------------------------
		vctDirection.resize(0); //delete all previous directions
		CDirection tempdir;
//		double maxpx=0,maxpy=0;
		int totalfound=0;
		int i,j,k,l;
		int opercent=-1;
		double unitpercent=double(nx)/100;
		for (i=1;i<nx;i++)
		{
			int percent=int(i/unitpercent);
			if(percent>opercent)
			{
				ms.Format("Analysing intercepted directions on image '%s'  %i%%",image->GetPictureName(),percent);
				DisplayStatus(ms);
				opercent=percent;
			}

			for (j=1;j<=ny;j++)
			{
				int blackfound=0;
				k=0;
				while (k<pImagePool1->raypx && blackfound==0)
				{	
					l=0;
					while(l<pImagePool1->raypy && blackfound==0)
					{
						int px=((i-1)*pImagePool1->raypx)+k;
						if(px>=ctrlImage.GetWidth()){px=ctrlImage.GetWidth()-1;}
						int py=(j-1)*pImagePool1->raypy+l;
						if(py>=ctrlImage.GetHeight()){py=ctrlImage.GetHeight()-1;}
					
						if(ctrlImage.GetPixel(px,py)==0)
						{
							blackfound=1;

							//find the middle pixel
							double px=(i-1)*pImagePool1->raypx+(double(pImagePool1->raypx)/2.0)+0.5;
							double py=(j-1)*pImagePool1->raypy+(double(pImagePool1->raypy)/2.0)+0.5;
							//calculate point of current direction
							tempdir.unitDirection=image->CalculUnitDirection(px,py);
							//tempdir.unitDirection=image->CalculUnitDirection(320,240);

							//calculate current direction
							vctDirection.push_back(tempdir);

							//cPoint testpoint=vctDirection[0].unitDirection;
							//double test=testpoint.GetX();

						}
						l++;

					} //while(l<=pImagePool1->raypy && blackfound=0)
					k++;
				} //while (k<=pImagePool1->raypx && blackfound=0)
				if (blackfound==1) //add direction to vctDirection
				{
					totalfound++;
				}
			} //for (j=1;j<=ny;j++)
		} //for (i=1;i<=nx;i++)
		ms.Format("		Total direction=%i (x=%i y=%i)  Intercepted found=%i",nx*ny,nx,ny,totalfound);
		AddDisplayText(ms);	
		ctrlImage.Destroy();
     } //else //if load image OK
}



void CTreeAnalyserDlg::OnProjectAnalyseall() 
{
	CTreeAnalyserDlg * pNewObject = this; //create pointer to this
	DWORD dwExitCode;
	if (pWorkThread == NULL) //if thread have not been created
	{
		WorkIndex=5;
		pWorkThread=AfxBeginThread(ThreadWork, pNewObject);
	}
	else
	{
		::GetExitCodeThread(pWorkThread->m_hThread,&dwExitCode);
		if(dwExitCode!=STILL_ACTIVE) //if working
		{
			WorkIndex=5;
			pWorkThread=AfxBeginThread(ThreadWork, pNewObject);
		}
		else //if thread is working
		{
			pWorkThread->SuspendThread();
			if(this->AskForStopThread()==IDNO){pWorkThread->ResumeThread();}
			else
			{
				::WaitForSingleObject(pWorkThread->m_hThread,0);
				delete pWorkThread;
				pWorkThread=NULL;
				this->DisplayTerminateMessage();
				WorkIndex=0;
				DisplayStatus();
				this->SetWorkingMenu();
			}
		}
	}
}//void CTreeAnalyserDlg::OnProjectAnalyseall() 

void CTreeAnalyserDlg::AnalyseAll()
{
//-------- initializing time used ---------------
   time_t   start, finish;
   double   elapsed_time;   
   time( &start );
//-------- end initializing time used ---------------
	this->EstimateTreeDimension();
	this->VolumeReconstruction();
	this->EstimateLeafArea();
//----------- calculate time used ---------------------------
   time( &finish );
   elapsed_time = difftime( finish, start );
   ms.Format( "Do All Calculations using = %6.0f seconds.", elapsed_time );
	AddDisplayText(ms);

	char datebuf[128];
	struct tm *today;__time64_t ltime;
	_time64( &ltime );
	today = _localtime64( &ltime );
	strftime( datebuf, 128,"%d-%b-%y %H:%M:%S", today );

	if(pImagePool1->m_iflogvolume==TRUE)
	{
		FILE * pFile;
		pFile=fopen(this->GetLogFilename(),"at");
		fprintf(pFile,"Do All Calculations was done on %s, using time = %i seconds.\n\n",datebuf,int(elapsed_time));
		fclose(pFile);
	}
	//MessageBox("Work Complete.");
//----------- calculate time used ---------------------------

}

void CTreeAnalyserDlg::TestPictureCombination(int picnum)
{
	if (pImagePool1->GetImageCount()!=0)
	{
		ms.Format("Start testing %i combination",picnum);
		AddDisplayText(ms);

		EstimateTreeDimension();
		CreateVirtualPlant(0); //use option=[0]
		
		//Write header of log file
		CString logfile=OutputLocation + "TestCombination.log";
		FILE * pFile;
		pFile=fopen(logfile,"wt");
		fprintf(pFile,"ImageNo.\tImageName\tEle\tAz\tIn_cell\tOut_cell\tVolume\n");
		fclose(pFile);


		int workcount=0;
		for(int i=0;i<pImagePool1->GetImageCount()-picnum;i++)
		{
			ms.Format("picture %i",i);
			AddDisplayText(ms);
			for(int j=i;j<i+picnum;j++)
			{



		CEachImage *tempImage=pImagePool1->GetImage(j);

		m_Tree1.SelectItem(vctTreeItem[j]);

		int in_cell=vp1->lifecell;
		ms.Format("	Start ray-cell intersection algorithm picture='%s'\r\n		Enter with %i voxels  volume=%6.3fm3",
			tempImage->GetPictureName(),vp1->lifecell,vp1->lifecell*vp1->GetCellVolume());
		AddDisplayText(ms);

		CalculDirections(tempImage);
		vp1->ClearHaveLeaf(); //set haveleaf of every cells to 0;
		int dirsize=vctDirection.size();
		double unitpercent=double(dirsize)/100;
		int opercent=-1;			
		for (int l=0;l<dirsize;l++) //do every intercepted directions
		{
			int percent=int(l/unitpercent);
			if(percent>opercent)
			{
				ms.Format("Running ray-cell intersection algorithm on image '%s'  %i%%",tempImage->GetPictureName(),percent);
				DisplayStatus(ms);
				opercent=percent;
			}
	
			//do every cell in virtual plant
			for (int i=1;i<=vp1->GetNx();i++)
			{
				for (int j=1;j<=vp1->GetNy();j++)
				{
					for (int k=1;k<=vp1->GetNz();k++)
					{
						cell *tempcell=vp1->GetCellPointer(i,j,k);

						if (tempcell->keepit==1 && tempcell->haveleaf==0) //do only cell that keepit=1
						{
							short int ans=vp1->RayCellIntersect(&tempImage->CameraCoordinate,&vctDirection[l].unitDirection,tempcell);
							if(ans==1)
							{
								tempcell->haveleaf=1;
							}
						}//if (tempcell->keepit)

					} //for (int k=1;k<=vp1->GetNz();k++)

				}//for (int j=1;j<=vp1->GetNy();j++);

			}//for (i=1;i<=vp1->GetNx();i++)
			
		}//for (int l=0;l<dirsize;l++)

		vp1->ResetKeepIt(); //reset keepit if no leaf set to 0


		//-------------- Start writing log file ------------------------
		if(pImagePool1->m_iflogvolume==TRUE)
		{
			FILE * pFile;
			pFile=fopen(logfile,"at");
			fprintf(pFile,"%i\t%s\t%6.2f\t%6.2f\t%i\t%i\t%6.2f\n",
				workcount,tempImage->GetPictureName(),tempImage->GetCameraInc(),tempImage->GetCameraDirection(),
				in_cell,vp1->lifecell,vp1->lifecell*vp1->GetCellVolume());
			fclose(pFile);

		}
		//-------------- End writing log file ------------------------


		//export cells after ray-box intersection
		if(pImagePool1->m_ifoutputbox==TRUE)
		{
			ms.Format("Volume_after%i.vgx",workcount);
			ms=OutputLocation+ms;
			AddDisplayText("		Export box volume to file "+ms);
			ExportVirtualPlant(2,ms,7); //export tree volume
		}
		
		






			}
			
		} //for(int i=0;i<pImagePool1->GetImageCount();i++)
	} //if (pImagePool1->GetImageCount()!=0)
	else {MessageBox("No picture to test");}

}


void CTreeAnalyserDlg::OnFileSaveAs()
{
	UpdateCheckItem();
	CString filename;
     static char BASED_CODE szFilter[] = "Tree Analyser Project (*.tre)|*.tre||";
     CFileDialog m_ldFile(FALSE, ".tre", CurrentPath+"*.tre",
          OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	if (m_ldFile.DoModal() == IDOK)
	{
		filename = m_ldFile.GetPathName();
		CurrentPath=filename.Left(filename.GetLength()-strlen(m_ldFile.GetFileName()));

		UpdateMenu();
		UpdateMRL(&filename); //update MRL list

		if(pImagePool1->SaveNewFormat(&filename))
		{
			ms="Tree Analyser: " + filename;
			ActiveProjectFilename=m_ldFile.GetPathName();
			this->SetWindowText(ms);
			UpdatePicture();
			flagChanged=0;
			this->SetWorkingMenu();
		}
		else {MessageBox("Unable to save file!");}
	} //if (m_ldFile.DoModal() == IDOK)
}


void CTreeAnalyserDlg::OnImageColorcounting()
{
	CTreeAnalyserDlg * pNewObject = this; //create pointer to this
	DWORD dwExitCode;
	if (pWorkThread == NULL) //if thread have not been created
	{
		WorkIndex=8;
		pWorkThread=AfxBeginThread(ThreadWork, pNewObject);
	}
	else
	{
		::GetExitCodeThread(pWorkThread->m_hThread,&dwExitCode);
		if(dwExitCode!=STILL_ACTIVE) //if thread is finish
		{
			WorkIndex=8;
			pWorkThread=AfxBeginThread(ThreadWork, pNewObject);
		}
		else //if thread is working
		{
			pWorkThread->SuspendThread();
			if(this->AskForStopThread()==IDNO){pWorkThread->ResumeThread();}
			else
			{
				::WaitForSingleObject(pWorkThread->m_hThread,0); //stop the thread suddenly
				delete pWorkThread;
				pWorkThread=NULL;
				this->DisplayTerminateMessage();
				WorkIndex=0;
				this->SetWorkingMenu();
				DisplayStatus();
			}
		}
	}
}

void CTreeAnalyserDlg::Colorcounting()
{
	CString output_filename=OutputLocation + ActivePictureName + "_color";

     static char BASED_CODE szFilter[] = "Tree Project (*.txt)|*.txt||";
     CFileDialog m_ldFile(FALSE, ".txt", output_filename,
          OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	int ans=m_ldFile.DoModal();
	if ( ans== IDOK)
	{
		output_filename = m_ldFile.GetPathName();

		// Get the filename selected
		CString bmp_filename;
        bmp_filename = pImagePool1->GetPictureFileName(ActivePictureName);
		CImage ctrlImage;
		// Load the selected bitmap file
		HRESULT loadresule= ctrlImage.Load(bmp_filename);
		if (FAILED(loadresule))
		{
			ms.Format("Error reading file '%s'\r\nSelected file may not existed or not be the bitmap file.",bmp_filename);
			AddDisplayText(ms);
			return;
		}
		else //if load image OK
		{
		/*
         HBITMAP hBitmap = (HBITMAP) ::LoadImage(AfxGetInstanceHandle(),
             bmp_filename, IMAGE_BITMAP, 0, 0, 
             LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	
         // Do we have a valid handle for the loaded image?
         if (hBitmap)
         {
             // Delete the current bitmap
             if (m_bmpBitmap.DeleteObject())
                 // If there was a bitmap, detach it
                 m_bmpBitmap.Detach();
             // Attach the currently loaded bitmap to the bitmap object
             m_bmpBitmap.Attach(hBitmap);

				BITMAP bm; //create handle for bitmap
				// Get the loaded bitmap
				CPaintDC dc(this);
				CDC m_bmpCDC;
				m_bmpCDC.CreateCompatibleDC(&dc);
				m_bmpCDC.SelectObject(m_bmpBitmap);
				m_bmpBitmap.GetBitmap(&bm);
*/
		 
		// ------------ Start read pixel -----------------------------

				COLORREF m_pixel_value; // Current Pixel
				int m_nR; // Red
				int m_nG; // Green
				int m_nB; // Blue

		//------Start write value for each pixel -------
				int picture_width=ctrlImage.GetWidth();
				int picture_height=ctrlImage.GetHeight();

				int px=0,py=0; //,percent=0;
				unsigned int i=0,ColorFound=0;
				bool MatchFound=false;

				std::vector<ColorCount> vctColor;
				//::SetCursor(::LoadCursor(NULL, IDC_WAIT));
				while (px<picture_width)
				{
					ms.Format("Analysing %s %3.0f%%. Color found=%i",ActivePictureName,double(px)/picture_width*100,ColorFound);
					DisplayStatus(ms);
					py=0;
					while (py<picture_height)
					{
						m_pixel_value = ctrlImage.GetPixel(px, py);
						i=0; MatchFound=false;
						
						while(i<vctColor.size() && MatchFound==false)
						{
							if(unsigned int(vctColor[i].ColorIndex)==m_pixel_value)
							{
								vctColor[i].SumPixel++;
								MatchFound=true;
								i=vctColor.size();
							}
							i++;
						}
						if (MatchFound==false)
						{
							ColorCount TempColor(m_pixel_value,1);
							vctColor.push_back(TempColor);
							ColorFound++;
						} 
						py++;
					}
					px++;
				} //while (px<bm.bmWidth)

				ms.Format("Total color found for image '%s' = %i",ActivePictureName,ColorFound);
				AddDisplayText(ms);
				//Print Output
				FILE * pFile;
				pFile=fopen(output_filename,"wt");

				//print header
				fprintf(pFile,"Picture name=%s\n",ActivePictureName);
				fprintf(pFile,"Bitmap source=%s\n",bmp_filename);
				fprintf(pFile,"Image height=%i  width=%i  image type=%i bit\nbBits=%i\n",
					ctrlImage.GetHeight(),ctrlImage.GetWidth(),ctrlImage.GetBPP(),
					ctrlImage.GetBits());
				fprintf(pFile,"Color found = %i\n",vctColor.size());
					
				fprintf(pFile,"RGB\tSumPixel\tR\tG\tB\n");
				//end print header

				//print data
				for(i=0;i<vctColor.size();i++)
				{

						m_nR = GetRValue(vctColor[i].ColorIndex);
						m_nG = GetGValue(vctColor[i].ColorIndex);
						m_nB = GetBValue(vctColor[i].ColorIndex);
				fprintf(pFile,"%i	%i	%i	%i	%i\n",
					vctColor[i].ColorIndex,vctColor[i].SumPixel,m_nR,m_nG,m_nB);
				}
				fclose(pFile);

				//::SetCursor(::LoadCursor(NULL, IDC_ARROW));
				
		//------End write value for each pixel -------

         } 

		//UpdatePicture();
		DisplayStatus();
	}//if (m_ldFile.DoModal() == IDOK)
}


/*
void CTreeAnalyserDlg::ExportP0csv() 
{//export data for Python model beer
	vctDirection.clear();
	int in_cell=vp1->GetTotal_cells();
	if(in_cell==0){MessageBox("Virtual canopy is not reconstructed.\r\nPlease reconstruct canopy volume before or open saved canopy.");goto end_process;}
	if(pImagePool1->GetImageCount()==0){MessageBox("No image for working.");goto end_process;}
	else // if have cell to work
	{


		CString rootname;
		static char BASED_CODE szFilter[] = "Comma separated value (*.csv)|*.csv||";
		rootname="";
		CFileDialog m_ldFile(FALSE, ".cpn", rootname,
			OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
		if (m_ldFile.DoModal() == IDOK)
		{
			rootname = m_ldFile.GetPathName();
			rootname=rootname.Left(rootname.GetLength()-4);
			//AddDisplayText(filename);
			//goto end_process;
		}
		else{return;}

		//-------- initializing time used ---------------
		time_t   start, finish;
		double   elapsed_time;   
		time( &start );
		//-------- end initializing time used ---------------

		DisplayStatus("Exporting csv file of P0..........");
		AddDisplayText(" ");
		AddDisplayText("******************** Export beam equations ********************");
		AddDisplayText("Using Beer's model");		

		ms.Format("Leaf angle distribution = %s",pImagePool1->GetLeafDistributionName());
		AddDisplayText(ms);
		if(pImagePool1->m_leaf_distribution==0)
		{
			ms.Format("Mean Leaf angle=%6.2f",pImagePool1->m_mean_leaf_inc);
			AddDisplayText(ms);	
		}

		if(pImagePool1->m_leaf_distribution==7)
		{
			ms.Format("Leaf angle distribution file='%s'",pImagePool1->LeafDistributionFile);
			AddDisplayText(ms);	
			if(!pImagePool1->ReadLeafDis(pImagePool1->LeafDistributionFile,pImagePool1->ActivePath))
			{
				AddDisplayText("Unable to read leaf angle distirbution file!\r\nEstimation of leaf area stop by error.");
				goto end_process;
			}
		}
	

		UpdateCheckItem();
	

		//do ray-box intersection for each image
		int workcount=0;
		//double minLA=9e9,maxLA=-9e9,sumLA=0,sumLA2=0;
		if (pImagePool1->m_leaf_distribution==7){pImagePool1->ReadLeafDis(pImagePool1->LeafDistributionFile,this->CurrentPath);}
		for (int pic=0;pic<pImagePool1->GetImageCount();pic++)
		{
			CEachImage *tempImage=pImagePool1->GetImage(pic);

			if (tempImage->GetFileName()!="empty" && tempImage->GetCheck()==1) //do only selected image
			{//if image is selected then do it

				//declearation of variables
				//double angle_x=0,angle_y=0;
				//double dpx1=0,dpx2=0,dpy1=0,dpy2=0;
				//double sum_volume=0,sum_leaf_area=0,sum_beam_volume=0,sum_leaf_area_nogap=0;
				//double sumLAD=0,sumLAD2=0,VLAD=0,LAD_max=0;
				//double sumLADW=0,VLADW=0;
				//double zero_gap_vol=0,full_gap_vol=0;
				//int dirsize=0;
				int zone_size_x=0,zone_size_y=0;
				//variables for gap fraction
				//double meanGap=0,VGap=0;
				//int zero_gap=0,full_gap=0;

				//step 1 detect optimal zoning size
				if(pImagePool1->gap_option==0)
				{
					zone_size_x=pImagePool1->CalculOptimalZoneSize(tempImage);
					zone_size_y=zone_size_x;
				}
				else
				{
					zone_size_x=pImagePool1->gpx;
					zone_size_y=pImagePool1->gpy;
				}

				m_Tree1.SelectItem(vctTreeItem[pic]);

				workcount++;

				tempImage->CalculBasic();

//-------------- add for LAD computation -----------------------------------------------------
				CString filename;
				filename.Format("%s%i.csv",rootname,workcount);

				ms.Format("Picture='%s'",tempImage->GetPictureName());
				AddDisplayText(ms);
				ms.Format("Save output to %s",filename);
				AddDisplayText(ms);

				FILE * pFile;
				pFile=fopen(filename,"wt");
				fprintf(pFile,"P0,G,s");
			
				for(unsigned int i=0;i<vp1->GetTotal_cells();i++)
				{
					fprintf(pFile,",%i",vp1->acell[i].ID);
				}
				fprintf(pFile,"\n");
				fclose(pFile);
//-------------- add for LAD computation -----------------------------------------------------

				int exportedline=CalculDirections6(tempImage,zone_size_x,zone_size_y,&filename,0);
				ms.Format("Exported data %i lines",exportedline);
				AddDisplayText(ms);

				vctDirection.clear(); //clear memory of directions

			}//if (tempImage->GetFileName()!="empty" && tempImage->GetCheck()==1)
			
		} //for (int pic=0;pic<pImagePool1->GetImageCount();pic++)
		
		//----------- calculate time used ---------------------------
		time( &finish );
		elapsed_time = difftime( finish, start );
		ms.Format( "Exporting csv file of P0 = %6.0f seconds.\n", elapsed_time );
			AddDisplayText(ms);
		//----------- calculate time used ---------------------------
	}
end_process:;
	vctDirection.clear();
	DisplayStatus();

} //void CTreeAnalyserDlg::ExportP0csv() 
*/




void CTreeAnalyserDlg::OnProjectLaTotalDirection()
{
	CTreeAnalyserDlg * pNewObject = this; //create pointer to this
	DWORD dwExitCode;
	if (pWorkThread == NULL) //if thread have not been created
	{
		WorkIndex=3;
		pWorkThread=AfxBeginThread(ThreadWork, pNewObject);
	}
	else
	{
		::GetExitCodeThread(pWorkThread->m_hThread,&dwExitCode);
		if(dwExitCode!=STILL_ACTIVE) //if thread is finish
		{
			WorkIndex=3;
			pWorkThread=AfxBeginThread(ThreadWork, pNewObject);
		}
		else //if thread is working
		{
			pWorkThread->SuspendThread();
			if(this->AskForStopThread()==IDNO){pWorkThread->ResumeThread();}
			else
			{
				::WaitForSingleObject(pWorkThread->m_hThread,0);
				delete pWorkThread;
				pWorkThread=NULL;
				this->DisplayTerminateMessage();
				WorkIndex=0;
				this->SetWorkingMenu();
				DisplayStatus();
			}
		}
	}

}//void CTreeAnalyserDlg::OnProjectLaTotalDirection()


CString CTreeAnalyserDlg::GetRootName(CString fname)
{
	int total=fname.GetLength();
	return fname.Left(total-4);

}

CString CTreeAnalyserDlg::GetDirectoryName(CString fullPathName)
{
	CString result="error";
	int total=fullPathName.GetLength();
	char a='a';
	int i=total;
	while (i>0)
	{
		a=fullPathName.GetAt(i);
		if (a=='\\') {result=fullPathName.Mid(0,i+1);break;}
		i--;
	}
	return result;
}

CString CTreeAnalyserDlg::GetFileName(CString fullPathName,bool include_ext)
{
	CString result="error";
	if(!include_ext)
	{
		int total=fullPathName.GetLength();
		char a='a';
		int i=total;
		while (i>0)
		{
			a=fullPathName.GetAt(i);
			if (a=='\\') {result=fullPathName.Right(total-i-1);break;}
			i--;
		}
	}
	else //if not include extention
	{
		int total=fullPathName.GetLength();
		char a='a';
		int i=total;
		while (i>0)
		{
			a=fullPathName.GetAt(i);
			if (a=='\\') {result=fullPathName.Right(total-i-1);break;}
			i--;
		}

		total=result.GetLength();
		a='a';
		i=0;
		while (i<total)
		{
			a=result.GetAt(i);
			if (a=='.') {result=result.Left(i);break;}
			i++;
		}

	}
	return result;
}


void CTreeAnalyserDlg::OnImageMarkall()
{
	CEachImage *pImage;
	for(unsigned int i=0;i<vctTreeItem.size();i++)
	{
		pImage=pImagePool1->GetImage(i);
		m_Tree1.SetCheck(vctTreeItem[i]);
		pImage->PutCheck(1);
		this->flagChanged=1;
	}
}

void CTreeAnalyserDlg::OnImageUnmarkall()
{
	CEachImage *pImage;
	for(unsigned int i=0;i<vctTreeItem.size();i++)
	{
		pImage=pImagePool1->GetImage(i);
		m_Tree1.SetCheck(vctTreeItem[i],0);
		pImage->PutCheck(0);
		this->flagChanged=1;
	}
}

void CTreeAnalyserDlg::OnImageToglemark()
{
	CEachImage *pImage;

	for(unsigned int i=0;i<vctTreeItem.size();i++)
	{
		pImage=pImagePool1->GetImage(i);
		bool tempbool;
		if(m_Tree1.GetCheck(vctTreeItem[i])==0)
		{
			m_Tree1.SetCheck(vctTreeItem[i]);
			tempbool=1;
		} 
		else {m_Tree1.SetCheck(vctTreeItem[i],0);tempbool=0;}
		this->flagChanged=1;
		pImage->PutCheck(tempbool);
	}
 /*
	for (i=0;i < uCount;i++)
	{
	ASSERT(hItem != NULL);
	pmyTreeCtrl->SetCheck(hItem, !pmyTreeCtrl->GetCheck(hItem));
	hItem = pmyTreeCtrl->GetNextVisibleItem(hItem);
	}
*/
}

void CTreeAnalyserDlg::OnCanopyDisplayproperties()
{
	vp1->DisplayCanopyProperties();
	//DisplayCanopyProperties();
}

//! Save canopy voxels to file.
/*!
*	Created : 6-Nov-03\n
*	Last modification: 6-Oct-04.\n
*	Called by:\n
*	-Menu
*/
void CTreeAnalyserDlg::OnCanopySavecanopy()
{
	int ncell=vp1->GetTotal_cells();
	if (ncell==0){MessageBox("No data to save!");}
	else
	{

		CString filename;
		static char BASED_CODE szFilter[] = "Tree Analyser Project (*.cpn)|*.cpn||";
		filename.Format("%s_%icm.cpn",this->GetRootName(this->ActiveProjectFilename),int(vp1->Getdx()*100));
		CFileDialog m_ldFile(FALSE, ".cpn", filename,
			OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
		if (m_ldFile.DoModal() == IDOK)
		{
			filename = m_ldFile.GetPathName();
			CurrentPath=filename.Left(filename.GetLength()-strlen(m_ldFile.GetFileName()));
			SaveCanopy(filename,vp1);
		}
	}

}

void CTreeAnalyserDlg::SaveCanopy(CString filename,cVirtualPlant *vp1)
{

	FILE * pFile;
	pFile=fopen(filename,"wt");
	fprintf(pFile,"%6.4f\t%6.4f\t%6.4f\n",vp1->Getdx(),vp1->Getdy(),vp1->Getdz());
	fprintf(pFile,"ID\tx\ty\tz\tleaf_area\tmax_leaf_area\n");

	for(unsigned int i=vp1->GetTotal_cells()-1;i<vp1->GetTotal_cells();i--)
	{
		fprintf(pFile,"%i\t%6.4f\t%6.4f\t%6.4f\t%10.8f\t%10.8f\n",vp1->acell[i].ID,vp1->acell[i].GetX(),vp1->acell[i].GetY(),vp1->acell[i].GetZ(),vp1->acell[i].sum_surface,vp1->acell[i].maximumLeafArea);
	}

	fclose(pFile);

}

void CTreeAnalyserDlg::OnCanopyOpencanopy()
{
	CString filename; 
     static char BASED_CODE szFilter[] = "Tree Project (*.cpn)|*.cpn||";
     CFileDialog m_ldFile(TRUE, ".cpn", CurrentPath+"*.cpn",
          OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);

	if (m_ldFile.DoModal() == IDOK)
	{
		filename = m_ldFile.GetPathName();
		OpenCanopy(filename,vp1);
		ms.Format("\r\nOpen saved canopy: dx=%6.2f  dy=%6.2f  dz=%6.2f   Total Voxels=%i",vp1->Getdx(),vp1->Getdy(),vp1->Getdz(),vp1->GetTotal_cells());
		AddDisplayText(ms);
		vp1->DisplayCanopyProperties();
		this->SetWorkingMenu();
		this->DisplaySettingProperties();
	}//if (m_ldFile.DoModal() == IDOK)

}//void CTreeAnalyserDlg::OnCanopyOpencanopy()


void CTreeAnalyserDlg::OpenCanopy(CString filename,cVirtualPlant *vp1)
{
		//ms.Format("FileName=%s",filename);
		//MessageBox(ms);
		if(!vp1->Resize(0)) {MessageBox("Resize canopy error");} //resize canopy to 0 cell

		char seps[]   = " ,\t\n"; //seperation of each value by line
		char *token;
		char buffer[256];
		FILE *datafile;
		datafile= fopen(filename,"r");
		int i; //n=0,
		double dx=0,dy=0,dz=0,temp=0;
		double sum_leaf=0;
		if (datafile!=NULL)
		{
			fgets(buffer,255,datafile);
			token = strtok( buffer, seps);
				i=0;
				while(token != NULL)
				{
					temp=atof(token);
					token = strtok( NULL, seps );
					i++;
				if (i==1){dx=temp;}
				if (i==2){dy=temp;}
				if (i==3){dz=temp;}
				}
			vp1->PutDxyz(dx,dy,dz);
			pImagePool1->cellSize_dx=dx;
			pImagePool1->cellSize_dy=dy;
			pImagePool1->cellSize_dz=dz;
		
			fgets(buffer,255,datafile); //throw out header

			while (!feof(datafile))
			{
				cell tempcell;
				tempcell.haveleaf=1;
				fgets(buffer,255,datafile);
					
				token = strtok( buffer, seps );// Establish string and get the first token: 
				i=0;
				tempcell.ID=atoi(token);
				token = strtok( NULL, seps );
				while(token != NULL)
				{
					temp=atof(token);
					token = strtok( NULL, seps );
					i++;
					if (i==1){tempcell.PutX(temp);}
					if (i==2){tempcell.PutY(temp);}
					if (i==3){tempcell.PutZ(temp);}
					if (i==4){tempcell.sum_surface=temp;sum_leaf+=temp;}
					if (i==5){tempcell.maximumLeafArea=temp;}
				}
				if(i==4||i==5){vp1->AddCell(tempcell);}
			}

			fclose(datafile);
			vp1->ResetKeepIt();
		} //if (datafile!=NULL)
		else {MessageBox("Error reading data file.","Error");}
}//void CTreeAnalyserDlg::OpenCanopy(CString filename,cVirtualPlant *vp1)

void CTreeAnalyserDlg::OnNMDblclkTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	if(pImagePool1->GetImageCount()){OnImageEditparameters();UpdatePicture();}
	*pResult = 0;
	pNMHDR=pNMHDR; // add to avoid warning
}





void CTreeAnalyserDlg::OnImageFractalanalysis()
{
	CTreeAnalyserDlg * pNewObject = this; //create pointer to this
	DWORD dwExitCode;
	if (pWorkThread == NULL) //if thread have not been created
	{
		WorkIndex=7;
		pWorkThread=AfxBeginThread(ThreadWork, pNewObject);
	}
	else
	{
		::GetExitCodeThread(pWorkThread->m_hThread,&dwExitCode);
		if(dwExitCode!=STILL_ACTIVE) //if thread is finish
		{
			WorkIndex=7;
			pWorkThread=AfxBeginThread(ThreadWork, pNewObject);
		}
		else //if thread is working
		{
			pWorkThread->SuspendThread();
			if(this->AskForStopThread()==IDNO){pWorkThread->ResumeThread();}
			else
			{
				::WaitForSingleObject(pWorkThread->m_hThread,0); //stop the thread suddenly
				delete pWorkThread;
				pWorkThread=NULL;
				this->DisplayTerminateMessage();
				WorkIndex=0;
				this->SetWorkingMenu();
				DisplayStatus();
			}
		}
	}
}

void CTreeAnalyserDlg::Fractalanalysis()
{
	if (pImagePool1->GetImageCount()!=0)
	{
		CString output_filename=OutputLocation + ActivePictureName + "_Fractal";

		static char BASED_CODE szFilter[] = "Text file (*.txt)|*.txt||";
		CFileDialog m_ldFile(FALSE, ".txt", output_filename,
			OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
		int ans=m_ldFile.DoModal();
		if ( ans == IDOK)
		{
			output_filename = m_ldFile.GetPathName();
			FILE * pFile;
			pFile=fopen(output_filename,"wt");
			fprintf(pFile,"dx\tdy\tTotal\tempty\tnon-empty\tBlackZone\tmeanGap\tvarGap\tLacunarity\n");
			fclose(pFile);

			CEachImage *temp=pImagePool1->GetImage(&ActivePictureName);
			int limit_size;
			if(temp->GetPictureHeight()<temp->GetPictureWidth()){limit_size=temp->GetPictureHeight();}
			else {limit_size=temp->GetPictureWidth();}
			limit_size=limit_size/2; //reduce to half

			AddDisplayText("------------------------------------ Analysing Fractal -----------------------------------");
			ms.Format("Working image='%s'",temp->GetPictureName());
			AddDisplayText(ms);


			int n=0;
			int current_number=0;
			for (int i=1;current_number<=limit_size;i++)
			{	
				current_number=int(pow(2,i));
				ms.Format("Analysing Fractal %ix%i",current_number,current_number);
				DisplayStatus(ms);
				n=0;
				CountFractal(temp,current_number,current_number,output_filename);
			}
			DisplayStatus();



		}//if (m_ldFile.DoModal() == IDOK)
		DisplayStatus();
	}
	else
	{
		MessageBox("No image for working.");
	}
} //void CTreeAnalyserDlg::Fractalanalysis()

bool CTreeAnalyserDlg::CountFractal(CEachImage *image,int gx,int gy,CString output_filename)
{
	bool result=0;
	std::vector<double> vctgap;

	CImage ctrlImage;
	// Load the selected bitmap file
	HRESULT loadresule= ctrlImage.Load(image->GetFileName());
	if (FAILED(loadresule))
	{
		ms.Format("Error reading file '%s'\r\nSelected file may not existed or not be the bitmap file.",image->GetFileName());
		AddDisplayText(ms);
		result=0;
		goto skipimage;
	}
	else //if load image OK
	{
		//calculate number of direction
		int nx=0,ny=0;
		if (mod(ctrlImage.GetWidth(),gx)==0)
		{
			nx=ctrlImage.GetWidth()/gx;
		}
		else {nx=(ctrlImage.GetWidth()/gx)+1;}

		if (mod(ctrlImage.GetHeight(),gy)==0)
		{
			ny=ctrlImage.GetHeight()/gy;
		}
		else {ny=(ctrlImage.GetHeight()/gy)+1;}



	// ------------ Start read pixel and calculate each direction-----------------------------
		//int totalfound=0;
		int i,j,k,l;
		int opercent=-1; double unitpercent=double(nx)/100;
		for (i=1;i<=nx;i++)
		{
			int percent=int(i/unitpercent);
			if(percent>opercent)
			{
				ms.Format("Analysing Fractal %ix%i %i%%",gx,gy,percent);
				DisplayStatus(ms);
				opercent=percent;
			}

			for (j=1;j<=ny;j++)
			{
				int blackfound=0;
				k=1;
				while (k<=gx)
				{	
					l=1;
					while(l<=gy)
					{
						int a=(i-1)*gx+k;
						int b=(j-1)*gy+l;
						if(a>=ctrlImage.GetWidth() || b>=ctrlImage.GetHeight()){break;}
						if(ctrlImage.GetPixel(a,b)==0) //black pixel found
						{
							blackfound++;
						}
						l++;

					} //while(l<=pImagePool1->raypy && blackfound=0)
					k++;
				} //while (k<=pImagePool1->raypx && blackfound=0)
				double gapfraction=double(blackfound)/(gx*gy);
				if(gapfraction>0){vctgap.push_back(gapfraction);}
			} //for (j=1;j<=ny;j++)
		} //for (i=1;i<=nx;i++)
		
		int blackzone=0;
		double sumGap=0,sumGap2=0,meanGap=0,VGap=0;
		for(i=0;i<int(vctgap.size());i++)
		{
			if(vctgap[i]==1){blackzone++;}
			sumGap+=vctgap[i];
			sumGap2+=pow(vctgap[i],2);
		}

		meanGap=sumGap/(nx*ny);
		VGap=sumGap2/(nx*ny)-pow(meanGap,2);
		double LacunarityVGap=(VGap/pow(meanGap,2))+1;


		ms.Format("Diminsion %ix%i  total=%i  empty=%i non-empty=%i  black_zone=%i",gx,gy,nx*ny,nx*ny-vctgap.size(),vctgap.size(),blackzone);
		AddDisplayText(ms);

		FILE * pFile;
		pFile=fopen(output_filename,"at");
		//fprintf(pFile,"dx\tdy\tTotal\tempty\tnon-empty\tBlackZone\tmeanGap\tvarGap\tLacunarity\n");
		fprintf(pFile,"%i\t%i\t%i\t%i\t%i\t%i\t",gx,gy,nx*ny,nx*ny-vctgap.size(),vctgap.size(),blackzone);
		fprintf(pFile,"%6.4f\t%6.4f\t%6.4f\n",meanGap,VGap,LacunarityVGap);
		fclose(pFile);
		result=1;
     } 
skipimage:;
	return result;

}


CString CTreeAnalyserDlg::GetLogFilename(void)
{
	CString logfile;
	if(this->ActiveProjectFilename==""){logfile=OutputLocation + "untitle.log";}
	else{logfile=this->GetRootName(this->ActiveProjectFilename)+".log";}
	return logfile;
}

void CTreeAnalyserDlg::OnCanopyRandomleafinvolume()
{
	if(!vp1->GetTotal_cells()==0)
	{
		CInputDlg dlg;
		dlg.DlgTitle="Random leaves in the canopy volume";
		dlg.InputLabelText="Number of leaf:";

		if (dlg.DoModal()==IDOK)
		{
			double total_leaf=dlg.m_input;
			std::vector<leaf> vctLeaf;

			srand( (unsigned)time( NULL ) );

			for (int i=0;i<total_leaf;i++)
			{
				ms.Format("Randoming leaf %i",i+1);
				DisplayStatus(ms);
				double x=0,y=0,z=0;
				double min_x=vp1->GetMinMax(0,1);
				double max_x=vp1->GetMinMax(1,1);
				double min_y=vp1->GetMinMax(0,2);
				double max_y=vp1->GetMinMax(1,2);
				double min_z=vp1->GetMinMax(0,3);
				double max_z=vp1->GetMinMax(1,3);

				leaf templeaf;

				bool isOK=0;
				while (!isOK)
				{
					double temp;
					temp=rand();
					x=((temp/32767)*(max_x-min_x)+min_x)*100;
					temp=rand();
					y=((temp/32767)*(max_y-min_y)+min_y)*100;
					temp=rand();
					z=((temp/32767)*(max_z-min_z)+min_z)*100;
					templeaf.PutXyz(x,y,z);
					isOK=vp1->IsLeafInVolume(templeaf);
				}
				vctLeaf.push_back(templeaf);
			}
			DisplayStatus();
			//save leaves to file
			CString filename;
			static char BASED_CODE szFilter[] = "Leaf output (*.txt)|*.txt||";
			filename=this->GetRootName(this->ActiveProjectFilename)+".txt";
			CFileDialog m_ldFile(FALSE, ".cpn", filename,
				OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
			if (m_ldFile.DoModal() == IDOK)
			{
				filename = m_ldFile.GetPathName();

				FILE * pFile;
				pFile=fopen(filename,"wt");
				fprintf(pFile,"x\ty\tz\t\n");
				total_leaf=vctLeaf.size();
				for(int i=0;i<total_leaf;i++)
				{
					fprintf(pFile,"%6.4f	%6.4f	%6.4f\n",vctLeaf[i].GetY(),vctLeaf[i].GetX(),vctLeaf[i].GetZ()*-1);
				}

				fclose(pFile);
				vctLeaf.clear();
			}//if (m_ldFile.DoModal() == IDOK)
		}//if (dlg.DoModal()==IDOK)
	}//if(!vp1->GetTotal_cells()==0)
	else
	{
		MessageBox("No canopy to work");
	}
}


void CTreeAnalyserDlg::OnProjectLaInterceptedDirection()
{
	CTreeAnalyserDlg * pNewObject = this; //create pointer to this
	DWORD dwExitCode;
	if (pWorkThread == NULL) //if thread have not been created
	{
		WorkIndex=4;
		pWorkThread=AfxBeginThread(ThreadWork, pNewObject);
	}
	else
	{
		::GetExitCodeThread(pWorkThread->m_hThread,&dwExitCode);
		if(dwExitCode!=STILL_ACTIVE) //if thread is finish
		{
			WorkIndex=4;
			pWorkThread=AfxBeginThread(ThreadWork, pNewObject);
		}
		else //if thread is working
		{
			pWorkThread->SuspendThread();
			if(this->AskForStopThread()==IDNO){pWorkThread->ResumeThread();}
			else
			{
				::WaitForSingleObject(pWorkThread->m_hThread,0);
				delete pWorkThread;
				pWorkThread=NULL;
				this->DisplayTerminateMessage();
				WorkIndex=0;
				this->SetWorkingMenu();
				DisplayStatus();
			}
		}
	}

}//void CTreeAnalyserDlg::OnProjectLaInterceptedDirection()



UINT CTreeAnalyserDlg::ThreadWork(LPVOID pParam)
{
    CTreeAnalyserDlg * pObject = (CTreeAnalyserDlg*)pParam;

    if (pObject == NULL) //|| !pObject->IsKindOf (RUNTIME_CLASS(tThread)));
    return 1;   // if pObject is not valid

    // do something with 'pObject'
	pObject->SetWorkingMenu();
	switch (pObject->WorkIndex)
	{
	case 1:
		pObject->EstimateTreeDimension(); 
		break;
	case 2:
		pObject->VolumeReconstruction(); 
		break;
	case 3:
		pObject->EstimateLeafArea();
		break;
	case 4: //LAD computation menu was removed
		
		break;
	case 5:
		pObject->AnalyseAll(); 
		break;
		
	case 6:
		//pObject->SolvingEquation(); function cleared
		//pObject->ComputeLADdistribution(); 
		//pObject->CaculLAD1();
		break;
		
	case 7:
		pObject->Fractalanalysis();
		break;
	case 8:
		pObject->Colorcounting();
		break;
	case 9:
		pObject->Savepixelvalue();
		break;
	case 10:
		//pObject->ExportP0csv();//function cleared
		break;
	case 11:
		//pObject->Test2(); function cleared
		break;
	case 12:
		//pObject->Solvinglinearregression(); function cleared
		break;
	case 13:
		//pObject->Computep0beer(); function cleared
		break;
	case 14:
		//pObject->Exportcsvfileofp0binom(); //function cleared
		break;
	case 15:
		pObject->Exportbeamequations();
		break;

	}
	pObject->WorkIndex=0;
	pObject->SetWorkingMenu();
	::MessageBeep(MB_ICONASTERISK);
    return 0;   // thread completed successfully
}//UINT CTreeAnalyserDlg::ThreadEstimateLeafAreaTotalDir( LPVOID pParam )

int CTreeAnalyserDlg::AskForStopThread(void)
{
		int ans=0;
		if (WorkIndex==1){ans=MessageBox("The estimation tree dimension is running.\nWould you like to stop working?","Tree Analyser",MB_YESNO);}
		if (WorkIndex==2){ans=MessageBox("Reconstruction of canopy volume is running.\nWould you like to stop working?","Tree Analyser",MB_YESNO);}
		if (WorkIndex==3){ans=MessageBox("The estimation leaf area is running.\nWould you like to stop working?","Tree Analyser",MB_YESNO);}
		if (WorkIndex==4){ans=MessageBox("The estimation LAD distribution is running.\nWould you like to stop working?","Tree Analyser",MB_YESNO);}
		if (WorkIndex==5){ans=MessageBox("Do All Calculations is running.\nWould you like to stop working?","Tree Analyser",MB_YESNO);}
		if (WorkIndex==6){ans=MessageBox("Computation of LAD distribution is running.\nWould you like to stop working?","Tree Analyser",MB_YESNO);}
		if (WorkIndex==7){ans=MessageBox("Analysis of fractal is running.\nWould you like to stop working?","Tree Analyser",MB_YESNO);}
		if (WorkIndex==8){ans=MessageBox("Color counting is running.\nWould you like to stop working?","Tree Analyser",MB_YESNO);}
		if (WorkIndex==9){ans=MessageBox("Save Pixel Value is running.\nWould you like to stop working?","Tree Analyser",MB_YESNO);}
		if (WorkIndex==11){ans=MessageBox("Random fitting is running.\nWould you like to stop working?","Tree Analyser",MB_YESNO);}
		if (WorkIndex==12){ans=MessageBox("Solving linear regression is running.\nWould you like to stop working?","Tree Analyser",MB_YESNO);}
		if (WorkIndex==13){ans=MessageBox("Computing P0 Beer is running.\nWould you like to stop working?","Tree Analyser",MB_YESNO);}
		if (WorkIndex==14){ans=MessageBox("Computing P0 Binom is running.\nWould you like to stop working?","Tree Analyser",MB_YESNO);}
		if (WorkIndex==15){ans=MessageBox("Exporting equations for Python is running.\nWould you like to stop working?","Tree Analyser",MB_YESNO);}
		return ans;
}

void CTreeAnalyserDlg::DisplayTerminateMessage(void)
{
		if(WorkIndex==1){this->AddDisplayText("\r\nEstimate of tree dimension terminated by user!!\r\n");}
		if(WorkIndex==2){this->AddDisplayText("\r\nReconstruction of canopy volume terminated by user!!\r\n");}
		if(WorkIndex==3){this->AddDisplayText("\r\nEstimate of leaf area terminated by user!!");}
		if(WorkIndex==4){this->AddDisplayText("\r\nEstimate of LAD distribution terminated by user!!\r\n");}
		if(WorkIndex==5){this->AddDisplayText("\r\nAnalyse all terminated by user!!\r\n");}
		if(WorkIndex==6){this->AddDisplayText("\r\nComputation of LAD distribution terminated by user!!\r\n");}
		if(WorkIndex==7){this->AddDisplayText("\r\nFractal analysis terminated by user!!\r\n");}
		if(WorkIndex==8){this->AddDisplayText("\r\nColor counting terminated by user!!\r\n");}
		if(WorkIndex==9){this->AddDisplayText("\r\nSave pixel value terminated by user!!\r\n");}
		if(WorkIndex==11){this->AddDisplayText("\r\nRandom fitting terminated by user!!\r\n");}
		if(WorkIndex==12){this->AddDisplayText("\r\nSolving linear regression terminated by user!!\r\n");}
		if(WorkIndex==13){this->AddDisplayText("\r\nCompute P0 Beer terminated by user!!\r\n");}
}

void CTreeAnalyserDlg::VolumeReconstruction(void)
{
//-------- initializing time used ---------------
   time_t   start, finish;
   double   elapsed_time;   
   time( &start );
//-------- end initializing time used ---------------

	DisplayStatus("Reconstructing tree volume");
	AddDisplayText(" ");
	ms.Format("******************** Reconstruction of Tree Volume. ********************");
	AddDisplayText(ms);		
	ms.Format("Estimated bounding box of vegetation canopy = %6.2fx%6.2fx%6.2f",
		pImagePool1->maxDiameter,pImagePool1->maxDiameter,pImagePool1->maxVHeight);
	AddDisplayText(ms);		

	UpdateCheckItem();
	//create virtual plant

	if(!CreateVirtualPlant(0)) //use option=[0]
	{
		MessageBox("Error reconstruction of volume!!\r\nThe number of voxel is over the limit.");
		goto endvolume;
	}
	ms.Format("	Calculate the intercepted directions using grid size=%ix%i pixel",
		pImagePool1->raypx,pImagePool1->raypy);
	AddDisplayText(ms);
	AddDisplayText(" ");

	//write header of log file
	bool error_open_log=0;
	if(pImagePool1->m_iflogvolume==TRUE)
	{
		FILE * LogFileStream;
		if((LogFileStream=fopen(this->GetLogFilename(),"at"))==NULL)
		{
			ms.Format("Error open log file '%s'. \nThis file maybe already opened.",this->GetLogFilename());
			MessageBox(ms);
			AddDisplayText("Error! Can not write to log file");
			error_open_log=1;
		}
		else
		{
		fprintf(LogFileStream,"ImageNo.\tImageName\tEle\tAz\tdx\tdy\tdz\tIn_cell\tOut_cell\tVolume\tCHeight\tVHeight\tN-S\tW-E\n");
		fclose(LogFileStream);
		}
	}
	//export first volume
	if(pImagePool1->m_ifoutputbox==TRUE)
	{
		ms=OutputLocation+"Volume0_bounding.vgx";
		AddDisplayText("		Export box volume to file "+ms);
		ExportVirtualPlant(2,ms,0); //export tree volume
	}

	//do ray-box intersection for each image
	int workcount=0;


	for (int pic=0;pic<pImagePool1->GetImageCount();pic++)
	{
		CEachImage *tempImage=pImagePool1->GetImage(pic);

		if (tempImage->GetFileName()!="empty" && tempImage->GetCheck()==1) //do only selected image
		{

			m_Tree1.SelectItem(vctTreeItem[pic]);

			int in_cell=vp1->lifecell;
			ms.Format("	Start canopy erosion process using picture='%s'\r\n		Input canopy with %i voxels  volume=%6.3fm3",
				tempImage->GetPictureName(),vp1->lifecell,vp1->lifecell*vp1->GetCellVolume());
			AddDisplayText(ms);

			workcount++;
			tempImage->CalculBasic();
			CalculDirections(tempImage);
			vp1->ClearHaveLeaf(); //set haveleaf of every cells to 0;
			int dirsize=vctDirection.size();
			double unitpercent=double(dirsize)/100;
			int opercent=-1;	

			//double mindt=9e9,maxdt=0;

			for (int l=0;l<dirsize;l++) //do every intercepted directions
			{
				
				//*****  determine in and out point of canopy ************
				double ttnear=0,ttfar=0;
				vp1->RayCanopyIntersect(&tempImage->CameraCoordinate,&vctDirection[l].unitDirection,&ttnear,&ttfar);

				cPoint inpoint,outpoint;
				inpoint.PutX(tempImage->CameraCoordinate.GetX()+vctDirection[l].unitDirection.GetX()*ttnear);
				inpoint.PutY(tempImage->CameraCoordinate.GetY()+vctDirection[l].unitDirection.GetY()*ttnear);
				inpoint.PutZ(tempImage->CameraCoordinate.GetZ()+vctDirection[l].unitDirection.GetZ()*ttnear);

				outpoint.PutX(tempImage->CameraCoordinate.GetX()+vctDirection[l].unitDirection.GetX()*ttfar);
				outpoint.PutY(tempImage->CameraCoordinate.GetY()+vctDirection[l].unitDirection.GetY()*ttfar);
				outpoint.PutZ(tempImage->CameraCoordinate.GetZ()+vctDirection[l].unitDirection.GetZ()*ttfar);
				double lowerx=9e9,lowery=9e9,lowerz=9e9;
				double upperx=-9e9,uppery=-9e9,upperz=-9e9;
				
				if(inpoint.GetX()<=outpoint.GetX()){lowerx=inpoint.GetX();upperx=outpoint.GetX();}
				else{lowerx=outpoint.GetX();upperx=inpoint.GetX();}

				if(inpoint.GetY()<=outpoint.GetY()){lowery=inpoint.GetY();uppery=outpoint.GetY();}
				else{lowery=outpoint.GetY();uppery=inpoint.GetY();}

				if(inpoint.GetZ()<=outpoint.GetZ()){lowerz=inpoint.GetZ();upperz=outpoint.GetZ();}
				else{lowerz=outpoint.GetZ();upperz=inpoint.GetZ();}
		
				//*****  End determine in and out point of canopy ************


				int percent=int(l/unitpercent);
				if(percent>opercent)
				{
					ms.Format("Running ray-cell intersection algorithm on image '%s'  %i%%",tempImage->GetPictureName(),percent);
					DisplayStatus(ms);
					opercent=percent;
				}
				for(unsigned int i=0;i<vp1->GetTotal_cells();i++)
				{
					if (vp1->acell[i].haveleaf==0) //do only cell that keepit=1
					{
							//ms.Format("lowerz=%6.3f acellz=%6.3f upperz=%6.3f acellz=%6.3f",lowerz,vp1->acell[i].GetZ(),upperz,vp1->acell[i].GetZ()+vp1->Getdz()*1.1);
							//this->DisplayStatus(ms);

						//check if current cell is in the rage of inpoint and outpoint
						//if(lowerx-vp1->Getdx()<=vp1->GetCellX(i) && upperx+vp1->Getdx()>=vp1->GetCellX(i))
						if(lowerx-vp1->Getdx()<=vp1->acell[i].GetX() && upperx+vp1->Getdx()>=vp1->acell[i].GetX())
						{
							//ms.Format("lowerx=%6.3f acellx=%6.3f upperx=%6.3f acellx=%6.3f",lowerx,vp1->GetCellX(i),upperx,vp1->GetCellX(i)+vp1->Getdx()*1.1);
							//this->AddDisplayText(ms);
							if(lowery-vp1->Getdy()<=vp1->acell[i].GetY() && uppery+vp1->Getdy()>=vp1->acell[i].GetY())
							{
								if(lowerz-vp1->Getdz()<=vp1->acell[i].GetZ() && upperz+vp1->Getdz()>=vp1->acell[i].GetZ())
								{
									short int ans=vp1->RayCellIntersect(&tempImage->CameraCoordinate,&vctDirection[l].unitDirection,&vp1->acell[i]);
									if(ans==1){vp1->acell[i].haveleaf=1;}
								}
							}
						}
					}
				}//for(unsigned int i=0;i<vp1->GetTotal_cells();i++)
			}//for (int l=0;l<dirsize;l++)

			vp1->ResetKeepIt(); //reset keepit if no leaf set to 0
			vctDirection.clear(); //clear memory of directions

			//-------------- Start writing log file ------------------------
			if(pImagePool1->m_iflogvolume==TRUE && !error_open_log)
			{
				FILE * pFile;
				pFile=fopen(this->GetLogFilename(),"at");
				fprintf(pFile,"%i\t%s\t%6.2f\t%6.2f\t%6.4f\t%6.4f\t%6.4f\t%i\t%i\t%6.4f\t%6.4f\t%6.4f\t%6.4f\t%6.4f\n",
					pic+1,tempImage->GetPictureName(),tempImage->GetCameraInc(),tempImage->GetCameraDirection(),
					vp1->Getdx(),vp1->Getdy(),vp1->Getdz(),
					in_cell,vp1->lifecell,vp1->lifecell*vp1->GetCellVolume(),
					vp1->GetMinMax(1,3),vp1->GetMinMax(1,3)-vp1->GetMinMax(0,3),
					vp1->GetMinMax(1,2)-vp1->GetMinMax(0,2),vp1->GetMinMax(1,1)-vp1->GetMinMax(0,1));
				fclose(pFile);
			}
			//-------------- End writing log file ------------------------


			//export cells after ray-box intersection
			if(pImagePool1->m_ifoutputbox==TRUE)
			{
				ms.Format("Vol%i_after_%s.vgx",workcount,tempImage->GetPictureName());
				ms=OutputLocation+ms;
				AddDisplayText("		Export box volume to file "+ms);
				ExportVirtualPlant(2,ms,7); //export tree volume
			}
			
		}//if (tempImage->GetFileName()!="empty" && tempImage->GetCheck()==1)
		
	} //for (int pic=0;pic<pImagePool1->GetImageCount();pic++)

	AddDisplayText(" ");
	vp1->DisplayCanopyProperties();
	AddDisplayText(" ");
	DisplayStatus();
	DisplaySettingProperties();

endvolume:;
//----------- calculate time used ---------------------------
   time( &finish );
   elapsed_time = difftime( finish, start );
   ms.Format( "Reconstruction of volume using = %6.0f seconds.", elapsed_time );
	AddDisplayText(ms);
//----------- End calculate time used ---------------------------

	char datebuf[128];
	struct tm *today;__time64_t ltime;
	_time64( &ltime );
	today = _localtime64( &ltime );
	strftime( datebuf, 128,"%d-%b-%y %H:%M:%S", today );

	if(pImagePool1->m_iflogvolume==TRUE && !error_open_log)
	{
		CString ms2;
		ms2.LoadString(IDS_VERSION);
		FILE * pFile;
		pFile=fopen(this->GetLogFilename(),"at");

		fprintf(pFile,"Reconstruction of volume was done by %s on %s, using time = %i seconds.\n\n",ms2,datebuf,int(elapsed_time));
		fclose(pFile);
		ms.Format("Log output to => %s",this->GetLogFilename());
		AddDisplayText(ms);
	}
	if(error_open_log){AddDisplayText("!Unable to write the log file!");}
	
	
	
} //void CTreeAnalyserDlg::VolumeReconstruction(void)



void CTreeAnalyserDlg::SetWorkingMenu(void)
{
	pMenu = this->GetMenu();
	switch (WorkIndex)
	{
	case 0: // no working
		//menu Calculation
		if(pImagePool1->GetImageCount()==0){pMenu->ModifyMenu(ID_PROJECT_ESTIMATETREEDIMENSION,MF_GRAYED,ID_PROJECT_ESTIMATETREEDIMENSION,"Estimate Dimension");}
		else{pMenu->ModifyMenu(ID_PROJECT_ESTIMATETREEDIMENSION,MF_BYCOMMAND,ID_PROJECT_ESTIMATETREEDIMENSION,"Estimate Dimension");}

		if(pImagePool1->GetImageCount()>0 && pImagePool1->maxTreeHeight>0){pMenu->ModifyMenu(ID_PROJECT_VOLUME,MF_BYCOMMAND,ID_PROJECT_VOLUME,"Reconstruct Volume");}
		else{pMenu->ModifyMenu(ID_PROJECT_VOLUME,MF_GRAYED,ID_PROJECT_VOLUME,"Reconstruct Volume");}

		if(vp1->GetTotal_cells()>0 && pImagePool1->GetImageCount()>0){pMenu->ModifyMenu(ID_PROJECT_LA_TOTAL,MF_BYCOMMAND,ID_PROJECT_LA_TOTAL,"Estimate Leaf Area");}
		else{pMenu->ModifyMenu(ID_PROJECT_LA_TOTAL,MF_GRAYED,ID_PROJECT_LA_TOTAL,"Estimate Leaf Area");}

		//if(vp1->GetTotal_cells()>0 && pImagePool1->GetImageCount()>0){pMenu->ModifyMenu(ID_PROJECT_COMPUTELADDISTRIBUTION,MF_BYCOMMAND,ID_PROJECT_COMPUTELADDISTRIBUTION,"Compute LAD Distribution");}
		//else{pMenu->ModifyMenu(ID_PROJECT_COMPUTELADDISTRIBUTION,MF_GRAYED,ID_PROJECT_COMPUTELADDISTRIBUTION,"Compute LAD Distribution");}

		if(pImagePool1->GetImageCount()>0){pMenu->ModifyMenu(ID_PROJECT_ANALYSEALL,MF_BYCOMMAND,ID_PROJECT_ANALYSEALL,"Do All Calculations");}
		else{pMenu->ModifyMenu(ID_PROJECT_ANALYSEALL,MF_GRAYED,ID_PROJECT_ANALYSEALL,"Do All Calculations");}

		if(vp1->GetTotal_cells()>0 && pImagePool1->GetImageCount()>0){pMenu->ModifyMenu(ID_CALCULATION_EXPORTEQUATIONSFORPYTHON,MF_BYCOMMAND,ID_CALCULATION_EXPORTEQUATIONSFORPYTHON,"Export equations for Python");}
		else{pMenu->ModifyMenu(ID_CALCULATION_EXPORTEQUATIONSFORPYTHON,MF_GRAYED,ID_CALCULATION_EXPORTEQUATIONSFORPYTHON,"Export equations for Python");}


		pMenu->EnableMenuItem(ID_VIEW_CLEAROUTPUT,MF_BYCOMMAND);
		pMenu->EnableMenuItem(ID_PROJECT_SETTING,MF_BYCOMMAND);


		//menu File
		pMenu->ModifyMenu(ID_FILE_1,MF_BYCOMMAND,ID_FILE_1,mrl[0]);
		pMenu->ModifyMenu(ID_FILE_2,MF_BYCOMMAND,ID_FILE_2,mrl[1]);
		pMenu->ModifyMenu(ID_FILE_3,MF_BYCOMMAND,ID_FILE_3,mrl[2]);
		pMenu->ModifyMenu(ID_FILE_4,MF_BYCOMMAND,ID_FILE_4,mrl[3]);
		pMenu->EnableMenuItem(ID_FILE_NEWPROJECT,MF_BYCOMMAND);
		pMenu->EnableMenuItem(ID_FILE_OPENPROJECT,MF_BYCOMMAND);
	

		if(ActiveProjectFilename==""){pMenu->EnableMenuItem(ID_FILE_SAVEPROJECT,MF_GRAYED);}
		else{pMenu->EnableMenuItem(ID_FILE_SAVEPROJECT,MF_BYCOMMAND);}
		
		//menu Canopy
		if(vp1->GetTotal_cells()>0){pMenu->EnableMenuItem(ID_CANOPY_DISPLAYPROPERTIES,MF_BYCOMMAND);}
		else{pMenu->EnableMenuItem(ID_CANOPY_DISPLAYPROPERTIES,MF_GRAYED);}
		if(vp1->GetTotal_cells()>0){pMenu->EnableMenuItem(ID_CANOPY_SAVECANOPY,MF_BYCOMMAND);}
		else{pMenu->EnableMenuItem(ID_CANOPY_SAVECANOPY,MF_GRAYED);}
		
		pMenu->EnableMenuItem(ID_CANOPY_OPENCANOPY,MF_BYCOMMAND);
		pMenu->EnableMenuItem(ID_CANOPY_COMPARECANOPY,MF_BYCOMMAND);
		
		if(vp1->GetTotal_cells()>0){pMenu->EnableMenuItem(ID_CANOPY_RANDOMLEAFINVOLUME,MF_BYCOMMAND);}
		else{pMenu->EnableMenuItem(ID_CANOPY_RANDOMLEAFINVOLUME,MF_GRAYED);}
		if(vp1->GetTotal_cells()>0){pMenu->EnableMenuItem(ID_CANOPY_EXPORTVEGESTARCANOPY,MF_BYCOMMAND);}
		else{pMenu->EnableMenuItem(ID_CANOPY_EXPORTVEGESTARCANOPY,MF_GRAYED);}
		if(vp1->GetTotal_cells()>0){pMenu->EnableMenuItem(ID_CANOPY_EXPORTRATPVOXELS,MF_BYCOMMAND);}
		else{pMenu->EnableMenuItem(ID_CANOPY_EXPORTRATPVOXELS,MF_GRAYED);}
		if(vp1->GetTotal_cells()>0){pMenu->EnableMenuItem(ID_CANOPY_VERTICALPROFILEOFLEAFAREA,MF_BYCOMMAND);}
		else{pMenu->EnableMenuItem(ID_CANOPY_VERTICALPROFILEOFLEAFAREA,MF_GRAYED);}
		if(vp1->GetTotal_cells()>0){pMenu->EnableMenuItem(ID_CANOPY_VERTICALPROFILEOFVOLUME,MF_BYCOMMAND);}
		else{pMenu->EnableMenuItem(ID_CANOPY_VERTICALPROFILEOFVOLUME,MF_GRAYED);}

		if(vp1->GetTotal_cells()>0){pMenu->EnableMenuItem(ID_CANOPY_EXPORTPLANTGLFILE,MF_BYCOMMAND);}
		else{pMenu->EnableMenuItem(ID_CANOPY_EXPORTPLANTGLFILE,MF_GRAYED);}
		if(vp1->GetTotal_cells()>0){pMenu->EnableMenuItem(ID_CANOPY_VIEWBYPLANTGL3DVIEWER,MF_BYCOMMAND);}
		else{pMenu->EnableMenuItem(ID_CANOPY_VIEWBYPLANTGL3DVIEWER,MF_GRAYED);}

		//not in used
		if(vp1->GetTotal_cells()>0){pMenu->EnableMenuItem(ID_CANOPY_DECREASE,MF_GRAYED);}
		else{pMenu->EnableMenuItem(ID_CANOPY_DECREASE,MF_GRAYED);}
		if(vp1->GetTotal_cells()>0){pMenu->EnableMenuItem(ID_CANOPY_RERUNRAY,MF_GRAYED);}
		else{pMenu->EnableMenuItem(ID_CANOPY_RERUNRAY,MF_GRAYED);}


		//menu Image
		pMenu->EnableMenuItem(ID_IMAGE_ADDIMAGETOPROJECT,MF_BYCOMMAND);
		if(pImagePool1->GetImageCount()==0){pMenu->ModifyMenu(ID_IMAGE_COLORCOUNTING,MF_GRAYED,ID_IMAGE_COLORCOUNTING,"&Color Counting");}
		else{pMenu->ModifyMenu(ID_IMAGE_COLORCOUNTING,MF_BYCOMMAND,ID_IMAGE_COLORCOUNTING,"&Color Counting");}

		if(pImagePool1->GetImageCount()==0){pMenu->ModifyMenu(ID_IMAGE_FRACTALANALYSIS,MF_GRAYED,ID_IMAGE_FRACTALANALYSIS,"&Fractal Analysis");}
		else{pMenu->ModifyMenu(ID_IMAGE_FRACTALANALYSIS,MF_BYCOMMAND,ID_IMAGE_FRACTALANALYSIS,"&Fractal Analysis");}

		if(pImagePool1->GetImageCount()==0){pMenu->ModifyMenu(ID_IMAGE_SAVEPIXELVALUE,MF_GRAYED,ID_IMAGE_SAVEPIXELVALUE,"&Save Pixel Value");}
		else{pMenu->ModifyMenu(ID_IMAGE_SAVEPIXELVALUE,MF_BYCOMMAND,ID_IMAGE_SAVEPIXELVALUE,"&Save Pixel Value");}

		if(pImagePool1->GetImageCount()==0){pMenu->EnableMenuItem(ID_IMAGE_CALCUALTEGAPFRACTION,MF_GRAYED);}
		else{pMenu->EnableMenuItem(ID_IMAGE_CALCUALTEGAPFRACTION,MF_BYCOMMAND);}
		

		if(pImagePool1->GetImageCount()==0){pMenu->EnableMenuItem(ID_IMAGE_MARKALL,MF_GRAYED);}
		else{pMenu->EnableMenuItem(ID_IMAGE_MARKALL,MF_BYCOMMAND);}
		if(pImagePool1->GetImageCount()==0){pMenu->EnableMenuItem(ID_IMAGE_UNMARKALL,MF_GRAYED);}
		else{pMenu->EnableMenuItem(ID_IMAGE_UNMARKALL,MF_BYCOMMAND);}
		if(pImagePool1->GetImageCount()==0){pMenu->EnableMenuItem(ID_IMAGE_TOGLEMARK,MF_GRAYED);}
		else{pMenu->EnableMenuItem(ID_IMAGE_TOGLEMARK,MF_BYCOMMAND);}
		//if(pImagePool1->GetImageCount()==0){pMenu->EnableMenuItem(ID_IMAGE_REGISTERIMAGE,MF_GRAYED);}
		//else{pMenu->EnableMenuItem(ID_IMAGE_REGISTERIMAGE,MF_BYCOMMAND);}
		if(pImagePool1->GetImageCount()==0){pMenu->EnableMenuItem(ID_IMAGE_EDITPARAMETERS,MF_GRAYED);}
		else{pMenu->EnableMenuItem(ID_IMAGE_EDITPARAMETERS,MF_BYCOMMAND);}
		if(pImagePool1->GetImageCount()==0){pMenu->EnableMenuItem(ID_IMAGE_DELETE,MF_GRAYED);}
		else{pMenu->EnableMenuItem(ID_IMAGE_DELETE,MF_BYCOMMAND);}

		break;
	case 1:
		this->DisableAllMenus();
		pMenu->ModifyMenu(ID_PROJECT_ESTIMATETREEDIMENSION,MF_BYCOMMAND,ID_PROJECT_ESTIMATETREEDIMENSION,"Stop Estimate Dimension");
		break;
	case 2:
		this->DisableAllMenus();
		pMenu->ModifyMenu(ID_PROJECT_VOLUME,MF_BYCOMMAND,ID_PROJECT_VOLUME,"Stop Volume Reconstruction");
		break;
	case 3:
		this->DisableAllMenus();
		pMenu->ModifyMenu(ID_PROJECT_LA_TOTAL,MF_BYCOMMAND,ID_PROJECT_LA_TOTAL,"Stop Estimate Leaf Area");
		break;
	case 4:
		this->DisableAllMenus();
		pMenu->ModifyMenu(ID_PROJECT_LA_INTERCEPTED,MF_BYCOMMAND,ID_PROJECT_LA_INTERCEPTED,"Stop Estimate Leaf Area");
		break;
	case 5:
		this->DisableAllMenus();
		pMenu->ModifyMenu(ID_PROJECT_ANALYSEALL,MF_BYCOMMAND,ID_PROJECT_ANALYSEALL,"Stop doing all calculation");
		break;

	case 6:
		//this->DisableAllMenus();
		//pMenu->ModifyMenu(ID_PROJECT_COMPUTELADDISTRIBUTION,MF_BYCOMMAND,ID_PROJECT_COMPUTELADDISTRIBUTION,"Stop estimate LAD Distribution");

		break;
	case 7:
		this->DisableAllMenus();
		pMenu->ModifyMenu(ID_IMAGE_FRACTALANALYSIS,MF_BYCOMMAND,ID_IMAGE_FRACTALANALYSIS,"Stop Fractal Analysis");
		break;

	case 8:
		this->DisableAllMenus();
		pMenu->ModifyMenu(ID_IMAGE_COLORCOUNTING,MF_BYCOMMAND,ID_IMAGE_COLORCOUNTING,"Stop Color Counting");
		break;
	case 9:
		this->DisableAllMenus();
		pMenu->ModifyMenu(ID_IMAGE_SAVEPIXELVALUE,MF_BYCOMMAND,ID_IMAGE_SAVEPIXELVALUE,"Stop Saving Pixel");
		break;
	case 15:
		this->DisableAllMenus();
		pMenu->ModifyMenu(ID_CALCULATION_EXPORTEQUATIONSFORPYTHON,MF_BYCOMMAND,ID_CALCULATION_EXPORTEQUATIONSFORPYTHON,"Stop Export equation for Python");
		break;

	}
}

void CTreeAnalyserDlg::DisableAllMenus(void)
{

		pMenu->EnableMenuItem(ID_PROJECT_ESTIMATETREEDIMENSION,MF_GRAYED);
		pMenu->EnableMenuItem(ID_PROJECT_VOLUME,MF_GRAYED);
		pMenu->EnableMenuItem(ID_PROJECT_LA_TOTAL,MF_GRAYED);
		pMenu->EnableMenuItem(ID_PROJECT_LA_INTERCEPTED,MF_GRAYED);
		//pMenu->EnableMenuItem(ID_PROJECT_COMPUTELADDISTRIBUTION,MF_GRAYED);
		pMenu->EnableMenuItem(ID_PROJECT_ANALYSEALL,MF_GRAYED);

		//pMenu->EnableMenuItem(ID_PROJECT_SETTING,MF_GRAYED);
		pMenu->EnableMenuItem(ID_VIEW_CLEAROUTPUT,MF_GRAYED);

		pMenu->EnableMenuItem(ID_FILE_NEWPROJECT,MF_GRAYED);
		pMenu->EnableMenuItem(ID_FILE_OPENPROJECT,MF_GRAYED);
		pMenu->EnableMenuItem(ID_FILE_1,MF_GRAYED);
		pMenu->EnableMenuItem(ID_FILE_2,MF_GRAYED);
		pMenu->EnableMenuItem(ID_FILE_3,MF_GRAYED);
		pMenu->EnableMenuItem(ID_FILE_4,MF_GRAYED);

		pMenu->EnableMenuItem(ID_CANOPY_SAVECANOPY,MF_GRAYED);
		pMenu->EnableMenuItem(ID_CANOPY_OPENCANOPY,MF_GRAYED);
		pMenu->EnableMenuItem(ID_CANOPY_RANDOMLEAFINVOLUME,MF_GRAYED);
		pMenu->EnableMenuItem(ID_CANOPY_DISPLAYPROPERTIES,MF_GRAYED);
		pMenu->EnableMenuItem(ID_CANOPY_DECREASE,MF_GRAYED);
		pMenu->EnableMenuItem(ID_CANOPY_RERUNRAY,MF_GRAYED);

		pMenu->EnableMenuItem(ID_CANOPY_COMPARECANOPY,MF_GRAYED);
		pMenu->EnableMenuItem(ID_CANOPY_EXPORTVEGESTARCANOPY,MF_GRAYED);
		pMenu->EnableMenuItem(ID_CANOPY_EXPORTRATPVOXELS,MF_GRAYED);
		pMenu->EnableMenuItem(ID_CANOPY_VERTICALPROFILEOFLEAFAREA,MF_GRAYED);
		pMenu->EnableMenuItem(ID_CANOPY_VERTICALPROFILEOFVOLUME,MF_GRAYED);

		pMenu->EnableMenuItem(ID_CANOPY_EXPORTPLANTGLFILE,MF_GRAYED);
		pMenu->EnableMenuItem(ID_CANOPY_VIEWBYPLANTGL3DVIEWER,MF_GRAYED);


		pMenu->EnableMenuItem(ID_IMAGE_ADDIMAGETOPROJECT,MF_GRAYED);
		pMenu->EnableMenuItem(ID_IMAGE_COLORCOUNTING,MF_GRAYED);
		pMenu->EnableMenuItem(ID_IMAGE_MARKALL,MF_GRAYED);
		pMenu->EnableMenuItem(ID_IMAGE_UNMARKALL,MF_GRAYED);
		pMenu->EnableMenuItem(ID_IMAGE_TOGLEMARK,MF_GRAYED);
		pMenu->EnableMenuItem(ID_IMAGE_FRACTALANALYSIS,MF_GRAYED);

		pMenu->EnableMenuItem(ID_IMAGE_SAVEPIXELVALUE,MF_GRAYED);
		pMenu->EnableMenuItem(ID_IMAGE_EDITPARAMETERS,MF_GRAYED);
		pMenu->EnableMenuItem(ID_IMAGE_DELETE,MF_GRAYED);
		pMenu->EnableMenuItem(ID_IMAGE_REGISTERIMAGE,MF_GRAYED);
		pMenu->EnableMenuItem(ID_IMAGE_CALCUALTEGAPFRACTION,MF_GRAYED);

}//void CTreeAnalyserDlg::DisableAllMenus(void)



//! The function to calculate beam directions intersected with canopy volume
/*!
*	Created : \n
*	Last modification: 5-Dec-05\n
*	Called by:\n
*	-# CTreeAnalyserDlg::EstimateLeafArea()
*/
void CTreeAnalyserDlg::CalculDirections4(CEachImage *image,int gx,int gy,BOOL is_division,int sx, int sy)
{ 
	vctDirection.clear();

	CImage ctrlImage;
	// Load the selected bitmap file
	HRESULT loadresule= ctrlImage.Load(image->GetFileName());
	if (FAILED(loadresule))
	{
		ms.Format("Error reading file '%s'\r\nSelected file may not existed or not be the bitmap file.",image->GetFileName());
		AddDisplayText(ms);
		goto skipimage;
	}
	else //if load image OK
	{

//Start to calcuate the directions that intersect the canopy --------------------------------------------
		//calculate total number of directions
		int nx=0,ny=0;
		vctDirection.clear(); //delete all previous directions
		if (mod(ctrlImage.GetWidth(),double(gx))==0)
		{
			nx=ctrlImage.GetWidth()/gx;
		}
		else {nx=(ctrlImage.GetWidth()/gx)+1;}

		if (mod(ctrlImage.GetHeight(),double(gy))==0)
		{
			ny=ctrlImage.GetHeight()/gy;
		}
		else {ny=(ctrlImage.GetHeight()/gy)+1;}

		//Start loop for each beam direction
		int i,j;
		int opercent=-1;
		double unitpercent=double(nx)/100;
		double all_intersected_volume=0;
		for (i=1;i<=nx;i++)
		{
			int percent=int(i/unitpercent);
			if(percent>opercent)
			{
				ms.Format("Analysing intercepted directions on image '%s'  %i%%",image->GetPictureName(),percent);
				DisplayStatus(ms);
				opercent=percent;
			}

			for (j=1;j<=ny;j++)
			{
				vp1->ResetVoxelPathLength(); //add for LAD

				CDirection tempdir; //create temporary direction

				//find the middle pixel
				double px=(i-1)*gx+double(gx)/2;
				double py=(j-1)*gy+double(gy)/2;

				image->CalculBasic();

				double current_zone_gapfraction=this->GetGapFraction(&ctrlImage,px,py,gx,gy);
/*
//-------------- Add for testing calculation of gap fraction -----------------------------------------------------
						FILE * pFile;
						pFile=fopen("d:\\z\\manuel\\gap_ta120.txt","at");
						fprintf(pFile,"%6.4f\t%6.4f\t%i\t%i\t%6.4f\n",px,py,gx,gy,current_zone_gapfraction);
						fclose(pFile);
//-------------- End add for testing calculation of gap fraction -----------------------------------------------------
*/

				if(current_zone_gapfraction<1)
				{
					//drawing zone
					if(ny<50 && nx<50){DrawZone(int(px),int(py),gx,gy);}
					
					int n_division_x=1,n_division_y=1; //if no subdivision then 1x1 is default

					if(pImagePool1->is_divide_pza==TRUE)
					{//if have sub division
						n_division_x=pImagePool1->division_vertical;
						n_division_y=pImagePool1->division_horizontal;
					}

					//find the size of sub division
					double dsx=double(gx)/double(n_division_x); //size of sub division
					double dsy=double(gy)/double(n_division_y);//size of sub division

					for (int snx=0;snx<n_division_x;snx++)
					{
						for (int sny=0;sny<n_division_y;sny++)
						{
							//find the middle pixel for current subdivision
							double subdivided_px=(i-1)*gx+snx*dsx+dsx/2;
							double subdivided_py=(j-1)*gy+sny*dsy+dsy/2;

							tempdir.gapfraction=current_zone_gapfraction;
							tempdir.unitDirection=image->CalculUnitDirection(subdivided_px,subdivided_py);
							tempdir.px=subdivided_px;
							tempdir.py=subdivided_py;

							//calcualte angle of current beam
							double upper_pixel_x=subdivided_px+(dsx/2.0);
							double lower_pixel_x=subdivided_px-(dsx/2.0);

							cPoint upper_dir_x=image->CalculUnitDirection(upper_pixel_x,subdivided_py);
							cPoint lower_dir_x=image->CalculUnitDirection(lower_pixel_x,subdivided_py);
							double angle_x=acos(abs(upper_dir_x*lower_dir_x)); //aa is dot product or cosbeta
							tempdir.angle_x=angle_x;

							double upper_pixel_y=subdivided_py+(dsy/2.0);
							double lower_pixel_y=subdivided_py-(dsy/2.0);
							cPoint upper_dir_y=image->CalculUnitDirection(subdivided_px,upper_pixel_y);
							cPoint lower_dir_y=image->CalculUnitDirection(subdivided_px,lower_pixel_y);
							double angle_y=acos(abs(upper_dir_y*lower_dir_y)); //aa is dot product or cosbeta
							tempdir.angle_y=angle_y;

							//calculate path length
							double sum_volume=0,this_path_length=0;
							double t1=0,t2=0;
							cPoint cam_cor=image->CameraCoordinate;
							cPoint beam_dir=image->CalculUnitDirection(subdivided_px,subdivided_py);

							if(vp1->RayCanopyIntersect(&cam_cor,&beam_dir,&t1,&t2))
							{//check if the ray intersect the bounding box

								for(unsigned int icell=0;icell<vp1->GetTotal_cells();icell++)
								{
									t1=0;t2=0;
									short int ans=vp1->RayCellIntersect(&cam_cor,&beam_dir,&vp1->acell[icell],&t1,&t2);
									if(ans==1) //if beam intersect this cell
									{	
										double dpx1=0,dpy1=0,dpx2=0,dpy2=0;
										//calculate cross section area for this cell
										dpx1=t1*tan(angle_x/2)*2;
										dpy1=t1*tan(angle_y/2)*2;
										dpx2=t2*tan(angle_x/2)*2;
										dpy2=t2*tan(angle_y/2)*2;
										double cross_area=(dpx1*dpy1+dpx2*dpy2)/2;

										//calculate volume and pathlength for this intersect
										double intersect_volume=cross_area*abs(t2-t1);

										vp1->acell[icell].pathLength=abs(t2-t1); // add for LAD

										sum_volume+=intersect_volume;
										this_path_length+=abs(t2-t1);
										vp1->acell[icell].intersect_volume+=intersect_volume;
									}//if(ans==1)
								}//for(unsigned int icell=0;icell<vp1->GetTotal_cells();icell++)
								tempdir.beam_volume=sum_volume;
								tempdir.PathLength=this_path_length;
								tempdir.section_area=image->CalculSectionAreaRectangle(gx,gy,px,py);
							} //if(vp1->RayCanopyIntersect(&image->CameraCoordinate,&tempdir.unitDirection,&t1,&t2))
							if(sum_volume>0){vctDirection.push_back(tempdir);}
							
							all_intersected_volume+=sum_volume;

						}//for (int sny=0;sny<pImagePool1->division_horizontal;sny++)

					}//for (int snx=0;snx<pImagePool1->division_vertical;snx++)

				}//if(current_zone_gapfraction<1)

			} //for (j=1;j<=ny;j++)

		} //for (i=1;i<=nx;i++)

		//ms.Format("Sum of intersected volume=%6.4f",all_intersected_volume);
		//AddDisplayText(ms);

		ctrlImage.Destroy(); //detach and destroy bitmap

     } //else //if load image OK
skipimage:;
}//void CTreeAnalyserDlg::CalculDirections4(CEachImage *image,int gx,int gy)

double CTreeAnalyserDlg::GetGapFraction(CImage *ctrlImage,double px, double py, int gx, int gy)
{//compute gap fraction of specified zone
	//px and py is middle point (type double) of specified zone
	//gx and gy is zone size in pixel
	int startx=int(px)-gx/2;
	if (startx<0){startx=0;}
	
	int endx=int(px)+gx/2;
	if(endx>ctrlImage->GetWidth()){endx=ctrlImage->GetWidth();}

	int starty=int(py)-gy/2;
	if (starty<0){starty=0;}

	int endy=int(py)+gy/2;
	if(endy>ctrlImage->GetHeight()){endy=ctrlImage->GetHeight();}
	
	int sumblack=0;
	for (int i=startx;i<endx;i++)
	{
		for(int j=starty;j<endy;j++)
		{
			if(ctrlImage->GetPixel(i,j)==0){sumblack++;}
		}//for(int j=starty;j<endy;j++)
	}//for (int i=startx;i<endx;i++)
	double total_pixel=(endx-startx)*(endy-starty);
	double gapfraction=1-sumblack/total_pixel;

	return gapfraction;
}

void CTreeAnalyserDlg::OnCanopyComparecanopy()
{
	CDlgCompareCanopy dlg;
	dlg.CurrentPath=this->CurrentPath;
	if (dlg.DoModal()==IDOK)
	{
		CompareCanopy(dlg.file_canopy1,dlg.file_canopy2);
	}

}

void CTreeAnalyserDlg::CompareCanopy(CString fname1,CString fname2)
{
	//----- Step1 open canopy file
	cVirtualPlant *vp_1=new cVirtualPlant(&m_statusBar,&m_EditOutput);
	this->OpenCanopy(fname1,vp_1);

	cVirtualPlant *vp_2=new cVirtualPlant(&m_statusBar,&m_EditOutput);
	this->OpenCanopy(fname2,vp_2);
	
	MatchedVoxel match_location,match_complete;

	std::vector<MatchedVoxel> vctMatchLocation;
	std::vector<MatchedVoxel> vctMatchComplete;

	//cVirtualPlant *vp_match_complete=new cVirtualPlant(&m_statusBar,&m_EditOutput);
	//cVirtualPlant *vp_match_location=new cVirtualPlant(&m_statusBar,&m_EditOutput);


	//----- Step2 Display informations
	ms.Format("\n\tCanopy1\tCanopy2\n");
	AddDisplayText(ms);
	ms.Format("Voxels\t%i\t%i\tcm",vp_1->GetTotal_cells(),vp_2->GetTotal_cells());
	AddDisplayText(ms);
	ms.Format("Volume\t%6.2f\t%6.2f\tm2",vp_1->GetCanopyVolume(),vp_2->GetCanopyVolume());
	AddDisplayText(ms);
	ms.Format("dx\t%6.2f\t%6.2f\tcm",vp_1->Getdx(),vp_2->Getdx());
	AddDisplayText(ms);
	ms.Format("dy\t%6.2f\t%6.2f\tcm",vp_1->Getdy(),vp_2->Getdy());
	AddDisplayText(ms);
	ms.Format("dz\t%6.2f\t%6.2f\tcm",vp_1->Getdz(),vp_2->Getdz());
	AddDisplayText(ms);

	//----- Step3 compare voxels size
	if(vp_1->Getdx()!=vp_2->Getdx()||vp_1->Getdy()!=vp_2->Getdy()||vp_1->Getdz()!=vp_2->Getdz())
	{
		AddDisplayText("Can not compare the canopy because the voxel size is not the same");
		goto end_compare;
	}

	//----check if there are some repeated cell?


	//----- Step4 compare location
	//int complete_match=0,location_match=0;
	int n1=vp_1->GetTotal_cells(),n2=vp_2->GetTotal_cells();
	int number_match=0;
	double floating_error=vp_1->Getdx()*1e-6;

	for(int i=0;i<n1;i++)
	{
		match_location.cell1i=i;
		match_location.cell2i=-1;
		int match_found=0;
		for(int j=0;j<n2;j++)
		{
			if((vp_1->acell[i].GetX()>vp_2->acell[j].GetX()-floating_error) && (vp_1->acell[i].GetX()<vp_2->acell[j].GetX()+floating_error))
			{
				if((vp_1->acell[i].GetY()>vp_2->acell[j].GetY()-floating_error) && (vp_1->acell[i].GetY()<vp_2->acell[j].GetY()+floating_error) )
				{
					if((vp_1->acell[i].GetZ()>vp_2->acell[j].GetZ()-floating_error) && (vp_1->acell[i].GetZ()<vp_2->acell[j].GetZ()+floating_error))
					{
						match_location.cell2i=j;
						match_found=1;
						number_match++;
						vctMatchLocation.push_back(match_location);
						if(vp_1->acell[i].GetSumSurface()==vp_2->acell[j].GetSumSurface())
						{
							match_complete.cell1i=i;
							match_complete.cell2i=j;
							vctMatchComplete.push_back(match_complete);							
						}
					}
				}
			}
		}//for(int j=0;j<n2;j++)
		if(!match_found){vctMatchLocation.push_back(match_location);}
	}//for(int i=0;i<n1;i++)

	//----- Step5 Export matched canopy
	FILE * pFile;
	AddDisplayText("");
	ms.Format("Location match %i voxels = %6.2f %%byCanopy1\n",number_match,number_match/double(n1)*100);
	AddDisplayText(ms);
	ms.Format("Location match %i voxels = %6.2f %%byCanopy2\n",number_match,number_match/double(n2)*100);
	AddDisplayText(ms);

	//Export matched location
	int j=0,k=0, nsize=vctMatchLocation.size();
	if(nsize>0){
		DisplayStatus("Exporting location matching....");
		ms.Format("Export matching location to %s",this->CurrentPath + "match_location.txt");
		AddDisplayText(ms);
		DisplayStatus("Exporting location matching....");
		pFile=fopen(this->CurrentPath + "match_location.txt","wt");
		fprintf(pFile,"Canopy1=%s\n",fname1);
		fprintf(pFile,"Canopy2=%s\n",fname2);

		fprintf(pFile,"\t\tCanopy1\t\t\t\t\t\tCanopy2\n");
		fprintf(pFile,"ID\tx\ty\tz\tleaf_area\t\tID\tx\ty\tz\tleaf_area\n");
		for(int i=0;i<nsize;i++)
		{
			j=vctMatchLocation[i].cell1i;
			k=vctMatchLocation[i].cell2i;
			fprintf(pFile,"%i\t%6.4f\t%6.4f\t%6.4f\t%10.8f\t\t",vp_1->acell[j].ID,vp_1->acell[j].GetX(),vp_1->acell[j].GetY(),vp_1->acell[j].GetZ(),vp_1->acell[j].sum_surface);
			if(k==-1)
			{
				fprintf(pFile,"0\t0\t0\t0\t0\n");
			}
			else {fprintf(pFile,"%i\t%6.4f\t%6.4f\t%6.4f\t%10.8f\n",vp_2->acell[k].ID,vp_2->acell[k].GetX(),vp_2->acell[k].GetY(),vp_2->acell[k].GetZ(),vp_2->acell[k].sum_surface);}
		}
		fclose(pFile);
	}

	//Export matched complete
	AddDisplayText("");
	ms.Format("Complete match %i voxels = %6.2f %%byCanopy1\n",vctMatchComplete.size(),vctMatchComplete.size()/double(n1)*100);
	AddDisplayText(ms);
	ms.Format("Complete match %i voxels = %6.2f %%byCanopy2\n",vctMatchComplete.size(),vctMatchComplete.size()/double(n2)*100);
	AddDisplayText(ms);

	j=0,k=0, nsize=vctMatchComplete.size();
	if(nsize>0){
		ms.Format("Export matching complete to %s",this->CurrentPath + "match_complete.txt");
		AddDisplayText(ms);
		DisplayStatus("Exporting complete matching....");
		pFile=fopen(this->CurrentPath + "match_complete.txt","wt");
		fprintf(pFile,"Canopy1=%s\n",fname1);
		fprintf(pFile,"Canopy2=%s\n",fname2);

		fprintf(pFile,"\t\tCanopy1\t\t\t\t\t\tCanopy2\n");
		fprintf(pFile,"ID\tx\ty\tz\tleaf_area\t\tID\tx\ty\tz\tleaf_area\n");
		for(int i=0;i<nsize;i++)
		{
			j=vctMatchComplete[i].cell1i;
			k=vctMatchComplete[i].cell2i;
			fprintf(pFile,"%i\t%6.4f\t%6.4f\t%6.4f\t%10.8f\t\t",vp_1->acell[j].ID,vp_1->acell[j].GetX(),vp_1->acell[j].GetY(),vp_1->acell[j].GetZ(),vp_1->acell[j].sum_surface);
			fprintf(pFile,"%i\t%6.4f\t%6.4f\t%6.4f\t%10.8f\n",vp_2->acell[k].ID,vp_2->acell[k].GetX(),vp_2->acell[k].GetY(),vp_2->acell[k].GetZ(),vp_2->acell[k].sum_surface);
		}
		fclose(pFile);
	}
end_compare:;
	//Last step clear memory
	delete vp_1;
	delete vp_2;
	DisplayStatus();
}//void CTreeAnalyserDlg::CompareCanopy(CString fname1,CString fname2)

void CTreeAnalyserDlg::OnCanopyExportvegestarcanopy()
{
	int ncell=vp1->GetTotal_cells();
	if (ncell==0){MessageBox("No data to save!");}
	else
	{

		CString filename;
		static char BASED_CODE szFilter[] = "VegeSTAR file (*.vgx)|*.vgx||";
		filename=this->GetRootName(this->ActiveProjectFilename)+".vgx";
		CFileDialog m_ldFile(FALSE, ".vgx", filename,
			OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
		if (m_ldFile.DoModal() == IDOK)
		{
			filename = m_ldFile.GetPathName();
			vp1->ExportCellVGS(filename,2,7); 
			ms.Format("Export canopy to VegeSTAR file %s",filename);
			AddDisplayText(ms);
			DisplayStatus();
		}
	}

}//void CTreeAnalyserDlg::OnCanopyExportvegestarcanopy()


void CTreeAnalyserDlg::OnCanopyVerticalprofileofleafarea()
{
	if(!vp1->GetTotal_cells()==0)
	{/*
		CInputDlg dlg;
		dlg.DlgTitle="Compute vertical profile of leaf area";
		dlg.InputLabelText="Thickness";
		dlg.m_input=vp1->Getdx()*100;
		dlg.UnitText="cm.";

		if (dlg.DoModal()==IDOK)
		{*/
			vp1->CalculVerticalProfileLeafArea();
			//int a=1;
	}
	else //if no voxel
	{
		MessageBox("No canopy to work");
	}

}//void CTreeAnalyserDlg::OnCanopyVerticalprofileofleafarea()


//! The function to calculate leaf area with included computation of LAD distribution (share method)
/*!
*	Created : \n
*	Last modification: 13-Oct-04\n
*	Called by:\n
*	-# CTreeAnalyserDlg::AnalyseAll()
*	-# CTreeAnalyserDlg::ThreadWork(LPVOID pParam)
*/
void CTreeAnalyserDlg::EstimateLeafArea(void)
{//Calculate leaf area and vertical profile
	vctDirection.clear();
	int in_cell=vp1->GetTotal_cells();

	cVirtualPlant *vp2=new cVirtualPlant(&m_statusBar,&m_EditOutput);
	if(pImagePool1->ifCalculVprofile==1)
	{
		// Prepare vp2 for the average canopy
		vp2->PutDxyz(vp1->Getdx(),vp1->Getdy(),vp1->Getdz());
		for(int i=0;i<in_cell;i++)
		{
			vp2->acell.push_back(vp1->GetCell(i));
		}//for(i=0;i<vp1->GetTotal_cells();i++)
		vp2->PutNxyz(vp1->GetNx(),vp1->GetNy(),vp1->GetNz());
		vp2->CalculBasic();
		vp2->ClearSumsurface();
	}	

	if(in_cell==0){MessageBox("Virtual canopy is not reconstructed.\r\nPlease reconstruct canopy volume before or open saved canopy.");goto end_process;}
	if(pImagePool1->GetImageCount()==0){MessageBox("No image for working.");goto end_process;}
	else // if have cell to work
	{
		//-------- initializing time used ---------------
		time_t   start, finish;
		double   elapsed_time;   
		time( &start );
		//-------- end initializing time used ---------------

		DisplayStatus("Starting calculation of leaf area....");
		AddDisplayText(" ");
		ms.Format("******************** Estimation of Leaf Area. ********************");
		AddDisplayText(ms);		

		if(pImagePool1->m_inversion_method==0){ms.Format("Using Positive Binomial Model");} 
		if(pImagePool1->m_inversion_method==1){ms.Format("Using Beer's Law");} 
		//if(pImagePool1->m_inversion_method==1){ms.Format("Using Negative Binomial Model");} 
		AddDisplayText(ms);

		ms.Format("Leaf angle distribution = %s",pImagePool1->GetLeafDistributionName());
		AddDisplayText(ms);
		if(pImagePool1->m_leaf_distribution==0)
		{
			ms.Format("Mean Leaf angle=%6.2f",pImagePool1->m_mean_leaf_inc);
			AddDisplayText(ms);	
		}

		if(pImagePool1->m_leaf_distribution==7)
		{
			ms.Format("Leaf angle distribution file='%s'",pImagePool1->LeafDistributionFile);
			AddDisplayText(ms);	
			if(!pImagePool1->ReadLeafDis(pImagePool1->LeafDistributionFile,pImagePool1->ActivePath))
			{
				AddDisplayText("Unable to read leaf angle distirbution file!\r\nEstimation of leaf area stop by error.");
				goto end_process;
			}
		}
	

		//write header of log file
		bool error_open_log=0;
		if(pImagePool1->m_iflogvolume==TRUE)
		{
			FILE * LogFileStream;
			if((LogFileStream=fopen(this->GetLogFilename(),"at"))==NULL)
			{
				ms.Format("Error open log file '%s'. \nThis file may be opening.",this->GetLogFilename());
				MessageBox(ms);
				AddDisplayText("Error! Can not write to log file");
				error_open_log=1;
			}
			else {
			fprintf(LogFileStream,"ImageNo\tName\tDirection\tEle\tdx\tdy\tdz\tCanopyVolume\tZeroGapVol\tFullGapVol\tGapX\tGapY\tGapMean\tGapVariance\t%%FullGap\t%%ZeroGap\tminGap\tModel\tLeafDistribution\tEstimatedLA\tMLAD\tVLAD\tMLAD_weight\tVLAD_weight\tLAD_max\tZeroGapLeafArea\tSamplingArea(cm2)\tMid_G\tMeanLeafSize(cm2)\n");
			fclose(LogFileStream);
			}
		}


		UpdateCheckItem();
	

		//do ray-box intersection for each image
		int workcount=0;
		double minLA=9e9,maxLA=-9e9,sumLA=0,sumLA2=0;
		if (pImagePool1->m_leaf_distribution==7){pImagePool1->ReadLeafDis(pImagePool1->LeafDistributionFile,this->CurrentPath);}
		for (int pic=0;pic<pImagePool1->GetImageCount();pic++)
		{
			CEachImage *tempImage=pImagePool1->GetImage(pic);

			if (tempImage->GetFileName()!="empty" && tempImage->GetCheck()==1) //do only selected image
			{//if image is selected then do it
				workcount++; //count number of working picture
				double sum_leaf_area=0,sum_beam_volume=0,sum_leaf_area_nogap=0;
				double sumLAD=0,sumLAD2=0,VLAD=0,LAD_max=0;
				double sumLADW=0,VLADW=0;
				double zero_gap_vol=0,full_gap_vol=0;
				int dirsize=0;
				int zone_size_x=0,zone_size_y=0;
				double mean_sampling_area=0, mid_G=0;

				//variables for gap fraction
				double meanGap=0,VGap=0;
				int zero_gap=0,full_gap=0;

				//step 1 detect optimal zoning size
				if(pImagePool1->gap_option==0)
				{
					zone_size_x=pImagePool1->CalculOptimalZoneSize(tempImage);
					zone_size_y=zone_size_x;
				}
				else if(pImagePool1->gap_option==1)
				{
					zone_size_x=pImagePool1->gpx;
					zone_size_y=pImagePool1->gpy;
				}
				else
				{
					zone_size_x=pImagePool1->CalculGivenZoneSize(tempImage,pImagePool1->fix_pza);
					zone_size_y=zone_size_x;
				}
				mean_sampling_area=pImagePool1->CalculSamplingArea(tempImage,zone_size_x,zone_size_y);
				mid_G=pImagePool1->CalculG(tempImage->Get_cam_ele_deg());

				//step 2 inversion
	//--------------------------Begin Positive or Negative Binomial -------------------------------------------
				m_Tree1.SelectItem(vctTreeItem[pic]);

				ms.Format("\r\nEstimation of leaf area on picture='%s'",tempImage->GetPictureName());
				AddDisplayText(ms);

				if(pImagePool1->gap_option==0)
				{
					if(pImagePool1->is_divide_pza==1)
					{
						ms.Format("Automatic gap fraction; size=%ix%i pixels with subdivision %iC x %iR",zone_size_x,zone_size_y,pImagePool1->division_vertical,pImagePool1->division_horizontal);
					}
					else
					{
						ms.Format("Automatic gap fraction; size=%ix%i pixels",zone_size_x,zone_size_y);
					}
				}
				else if(pImagePool1->gap_option==1)
				{
					ms.Format("Custom gap fraction; size=%ix%i pixels",zone_size_x,zone_size_y);
				}
				else
				{
					ms.Format("Fixed PZA=%2.2f; Gap fraction size=%ix%i pixels",pImagePool1->fix_pza,zone_size_x,zone_size_y);
				}

				AddDisplayText(ms);		


				tempImage->CalculBasic();

				CalculDirections4(tempImage,zone_size_x,zone_size_y,pImagePool1->is_divide_pza,pImagePool1->division_vertical,pImagePool1->division_horizontal);

				for(unsigned int i=0;i<vp1->GetTotal_cells();i++)
				{
					vp1->acell[i].intersect_volume=0;
				}

				dirsize=vctDirection.size();

		//-------------- Start calculate average gap and variance ----------------------------------------
				double sumGap=0,sumGap2=0,sum_vol=0;
				for(int gapdir=0;gapdir<dirsize;gapdir++)
				{	
					if(vctDirection[gapdir].gapfraction==0){zero_gap++;zero_gap_vol+=vctDirection[gapdir].beam_volume;}
					if(vctDirection[gapdir].gapfraction==1){full_gap++;full_gap_vol+=vctDirection[gapdir].beam_volume;}
					sumGap+=vctDirection[gapdir].gapfraction;
					sumGap2+=pow(vctDirection[gapdir].gapfraction,2);
					sum_vol+=vctDirection[gapdir].beam_volume;
				}
				meanGap=sumGap/vctDirection.size();
				VGap=sumGap2/vctDirection.size()-pow(meanGap,2);
				double LacunarityVGap=(VGap/pow(meanGap,2))+1;
				//display information
				ms.Format("Total directions=%i  Zero gap=%6.4f%%  Full gap=%6.4f%%",dirsize,double(zero_gap)/dirsize,double(full_gap)/dirsize);
				this->AddDisplayText(ms);
				ms.Format("Average gap fraction=%6.4f  Variance of gap fraction=%6.4f Lacunarity of gap fraction=%6.4f",meanGap,VGap,LacunarityVGap);
				this->AddDisplayText(ms);
		//-------------- End calculate average gap and variance ----------------------------------------


				double unitpercent=double(dirsize)/100;
				int opercent=-1;
				
				for (int l=0;l<dirsize;l++) //do every intercepted directions
				{
					int percent=int(l/unitpercent);
					if(percent>opercent)
					{
						ms.Format("Computing total leaf area on image '%s'  %i%%",tempImage->GetPictureName(),percent);
						DisplayStatus(ms);
						opercent=percent;
					}//if(percent>opercent)

					//calculate G for this direction= function of beam direction
					double G=pImagePool1->CalculG(vctDirection[l].CalculNormalInc()*180/pii);
					

					//calculate leaf area density from gap fraction, G and path length
					double u=0,a=0,b=1;
					double sampling_area=vctDirection[l].section_area;
					double MeanLeafArea=pImagePool1->GetMeanLeafArea();
					double gap=vctDirection[l].gapfraction;
					if(gap==0){gap=pImagePool1->minimumGap;}
					a=MeanLeafArea*log(gap);

			// Start positive binomial-----------------------------------------------------------
					bool iftoosmallsampling=0;
					if(pImagePool1->m_inversion_method==0)
					{//positive binomial
						//autodetect clumping (not finish)
						if(pImagePool1->m_leaf_dispersion_option==0)
						{
							b=vctDirection[l].section_area*vctDirection[l].PathLength*log(1-(pImagePool1->GetMeanLeafArea()*G)/vctDirection[l].section_area);
						} 
						
						//for random dispersion
						if(pImagePool1->m_leaf_dispersion_option==1)
						{
							if(MeanLeafArea*G>sampling_area)
							{
								this->AddDisplayText("Error! Sampling area is too small for binomial law");
								iftoosmallsampling=1;
								//b=0.001;
							}
							else 
							{
								b=vctDirection[l].section_area*vctDirection[l].PathLength*log(1-(pImagePool1->GetMeanLeafArea()*G)/vctDirection[l].section_area);
							}
						}

						//for custom dispersion (not finish)
						if(pImagePool1->m_leaf_dispersion_option==2)
						{
							b=vctDirection[l].section_area*vctDirection[l].PathLength*log(1-(pImagePool1->GetMeanLeafArea()*G)/vctDirection[l].section_area);
						}
					}//if(pImagePool1->m_inversion_method==0)
			// End positive binomial-----------------------------------------------------------

					if(pImagePool1->m_inversion_method==1 || iftoosmallsampling==1)
					{//Beer's law
						a=log(gap);
						if(pImagePool1->m_leaf_dispersion_option==0){b=(G*vctDirection[l].PathLength*-1);} //autodetect clumping (not finish)
						if(pImagePool1->m_leaf_dispersion_option==1){b=(G*vctDirection[l].PathLength*-1);} //no clumping
						if(pImagePool1->m_leaf_dispersion_option==2){b=(G*vctDirection[l].PathLength*(-1)*pImagePool1->m_leaf_dispersion);}
					}

					if(b==0){u=0;}
					else {u=a/b;}

					if(pImagePool1->IfSetMaxLAD==1)
					{
						if(u>pImagePool1->MaxLAD){u=pImagePool1->MaxLAD;}
					}
					

					vctDirection[l].LAD=u;

					if(LAD_max<u){LAD_max=u;}

					//calcualte leaf area for each cell from path length
					sum_leaf_area+=u*vctDirection[l].beam_volume;
					sumLAD+=u;
					sumLAD2+=u*u;
					sumLADW+=u*vctDirection[l].beam_volume;
					sum_beam_volume+=vctDirection[l].beam_volume;
					if(vctDirection[l].gapfraction==0){sum_leaf_area_nogap+=u*vctDirection[l].beam_volume;}
				}//for (int l=0;l<dirsize;l++)
					
				//ms.Format("Sum beam volume=%6.4f",sum_beam_volume);
				//AddDisplayText(ms);

				DisplayStatus("Calculation of mean and variance....");
				if(minLA>sum_leaf_area){minLA=sum_leaf_area;}
				if(maxLA<sum_leaf_area){maxLA=sum_leaf_area;}
				sumLA+=sum_leaf_area;
				sumLA2+=pow(sum_leaf_area,2);

				//calculate variance of LAD
				double meanLAD=sumLAD/vctDirection.size();
				double meanLADW=sumLADW/sum_beam_volume;
				VLAD=sumLAD2/dirsize-pow(meanLAD,2);
				double VLADlacunarity=(VLAD/pow(meanLAD,2))+1;
					

				//writing display
				ms.Format("Canopy volume=%6.4f m3  Zero gap volume=%6.4f(%4.2f%%)   Full gap volume=%6.4f(%4.2f%%)",vp1->GetCanopyVolume(),zero_gap_vol,zero_gap_vol/vp1->GetCanopyVolume()*100,full_gap_vol,full_gap_vol/vp1->GetCanopyVolume()*100);
				this->AddDisplayText(ms);
				ms.Format("Mean LAD=%6.4f m2/m3  Maximum LAD=%6.4f m2/m3",meanLADW,LAD_max);
				this->AddDisplayText(ms);
				ms.Format("Variance LAD=%6.4f  Lacunarity=%6.4f",VLAD,VLADlacunarity);
				this->AddDisplayText(ms);
				ms.Format("Estimated leaf area=%6.4f m2  Zero-gap leaf area=%6.4f (%4.2f%%)\r\n",sum_leaf_area,sum_leaf_area_nogap,sum_leaf_area_nogap/sum_leaf_area*100);
				this->AddDisplayText(ms);

				//writing output
				ms.Format("Writing output to file '%s'",this->GetLogFilename());
				DisplayStatus(ms);
				if(pImagePool1->m_iflogvolume==TRUE && !error_open_log)
				{

					FILE * LogFileStream;
					LogFileStream=fopen(this->GetLogFilename(),"at");
					fprintf(LogFileStream,"%i\t%s\t%6.2f\t%6.2f\t",pic+1,tempImage->GetPictureName(),tempImage->GetCameraDirection(),tempImage->GetCameraInc);
					fprintf(LogFileStream,"%6.4f\t%6.4f\t%6.4f\t%6.4f\t%6.4f\t%6.4f\t",vp1->Getdx(),vp1->Getdy(),vp1->Getdz(),vp1->GetCanopyVolume(),zero_gap_vol,full_gap_vol);
					fprintf(LogFileStream,"%i\t%i\t",zone_size_x,zone_size_y);
					fprintf(LogFileStream,"%6.4f\t%6.4f\t%6.4f\t%6.4f\t%8.6f\t",meanGap,VGap,double(full_gap)/dirsize,double(zero_gap)/dirsize,pImagePool1->minimumGap);
					fprintf(LogFileStream,"%s\t%s\t",pImagePool1->GetModelName(),pImagePool1->GetLeafDistributionName());
					fprintf(LogFileStream,"%6.4f\t%6.4f\t%6.4f\t%6.4f\t%6.4f\t%6.4f\t%6.4f\t",sum_leaf_area,meanLAD,VLAD,meanLADW,VLADW,LAD_max,sum_leaf_area_nogap);
					fprintf(LogFileStream,"%6.4f\t%6.4f\t%6.4f\t",mean_sampling_area,mid_G,pImagePool1->m_mean_leaf_area);
					fprintf(LogFileStream,"%6.4f\n",sum_vol);
					fclose(LogFileStream);
			
				}


//----------------- Start computation of LAD by sharing leaf area along path length ------------
				if(pImagePool1->ifCalculVprofile==1)
				{		
					opercent=-1;
					vp1->ClearSumsurface(); //reset leaf area of every voxels to 0
					for (int l=0;l<dirsize;l++)
					{
						int percent=int(l/unitpercent);
						if(percent>opercent)
						{
							ms.Format("Computing LAD distribution on image '%s'  %i%%",tempImage->GetPictureName(),percent);
							DisplayStatus(ms);
							opercent=percent;
						}//if(percent>opercent)

						cPoint cam_cor=tempImage->CameraCoordinate;
						cPoint beam_dir=tempImage->CalculUnitDirection(vctDirection[l].px,vctDirection[l].py);
						double beam_leaf_area=vctDirection[l].LAD*vctDirection[l].beam_volume;
						double t1=0,t2=0;
						for(unsigned int i=0;i<vp1->GetTotal_cells();i++)
						{
							t1=0;t2=0;
							short int ans=vp1->RayCellIntersect(&cam_cor,&beam_dir,&vp1->acell[i],&t1,&t2);
							if(ans==1) //if beam intersect this cell
							{	
								double dpx1=0,dpy1=0,dpx2=0,dpy2=0;
								//calculate cross section area for this cell
								dpx1=t1*tan(vctDirection[l].angle_x/2)*2;
								dpy1=t1*tan(vctDirection[l].angle_y/2)*2;
								dpx2=t2*tan(vctDirection[l].angle_x/2)*2;
								dpy2=t2*tan(vctDirection[l].angle_y/2)*2;
								double cross_area=(dpx1*dpy1+dpx2*dpy2)/2;

								double a=vctDirection[l].angle_x;
								double b=vctDirection[l].angle_y;

								//calculate intersected volume
								double intersect_volume=cross_area*abs(t2-t1);
								double this_area=beam_leaf_area*(intersect_volume/vctDirection[l].beam_volume);
								vp1->acell[i].sum_surface+=this_area;

							}//if(ans==1)
						}//for(unsigned int i=0;i<vp1->GetTotal_cells();i++)
					}//for (int l=0;l<dirsize;l++)

					//update average leaf area in canopy2
					for(int i=0;i<in_cell;i++)
					{
						vp2->acell[i].sum_surface=(vp2->acell[i].sum_surface*(workcount-1)+vp1->acell[i].sum_surface)/workcount;
					}

					vp1->CalculVerticalProfileLeafArea();

					//ms.Format("%sLAD%i_%s.cpn",this->CurrentPath,workcount,tempImage->GetPictureName());
					//this->SaveCanopy(ms,vp1);
					//ms.Format("Save canopy to %sLAD%i_%s.cpn",this->CurrentPath,workcount,tempImage->GetPictureName());
					//AddDisplayText(ms);
				}
//----------------- End computation of LAD ----------------------------------------------

			}//if (tempImage->GetFileName()!="empty" && tempImage->GetCheck()==1)
			
		} //for (int pic=0;pic<pImagePool1->GetImageCount();pic++)
		
		AddDisplayText("----------------- Average ------------------");
		pImagePool1->EsimatedLeafArea=sumLA/workcount;
		double sd=sqrt((sumLA2-pow(sumLA,2)/workcount)/(workcount-1));
		double sderr=sd/sqrt(double(workcount));
		ms.Format("Leaf area average=%6.4f  Max=%6.4f  Min=%6.4f",pImagePool1->EsimatedLeafArea,maxLA,minLA);
		AddDisplayText(ms);
		ms.Format("Standard deviation=%6.4f   Standard error of mean=%6.4f",sd,sderr);
		AddDisplayText(ms);
		if(pImagePool1->ifCalculVprofile==1)
		{
			AddDisplayText("------- Vertical profile of average canopy ---------");
			vp2->CalculVerticalProfileLeafArea();
			//ms.Format("%sLAD_average.cpn",this->CurrentPath);
			//this->SaveCanopy(ms,vp2);
			//ms.Format("Average LAD of canopy was saved to %sLAD_average.cpn",this->CurrentPath);
			//AddDisplayText(ms);

			//copy average canopy to vp1
			for(unsigned int i=0;i<vp1->GetTotal_cells();i++){vp1->acell[i].sum_surface=vp2->acell[i].sum_surface;}
			vp1->DisplayCanopyProperties();
		}
		//----------- calculate time used ---------------------------
		time( &finish );
		elapsed_time = difftime( finish, start );
		ms.Format( "Estimation of leaf area using = %6.0f seconds.", elapsed_time );
			AddDisplayText(ms);

		char datebuf[128];
		struct tm *today;__time64_t ltime;
		_time64( &ltime );
		today = _localtime64( &ltime );
		strftime( datebuf, 128,"%d-%b-%y %H:%M:%S", today );

		if(pImagePool1->m_iflogvolume==TRUE && !error_open_log)
		{
			CString ms2;
			ms2.LoadString(IDS_VERSION);
			FILE * pFile;
			pFile=fopen(this->GetLogFilename(),"at");
			fprintf(pFile,"Mean=%6.4f\tMax=%6.4f\tMin=%6.4f\tSD=%6.4f\tSTDERR mean=%6.4f\n",pImagePool1->EsimatedLeafArea,maxLA,minLA,sd,sderr);
			fprintf(pFile,"Estimation of leaf area was done by %s on %s, using time = %i seconds.\n\n", ms2, datebuf,int(elapsed_time));
			fclose(pFile);
			ms.Format("Log output to => %s",this->GetLogFilename());
			AddDisplayText(ms);
		}
		if(error_open_log){AddDisplayText("!Unable to write the log file!");}
		//----------- calculate time used ---------------------------
	}
end_process:;
	vctDirection.clear();
	DisplayStatus();
	DisplaySettingProperties();
	delete vp2;

} //void CTreeAnalyserDlg::EstimateLeafArea(void)


void CTreeAnalyserDlg::OnCanopyExportratpvoxels()
{
	CString filename;
	static char BASED_CODE szFilter[] = "Tree Analyser Canopy file (*.rav)|*.rav||";
	filename=CurrentPath + "leafarea.rav";
	CFileDialog m_ldFile(FALSE, ".rav", filename,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	if (m_ldFile.DoModal() == IDOK)
	{
		filename = m_ldFile.GetPathName();
		CurrentPath=m_ldFile.GetPathName();
		CurrentPath=CurrentPath.Left(CurrentPath.GetLength()-m_ldFile.GetFileName().GetLength());
		ms.Format("------------ Export RATP voxels to file --------------");
		AddDisplayText(ms);
		ms.Format("Output to %s",filename);
		AddDisplayText(ms);

		vp1->ExportRATPvoxel(filename);
		filename=this->CurrentPath + "grid3D.rav";
		ms.Format("Save grid parameters to %s",filename);
		AddDisplayText(ms);
		vp1->SaveGridParameters(filename);

	}
}

int CTreeAnalyserDlg::CalculDirections6(CEachImage *image,int gx,int gy,CString *fname,bool model_option)
{ //Use for exporting data to Python both beer and binomial model
	//model_option 0=Beer's  1=Binomial

	vctDirection.clear();
	int linecount=0;
	CImage ctrlImage;
	// Load the selected bitmap file
	HRESULT loadresule= ctrlImage.Load(image->GetFileName());
	if (FAILED(loadresule))
	{
		ms.Format("Error reading file '%s'\r\nSelected file may not existed or not be the bitmap file.",image->GetFileName());
		AddDisplayText(ms);
		goto skipimage;
	}
	else //if load image OK
	{

//Start to calcuate the directions that intersect the canopy --------------------------------------------
		//calculate total number of directions
		int nx=0,ny=0;
		vctDirection.clear(); //delete all previous directions
		if (mod(ctrlImage.GetWidth(),gx)==0)
		{
			nx=ctrlImage.GetWidth()/gx;
		}
		else {nx=(ctrlImage.GetWidth()/gx)+1;}

		if (mod(ctrlImage.GetHeight(),gy)==0)
		{
			ny=ctrlImage.GetHeight()/gy;
		}
		else {ny=(ctrlImage.GetHeight()/gy)+1;}


		//ms.Format("gx=%i  gy=%i  nx=%i  ny=%i",gy,gy,nx,ny);
		//AddDisplayText(ms);

		//int totalfound=0;
		int i,j;
		int opercent=-1;
		double unitpercent=double(nx)/100;
		for (i=1;i<=nx;i++)
		{
			int percent=int(i/unitpercent);
			if(percent>opercent)
			{
				ms.Format("Analysing intercepted directions on image '%s'  %i%%",image->GetPictureName(),percent);
				DisplayStatus(ms);
				opercent=percent;
			}

			for (j=1;j<=ny;j++)
			{
				vp1->ResetVoxelPathLength(); //add for LAD

				CDirection tempdir; //create temporaly directory

				//find the middle pixel
				double px=(i-1)*gx+gx/2;
				double py=(j-1)*gy+gy/2;
				image->CalculBasic();
				tempdir.unitDirection=image->CalculUnitDirection(px,py);
				tempdir.px=px;
				tempdir.py=py;
				//int total_cell=vp1->GetTotal_cells();
				
				//calcualte angle of current beam
				double upper_pixel_x=px+(gx/2.0);
				double lower_pixel_x=px-(gx/2.0);
				cPoint upper_dir_x=image->CalculUnitDirection(upper_pixel_x,py);
				cPoint lower_dir_x=image->CalculUnitDirection(lower_pixel_x,py);
				double angle_x=acos(abs(upper_dir_x*lower_dir_x)); //aa is dot product or cosbeta

				double upper_pixel_y=py+(gy/2.0);
				double lower_pixel_y=py-(gy/2.0);
				cPoint upper_dir_y=image->CalculUnitDirection(px,upper_pixel_y);
				cPoint lower_dir_y=image->CalculUnitDirection(px,lower_pixel_y);
				double angle_y=acos(abs(upper_dir_y*lower_dir_y)); //aa is dot product or cosbeta

				//calculate path length
				double sum_volume=0,this_path_length=0;
				double t1=0,t2=0;
				cPoint cam_cor=image->CameraCoordinate;
				cPoint beam_dir=image->CalculUnitDirection(px,py);
				if(vp1->RayCanopyIntersect(&cam_cor,&beam_dir,&t1,&t2))
					{//check if the ray intersect the bounding box
					for(unsigned int i=0;i<vp1->GetTotal_cells();i++)
					{
						t1=0;t2=0;
						short int ans=vp1->RayCellIntersect(&cam_cor,&beam_dir,&vp1->acell[i],&t1,&t2);
						if(ans==1) //if beam intersect this cell
						{	
							double dpx1=0,dpy1=0,dpx2=0,dpy2=0;
							//calculate cross section area for this cell
							dpx1=t1*tan(angle_x/2)*2;
							dpy1=t1*tan(angle_y/2)*2;
							dpx2=t2*tan(angle_x/2)*2;
							dpy2=t2*tan(angle_y/2)*2;
							double cross_area=(dpx1*dpy1+dpx2*dpy2)/2;

							//calculate volume and pathlength for this intersect
							double intersect_volume=cross_area*abs(t2-t1);

							vp1->acell[i].pathLength=abs(t2-t1); // add for LAD
							vp1->acell[i].section_area=cross_area;

							sum_volume+=intersect_volume;
							this_path_length+=abs(t2-t1);
							vp1->acell[i].intersect_volume+=intersect_volume;
						}//if(ans==1)
					}//for(unsigned int i=0;i<vp1->GetTotal_cells();i++)
					tempdir.beam_volume=sum_volume;
					tempdir.PathLength=this_path_length;
					//tempdir.section_area=sum_volume/this_path_length;
					int zone_width=0;if(gy<gx){zone_width=gy;}else{zone_width=gx;}
					tempdir.section_area=image->CalculSectionArea(zone_width,px,py);

//------------- Begin calculate gap fraction ----------------------------------------------------					
					int startx=int(px)-gx/2;
					if (startx<0){startx=0;}
					
					int endx=int(px)+gx/2;
					if(endx>ctrlImage.GetWidth()){endx=ctrlImage.GetWidth();}

					int starty=int(py)-gy/2;
					if (starty<0){starty=0;}

					int endy=int(py)+gy/2;
					if(endy>ctrlImage.GetHeight()){endy=endy>ctrlImage.GetHeight();}
					
					//ms.Format("x=%i-%i,y=%i-%i",startx,endx,starty,endy);
					//AfxMessageBox(ms);

					int sumblack=0;
					for (int i=startx;i<endx;i++)
					{
						for(int j=starty;j<endy;j++)
						{
							if(ctrlImage.GetPixel(i,j)==0){sumblack++;}
						}//for(int j=starty;j<endy;j++)
					}//for (int i=startx;i<endx;i++)
					double total_pixel=(endx-startx)*(endy-starty);
					double gapfraction=1-sumblack/total_pixel;
//------------- end calculate gap fraction ----------------------------------------------------
					//if(gapfraction==0){gapfraction=pImagePool1->minimumGap;}
					tempdir.gapfraction=gapfraction;

//-------------- add for LAD computation -----------------------------------------------------
					if(tempdir.gapfraction>=0 && tempdir.PathLength>0)
					{
						linecount++;
						double G=pImagePool1->CalculG(tempdir.CalculNormalInc()*180/pii);

						FILE * pFile;
						pFile=fopen(*fname,"at");

						fprintf(pFile,"%6.8f",tempdir.gapfraction);
						if(model_option==0)
						{//for Beer's
							for(unsigned int i=0;i<vp1->GetTotal_cells();i++)
							{
								fprintf(pFile,",%6.8f",vp1->acell[i].pathLength*G*-1);
							}
						}
						else
						{//for Binomial
							for(unsigned int i=0;i<vp1->GetTotal_cells();i++)
							{
								double a=pImagePool1->GetMeanLeafArea();
								double si=vp1->acell[i].section_area;
								double Li=vp1->acell[i].pathLength;
								double output=0;
								if(Li>0){output=log(1-(a*G/si))*si*Li/a;}
								fprintf(pFile,",%6.8f",output);
							}
						}
						fprintf(pFile,"\n");
						fclose(pFile);
					}

//-------------- add for LAD computation -----------------------------------------------------
				
				} //if(vp1->RayCanopyIntersect(&image->CameraCoordinate,&tempdir.unitDirection,&t1,&t2))
				if(sum_volume>0){vctDirection.push_back(tempdir);}
			} //for (j=1;j<=ny;j++)
		} //for (i=1;i<=nx;i++)
//End the calcuation of the directions that intersect the canopy --------------------------------------------
		ctrlImage.Destroy(); //detach and destroy bitmap
     } //else //if load image OK
skipimage:;
	 return linecount;
	 //else {MessageBox("Reading file error!\nSelected file may not be bitmap file.");}
}//void CTreeAnalyserDlg::CalculDirections6(CEachImage *image,int gx,int gy)

void CTreeAnalyserDlg::OnImageCalcualtegapfraction()
{
//-------- initializing time used ---------------
   time_t   start, finish;
   double   elapsed_time;   
   time( &start );
//-------- end initializing time used ---------------

	UpdateCheckItem();
	DisplayStatus("Calculate");
	AddDisplayText("");
	AddDisplayText("******************** Calculation of whole image gapfraction **********************");

	int workedPic=0;
	CImage ctrlImage;

	for(int pic=0;pic<pImagePool1->GetImageCount();pic++)
	{

		ms.Format("Calculation of whole image gapfraction on image %i of %i",pic+1,pImagePool1->GetImageCount());
		DisplayStatus(ms);
		CEachImage *tempImage=pImagePool1->GetImage(pic);
		AddDisplayText("");
		ms.Format("Image name = %s",tempImage->GetPictureName());
		AddDisplayText(ms);


		if (tempImage->GetFileName()!="empty" && tempImage->GetCheck()==1)
		{

			m_Tree1.SelectItem(vctTreeItem[pic]);

			 // Load the selected bitmap file
			HRESULT loadresule= ctrlImage.Load(tempImage->GetFileName());

			if (FAILED(loadresule))
			{
				ms.Format("Error reading file '%s'\r\nSelected file may not existed or not be the bitmap file.",tempImage->GetFileName());
				AddDisplayText(ms);
				goto skipimage;
			}
			else //if load image OK
			{
				
				//search for black pixel
				int sumblack=0,m_pixel_value;
				int totalpixel=ctrlImage.GetWidth()*ctrlImage.GetHeight();
				int percent=0,opercent=-1;
				double unitpercent=ctrlImage.GetWidth()/100.00;
				for(int i=0;i<ctrlImage.GetWidth();i++)//bm.bmWidth)
				{
					percent=int(i/unitpercent);
					if(percent>opercent)
					{
						ms.Format("Calculation of gap fraction image %i of %i  %i%%.",pic+1,pImagePool1->GetImageCount(),percent);
						DisplayStatus(ms);
						opercent=percent;
					}

					for(int j=0;j<ctrlImage.GetHeight();j++) //bm.bmHeight)
					{
						m_pixel_value = ctrlImage.GetPixel(i,j); //m_bmpCDC.GetPixel(i,j );
						if(m_pixel_value==CLR_INVALID){ms.Format("Error reading left pixel i=%i j=%i",i,j);MessageBox(ms);}
						if (m_pixel_value==0){sumblack++;}
					}
				}
				
				double gapfraction=double(sumblack)/totalpixel;

				ms.Format("Height=%i\tWidth=%i\tTotal pixel=%i",ctrlImage.GetWidth(),ctrlImage.GetHeight(),ctrlImage.GetWidth()*ctrlImage.GetHeight());
				AddDisplayText(ms);

				ms.Format("Black pixel=%i\tGap fraction = %6.4f",sumblack,gapfraction);
				AddDisplayText(ms);

				ctrlImage.Destroy(); //detach and destroy bitmap
				workedPic++;
			} //else //if load image OK
		}//if (tempImage->GetFileName()!="empty" && tempImage->GetCheck()==1)
		else {AddDisplayText("	Image skipped.");}
skipimage:;
	} //for(int pic=0;pic<pImagePool1->GetImageCount();pic++)


	DisplayStatus();

//----------- calculate time used ---------------------------
   time( &finish );
   elapsed_time = difftime( finish, start );
   ms.Format( "Calculation of gap fraction using = %6.0f seconds.", elapsed_time );
	AddDisplayText(ms);
//----------- calculate time used ---------------------------

}//void CTreeAnalyserDlg::OnImageCalcualtegapfraction()


/*
void CTreeAnalyserDlg::SolvingEquation()
{
	vctDirection.clear();
	int in_cell=vp1->GetTotal_cells();
	if(pImagePool1->GetImageCount()==0){MessageBox("No image for working.");goto end_process;}
	if(in_cell==0){MessageBox("No voxel for working.");goto end_process;}
	else // if have cell to work
	{

		//-------- initializing time used ---------------
		time_t   start, finish;
		double   elapsed_time;   
		time( &start );
		//-------- end initializing time used ---------------

		DisplayStatus("Exporting csv file of P0..........");
		AddDisplayText(" ");
		ms.Format("******************** Solving Linear Equation for Leaf Area. ********************");
		AddDisplayText(ms);		

		if(pImagePool1->m_inversion_method==0){ms.Format("Using Positive Binomial Model");} 
		if(pImagePool1->m_inversion_method==1){ms.Format("Using Beer's Law");} 
//		if(pImagePool1->m_inversion_method==1){ms.Format("Using Negative Binomial Model");} 
		AddDisplayText(ms);

		ms.Format("Leaf angle distribution = %s",pImagePool1->GetLeafDistributionName());
		AddDisplayText(ms);
		if(pImagePool1->m_leaf_distribution==0)
		{
			ms.Format("Mean Leaf angle=%6.2f",pImagePool1->m_mean_leaf_inc);
			AddDisplayText(ms);	
		}

		if(pImagePool1->m_leaf_distribution==7)
		{
			ms.Format("Leaf angle distribution file='%s'",pImagePool1->LeafDistributionFile);
			AddDisplayText(ms);	
			if(!pImagePool1->ReadLeafDis(pImagePool1->LeafDistributionFile,pImagePool1->ActivePath))
			{
				AddDisplayText("Unable to read leaf angle distirbution file!\r\nEstimation of leaf area stop by error.");
				goto end_process;
			}
		}
	
		UpdateCheckItem();

		//do ray-box intersection for each image
		CEquationPool *eqPool=new CEquationPool(&m_statusBar,&m_EditOutput,vp1->GetTotal_cells());
		int workcount=0;
		//double minLA=9e9,maxLA=-9e9,sumLA=0,sumLA2=0;
		if (pImagePool1->m_leaf_distribution==7){pImagePool1->ReadLeafDis(pImagePool1->LeafDistributionFile,this->CurrentPath);}
		for (int pic=0;pic<pImagePool1->GetImageCount();pic++)
		{
			CEachImage *tempImage=pImagePool1->GetImage(pic);

			if (tempImage->GetFileName()!="empty" && tempImage->GetCheck()==1) //do only selected image
			{//if image is selected then do it

				//declearation of variables
				//double angle_x=0,angle_y=0;
				//double dpx1=0,dpy1=0; //,dpx2=0,dpy2=0;
				//double sum_volume=0,sum_leaf_area=0,sum_beam_volume=0,sum_leaf_area_nogap=0;
				//double sumLAD=0,sumLAD2=0,VLAD=0,LAD_max=0;
				//double sumLADW=0,VLADW=0;
				//double zero_gap_vol=0,full_gap_vol=0;
				//int dirsize=0;
				int zone_size_x=0,zone_size_y=0;
				//variables for gap fraction
				//double meanGap=0,VGap=0;
				//int zero_gap=0,full_gap=0;

				//step 1 detect optimal zoning size
				if(pImagePool1->gap_option==0)
				{
					zone_size_x=pImagePool1->CalculOptimalZoneSize(tempImage);
					zone_size_y=zone_size_x;
				}
				else
				{
					zone_size_x=pImagePool1->gpx;
					zone_size_y=pImagePool1->gpy;
				}

				m_Tree1.SelectItem(vctTreeItem[pic]);

				workcount++;

				tempImage->CalculBasic();

				CalculDirections7(tempImage,zone_size_x,zone_size_y,eqPool);

				vctDirection.clear(); //clear memory of directions

			}//if (tempImage->GetFileName()!="empty" && tempImage->GetCheck()==1)
			
		} //for (int pic=0;pic<pImagePool1->GetImageCount();pic++)
		ms.Format("Total Equation=%i",eqPool->vctEquation.size());
		AddDisplayText(ms);
		eqPool->DisplayInformation();


		delete eqPool;

		//----------- calculate time used ---------------------------
		time( &finish );
		elapsed_time = difftime( finish, start );
		ms.Format( "Exporting csv file of P0 = %6.0f seconds.\n", elapsed_time );
			AddDisplayText(ms);
		//----------- calculate time used ---------------------------
	}
end_process:;
	vctDirection.clear();
	DisplayStatus();

}//void CTreeAnalyserDlg::SolvingEquation()
*/

/*
void CTreeAnalyserDlg::CalculDirections7(CEachImage *image,int gx,int gy, CEquationPool *eqPool)
{ //for solving equation
	vctDirection.clear();
	CImage ctrlImage;
	// Load the selected bitmap file
	HRESULT loadresule= ctrlImage.Load(image->GetFileName());
	if (FAILED(loadresule))
	{
		ms.Format("Error reading file '%s'\r\nSelected file may not existed or not be the bitmap file.",image->GetFileName());
		AddDisplayText(ms);
		goto skipimage;
	}
	else //if load image OK
	{

//Start to calcuate the directions that intersect the canopy --------------------------------------------
		//calculate total number of directions
		int nx=0,ny=0;
		vctDirection.clear(); //delete all previous directions
		if (mod(ctrlImage.GetWidth(),gx)==0)
		{
			nx=ctrlImage.GetWidth()/gx;
		}
		else {nx=(ctrlImage.GetWidth()/gx)+1;}

		if (mod(ctrlImage.GetHeight(),gy)==0)
		{
			ny=ctrlImage.GetHeight()/gy;
		}
		else {ny=(ctrlImage.GetHeight()/gy)+1;}

		//int totalfound=0;
		int i,j;
		int opercent=-1;
		double unitpercent=double(nx)/100;
		for (i=1;i<=nx;i++)
		{
			int percent=int(i/unitpercent);
			if(percent>opercent)
			{
				ms.Format("Analysing intercepted directions on image '%s'  %i%%",image->GetPictureName(),percent);
				DisplayStatus(ms);
				opercent=percent;
			}

			for (j=1;j<=ny;j++)
			{
				vp1->ResetVoxelPathLength(); //add for LAD

				CDirection tempdir; //create temporaly directory

				//find the middle pixel
				double px=(i-1)*gx+gx/2;
				double py=(j-1)*gy+gy/2;
				image->CalculBasic();
				tempdir.unitDirection=image->CalculUnitDirection(px,py);
				tempdir.px=px;
				tempdir.py=py;
				//int total_cell=vp1->GetTotal_cells();
				
				//calcualte angle of current beam
				double upper_pixel_x=px+(gx/2.0);
				double lower_pixel_x=px-(gx/2.0);
				cPoint upper_dir_x=image->CalculUnitDirection(upper_pixel_x,py);
				cPoint lower_dir_x=image->CalculUnitDirection(lower_pixel_x,py);
				double angle_x=acos(abs(upper_dir_x*lower_dir_x)); //aa is dot product or cosbeta

				double upper_pixel_y=py+(gy/2.0);
				double lower_pixel_y=py-(gy/2.0);
				cPoint upper_dir_y=image->CalculUnitDirection(px,upper_pixel_y);
				cPoint lower_dir_y=image->CalculUnitDirection(px,lower_pixel_y);
				double angle_y=acos(abs(upper_dir_y*lower_dir_y)); //aa is dot product or cosbeta

				//calculate path length
				double sum_volume=0,this_path_length=0;
				double t1=0,t2=0;
				cPoint cam_cor=image->CameraCoordinate;
				cPoint beam_dir=image->CalculUnitDirection(px,py);
				if(vp1->RayCanopyIntersect(&cam_cor,&beam_dir,&t1,&t2))
					{//check if the ray intersect the bounding box
					for(unsigned int i=0;i<vp1->GetTotal_cells();i++)
					{
						t1=0;t2=0;
						short int ans=vp1->RayCellIntersect(&cam_cor,&beam_dir,&vp1->acell[i],&t1,&t2);
						if(ans==1) //if beam intersect this cell
						{	
							double dpx1=0,dpy1=0,dpx2=0,dpy2=0;
							//calculate cross section area for this cell
							dpx1=t1*tan(angle_x/2)*2;
							dpy1=t1*tan(angle_y/2)*2;
							dpx2=t2*tan(angle_x/2)*2;
							dpy2=t2*tan(angle_y/2)*2;
							double cross_area=(dpx1*dpy1+dpx2*dpy2)/2;

							//calculate volume and pathlength for this intersect
							double intersect_volume=cross_area*abs(t2-t1);

							vp1->acell[i].pathLength=abs(t2-t1); // add for LAD

							sum_volume+=intersect_volume;
							this_path_length+=abs(t2-t1);
							vp1->acell[i].intersect_volume+=intersect_volume;
						}//if(ans==1)
					}//for(unsigned int i=0;i<vp1->GetTotal_cells();i++)
					tempdir.beam_volume=sum_volume;
					tempdir.PathLength=this_path_length;
					//tempdir.section_area=sum_volume/this_path_length;
					int zone_width=0;if(gy<gx){zone_width=gy;}else{zone_width=gx;}
					tempdir.section_area=image->CalculSectionArea(zone_width,px,py);

//------------- Begin calculate gap fraction ----------------------------------------------------					
					int startx=int(px)-gx/2;
					if (startx<0){startx=0;}
					
					int endx=int(px)+gx/2;
					if(endx>ctrlImage.GetWidth()){endx=ctrlImage.GetWidth();}

					int starty=int(py)-gy/2;
					if (starty<0){starty=0;}

					int endy=int(py)+gy/2;
					if(endy>ctrlImage.GetHeight()){endy=endy>ctrlImage.GetHeight();}

					int sumblack=0;
					for (int i=startx;i<endx;i++)
					{
						for(int j=starty;j<endy;j++)
						{
							if(ctrlImage.GetPixel(i,j)==0){sumblack++;}
						}//for(int j=starty;j<endy;j++)
					}//for (int i=startx;i<endx;i++)
					double total_pixel=(endx-startx)*(endy-starty);
					double gapfraction=1-sumblack/total_pixel;
//------------- end calculate gap fraction ----------------------------------------------------
					//if(gapfraction==0){gapfraction=pImagePool1->minimumGap;}
					tempdir.gapfraction=gapfraction;

					if(tempdir.gapfraction<1 && tempdir.gapfraction>0)
					{
						CEachEquation tempeq;

						double G=pImagePool1->CalculG(tempdir.CalculNormalInc()*180/pii);
						tempeq.lnP0=log(gapfraction);
						tempeq.SectionArea=tempdir.section_area;
						tempeq.G=G;

						for(unsigned int i=0;i<vp1->GetTotal_cells();i++)
						{
							if(vp1->acell[i].pathLength>0)
							{
								tempeq.vctID.push_back(vp1->acell[i].ID);
								//tempeq.vctValue.push_back(vp1->acell[i].pathLength*G);
							}
						}
						eqPool->vctEquation.push_back(tempeq);
					}

				
				} //if(vp1->RayCanopyIntersect(&image->CameraCoordinate,&tempdir.unitDirection,&t1,&t2))
				if(sum_volume>0){vctDirection.push_back(tempdir);}
			} //for (j=1;j<=ny;j++)
		} //for (i=1;i<=nx;i++)
//End the calcuation of the directions that intersect the canopy --------------------------------------------
		ctrlImage.Destroy(); //detach and destroy bitmap
     } //else //if load image OK
skipimage:;
	 //else {MessageBox("Reading file error!\nSelected file may not be bitmap file.");}
}//void CTreeAnalyserDlg::CalculDirections7(CEachImage *image,int gx,int gy)
*/


//! A function to to display profile of volume occupied by the canopy
/*!
*	Created: 21-Sep-04
*	\n Last modification: 21-Sep-04.\n
*	Called by:
*	-Menu 
*	
*/
void CTreeAnalyserDlg::OnCanopyVerticalprofileofvolume()
{
	if(!vp1->GetTotal_cells()==0)
	{
			vp1->CalculVerticalProfileVolume();
	}
	else //if no voxel
	{
		MessageBox("No canopy to work");
	}
}

//! A function to check if user prees specified key
/*!
*	Created: 21-Sep-04
*	\n Last modification: 21-Sep-04.\n
*	Called when user press key
*/
BOOL CTreeAnalyserDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN)  // ถ้ามีการกดคีย์
	{
		if(pMsg->wParam == VK_ESCAPE)// ถ้าคีย์ที่กด คือ คีย์ ESC
		{
			CTreeAnalyserDlg * pNewObject = this; //create pointer to this
			DWORD dwExitCode;
			if (pWorkThread != NULL) //if thread had been created
			{
				::GetExitCodeThread(pWorkThread->m_hThread,&dwExitCode);
				if(dwExitCode==STILL_ACTIVE) //if thread is working
				{
					pWorkThread->SuspendThread();
					if(this->AskForStopThread()==IDNO){pWorkThread->ResumeThread();}
					else
					{
						::WaitForSingleObject(pWorkThread->m_hThread,0); //stop the thread suddenly
						delete pWorkThread;
						pWorkThread=NULL;
						this->DisplayTerminateMessage();
						WorkIndex=0;
						this->SetWorkingMenu();
						DisplayStatus();
					}
				}//if(dwExitCode!=STILL_ACTIVE) //if thread is working
			}//if (pWorkThread != NULL) //if thread had been created
		}//f(pMsg->wParam == VK_ESCAPE)// ถ้าคีย์ที่กด คือ คีย์ ESC
	}//if(pMsg->message == WM_KEYDOWN)  // ถ้ามีการกดคีย์

	return 0; // ให้ออกจากฟังก์ชั่นนี้ //return CDialog::PreTranslateMessage(pMsg);
} 

/*
void CTreeAnalyserDlg::Solvinglinearregression()
{
	AddDisplayText("Start solving linear regression");
	int in_cell=vp1->GetTotal_cells();
	if(in_cell==0){MessageBox("Virtual canopy is not reconstructed.\r\nPlease reconstruct canopy volume before or open saved canopy.");return;}
	if(pImagePool1->GetImageCount()==0){MessageBox("No image for working.");return;}
	if(vp1->GetTotalLeafArea()==0){MessageBox("No leaf in the canopy.");return;}

	//-------- initializing time used ---------------
	time_t   start, finish;
	double   elapsed_time;   
	time( &start );
	//-------- end initializing time used ---------------

//Step1 initialize required variables
	CEquationPool *pEquationPool1=new CEquationPool(&m_statusBar,&m_EditOutput,vp1->GetTotal_cells());
	vp1->ResetVoxelID();



//Step 2 get the set of equations
		UpdateCheckItem();
		//do ray-box intersection for each image
		int workcount=0;
		if (pImagePool1->m_leaf_distribution==7){pImagePool1->ReadLeafDis(pImagePool1->LeafDistributionFile,this->CurrentPath);}
		for (int pic=0;pic<pImagePool1->GetImageCount();pic++)
		{
			CEachImage *tempImage=pImagePool1->GetImage(pic);

			if (tempImage->GetFileName()!="empty" && tempImage->GetCheck()==1) //do only selected image
			{//if image is selected then do it
				int zone_size_x=0,zone_size_y=0;

				//detect optimal zoning size
				if(pImagePool1->gap_option==0)
				{
					zone_size_x=pImagePool1->CalculOptimalZoneSize(tempImage);
					zone_size_y=zone_size_x;
				}
				else //if custom zoning size
				{
					zone_size_x=pImagePool1->gpx;
					zone_size_y=pImagePool1->gpy;
				}

				m_Tree1.SelectItem(vctTreeItem[pic]); //select the picture to work
				workcount++;
				tempImage->CalculBasic();

				int linecount=0;
				CImage ctrlImage;

				// Load the selected bitmap file
				HRESULT loadresule= ctrlImage.Load(tempImage->GetFileName());
				if (FAILED(loadresule))
				{
					ms.Format("Error reading file '%s'\r\nSelected file may not existed or not be the bitmap file.",tempImage->GetFileName());
					AddDisplayText(ms);
					goto skipimage;
				}
				else //if load image OK
				{

			//Start to calcuate the directions that intersect the canopy --------------------------------------------
					//calculate total number of directions
					int nx=0,ny=0;
					vctDirection.clear(); //delete all previous directions
					if (mod(ctrlImage.GetWidth(),zone_size_x)==0)
					{
						nx=ctrlImage.GetWidth()/zone_size_x;
					}
					else {nx=(ctrlImage.GetWidth()/zone_size_x)+1;}

					if (mod(ctrlImage.GetHeight(),zone_size_y)==0)
					{
						ny=ctrlImage.GetHeight()/zone_size_y;
					}
					else {ny=(ctrlImage.GetHeight()/zone_size_y)+1;}

					int i,j;
					int opercent=-1;
					double unitpercent=double(nx)/100;
					AddDisplayText(tempImage->GetPictureName());
					ms.Format("gx=%i  gy=%i  nx=%i  ny=%i",zone_size_x,zone_size_y,nx,ny);
					AddDisplayText(ms);

					for (i=1;i<=nx;i++)
					{
						int percent=int(i/unitpercent);
						if(percent>opercent)
						{
							ms.Format("Analysing intercepted directions on image '%s'  %i%%",tempImage->GetPictureName(),percent);
							DisplayStatus(ms);
							opercent=percent;
						}

						for (j=1;j<=ny;j++)
						{
							//vp1->ResetVoxelPathLength(); //add for LAD

							//CDirection tempdir; //create temporary directory
							CEachEquation tempeq;//create temporary direction

							//find the middle pixel
							double px=(i-1)*zone_size_x+zone_size_x/2;
							double py=(j-1)*zone_size_y+zone_size_y/2;
							tempImage->CalculBasic();

			//------------- Begin calculate gap fraction ----------------------------------------------------					
							int startx=int(px)-zone_size_x/2;
							if (startx<0){startx=0;}
							
							int endx=int(px)+zone_size_x/2;
							if(endx>ctrlImage.GetWidth()){endx=ctrlImage.GetWidth();}

							int starty=int(py)-zone_size_y/2;
							if (starty<0){starty=0;}

							int endy=int(py)+zone_size_y/2;
							if(endy>ctrlImage.GetHeight()){endy=endy>ctrlImage.GetHeight();}

							int sumblack=0;
							for (int i=startx;i<endx;i++)
							{
								for(int j=starty;j<endy;j++)
								{
									if(ctrlImage.GetPixel(i,j)==0){sumblack++;}
								}//for(int j=starty;j<endy;j++)
							}//for (int i=startx;i<endx;i++)
							double total_pixel=(endx-startx)*(endy-starty);
							double gapfraction=1-sumblack/total_pixel;
							if(gapfraction==0){gapfraction=pImagePool1->minimumGap;}

							//tempdir.gapfraction=gapfraction;
							tempeq.lnP0=log(gapfraction);
			//------------- end calculate gap fraction ----------------------------------------------------

							if(tempeq.lnP0<0)
							{
								CDirection tempdir;
								tempdir.unitDirection=tempImage->CalculUnitDirection(px,py);
								tempdir.px=px;
								tempdir.py=py;
								tempeq.G=pImagePool1->CalculG(tempdir.CalculNormalInc()*180/pii);
								

								//calcualte angle of current beam
								double upper_pixel_x=px+(zone_size_x/2.0);
								double lower_pixel_x=px-(zone_size_x/2.0);
								cPoint upper_dir_x=tempImage->CalculUnitDirection(upper_pixel_x,py);
								cPoint lower_dir_x=tempImage->CalculUnitDirection(lower_pixel_x,py);
								double angle_x=acos(abs(upper_dir_x*lower_dir_x)); //aa is dot product or cosbeta

								double upper_pixel_y=py+(zone_size_y/2.0);
								double lower_pixel_y=py-(zone_size_y/2.0);
								cPoint upper_dir_y=tempImage->CalculUnitDirection(px,upper_pixel_y);
								cPoint lower_dir_y=tempImage->CalculUnitDirection(px,lower_pixel_y);
								double angle_y=acos(abs(upper_dir_y*lower_dir_y)); //aa is dot product or cosbeta

								//calculate path length
								double sum_volume=0,this_path_length=0;
								double t1=0,t2=0;
								cPoint cam_cor=tempImage->CameraCoordinate;
								cPoint beam_dir=tempImage->CalculUnitDirection(px,py);
								if(vp1->RayCanopyIntersect(&cam_cor,&beam_dir,&t1,&t2))
									{//check if the ray intersect the bounding box
									for(unsigned int i=0;i<vp1->GetTotal_cells();i++)
									{
										t1=0;t2=0;
										short int ans=vp1->RayCellIntersect(&cam_cor,&beam_dir,&vp1->acell[i],&t1,&t2);
										if(ans==1) //if beam intersect this cell
										{	
											double dpx1=0,dpy1=0,dpx2=0,dpy2=0;
											//calculate cross section area for this cell
											dpx1=t1*tan(angle_x/2)*2;
											dpy1=t1*tan(angle_y/2)*2;
											dpx2=t2*tan(angle_x/2)*2;
											dpy2=t2*tan(angle_y/2)*2;
											double cross_area=(dpx1*dpy1+dpx2*dpy2)/2;

											//calculate volume and pathlength for this intersect
											double intersect_volume=cross_area*abs(t2-t1);

											//vp1->acell[i].pathLength=abs(t2-t1); // add for LAD
											tempeq.vctID.push_back(i);
											tempeq.vctPathLength.push_back(abs(t2-t1));

											sum_volume+=intersect_volume;
											this_path_length+=abs(t2-t1);

											//vp1->acell[i].intersect_volume+=intersect_volume;

										}//if(ans==1)
									}//for(unsigned int i=0;i<vp1->GetTotal_cells();i++)

									//tempdir.beam_volume=sum_volume;
									tempeq.sumVolume=sum_volume;

									//tempdir.PathLength=this_path_length;
									tempeq.sumPathLength=this_path_length;
									
									//tempdir.section_area=sum_volume/this_path_length;
									int zone_width=0;if(zone_size_y<zone_size_x){zone_width=zone_size_y;}else{zone_width=zone_size_x;}

									//tempdir.section_area=image->CalculSectionArea(zone_width,px,py);
									tempeq.SectionArea=tempImage->CalculSectionArea(zone_width,px,py);
									pEquationPool1->vctEquation.push_back(tempeq);
								} //if(vp1->RayCanopyIntersect(&image->CameraCoordinate,&tempdir.unitDirection,&t1,&t2))
								
							}
							//if(sum_volume>0){vctDirection.push_back(tempdir);}

						} //for (j=1;j<=ny;j++)
					} //for (i=1;i<=nx;i++)
				//End the calcuation of the directions that intersect the canopy --------------------------------------------
					ctrlImage.Destroy(); //detach and destroy bitmap
					ms.Format("Equation found=%i",pEquationPool1->vctEquation.size());
					AddDisplayText(ms);
				} //else //if load image OK
skipimage:;

			}//if (tempImage->GetFileName()!="empty" && tempImage->GetCheck()==1)
			
		} //for (int pic=0;pic<pImagePool1->GetImageCount();pic++)
		DisplayStatus();
		//pEquationPool1->WriteFile("c:\\z\\test.txt");
//End step 2 now we have all equations

//Step 3 solving equation cell by cell
	int i=0,ncell=vp1->GetTotal_cells();
	for(i=0;i<ncell;i++)
	{
		CEquationPool *pEquationPool2=new CEquationPool(&m_statusBar,&m_EditOutput,vp1->GetTotal_cells());
		//select equations for each voxel
		for(unsigned int j=0;j<pEquationPool1->vctEquation.size();j++)
		{
			if(pEquationPool1->vctEquation[j].IsHaveThisID(i))
			{
				pEquationPool2->vctEquation.push_back(pEquationPool1->vctEquation[j]);
			}
			
		}
		//ms.Format("Voxel%i  equation match=%i",i,pEquationPool2->vctEquation.size());
		//AddDisplayText(ms);

		//calculate r between lnP0 and pathlength for each voxel
		double sumxy=0,sumx=0,sumy=0; //x is pathlength, y is lnP0
		double sumx2=0,sumy2=0;
		AddDisplayText("\nVoxel\tr\n");
		unsigned int n=pEquationPool2->vctEquation.size();
		for(unsigned int k=0;k<n;k++)
		{
			double x=pEquationPool2->vctEquation[k].GetPathLength(i);
			double y=pEquationPool2->vctEquation[k].lnP0/pEquationPool2->vctEquation[k].G;
			double intercept=0;
			for(unsigned int l=0;l<pEquationPool2->vctEquation[k].vctID.size();l++)
			{
				int currentID=pEquationPool2->vctEquation[k].vctID[l];
				if(currentID!=i)
				{
					intercept+=pEquationPool2->vctEquation[k].vctPathLength[l]*vp1->acell[currentID].sum_surface/vp1->GetCellVolume();
				}
			}
			y=y-intercept;

			//ms.Format("%6.4f\t%6.4f\n",x,y);
			//AddDisplayText(ms);
			sumx+=x;
			sumy+=y;
			sumxy+=x*y;
			sumx2+=x*x;
			sumy2+=y*y;
		}
		double r=(sumxy-(sumx*sumy)/n)/pow(((sumx2-(sumx*sumx/n))*(sumy2-(sumy*sumy/n))),0.5);
		ms.Format("%i\t%6.4f",i,r);
		AddDisplayText(ms);
		delete pEquationPool2;
	}
//End step 3 solving equation cell by cell

//----------- calculate time used ---------------------------
	delete pEquationPool1;

	time( &finish );
	elapsed_time = difftime( finish, start );
	ms.Format( "Solving linear regression using = %6.0f seconds.", elapsed_time );
	AddDisplayText(ms);
	//----------- calculate time used ---------------------------
}//void CTreeAnalyserDlg::Solvinglinearregression()
*/

/*
void CTreeAnalyserDlg::Computep0beer()
{
	vctDirection.clear();
	int in_cell=vp1->GetTotal_cells();
	if(in_cell==0){MessageBox("Virtual canopy is not reconstructed.\r\nPlease reconstruct canopy volume before or open saved canopy.");goto end_process;}
	if(vp1->GetTotalLeafArea()==0){MessageBox("No leaf area to work");return;}
	if(pImagePool1->GetImageCount()==0){MessageBox("No image for working.");goto end_process;}
	else // if have cell to work
	{


		CString rootname;
		static char BASED_CODE szFilter[] = "Comma separated value (*.csv)|*.csv||";
		rootname="";
		CFileDialog m_ldFile(FALSE, ".cpn", rootname,
			OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
		if (m_ldFile.DoModal() == IDOK)
		{
			rootname = m_ldFile.GetPathName();
			rootname=rootname.Left(rootname.GetLength()-4);
			//AddDisplayText(filename);
			//goto end_process;
		}

		//-------- initializing time used ---------------
		time_t   start, finish;
		double   elapsed_time;   
		time( &start );
		//-------- end initializing time used ---------------

		DisplayStatus("Exporting csv file of P0..........");
		AddDisplayText(" ");
		ms.Format("******************** Compute P0 Beer ********************");
		AddDisplayText(ms);		

		ms.Format("Leaf angle distribution = %s",pImagePool1->GetLeafDistributionName());
		AddDisplayText(ms);
		if(pImagePool1->m_leaf_distribution==0)
		{
			ms.Format("Mean Leaf angle=%6.2f",pImagePool1->m_mean_leaf_inc);
			AddDisplayText(ms);	
		}

		if(pImagePool1->m_leaf_distribution==7)
		{
			ms.Format("Leaf angle distribution file='%s'",pImagePool1->LeafDistributionFile);
			AddDisplayText(ms);	
			if(!pImagePool1->ReadLeafDis(pImagePool1->LeafDistributionFile,pImagePool1->ActivePath))
			{
				AddDisplayText("Unable to read leaf angle distirbution file!\r\nEstimation of leaf area stop by error.");
				goto end_process;
			}
		}
	

		UpdateCheckItem();
	

		//do ray-box intersection for each image
		int workcount=0;
		//double minLA=9e9,maxLA=-9e9,sumLA=0,sumLA2=0;
		if (pImagePool1->m_leaf_distribution==7){pImagePool1->ReadLeafDis(pImagePool1->LeafDistributionFile,this->CurrentPath);}
		for (int pic=0;pic<pImagePool1->GetImageCount();pic++)
		{
			CEachImage *tempImage=pImagePool1->GetImage(pic);

			if (tempImage->GetFileName()!="empty" && tempImage->GetCheck()==1) //do only selected image
			{//if image is selected then do it

				//declearation of variables
				//double angle_x=0,angle_y=0;
				//double dpx1=0,dpx2=0,dpy1=0,dpy2=0;
				//double sum_volume=0,sum_leaf_area=0,sum_beam_volume=0,sum_leaf_area_nogap=0;
				//double sumLAD=0,sumLAD2=0,VLAD=0,LAD_max=0;
				//double sumLADW=0,VLADW=0;
				//double zero_gap_vol=0,full_gap_vol=0;
				//int dirsize=0;
				int zone_size_x=0,zone_size_y=0;
				//variables for gap fraction
				//double meanGap=0,VGap=0;
				//int zero_gap=0,full_gap=0;

				//step 1 detect optimal zoning size
				if(pImagePool1->gap_option==0)
				{
					zone_size_x=pImagePool1->CalculOptimalZoneSize(tempImage);
					zone_size_y=zone_size_x;
				}
				else
				{
					zone_size_x=pImagePool1->gpx;
					zone_size_y=pImagePool1->gpy;
				}

				m_Tree1.SelectItem(vctTreeItem[pic]);

				workcount++;

				tempImage->CalculBasic();

//-------------- add for LAD computation -----------------------------------------------------
				CString filename;
				filename.Format("%s%i.csv",rootname,workcount);

				ms.Format("Picture='%s'",tempImage->GetPictureName());
				AddDisplayText(ms);
				ms.Format("Save output to %s",filename);
				AddDisplayText(ms);

				FILE * pFile;
				pFile=fopen(filename,"wt");
				fprintf(pFile,"P0,G");
			
				for(unsigned int i=0;i<vp1->GetTotal_cells();i++)
				{
					fprintf(pFile,",%i",vp1->acell[i].ID);
				}
				fprintf(pFile,"\n");
				fclose(pFile);
//-------------- add for LAD computation -----------------------------------------------------

				int exportedline=CalculDirections8(tempImage,zone_size_x,zone_size_y,&filename);
				ms.Format("Exported data %i lines",exportedline);
				AddDisplayText(ms);

				vctDirection.clear(); //clear memory of directions

			}//if (tempImage->GetFileName()!="empty" && tempImage->GetCheck()==1)
			
		} //for (int pic=0;pic<pImagePool1->GetImageCount();pic++)
		
		//----------- calculate time used ---------------------------
		time( &finish );
		elapsed_time = difftime( finish, start );
		ms.Format( "Compute P0 Beer from virtual canopy using time = %6.0f seconds.\n", elapsed_time );
			AddDisplayText(ms);
		//----------- calculate time used ---------------------------
	}
end_process:;
	vctDirection.clear();
	DisplayStatus();
}
*/

int CTreeAnalyserDlg::CalculDirections8(CEachImage *image,int gx,int gy,CString *fname)
{ //for exporting data for Christophe
	vctDirection.clear();
	int linecount=0;
	CImage ctrlImage;
	// Load the selected bitmap file
	HRESULT loadresule= ctrlImage.Load(image->GetFileName());
	if (FAILED(loadresule))
	{
		ms.Format("Error reading file '%s'\r\nSelected file may not existed or not be the bitmap file.",image->GetFileName());
		AddDisplayText(ms);
		goto skipimage;
	}
	else //if load image OK
	{

//Start to calcuate the directions that intersect the canopy --------------------------------------------
		//calculate total number of directions
		int nx=0,ny=0;
		vctDirection.clear(); //delete all previous directions
		if (mod(ctrlImage.GetWidth(),gx)==0)
		{
			nx=ctrlImage.GetWidth()/gx;
		}
		else {nx=(ctrlImage.GetWidth()/gx)+1;}

		if (mod(ctrlImage.GetHeight(),gy)==0)
		{
			ny=ctrlImage.GetHeight()/gy;
		}
		else {ny=(ctrlImage.GetHeight()/gy)+1;}


		ms.Format("gx=%i  gy=%i  nx=%i  ny=%i",gy,gy,nx,ny);
		AddDisplayText(ms);

		//int totalfound=0;
		int i,j;
		int opercent=-1;
		double unitpercent=double(nx)/100;
		for (i=1;i<=nx;i++)
		{
			int percent=int(i/unitpercent);
			if(percent>opercent)
			{
				ms.Format("Analysing intercepted directions on image '%s'  %i%%",image->GetPictureName(),percent);
				DisplayStatus(ms);
				opercent=percent;
			}

			for (j=1;j<=ny;j++)
			{
				vp1->ResetVoxelPathLength(); //add for LAD
				CDirection tempdir; //create temporaly directory
				double px=0,py=0;
				int point=5;
				for(int k=0;k<point;k++) //do 5 point
				{

					//find the middle pixel
					switch (k)
					{
					case 0: //top left
						px=(i-1)*gx+gx/3;
						py=(j-1)*gy;
						break;
					case 1: //bottom left
						px=(i-1)*gx;
						py=j*gy;
						break;
					case 2: //top right
						px=i*gx;
						py=(j-1)*gy;
						break;
					case 3: //bottom right
						px=i*gx;
						py=j*gy;
						break;
					case 4:
						px=(i-1)*gx+gx/2;
						py=(j-1)*gy+gy/2;
						break;
					}					
					image->CalculBasic();
					tempdir.unitDirection=image->CalculUnitDirection(px,py);
					tempdir.px=px;
					tempdir.py=py;
					//int total_cell=vp1->GetTotal_cells();
					
					//calcualte angle of current beam
					double upper_pixel_x=px+(gx/2.0);
					double lower_pixel_x=px-(gx/2.0);
					cPoint upper_dir_x=image->CalculUnitDirection(upper_pixel_x,py);
					cPoint lower_dir_x=image->CalculUnitDirection(lower_pixel_x,py);
					double angle_x=acos(abs(upper_dir_x*lower_dir_x)); //aa is dot product or cosbeta

					double upper_pixel_y=py+(gy/2.0);
					double lower_pixel_y=py-(gy/2.0);
					cPoint upper_dir_y=image->CalculUnitDirection(px,upper_pixel_y);
					cPoint lower_dir_y=image->CalculUnitDirection(px,lower_pixel_y);
					double angle_y=acos(abs(upper_dir_y*lower_dir_y)); //aa is dot product or cosbeta

					//calculate path length
					double sum_volume=0,this_path_length=0;
					double t1=0,t2=0;
					cPoint cam_cor=image->CameraCoordinate;
					cPoint beam_dir=image->CalculUnitDirection(px,py);
					for(unsigned int i=0;i<vp1->GetTotal_cells();i++)
					{
						t1=0;t2=0;
						short int ans=vp1->RayCellIntersect(&cam_cor,&beam_dir,&vp1->acell[i],&t1,&t2);
						if(ans==1) //if beam intersect this cell
						{	
							double dpx1=0,dpy1=0,dpx2=0,dpy2=0;

							vp1->acell[i].pathLength+=abs(t2-t1); // add for LAD

							this_path_length+=abs(t2-t1);
						}//if(ans==1)
					}//for(unsigned int i=0;i<vp1->GetTotal_cells();i++)
					tempdir.PathLength=this_path_length;
					int zone_width=0;if(gy<gx){zone_width=gy;}else{zone_width=gx;}
				}//for(int k=0;k<5;k++) //do 5 point

//------------- Begin calculate gap fraction ----------------------------------------------------					
					int startx=int(px)-gx/2;
					if (startx<0){startx=0;}
					
					int endx=int(px)+gx/2;
					if(endx>ctrlImage.GetWidth()){endx=ctrlImage.GetWidth();}

					int starty=int(py)-gy/2;
					if (starty<0){starty=0;}

					int endy=int(py)+gy/2;
					if(endy>ctrlImage.GetHeight()){endy=endy>ctrlImage.GetHeight();}

					int sumblack=0;
					for (int i=startx;i<endx;i++)
					{
						for(int j=starty;j<endy;j++)
						{
							if(ctrlImage.GetPixel(i,j)==0){sumblack++;}
						}//for(int j=starty;j<endy;j++)
					}//for (int i=startx;i<endx;i++)
					double total_pixel=(endx-startx)*(endy-starty);
					double gapfraction=1-sumblack/total_pixel;
//------------- end calculate gap fraction ----------------------------------------------------
					//if(gapfraction==0){gapfraction=pImagePool1->minimumGap;}
					tempdir.gapfraction=gapfraction;


//-------------- add for LAD computation -----------------------------------------------------
					if(tempdir.gapfraction<1 && tempdir.gapfraction>0)
					{
						linecount++;
						double G=pImagePool1->CalculG(tempdir.CalculNormalInc()*180/pii);

						FILE * pFile;
						pFile=fopen(*fname,"at");
				
						//P0=exp(-k*LAD*L)
						//P0=exp(-k*sum(LADi*Li)

						double sumProduct=0;
						for(unsigned int i=0;i<vp1->GetTotal_cells();i++)
						{
							sumProduct+=(vp1->acell[i].pathLength/point)*(vp1->acell[i].sum_surface/vp1->GetCellVolume());
						}

						double P0=exp(-1*G*sumProduct);
						fprintf(pFile,"%6.8f,%6.8f",P0,G);
						for(unsigned int i=0;i<vp1->GetTotal_cells();i++)
						{
							fprintf(pFile,",%6.4f",vp1->acell[i].pathLength*G);
						}
						fprintf(pFile,"\n");
						fclose(pFile);
					}

//-------------- add for LAD computation -----------------------------------------------------
				
			} //for (j=1;j<=ny;j++)
		} //for (i=1;i<=nx;i++)
//End the calcuation of the directions that intersect the canopy --------------------------------------------
		ctrlImage.Destroy(); //detach and destroy bitmap
     } //else //if load image OK
skipimage:;
	 return linecount;
	 //else {MessageBox("Reading file error!\nSelected file may not be bitmap file.");}
}//void CTreeAnalyserDlg::CalculDirections8(CEachImage *image,int gx,int gy)



//! A function to view canopy 3D by PlantGL viewer.
/*!
*	Created: 11-Dec-04.\n
*	Last modification: 22-Jan-05.\n
*	Called by:
*	-# Menu Canopy->View by PlantGL 3D Viewer
*/
void CTreeAnalyserDlg::OnCanopyViewbyplantgl3dviewer()
{
	if(vp1->GetTotal_cells())
	{
		ms.Format("%stemp_canopy.geom",this->CurrentPath);
		vp1->ExportPlantGL(ms);

		//::ShellExecute(NULL,NULL,"F:\\Program Files\\AMAPmod\\Bin\\GeomViewer.exe","temp_canopy.geom", this->CurrentPath, SW_SHOWNORMAL);
		if(!::ShellExecute(NULL,"open","temp_canopy.geom",NULL,this->CurrentPath, SW_SHOWNORMAL))
		{
			MessageBox("Can not open PlantGL Viewer!!");
		}
	}
	else {MessageBox("No voxels to veiw.");}
} //void CTreeAnalyserDlg::OnCanopyViewbyplantgl3dviewer()

void CTreeAnalyserDlg::OnCanopyExportplantglfile()
{
	int ncell=vp1->GetTotal_cells();
	if (ncell==0){MessageBox("No data to save!");}
	else
	{

		CString filename;
		static char BASED_CODE szFilter[] = "PlantGL 3D Viewer file (*.geom)|*.geom||";
		filename=this->GetRootName(this->ActiveProjectFilename)+".geom";
		CFileDialog m_ldFile(FALSE, ".geom", filename,
			OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
		if (m_ldFile.DoModal() == IDOK)
		{
			filename = m_ldFile.GetPathName();
			vp1->ExportPlantGL(filename); 
			ms.Format("Export canopy to PlantGL file %s",filename);
			AddDisplayText(ms);
			DisplayStatus();
		}
	}
}

void CTreeAnalyserDlg::DrawZone(int px, int py, int gx, int gy)
{
	//CEachImage *tempImage=pImagePool1->GetImage(&ActivePictureName);

	CDC *pDC=CTreeAnalyserDlg::GetDC();//this->GetDC();

		//Draw with a red pen.
		pDC->SelectObject(&penBlue);
		//pDC->SetBkMode(TRANSPARENT);

		int ly=0,hy=0,lx=0,hx=0;

/*
		//test draw rectangle
		double temp=(px-gx/2)*CurrentScale+rectPic.left;
		if(temp-int(temp)>0.5){lx=int(temp)+1;}
		else{lx=int(temp);}

		temp=(py-gy/2)*CurrentScale+rectPic.top;
		if(temp-int(temp)>0.5){ly=int(temp)+1;}
		else{ly=int(temp);}

		temp=(px+gx/2)*CurrentScale+rectPic.left;
		if(temp-int(temp)>0.5){hx=int(temp)+1;}
		else{hx=int(temp);}

		temp=(py+gy/2)*CurrentScale+rectPic.top;
		if(temp-int(temp)>0.5){hy=int(temp)+1;}
		else{hy=int(temp);}
*/

		lx=int((px-gx/2)*CurrentScale+rectPic.left);
		ly=int((py-gy/2)*CurrentScale+rectPic.top);
		hx=int((px+gx/2)*CurrentScale+rectPic.left);
		hy=int((py+gy/2)*CurrentScale+rectPic.top);

		if(hx>rectPic.right){hx=rectPic.right;}
		if(hy>rectPic.bottom){hy=rectPic.bottom;}

		//left
		pDC->MoveTo(lx,ly);
		pDC->LineTo(lx,hy);

		//right
		pDC->MoveTo(hx,ly);
		pDC->LineTo(hx,hy);

		//top
		pDC->MoveTo(lx,ly);
		pDC->LineTo(hx,ly);

		//bottom
		pDC->MoveTo(lx,hy);
		pDC->LineTo(hx,hy);

		pDC->ReleaseOutputDC();
}

void CTreeAnalyserDlg::OnImageAddimagetoproject()
{
	opening=0;//for open update OnSelchangedTree1

	CDlgAdd addDlg;

	addDlg.pCameraPool=pCameraPool;
	addDlg.CurrentPath=CurrentPath;
	addDlg.tempImagePool=pImagePool1;
	addDlg.dlgTitle="Add Image to Project";
	addDlg.callType=0;
	addDlg.CameraName=LastCamera;
	addDlg.m_distance=last_cam_distance;
	addDlg.m_height =last_cam_height;
	addDlg.m_focalLength=last_fc;
	addDlg.m_inc=last_ele;
	if (addDlg.DoModal()==IDOK) //user click OK
	{
		CurrentPath=addDlg.CurrentPath;
		LastCamera=addDlg.CameraName;
		last_cam_distance=addDlg.m_distance;
		last_cam_height=addDlg.m_height;
		last_fc=addDlg.m_focalLength;
		last_ele=addDlg.m_inc;

		CEachImage tempImage;
		tempImage.camera_name=addDlg.CameraName;
		tempImage.PutCameraBending(addDlg.m_bending);
		tempImage.PutCameraCCDSize(addDlg.camera_cal);
		tempImage.PutCameraDirection(addDlg.m_direction);
		tempImage.PutCameraDistance(addDlg.m_distance);
		tempImage.PutCameraFocalLength(addDlg.m_focalLength);
		tempImage.PutCameraHeight(addDlg.m_height);
		tempImage.PutCameraInc(addDlg.m_inc);
		tempImage.PutFileName(addDlg.m_fname);
		tempImage.PutPictureBit(addDlg.m_pictureBit);
		tempImage.PutPictureHeight(addDlg.m_pictureHeight);
		tempImage.PutPictureName(addDlg.m_picName);
		tempImage.PutCameraRot(addDlg.m_rot);
		tempImage.PutPictureWidth(addDlg.m_pictureWidth);
		tempImage.PutSelectedDirection(addDlg.m_selectedDirection);


		//add image to image pool
		pImagePool1->PutImage(&tempImage);

//------Start add item to tree ----------------	
		ms=addDlg.m_picName;
		int n=ms.GetLength();
		char * aa;
		aa=new char[n];
		for (int i=0;i<n;i++)
		{
			aa[i]=ms.GetAt(i);
		}
		aa[n]='\0';
		ActivePictureName=ms;

		TV_INSERTSTRUCT TreeCtrlItem;
		TreeCtrlItem.hParent = TVI_ROOT;
		TreeCtrlItem.hInsertAfter = TVI_LAST;
		TreeCtrlItem.item.mask = TVIF_TEXT | TVIF_PARAM;
		TreeCtrlItem.item.pszText = aa;
		TreeCtrlItem.item.lParam = 1;
		HTREEITEM hTreeItem1 = m_Tree1.InsertItem(&TreeCtrlItem);
		m_Tree1.SetCheck(hTreeItem1,TRUE);
		vctTreeItem.push_back(hTreeItem1);

		m_Tree1.SelectItem(hTreeItem1);
		//delete aa;
//------End add item to tree ----------------
		this->SetWorkingMenu();
		flagChanged=1;

	}
	else {UpdatePicture();}
}//void CTreeAnalyserDlg::OnImageAddimagetoproject()

void CTreeAnalyserDlg::OnViewClearoutput()
{
	m_EditOutput.SetWindowText("");
} //void CTreeAnalyserDlg::OnViewClearoutput()

void CTreeAnalyserDlg::OnViewCentermark()
{
	pMenu = this->GetMenu();
	if(center_mark)
	{
		center_mark=false;
		pMenu->ModifyMenu(ID_VIEW_CENTERMARK,MF_UNCHECKED,ID_VIEW_CENTERMARK,"&Center mark");
		if(pImagePool1->GetImageCount()){UpdatePicture();} //if there are some images
	}
	else
	{
		center_mark=true;
		pMenu->ModifyMenu(ID_VIEW_CENTERMARK,MF_CHECKED,ID_VIEW_CENTERMARK,"&Center mark");
		if(pImagePool1->GetImageCount()){UpdatePicture();} //if there are some images
	}
}//void CTreeAnalyserDlg::OnViewCentermark()

void CTreeAnalyserDlg::OnViewMarginmark()
{
	pMenu = this->GetMenu();
	if(margin_mark)
	{
		margin_mark=false;
		pMenu->ModifyMenu(ID_VIEW_MARGINMARK,MF_UNCHECKED,ID_VIEW_MARGINMARK,"&Border mark");
		if(pImagePool1->GetImageCount()) //if there are some images
		{
			CEachImage *tempImage=pImagePool1->GetImage(&ActivePictureName);
			if(tempImage->ifMarkedPoint){UpdatePicture();}
		}
	}
	else
	{
		margin_mark=true;
		pMenu->ModifyMenu(ID_VIEW_MARGINMARK,MF_CHECKED,ID_VIEW_MARGINMARK,"&Border mark");
		if(pImagePool1->GetImageCount()) //if there are some images
		{
			CEachImage *tempImage=pImagePool1->GetImage(&ActivePictureName);
			if(tempImage->ifMarkedPoint){UpdatePicture();}
		}
	}
}

void CTreeAnalyserDlg::OnHelpTreeanalyserusermanual()
{
	ms=ProgramPath + "Manual\\";
	//ShellExecute(0, "open", "c:\mydoc.doc", NULL, NULL, SW_SHOWNORMAL);
	if(!::ShellExecute(NULL,"open","Tree Analayser manual.pdf",NULL,ms, SW_SHOWNORMAL))
	{
		MessageBox("Can not open Tree Analyser manual!!");
	}
}//void CTreeAnalyserDlg::OnHelpTreeanalyserusermanual()



/*
void CTreeAnalyserDlg::Exportcsvfileofp0binom()
{//Export beam equations as csv file for Python for model binomial
	vctDirection.clear();
	int in_cell=vp1->GetTotal_cells();
	if(in_cell==0){MessageBox("Virtual canopy is not reconstructed.\r\nPlease reconstruct canopy volume before or open saved canopy.");goto end_process;}
	if(pImagePool1->GetImageCount()==0){MessageBox("No image for working.");goto end_process;}
	else // if have cell to work
	{


		CString rootname;
		static char BASED_CODE szFilter[] = "Comma separated value (*.csv)|*.csv||";
		rootname="";
		CFileDialog m_ldFile(FALSE, ".cpn", rootname,
			OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
		if (m_ldFile.DoModal() == IDOK)
		{
			rootname = m_ldFile.GetPathName();
			rootname=rootname.Left(rootname.GetLength()-4);
			//AddDisplayText(filename);
			//goto end_process;
		}
		else{return;}

		//-------- initializing time used ---------------
		time_t   start, finish;
		double   elapsed_time;   
		time( &start );
		//-------- end initializing time used ---------------

		DisplayStatus("Exporting csv file of P0..........");
		AddDisplayText(" ");
		AddDisplayText("******************** Export beam equations ********************");
		AddDisplayText("Using Binomial model");		

		ms.Format("Leaf angle distribution = %s",pImagePool1->GetLeafDistributionName());
		AddDisplayText(ms);
		if(pImagePool1->m_leaf_distribution==0)
		{
			ms.Format("Mean Leaf angle=%6.2f",pImagePool1->m_mean_leaf_inc);
			AddDisplayText(ms);	
		}

		if(pImagePool1->m_leaf_distribution==7)
		{
			ms.Format("Leaf angle distribution file='%s'",pImagePool1->LeafDistributionFile);
			AddDisplayText(ms);	
			if(!pImagePool1->ReadLeafDis(pImagePool1->LeafDistributionFile,pImagePool1->ActivePath))
			{
				AddDisplayText("Unable to read leaf angle distirbution file!\r\nEstimation of leaf area stop by error.");
				goto end_process;
			}
		}
	

		UpdateCheckItem();
	

		//do ray-box intersection for each image
		int workcount=0;
		//double minLA=9e9,maxLA=-9e9,sumLA=0,sumLA2=0;
		if (pImagePool1->m_leaf_distribution==7){pImagePool1->ReadLeafDis(pImagePool1->LeafDistributionFile,this->CurrentPath);}
		for (int pic=0;pic<pImagePool1->GetImageCount();pic++)
		{
			CEachImage *tempImage=pImagePool1->GetImage(pic);

			if (tempImage->GetFileName()!="empty" && tempImage->GetCheck()==1) //do only selected image
			{//if image is selected then do it

				//declearation of variables
				//double angle_x=0,angle_y=0;
				//double dpx1=0,dpx2=0,dpy1=0,dpy2=0;
				//double sum_volume=0,sum_leaf_area=0,sum_beam_volume=0,sum_leaf_area_nogap=0;
				//double sumLAD=0,sumLAD2=0,VLAD=0,LAD_max=0;
				//double sumLADW=0,VLADW=0;
				//double zero_gap_vol=0,full_gap_vol=0;
				//int dirsize=0;
				int zone_size_x=0,zone_size_y=0;
				//variables for gap fraction
				//double meanGap=0,VGap=0;
				//int zero_gap=0,full_gap=0;

				//step 1 detect optimal zoning size
				if(pImagePool1->gap_option==0)
				{
					zone_size_x=pImagePool1->CalculOptimalZoneSize(tempImage);
					zone_size_y=zone_size_x;
				}
				else
				{
					zone_size_x=pImagePool1->gpx;
					zone_size_y=pImagePool1->gpy;
				}

				m_Tree1.SelectItem(vctTreeItem[pic]);

				workcount++;

				tempImage->CalculBasic();

//-------------- add for LAD computation -----------------------------------------------------
				CString filename;
				filename.Format("%s%i.csv",rootname,workcount);

				ms.Format("Picture='%s'",tempImage->GetPictureName());
				AddDisplayText(ms);
				ms.Format("Save output to %s",filename);
				AddDisplayText(ms);
				
				//Write headerline
				FILE * pFile;
				pFile=fopen(filename,"wt");
				fprintf(pFile,"P0");
				for(unsigned int i=0;i<vp1->GetTotal_cells();i++)
				{
					fprintf(pFile,",%i",vp1->acell[i].ID);
				}
				fprintf(pFile,"\n");
				fclose(pFile);
//-------------- add for LAD computation -----------------------------------------------------

				int exportedline=CalculDirections6(tempImage,zone_size_x,zone_size_y,&filename,1);
				ms.Format("Exported data %i lines",exportedline);
				AddDisplayText(ms);

				vctDirection.clear(); //clear memory of directions

			}//if (tempImage->GetFileName()!="empty" && tempImage->GetCheck()==1)
			
		} //for (int pic=0;pic<pImagePool1->GetImageCount();pic++)
		
		//----------- calculate time used ---------------------------
		time( &finish );
		elapsed_time = difftime( finish, start );
		ms.Format( "Exporting csv file of P0 = %6.0f seconds.\n", elapsed_time );
			AddDisplayText(ms);
		//----------- calculate time used ---------------------------
	}
end_process:;
	vctDirection.clear();
	DisplayStatus();

}//void CTreeAnalyserDlg::OnTestExportcsvfileofp0binom()
*/



void CTreeAnalyserDlg::Exportbeamequations()
{
	int in_cell=vp1->GetTotal_cells();
	if(in_cell==0){MessageBox("Virtual canopy is not reconstructed.\r\nPlease reconstruct canopy volume before or open saved canopy.");return;}
	if(pImagePool1->GetImageCount()==0){MessageBox("No image for working.");return;}
	else // if have cell to work
	{
		CExportBeamDlg dlg;
		dlg.txt_directory=this->CurrentPath;
		dlg.txt_filename.Format("%s%icm",this->GetFileName(this->ActiveProjectFilename,1),int(vp1->Getdx()*100));
		if(dlg.DoModal()==IDOK)
		{
			//get value from dialog
			BOOL selected_model=dlg.m_model;
			BOOL export_python=dlg.m_output_python_code;
			BOOL export_separated=dlg.m_separate_file;
			CString csvfilename=dlg.txt_directory + dlg.txt_filename;
			CString pyfilename=dlg.txt_filename;
			int python_version=dlg.m_python_version;

			//output information
			//AddDisplayText("");
			//AddDisplayText("Export beam equation to solve for leaf area density by Python"); 

			DisplayStatus("Exporting csv file of P0..........");
			AddDisplayText(" ");
			AddDisplayText("******************** Export beam equations for Python ********************");
			ms.Format("Leaf angle distribution = %s",pImagePool1->GetLeafDistributionName());
			AddDisplayText(ms);

			if(!selected_model)
			{
				//AddDisplayText("Model = Beer's");
				csvfilename+="Beer";
				pyfilename+="Beer";
				//ExportP0Beer(csvfilename, export_separated);
				ExportBeamToFile(csvfilename, export_separated,0);
			}
			else
			{
				//AddDisplayText("Model = Binomial");
				csvfilename+="Binom";
				pyfilename+="Binom";
				//ExportP0Binom(csvfilename, export_separated);
				ExportBeamToFile(csvfilename, export_separated,1);
			}

			ms.Format("Data was exported to '%s'",csvfilename+".csv");
			AddDisplayText(ms);
			WritePythonCode(dlg.txt_directory,pyfilename,python_version);

		}
	}
}//void CTreeAnalyserDlg::Exportbeamequations()

/*
void CTreeAnalyserDlg::ExportP0Beer(CString ExportName, BOOL separate) 
{//export data for Python model beer
	vctDirection.clear();
	CString filename;

	//-------- initializing time used ---------------
	time_t   start, finish;
	double   elapsed_time;   
	time( &start );
	//-------- end initializing time used ---------------

	AddDisplayText("Using Beer's model");		

	if(pImagePool1->m_leaf_distribution==0)
	{
		ms.Format("Mean Leaf angle=%6.2f",pImagePool1->m_mean_leaf_inc);
		AddDisplayText(ms);	
	}

	if(pImagePool1->m_leaf_distribution==7)
	{
		//ms.Format("Leaf angle distribution file='%s'",pImagePool1->LeafDistributionFile);
		//AddDisplayText(ms);	
		if(!pImagePool1->ReadLeafDis(pImagePool1->LeafDistributionFile,pImagePool1->ActivePath))
		{
			AddDisplayText("Unable to read leaf angle distirbution file!\r\nEstimation of leaf area stop by error.");
			goto end_process;
		}
	}


	UpdateCheckItem();

	if(!separate)
	{//writing header of output file
		AddDisplayText("Export 1 file for all");
		filename.Format("%s.csv",ExportName);

		FILE * pFile;
		pFile=fopen(filename,"wt");
		fprintf(pFile,"P0,G,s");

		for(unsigned int i=0;i<vp1->GetTotal_cells();i++)
		{
			fprintf(pFile,",%i",vp1->acell[i].ID);
		}
		fprintf(pFile,"\n");
		fclose(pFile);
	}

	//Start iterated work
	int workcount=0;
	if (pImagePool1->m_leaf_distribution==7){pImagePool1->ReadLeafDis(pImagePool1->LeafDistributionFile,this->CurrentPath);}
	for (int pic=0;pic<pImagePool1->GetImageCount();pic++)
	{
		CEachImage *tempImage=pImagePool1->GetImage(pic);

		if (tempImage->GetFileName()!="empty" && tempImage->GetCheck()==1) //do only selected image
		{//if image is selected then do it

			int zone_size_x=0,zone_size_y=0;

			//step 1 detect optimal zoning size
			if(pImagePool1->gap_option==0)
			{
				zone_size_x=pImagePool1->CalculOptimalZoneSize(tempImage);
				zone_size_y=zone_size_x;
			}
			else
			{
				zone_size_x=pImagePool1->gpx;
				zone_size_y=pImagePool1->gpy;
			}

			m_Tree1.SelectItem(vctTreeItem[pic]);

			workcount++;

			tempImage->CalculBasic();

			if(separate)
			{//writing header for each saparated file
				AddDisplayText("Export each file for each image");
				filename.Format("%sPic%i.csv",ExportName,workcount);

				ms.Format("Picture='%s'",tempImage->GetPictureName());
				AddDisplayText(ms);
				ms.Format("Save output to %s",filename);
				AddDisplayText(ms);

				FILE * pFile;
				pFile=fopen(filename,"wt");
				fprintf(pFile,"P0,G,s");

				for(unsigned int i=0;i<vp1->GetTotal_cells();i++)
				{
					fprintf(pFile,",%i",vp1->acell[i].ID);
				}
				fprintf(pFile,"\n");
				fclose(pFile);
			}

			int exportedline=CalculDirections6(tempImage,zone_size_x,zone_size_y,&filename,0);
			ms.Format("'%s' exported %i equations",tempImage->GetPictureName(),exportedline);
			AddDisplayText(ms);

			vctDirection.clear(); //clear memory of directions

		}//if (tempImage->GetFileName()!="empty" && tempImage->GetCheck()==1)
		
	} //for (int pic=0;pic<pImagePool1->GetImageCount();pic++)
	
	//----------- calculate time used ---------------------------
	time( &finish );
	elapsed_time = difftime( finish, start );
	ms.Format( "Export beam equations for Python used %6.0f seconds.\n", elapsed_time );
		AddDisplayText(ms);
	//----------- calculate time used ---------------------------
end_process:;
	vctDirection.clear();
	DisplayStatus();

} //void CTreeAnalyserDlg::ExportP0Beer(CString ExportName, BOOL separated)



void CTreeAnalyserDlg::ExportP0Binom(CString ExportName, BOOL separate)
{//Export beam equations as csv file for Python for model binomial
	vctDirection.clear();
	CString filename;

	//-------- initializing time used ---------------
	time_t   start, finish;
	double   elapsed_time;   
	time( &start );
	//-------- end initializing time used ---------------

	AddDisplayText("Using Binomial model");		

	if(pImagePool1->m_leaf_distribution==0)
	{
		ms.Format("Mean Leaf angle=%6.2f",pImagePool1->m_mean_leaf_inc);
		AddDisplayText(ms);	
	}

	if(pImagePool1->m_leaf_distribution==7)
	{
		//ms.Format("Leaf angle distribution file='%s'",pImagePool1->LeafDistributionFile);
		//AddDisplayText(ms);	
		if(!pImagePool1->ReadLeafDis(pImagePool1->LeafDistributionFile,pImagePool1->ActivePath))
		{
			AddDisplayText("Unable to read leaf angle distirbution file!\r\nEstimation of leaf area stop by error.");
			goto end_process;
		}
	}


	UpdateCheckItem();


	if(!separate)
	{//writing header of output file
		AddDisplayText("Export 1 file for all");
		filename.Format("%s.csv",ExportName);

		FILE * pFile;
		pFile=fopen(filename,"wt");
		fprintf(pFile,"P0,G,s");

		for(unsigned int i=0;i<vp1->GetTotal_cells();i++)
		{
			fprintf(pFile,",%i",vp1->acell[i].ID);
		}
		fprintf(pFile,"\n");
		fclose(pFile);
	}

	int workcount=0;
	if (pImagePool1->m_leaf_distribution==7){pImagePool1->ReadLeafDis(pImagePool1->LeafDistributionFile,this->CurrentPath);}
	for (int pic=0;pic<pImagePool1->GetImageCount();pic++)
	{
		CEachImage *tempImage=pImagePool1->GetImage(pic);

		if (tempImage->GetFileName()!="empty" && tempImage->GetCheck()==1) //do only selected image
		{//if image is selected then do it

			int zone_size_x=0,zone_size_y=0;

			//step 1 detect optimal zoning size
			if(pImagePool1->gap_option==0)
			{
				zone_size_x=pImagePool1->CalculOptimalZoneSize(tempImage);
				zone_size_y=zone_size_x;
			}
			else
			{
				zone_size_x=pImagePool1->gpx;
				zone_size_y=pImagePool1->gpy;
			}

			m_Tree1.SelectItem(vctTreeItem[pic]);

			workcount++;

			tempImage->CalculBasic();

			if(separate)
			{//writing header for each saparated file
				AddDisplayText("Export each file for each image");
				filename.Format("%sPic%i.csv",ExportName,workcount);

				ms.Format("Picture='%s'",tempImage->GetPictureName());
				AddDisplayText(ms);
				ms.Format("Save output to %s",filename);
				AddDisplayText(ms);

				FILE * pFile;
				pFile=fopen(filename,"wt");
				fprintf(pFile,"P0,G,s");

				for(unsigned int i=0;i<vp1->GetTotal_cells();i++)
				{
					fprintf(pFile,",%i",vp1->acell[i].ID);
				}
				fprintf(pFile,"\n");
				fclose(pFile);
			}

			int exportedline=CalculDirections6(tempImage,zone_size_x,zone_size_y,&filename,1);
			ms.Format("'%s' exported %i equations",tempImage->GetPictureName(),exportedline);
			AddDisplayText(ms);

			vctDirection.clear(); //clear memory of directions

		}//if (tempImage->GetFileName()!="empty" && tempImage->GetCheck()==1)
		
	} //for (int pic=0;pic<pImagePool1->GetImageCount();pic++)
	
	//----------- calculate time used ---------------------------
	time( &finish );
	elapsed_time = difftime( finish, start );
	ms.Format( "Export beam equations for Python used  %6.0f seconds.\n", elapsed_time );
		AddDisplayText(ms);
	//----------- calculate time used ---------------------------

end_process:;
	vctDirection.clear();
	DisplayStatus();

}//void CTreeAnalyserDlg::ExportP0Binom(CString ExportName, BOOL separate)
*/


void CTreeAnalyserDlg::WritePythonCode(CString output_directory, CString filename,int python_version)
{
	CString savefilename;
	savefilename.Format("%s%s.py",output_directory,filename);

	if(python_version==0){ms.Format("Python code for Python 2.3 was written to '%s'",savefilename);}
	if(python_version==1){ms.Format("Python code for Python 2.4 was written to '%s'",savefilename);}

	AddDisplayText(ms);
	AddDisplayText("\nExecute this code from Python command line type the following:\n");
	ms.Format(">>> execfile('%s')\n\n",savefilename);
	AddDisplayText(ms);

	FILE * pFile;
	pFile=fopen(savefilename,"wt");
	fprintf(pFile,"#This source code was first developed by Christophe GODIN 2004\n");
	fprintf(pFile,"#Modified by Jessada PHATTARALERPHONG for using with data exported from Tree Analyser 21 Mar 2005\n\n");
	fprintf(pFile,"#Execute this code from Python command line type the following:\n");
	fprintf(pFile,"#>>> execfile('%s')\n",savefilename);

	if(python_version==0){fprintf(pFile,"\n#Running this code needs \n#1. Python 2.3.3\n#2. Numeric Python 23.8\n#3. SciPy 0.3.2\n\n");}
	if(python_version==1){fprintf(pFile,"\n#Running this code needs \n#1. Python 2.4.2\n#2. numpy 0.96.2\n#3. SciPy 0.4.4\n\n");}

	fprintf(pFile,"import os\n");
	//fprintf(pFile,"import copy\n");
	if(python_version==0){fprintf(pFile,"from Numeric import *\n");}
	if(python_version==1){fprintf(pFile,"from numpy import *\n");}

	//fprintf(pFile,"from RandomArray import *\n");
	fprintf(pFile,"from time import time\n");
	fprintf(pFile,"from scipy import *\n");
	fprintf(pFile,"gtime=time()\n");

	ms="";
	for(int i=0;i<output_directory.GetLength();i++)	
	{
		if(output_directory.GetAt(i)=='\\'){ms+="\\\\";}
		else{ms+=output_directory.GetAt(i);}
	}
	fprintf(pFile,"working_directory=%c%s%c\n",char(34),ms,char(34));
	fprintf(pFile,"os.chdir(working_directory)\n\n");


	fprintf(pFile,"#-------------------------------------------------------------------------------------\n");
	fprintf(pFile,"#****************** Start variable initiation ****************************************\n");
	fprintf(pFile,"nbe=0 #number of equation\n");
	fprintf(pFile,"nbv=0 #number of voxel\n");
	fprintf(pFile,"measured_gap_fraction=[] #Array of ln(P0) computed from photographs\n");
	fprintf(pFile,"L=[] #Array variables of equations\n");
	fprintf(pFile,"ID_array=[] #Array of voxel ID\n");
	fprintf(pFile,"file_order=0\n");
	fprintf(pFile,"voxel_volume=%2.8f #m3\n",vp1->Getdx()*vp1->Getdy()*vp1->Getdz());
	fprintf(pFile,"#****************** End variable initiation ******************************************\n");
	fprintf(pFile,"#-------------------------------------------------------------------------------------\n\n");


	fprintf(pFile,"#-------------------------------------------------------------------------------------\n");
	fprintf(pFile,"#****************** Start function zone **********************************************\n");


	fprintf(pFile,"def read_equations(filename):\n");
	fprintf(pFile,"    print %creading file : %c, filename\n",char(34),char(34));       
	fprintf(pFile,"    f1 = file(filename,%cr%c)\n",char(34),char(34));
	fprintf(pFile,"    if f1 :\n");
	fprintf(pFile,"        l = f1.readline() # these lines are discarded\n");
	fprintf(pFile,"        values = l.split(%c,%c)\n",char(34),char(34));
	fprintf(pFile,"        voxelID=[]\n");
	fprintf(pFile,"        voxelID.append([int(v) for v in values[3:]])\n");
	fprintf(pFile,"        p0_vec = [] #array of read p0\n");
	fprintf(pFile,"        vals = []\n");
	fprintf(pFile,"        lnb = 0\n");
	fprintf(pFile,"        cnb = 0\n");
	fprintf(pFile,"        for l in f1:\n");
	fprintf(pFile,"            values = l.split(%c,%c)\n",char(34),char(34));
	fprintf(pFile,"            p0_vec.append(float(values[0]))\n");
	fprintf(pFile,"            cnb = len(values)-1\n");
	fprintf(pFile,"            lnb +=1\n");
	fprintf(pFile,"            vals.append([float(v) for v in values[1:]])\n\n");
	fprintf(pFile,"        L_array = reshape(array(vals),(lnb,cnb))\n");
	fprintf(pFile,"        f1.close()\n\n");
	fprintf(pFile,"    else:\n");
	fprintf(pFile,"        print %cFile %c, filename, %cnot found%c\n",char(34),char(34),char(34),char(34));
	fprintf(pFile,"        return\n\n");
	fprintf(pFile,"    if file_order==0:\n");
	fprintf(pFile,"        L=L_array\n");
	fprintf(pFile,"        measured_gap_fraction=array(p0_vec)\n");
	fprintf(pFile,"    else:\n");
	fprintf(pFile,"        measured_gap_fraction=concatenate((measured_gap_fraction,array(p0_vec)))\n");
	fprintf(pFile,"        L = concatenate((L,L_array))\n\n");        
	fprintf(pFile,"    return (L,measured_gap_fraction,voxelID)\n\n");


	fprintf(pFile,"def gap_fraction(D) :\n");      
	fprintf(pFile,"    k=matrixmultiply(L,D)\n");
	fprintf(pFile,"    return exp(k) #return matrix of size nbe of predicted gap fraction\n\n");


	fprintf(pFile,"def residuals(D):\n");
	fprintf(pFile,"    err = measured_gap_fraction - gap_fraction(D)\n");
	fprintf(pFile,"    return err #retrun maxtrix of size nbe of different predict and observed gap fraction\n\n");

  
	fprintf(pFile,"def square_residuals(D):\n");
	fprintf(pFile,"    r = residuals(D)\n");
	fprintf(pFile,"    norm = matrixmultiply(r,transpose(r))\n");
	fprintf(pFile,"    return norm\n\n");

	fprintf(pFile,"def computeLAD(d0, b): #initial density, bounds for this density\n");
	fprintf(pFile,"    from scipy.optimize import fmin_l_bfgs_b\n");  
	fprintf(pFile,"    start_time=time()\n");
	fprintf(pFile,"    output=[]\n");
	fprintf(pFile,"    print %c%c\n",char(34),char(34));
	fprintf(pFile,"    print %cComputing leaf area density ......\\n%c\n",char(34),char(34));
	fprintf(pFile,"    res = fmin_l_bfgs_b(square_residuals,d0, approx_grad = True, bounds = b)\n");
	fprintf(pFile,"    output=concatenate((output,res[0])) # stores first result for d0\n");
	fprintf(pFile,"    print %cNumber of funcalls = %c, res[2][%cfuncalls%c]\n",char(34),char(34),char(34),char(34));
	fprintf(pFile,"    print %cSolving time = %c, time()-start_time, %cs%c\n",char(34),char(34),char(34),char(34));
	fprintf(pFile,"    return output\n\n");

	fprintf(pFile,"def SaveOutput(voxelID,LAD,filename):\n");
	fprintf(pFile,"    f1 = file(filename, %cwa%c)\n",char(34),char(34));
	fprintf(pFile,"    print %cSave output to: %c, filename\n",char(34),char(34));
	fprintf(pFile,"    f1.write('Voxel_ID'+'\\t'+'LAD(m2/m3)'+'\\n')\n");
	fprintf(pFile,"    for i in range(size(voxelID)):\n");
	fprintf(pFile,"        f1.write(str(voxelID[0][i])+'\\t'+str(LAD[i])+'\\n')\n\n");
	fprintf(pFile,"    f1.close()\n");
	fprintf(pFile,"    return \n");


	fprintf(pFile,"#****************** End function zone ************************************************\n");
	fprintf(pFile,"#-------------------------------------------------------------------------------------\n\n");



	fprintf(pFile,"#-------------------------------------------------------------------------------------\n");
	fprintf(pFile,"#****************** Start reading data ***********************************************\n");
	fprintf(pFile,"(L,measured_gap_fraction,ID_array)=read_equations(%c%s%c)\n",char(34),filename+".csv",char(34));
	fprintf(pFile,"nbe = shape(L)[0]\n");
	fprintf(pFile,"nbv = shape(L)[1]\n");
	fprintf(pFile,"print %cTotal equation lines =%c, nbe\n",char(34),char(34));
	fprintf(pFile,"print %cTotal voxels =%c, nbv\n",char(34),char(34));
	fprintf(pFile,"#****************** End reading data *************************************************\n");
	fprintf(pFile,"#-------------------------------------------------------------------------------------\n\n");



	fprintf(pFile,"#-------------------------------------------------------------------------------------\n");
	fprintf(pFile,"#****************** Start calculation ************************************************\n");
	fprintf(pFile,"upper_bound =10.0        #Upper boundary\n");
	fprintf(pFile,"mean_lad=2.0\n");
	fprintf(pFile,"d0 = zeros(nbv) + mean_lad\n");
	fprintf(pFile,"dbounds = [(0.0,upper_bound)]*len(d0) #set boundary\n");
	fprintf(pFile,"lad = computeLAD(d0,dbounds)\n");
	fprintf(pFile,"print %cTotal leaf area=%c,sum(lad)*voxel_volume\n",char(34),char(34));
	fprintf(pFile,"SaveOutput(ID_array,lad,%c%s%c)\n",char(34),filename+"_output.txt",char(34));
	fprintf(pFile,"#****************** End calculation ************************************************\n");
	fprintf(pFile,"#-------------------------------------------------------------------------------------\n\n\n");
	fprintf(pFile,"print %cTotal time = %c,time()-gtime, %c s%c\n",char(34),char(34),char(34),char(34));



	fclose(pFile);

}//CTreeAnalyserDlg::WritePythonCode(CString filename)

void CTreeAnalyserDlg::OnCalculationExportequationsforpython()
{
	CTreeAnalyserDlg * pNewObject = this; //create pointer to this
	DWORD dwExitCode;
	if (pWorkThread == NULL) //if thread have not been created
	{
		WorkIndex=15;
		pWorkThread=AfxBeginThread(ThreadWork, pNewObject);
	}
	else
	{
		::GetExitCodeThread(pWorkThread->m_hThread,&dwExitCode);
		if(dwExitCode!=STILL_ACTIVE) //if thread is finish
		{
			WorkIndex=15;
			pWorkThread=AfxBeginThread(ThreadWork, pNewObject);
		}
		else //if thread is working
		{
			pWorkThread->SuspendThread();
			if(this->AskForStopThread()==IDNO){pWorkThread->ResumeThread();}
			else
			{
				::WaitForSingleObject(pWorkThread->m_hThread,0); //stop the thread suddenly
				delete pWorkThread;
				pWorkThread=NULL;
				this->DisplayTerminateMessage();
				WorkIndex=0;
				this->SetWorkingMenu();
				DisplayStatus();
			}
		}
	}
}//CTreeAnalyserDlg::OnCalculationExportequationsforpython()



void CTreeAnalyserDlg::ExportBeamToFile(CString ExportName, BOOL separate,bool model) 
{//export data for Python, model 0 is Beer, 1 is binomail
	vctDirection.clear();
	CString filename;

	//-------- initializing time used ---------------
	time_t   start, finish;
	double   elapsed_time;   
	time( &start );
	//-------- end initializing time used ---------------

	AddDisplayText("Using Beer's model");		

	if(pImagePool1->m_leaf_distribution==0)
	{
		ms.Format("Mean Leaf angle=%6.2f",pImagePool1->m_mean_leaf_inc);
		AddDisplayText(ms);	
	}

	if(pImagePool1->m_leaf_distribution==7)
	{
		//ms.Format("Leaf angle distribution file='%s'",pImagePool1->LeafDistributionFile);
		//AddDisplayText(ms);	
		if(!pImagePool1->ReadLeafDis(pImagePool1->LeafDistributionFile,pImagePool1->ActivePath))
		{
			AddDisplayText("Unable to read leaf angle distirbution file!\r\nEstimation of leaf area stop by error.");
			goto end_process;
		}
	}


	UpdateCheckItem();

	if(!separate) //separate equation file for each images
	{//writing header of output file
		AddDisplayText("Export 1 file for all");
		filename.Format("%s.csv",ExportName);

		FILE * pFile;
		pFile=fopen(filename,"wt");
		fprintf(pFile,"P0,G,s");

		for(unsigned int i=0;i<vp1->GetTotal_cells();i++)
		{
			fprintf(pFile,",%i",vp1->acell[i].ID);
		}
		fprintf(pFile,"\n");
		fclose(pFile);
	}

	//Start iterated work
	int workcount=0;
	if (pImagePool1->m_leaf_distribution==7){pImagePool1->ReadLeafDis(pImagePool1->LeafDistributionFile,this->CurrentPath);}
	for (int pic=0;pic<pImagePool1->GetImageCount();pic++)
	{
		CEachImage *tempImage=pImagePool1->GetImage(pic);

		if (tempImage->GetFileName()!="empty" && tempImage->GetCheck()==1) //do only selected image
		{//if image is selected then do it

			int zone_size_x=0,zone_size_y=0;

			//step 1 detect optimal zoning size
			if(pImagePool1->gap_option==0)
			{
				zone_size_x=pImagePool1->CalculOptimalZoneSize(tempImage);
				zone_size_y=zone_size_x;
			}
			else
			{
				zone_size_x=pImagePool1->gpx;
				zone_size_y=pImagePool1->gpy;
			}

			m_Tree1.SelectItem(vctTreeItem[pic]);

			workcount++;

			tempImage->CalculBasic();

			if(separate)
			{//writing header for each saparated file
				AddDisplayText("Export each file for each image");
				filename.Format("%sPic%i.csv",ExportName,workcount);

				ms.Format("Picture='%s'",tempImage->GetPictureName());
				AddDisplayText(ms);
				ms.Format("Save output to %s",filename);
				AddDisplayText(ms);

				FILE * pFile;
				pFile=fopen(filename,"wt");
				fprintf(pFile,"P0,G,s");

				for(unsigned int i=0;i<vp1->GetTotal_cells();i++)
				{
					fprintf(pFile,",%i",vp1->acell[i].ID);
				}
				fprintf(pFile,"\n");
				fclose(pFile);
			}

			int exportedline=CalculDirections6(tempImage,zone_size_x,zone_size_y,&filename,model);
			ms.Format("'%s' exported %i equations",tempImage->GetPictureName(),exportedline);
			AddDisplayText(ms);

			vctDirection.clear(); //clear memory of directions

		}//if (tempImage->GetFileName()!="empty" && tempImage->GetCheck()==1)
		
	} //for (int pic=0;pic<pImagePool1->GetImageCount();pic++)
	
	//----------- calculate time used ---------------------------
	time( &finish );
	elapsed_time = difftime( finish, start );
	ms.Format( "Export beam equations for Python used %6.0f seconds.\n", elapsed_time );
		AddDisplayText(ms);
	//----------- calculate time used ---------------------------
end_process:;
	vctDirection.clear();
	DisplayStatus();

} //void CTreeAnalyserDlg::ExportBeamToFile(CString ExportName, BOOL separated)




void CTreeAnalyserDlg::OnHelpVersionhistory()
{

	if(!::ShellExecute(NULL,"open","Version History.txt",NULL,ProgramPath, SW_SHOWNORMAL))
	{
		MessageBox("Can not open file 'Version History.txt'!!");
	}
}//void CTreeAnalyserDlg::OnHelpVersionhistory()
