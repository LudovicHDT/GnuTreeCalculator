// CaptureDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Tree Analyser.h"
#include "CaptureDlg.h"
#include ".\capturedlg.h"

//video
#include "Vfw.h"
HWND capvideo1 ;
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "vfw32.lib")

//serveur
#include <io.h> //ouverture et lecture des fichiers
#include <fcntl.h>//definit les constantes _readonly...
#include <time.h>//fonction sur les dates
#include <sys/stat.h>//pour les status
#include <stdio.h>


// CCaptureDlg dialog

IMPLEMENT_DYNAMIC(CCaptureDlg, CDialog)
CCaptureDlg::CCaptureDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCaptureDlg::IDD, pParent)
{
	b_button1_on=false;
	posx = 100 , posy = 10 , sizex = 320, sizey = 240;
	capture_status=0;
}

CCaptureDlg::~CCaptureDlg()
{
}

void CCaptureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ImageFrame, m_frame);
	DDX_Control(pDX, IDC_CaptureBtn, ctrlBtnCapture);
}


BEGIN_MESSAGE_MAP(CCaptureDlg, CDialog)
	ON_BN_CLICKED(IDC_CaptureBtn, OnBnClickedCapturebtn)
END_MESSAGE_MAP()


// CCaptureDlg message handlers

void CCaptureDlg::OnBnClickedCapturebtn()
{
	if(!capture_status)
	{
		CString Filter;
		CString Filename;
		Filter = "Bitmap Files (*.bmp)|*.bmp|Bitmap Files (*.bmp) tu es sourd :op|*.bmp||";
		CFileDialog FileDlg(FALSE, "BMP", NULL,
						OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
						Filter,
						this);
		capGrabFrame(capvideo1);
		
		if (FileDlg.DoModal() == IDOK)
		{
			RedrawWindow();
			Filename = FileDlg.GetPathName();

			if(!Filename.IsEmpty())
			{
				CString fichier_tempo_bmp = Filename;
				if(capFileSaveDIB(capvideo1,fichier_tempo_bmp.GetBuffer(0)))
				{
				}
				else
				{
					AfxMessageBox("Impossible de faire une capture");
				}
			}
		}
		ctrlBtnCapture.SetWindowText("Reflesh");
		ctrlBtnCapture.UpdateWindow();
		capture_status=1;
	}
	else
	{
		//capGrabFrameNoStop(capvideo1);
		//capSetCallbackOnFrame(capvideo1,NULL);
		capPreview(capvideo1,true);
		ctrlBtnCapture.SetWindowText(_T("My button"));
		ctrlBtnCapture.UpdateWindow();
		capture_status=0;
		UpdateWindow();
	}
}

BOOL CCaptureDlg::OnInitDialog()
{
	CDialog::OnInitDialog(); //call to prevent assertion failed

	m_frame.SetWindowPos(NULL,posx,posy,sizex,sizey,1);
	
	if (!b_button1_on)
	{
		//activation de la webcam
		//m_frame.SetWindowPos(NULL,posx,posy,sizex,sizey,1);

		// Affichage de la fenetre de parametrage de l'affichage (si le peripherique le permet)
		capvideo1 = capCreateCaptureWindow(NULL,WS_CHILD|WS_VISIBLE,posx,posy,sizex,sizey,m_hWnd,1) ;

		if(capvideo1)
		{
			if(capDriverConnect(capvideo1,0))
			{
				capPreview(capvideo1,true);//active l'image
				capPreviewRate(capvideo1,33);//vitesse de rafraichissement

				CAPDRIVERCAPS caracwebcam ;//pour recuperer les options des drivers de la webcam

				if(capDriverGetCaps(capvideo1,&caracwebcam,sizeof(caracwebcam)))
				{
				  
				   b_button1_on = true;
								
				}
			}else
			{
				AfxMessageBox("Unable to locate the camera!\r\nPlease make sure that the camera is connect to the computer.");
				OnOK();
			}
		}else	
		{
			MessageBox("impossible de creer la capture d'image");
			if(capvideo1) ::DestroyWindow(capvideo1);
		}
		
	}

	return TRUE;
}