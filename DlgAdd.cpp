// DlgAdd.cpp : implementation file
//

#include "stdafx.h"
#include "Tree Analyser.h"
#include "EachImage.h"
#include "ImagePool.h"
#include "DlgAdd.h"
#include ".\dlgadd.h"
#include "CaptureDlg.h"
#include "AddCameraDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgAdd dialog

CDlgAdd::CDlgAdd(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAdd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdd)
	m_fname = _T("");
	m_distance = 5;
	m_direction = 0.0;
	m_height = 1;
	m_bending = 0.0;
	m_rot = 0.0;
	m_inc = 0.0;
	m_pictureWidth = 0;
	m_pictureHeight = 0;
	camera_cal = 6.7603;
	m_picName = _T("");
	m_focalLength = 9;
	m_selectedDirection = 0;
	CameraName="Not defined";
	//}}AFX_DATA_INIT
}


void CDlgAdd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdd)
	DDX_Control(pDX, IDC_EDIT_DIRECTION, m_editDirection);
	DDX_Control(pDX, IDC_COMBO_DIRECTION, m_combodirection);
	DDX_Control(pDX, IDC_EDIT_FOCAL, m_focalCtrl);
	DDX_Control(pDX, IDC_EDIT_VIEWANGLE, m_viewangle);
	DDX_Control(pDX, IDC_EDIT_CCDSIZE, m_editCCDSize);
	DDX_Control(pDX, IDC_COMBO_CAMERA, m_combocamera);
	DDX_Text(pDX, IDC_EDIT_FNAME, m_fname);
	DDX_Text(pDX, IDC_EDIT_DISTANCE, m_distance);
	DDV_MinMaxDouble(pDX, m_distance, 0., 100.);
	DDX_Text(pDX, IDC_EDIT_DIRECTION, m_direction);
	DDV_MinMaxDouble(pDX, m_direction, -360., 360.);
	DDX_Text(pDX, IDC_EDIT_HEIGHT, m_height);
	DDV_MinMaxDouble(pDX, m_height, -100., 1000.);
	DDX_Text(pDX, IDC_EDIT_BENDING, m_bending);
	DDV_MinMaxDouble(pDX, m_bending, -360., 360.);
	DDX_Text(pDX, IDC_EDIT_ROT, m_rot);
	DDV_MinMaxDouble(pDX, m_rot, -360., 360.);
	DDX_Text(pDX, IDC_EDIT_INC, m_inc);
	DDV_MinMaxDouble(pDX, m_inc, -90., 90.);
	DDX_Text(pDX, IDC_EDIT_PICW, m_pictureWidth);
	DDV_MinMaxInt(pDX, m_pictureWidth, 1, 10000);
	DDX_Text(pDX, IDC_EDIT_PICH, m_pictureHeight);
	DDV_MinMaxInt(pDX, m_pictureHeight, 1, 10000);
	DDX_Text(pDX, IDC_EDIT_CCDSIZE, camera_cal);
	DDV_MinMaxDouble(pDX, camera_cal, 0., 100.);
	DDX_Text(pDX, IDC_EDIT_PICNAME, m_picName);
	DDX_Text(pDX, IDC_EDIT_FOCAL, m_focalLength);
	DDV_MinMaxDouble(pDX, m_focalLength, 1., 1000.);
	DDX_CBIndex(pDX, IDC_COMBO_DIRECTION, m_selectedDirection);
	//}}AFX_DATA_MAP
}

//DDX_Radio(pDX, IDC_PICTYPE, m_picType);
//DDX_CBIndex(pDX, IDC_COMBO_CAMERA, m_selectedCamera);


BEGIN_MESSAGE_MAP(CDlgAdd, CDialog)
	//{{AFX_MSG_MAP(CDlgAdd)
	ON_BN_CLICKED(IDC_btnBrowse, OnbtnBrowse)
	ON_CBN_SELCHANGE(IDC_COMBO_CAMERA, OnSelchangeComboCamera)
	ON_EN_CHANGE(IDC_EDIT_FOCAL, OnChangeEditFocal)
	ON_EN_CHANGE(IDC_EDIT_CCDSIZE, OnChangeEditCcdsize)
	ON_CBN_SELCHANGE(IDC_COMBO_DIRECTION, OnSelchangeComboDirection)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_btnCapture, OnBnClickedbtncapture)
	ON_BN_CLICKED(ID_ADD_CAMERA, OnBnClickedAddCamera)
	ON_BN_CLICKED(ID_VIEW_CAMERA_INFO, OnBnClickedViewCameraInfo)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAdd message handlers

void CDlgAdd::OnbtnBrowse() 
{
	//CString ms;ms.Format("%s",CurrentPath);MessageBox(ms);
      static char BASED_CODE szFilter[] = "Bitmap Files (*.bmp)|*.bmp||";
      // Create the File Open dialog
	  CString bmp_filename;
      CFileDialog m_ldFile(TRUE, ".bmp",CurrentPath+"*.bmp", 
          OFN_HIDEREADONLY |OFN_OVERWRITEPROMPT, szFilter);
	
     // Show the File Open dialog and capture the result
     if (m_ldFile.DoModal() == IDOK)
     {
		// Get the filename selected
         bmp_filename = m_ldFile.GetPathName();
		 //MessageBox(bmp_filename);
         // Load the selected bitmap file
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

				m_pictureBit=bm.bmBitsPixel;
				m_pictureHeight=bm.bmHeight;
				m_pictureWidth=bm.bmWidth;

				CString ms;

				m_fname=bmp_filename;
				m_picName=m_ldFile.GetFileTitle();

				CString duplicate_picture;
				if(ValidateImage(0,&duplicate_picture))//chek for duplicate image file and name
				{//if duplicate file
					ms.Format("Selected file=>%s\nThis file already assign to picture=%s\nPlease select another file.",
						m_fname,duplicate_picture);
					MessageBox(ms);
				}
				else
				{//if OK

					UpdateData(false);
					EnabledAll();
				}
				CurrentPath=m_ldFile.GetPathName();
				CurrentPath=CurrentPath.Left(CurrentPath.GetLength()-m_ldFile.GetFileName().GetLength());

		 } //if (hBitmap)
		 else  //if can not read image to memory
		 {
			 CString ms;
			 ms.Format("%s\nReading file error!\nSelected file may not be the bitmap file.",bmp_filename);
			 MessageBox(ms);
		 }
	 }
}


BOOL CDlgAdd::OnInitDialog() 
{
	CDialog::OnInitDialog(); //call to prevent assertion failed


	this->SetWindowText(dlgTitle);
	AddCameraToList();

	CString ms;
	ms.Format("%6.2f",atan(camera_cal/(m_focalLength*2))*2*180/acos(double(-1)));
	m_viewangle.SetWindowText(ms);

	if (callType==0)
	{//if add new image
		m_combocamera.SelectString(-1,CameraName); //get selected camera name
		camera_cal=pCameraPool->GetCalibrationValue(CameraName); //get calibration value
		UpdateData(false);
		DisableAll();
	}
	else 
	{//if edit parameter
		EnabledAll();
		UpdateData(true);
		m_combocamera.SelectString(-1,CameraName); //get selected camera name
	}
	return TRUE;  
}


void CDlgAdd::AddCameraToList()
{
	//m_combocamera.AddString("Custom");
	for(unsigned int i=0;i<pCameraPool->vctCamera.size();i++)
	{
		this->m_combocamera.AddString(pCameraPool->vctCamera[i].CameraName);
	}
}

void CDlgAdd::DisableAll()
{
	CEditView* pEV;
	pEV = (CEditView*) GetDlgItem(IDOK);
    pEV->EnableWindow(FALSE);

	pEV = (CEditView*) GetDlgItem(IDC_EDIT_FNAME);
    pEV->EnableWindow(FALSE);

	pEV = (CEditView*) GetDlgItem(IDC_EDIT_PICNAME);
    pEV->EnableWindow(FALSE);

    pEV = (CEditView*) GetDlgItem(IDC_EDIT_DISTANCE);
    pEV->EnableWindow(FALSE);

    pEV = (CEditView*) GetDlgItem(IDC_EDIT_HEIGHT);
    pEV->EnableWindow(FALSE);

	pEV = (CEditView*) GetDlgItem(IDC_EDIT_DIRECTION);
    pEV->EnableWindow(FALSE);

	pEV = (CEditView*) GetDlgItem(IDC_EDIT_INC);
    pEV->EnableWindow(FALSE);

	pEV = (CEditView*) GetDlgItem(IDC_EDIT_BENDING);
    pEV->EnableWindow(FALSE);

	pEV = (CEditView*) GetDlgItem(IDC_EDIT_ROT);
    pEV->EnableWindow(FALSE);

	pEV = (CEditView*) GetDlgItem(IDC_EDIT_CCDSIZE);
    pEV->EnableWindow(FALSE);

	pEV = (CEditView*) GetDlgItem(IDC_EDIT_FOCAL);
    pEV->EnableWindow(FALSE);

	CListBox *pLB;
	pLB = (CListBox*) GetDlgItem(IDC_COMBO_CAMERA);
    pLB->EnableWindow(FALSE);

	pLB = (CListBox*) GetDlgItem(IDC_COMBO_DIRECTION);
    pLB->EnableWindow(FALSE);
}



void CDlgAdd::EnabledAll()
{

	CEditView* pEV;
	pEV = (CEditView*) GetDlgItem(IDOK);
    pEV->EnableWindow(TRUE);

	pEV = (CEditView*) GetDlgItem(IDC_EDIT_FNAME);
    pEV->EnableWindow(TRUE);

	pEV = (CEditView*) GetDlgItem(IDC_EDIT_PICNAME);
    pEV->EnableWindow(TRUE);

    pEV = (CEditView*) GetDlgItem(IDC_EDIT_DISTANCE);
    pEV->EnableWindow(TRUE);

    pEV = (CEditView*) GetDlgItem(IDC_EDIT_HEIGHT);
    pEV->EnableWindow(TRUE);

	pEV = (CEditView*) GetDlgItem(IDC_EDIT_INC);
    pEV->EnableWindow(TRUE);

	
	if(this->m_selectedDirection==8)
	{
		pEV = (CEditView*) GetDlgItem(IDC_EDIT_DIRECTION);
		pEV->EnableWindow(TRUE);
	}

/*
	pEV = (CEditView*) GetDlgItem(IDC_EDIT_ROT);
    pEV->EnableWindow(TRUE);

	pEV = (CEditView*) GetDlgItem(IDC_EDIT_BENDING);
    pEV->EnableWindow(TRUE);


	pEV = (CEditView*) GetDlgItem(IDC_EDIT_CCDSIZE);
    pEV->EnableWindow(TRUE);
*/

	pEV = (CEditView*) GetDlgItem(IDC_EDIT_FOCAL);
    pEV->EnableWindow(TRUE);

	//CButton *pBtn;
	//pBtn = (CButton*) GetDlgItem(IDC_PICTYPE);
    //pBtn->EnableWindow(TRUE);
	//pBtn = (CButton*) GetDlgItem(IDC_RADIO2);
    //pBtn->EnableWindow(TRUE);

	CListBox *pLB;
	pLB = (CListBox*) GetDlgItem(IDC_COMBO_CAMERA);
    pLB->EnableWindow(TRUE);

	pLB = (CListBox*) GetDlgItem(IDC_COMBO_DIRECTION);
    pLB->EnableWindow(TRUE);

}

void CDlgAdd::OnOK() 
{
	if(callType==0) //if add image
	{
		UpdateData(TRUE);
		if (m_fname!="") 
		{
			if (m_picName!="")
			{
				//UpdateData(FALSE); //put entered value in the variables
				CString temp;
				if(ValidateImage(1,&temp))//chek for duplicate image file and name
				{
					MessageBox("Duplicated image name!\nPlease assign another name.");
				} 
				else 
				{
					CDialog::OnOK();
				}
			}
			else {MessageBox("No Picture name");}
		}
		else {MessageBox("No input file name");}
	}
	else //edit properties
	{
		UpdateData(TRUE); //send data form dialog to variables
		CDialog::OnOK();
	}
}


bool CDlgAdd::ValidateImage(bool option,CString *picname)
{//chek for duplicate imagefile(option=0) and name(other option)
	if (option==0)
	{
		for (int i=0;i<tempImagePool->GetImageCount();i++)
		{
			if(tempImagePool->GetImage(i)->GetFileName()==m_fname) 
			{*picname=tempImagePool->GetImage(i)->GetPictureName();return 1;}
		}
	}
	else
	{
		for (int i=0;i<tempImagePool->GetImageCount();i++)
		{
			if(tempImagePool->GetImage(i)->GetPictureName()==m_picName) {return 1;}
		}
	}
	return 0;
	
}

void CDlgAdd::OnSelchangeComboCamera() 
{
	UpdateData(true);
	CString ms;
	m_combocamera.GetWindowText(ms); //get selected camera name
	this->CameraName=ms; //put camera name to variable
	camera_cal=pCameraPool->GetCalibrationValue(CameraName); //get calibration value

	ms.Format("%6.2f",atan(camera_cal/(this->m_focalLength*2))*2*180/acos(-1.0));
	m_viewangle.SetWindowText(ms);
	m_viewangle.UpdateWindow();

	UpdateData(false);
	this->m_editCCDSize.EnableWindow(FALSE);

}//void CDlgAdd::OnSelchangeComboCamera() 


void CDlgAdd::OnChangeEditFocal() 
{
	CString ms;
	m_focalCtrl.GetWindowText(ms);
	double focal=atof(ms);
	m_editCCDSize.GetWindowText(ms);
	double ccd=atof(ms);
	ms.Format("%6.2f",atan(ccd/(focal*2))*2*180/acos(-1.0));
	m_viewangle.SetWindowText(ms);
	m_viewangle.UpdateWindow();
	
}//void CDlgAdd::OnChangeEditFocal() 

void CDlgAdd::OnChangeEditCcdsize() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	OnChangeEditFocal();
}

void CDlgAdd::OnSelchangeComboDirection() 
{
	int ans=m_combodirection.GetCurSel();

	CEditView* pEV;
	if (ans==8) //user select custom direction
	{
		pEV = (CEditView*) GetDlgItem(IDC_EDIT_DIRECTION);
		pEV->EnableWindow(TRUE);
	}
	else
	{
		CString sdir;
		switch (ans)
		{
		case 0:
			sdir="0";
			break;
		case 1:
			sdir="45";
			break;
		case 2:
			sdir="90";
			break;
		case 3:
			sdir="135";
			break;
		case 4:
			sdir="180";
			break;
		case 5:
			sdir="225";
			break;
		case 6:
			sdir="270";
			break;
		case 7:
			sdir="315";
			break;

		}
		m_editDirection.SetWindowText(sdir);
		m_editDirection.UpdateWindow();
		pEV = (CEditView*) GetDlgItem(IDC_EDIT_DIRECTION);
		pEV->EnableWindow(FALSE);
	}
}

void CDlgAdd::OnBnClickedbtncapture()
{
	CCaptureDlg dlg;
	dlg.DoModal();

}



void CDlgAdd::OnBnClickedAddCamera()
{
	CAddCameraDlg dlg;
	dlg.dlg_title="Add new camera";
	dlg.show_option=0;

	if(dlg.DoModal()==IDOK)
	{
		CEachCamera temp_camera;

		temp_camera.CameraName=dlg.m_camera_name;
		temp_camera.calibration_value=dlg.m_calibration_value;
		temp_camera.clibration_date=dlg.m_calibration_date;
		temp_camera.minimum_fc=dlg.m_min_fc;
		temp_camera.maximum_fc=dlg.m_max_fc;
		temp_camera.owner=dlg.m_owner;
		temp_camera.note=dlg.m_note;

		pCameraPool->vctCamera.push_back(temp_camera);
		m_combocamera.AddString(temp_camera.CameraName);
		m_combocamera.SelectString(-1,temp_camera.CameraName);
		pCameraPool->SaveCameras();

		this->OnSelchangeComboCamera();
	}
}

void CDlgAdd::OnBnClickedViewCameraInfo()
{
	CEachCamera *pCamera=pCameraPool->GetpCamera(this->CameraName);
	CAddCameraDlg dlg;
	dlg.dlg_title=this->CameraName;
	dlg.show_option=1;

	dlg.m_camera_name=pCamera->CameraName;
	dlg.m_calibration_value=pCamera->calibration_value;
	dlg.m_calibration_date=pCamera->clibration_date;
	dlg.m_min_fc=pCamera->minimum_fc;
	dlg.m_max_fc=pCamera->maximum_fc;
	dlg.m_owner=pCamera->owner;
	dlg.m_note=pCamera->note;

	if(dlg.DoModal()==IDOK)
	{
		pCamera->CameraName=dlg.m_camera_name;
		pCamera->calibration_value=dlg.m_calibration_value;
		pCamera->clibration_date=dlg.m_calibration_date;
		pCamera->minimum_fc=dlg.m_min_fc;
		pCamera->maximum_fc=dlg.m_max_fc;
		pCamera->owner=dlg.m_owner;
		pCamera->note=dlg.m_note;
		pCameraPool->SaveCameras();

		// Delete every other item from the combo box.
		while(m_combocamera.GetCount()>0)
		{
			m_combocamera.DeleteString(0);
		}

		this->AddCameraToList();
		m_combocamera.SelectString(-1,dlg.m_camera_name);
		this->OnSelchangeComboCamera();

	}
}//void CDlgAdd::OnBnClickedViewCameraInfo()
