#include "stdafx.h"
#include ".\camerapool.h"

CCameraPool::CCameraPool(void)
{
}

CCameraPool::~CCameraPool(void)
{
}

double CCameraPool::GetCalibrationValue(CString CameraName)
{
	for(unsigned int i=0;i<vctCamera.size();i++)
	{
		if(vctCamera[i].CameraName==CameraName){return vctCamera[i].calibration_value;}
	}
	return 0;
}
CEachCamera * CCameraPool::GetpCamera(CString camera_name)
{
	for(unsigned int i=0;i<vctCamera.size();i++)
	{
		if(vctCamera[i].CameraName==camera_name){return &vctCamera[i];}
	}
	return NULL;
}

bool CCameraPool::Cameras_Load()
{
	CFile theFile;
	CFileException e;
	if( !theFile.Open( filename.GetString(), CFile::modeRead, &e ) )
	{
		#ifdef _DEBUG
		afxDump << "File could not be opened " << e.m_cause << "\n";
		#endif
		return false;
	}

	CArchive ar(&theFile, CArchive::load);
	int total_camera=0;
	ar>>total_camera;

	CEachCamera temp_camera;
	for(int i=0;i<total_camera;i++)
	{
		ar>>temp_camera.CameraName;
		ar>>temp_camera.calibration_value;
		ar>>temp_camera.minimum_fc;
		ar>>temp_camera.maximum_fc;
		ar>>temp_camera.clibration_date;
		ar>>temp_camera.owner;
		ar>>temp_camera.note;

		vctCamera.push_back(temp_camera);
	}
	ar.Close();
	theFile.Close();
	return true;
}



void CCameraPool::SaveCameras()
{
	CFile theFile;
	CFileException e;
	if( !theFile.Open( filename.GetString(), CFile::modeCreate | CFile::modeWrite, &e ) )
	{
		#ifdef _DEBUG
		afxDump << "File could not be opened " << e.m_cause << "\n";
		#endif
		return;
	}
	CArchive ar(&theFile, CArchive::store);

	//write data
	unsigned int total_camera=vctCamera.size();
	ar<<total_camera;
	
	for(unsigned int i=0;i<total_camera;i++)
	{
		ar<<vctCamera[i].CameraName;
		ar<<vctCamera[i].calibration_value;
		ar<<vctCamera[i].minimum_fc;
		ar<<vctCamera[i].maximum_fc;
		ar<<vctCamera[i].clibration_date;
		ar<<vctCamera[i].owner;
		ar<<vctCamera[i].note;
	}
	
	ar.Close();
	theFile.Close();
}

void CCameraPool::LoadDefault()
{
	vctCamera.clear();

	CEachCamera temp_camera;

	//model1
	temp_camera.CameraName="Fuji FinePix1400";
	temp_camera.calibration_value=6.903;
	temp_camera.minimum_fc=6;
	temp_camera.maximum_fc=18;
	temp_camera.clibration_date="4-Jul-03";
	temp_camera.owner="Jessada PHATTARALERPHONG";
	temp_camera.note="Max resolution=1280x960";
	vctCamera.push_back(temp_camera);

	//model2
	temp_camera.CameraName="Casio QV-3500EX";
	temp_camera.calibration_value=9.3196;
	temp_camera.minimum_fc=7.13;
	temp_camera.maximum_fc=20.1;
	temp_camera.clibration_date="15-Jul-03";
	temp_camera.owner="Unakorn SILPI";
	temp_camera.note="Max resolution=2048x1536";
	vctCamera.push_back(temp_camera);

	//model3
	temp_camera.CameraName="Epson PhotoPC 3100Z";
	temp_camera.calibration_value=8.9623;
	temp_camera.minimum_fc=7;
	temp_camera.maximum_fc=20.7;
	temp_camera.clibration_date="22-Jul-03";
	temp_camera.owner="Prasert PHATTARALERPHONG";
	temp_camera.note="Max resolution=2048x1536";
	vctCamera.push_back(temp_camera);


	//model4
	temp_camera.CameraName="Minolta Dimage 7i";
	temp_camera.calibration_value=10.931;
	temp_camera.minimum_fc=7.2;
	temp_camera.maximum_fc=50.8;
	temp_camera.clibration_date="22-7-03";
	temp_camera.owner="Jate SATHORNKICH";
	temp_camera.note="Max resolution=2560x1920";
	vctCamera.push_back(temp_camera);

	//model5
	temp_camera.CameraName="Nikon CoolPix885";
	temp_camera.calibration_value=8.8532;
	temp_camera.minimum_fc=8;
	temp_camera.maximum_fc=24;
	temp_camera.clibration_date="19-Aug-03";
	temp_camera.owner="Sornprach THANISAWANYANGKURA";
	temp_camera.note="Max resolution=2048x1536";
	vctCamera.push_back(temp_camera);

	//model6
	temp_camera.CameraName="Sony CyberShot DSC-P50";
	temp_camera.calibration_value=6.4985;
	temp_camera.minimum_fc=6.4;
	temp_camera.maximum_fc=19.2;
	temp_camera.clibration_date="29-Aug-03";
	temp_camera.owner="Patchareeya BOONKORKAEW";
	temp_camera.note="";
	vctCamera.push_back(temp_camera);

	//model7
	temp_camera.CameraName="Olympus C2020Z";
	temp_camera.calibration_value=7.8036;
	temp_camera.minimum_fc=6.5;
	temp_camera.maximum_fc=19.5;
	temp_camera.clibration_date="26-Mar-04";
	temp_camera.owner="Phunsup SEUBMA";
	temp_camera.note="Max resolution=1600x1200";
	vctCamera.push_back(temp_camera);

	//model8
	temp_camera.CameraName="Nikon CoolPix4500";
	temp_camera.calibration_value=9.0602;
	temp_camera.minimum_fc=7.84;
	temp_camera.maximum_fc=32;
	temp_camera.clibration_date="28-May-04";
	temp_camera.owner="Pierre CRUIZAIT";
	temp_camera.note="Max resolution=2272x1704";
	vctCamera.push_back(temp_camera);

	//model9
	temp_camera.CameraName="Nikon E995";
	temp_camera.calibration_value=8.8481;
	temp_camera.minimum_fc=8;
	temp_camera.maximum_fc=32;
	temp_camera.clibration_date="7-Jul-04";
	temp_camera.owner="CEMAGREF";
	temp_camera.note="Max resolution=2048x1536";
	vctCamera.push_back(temp_camera);

	//model10
	temp_camera.CameraName="Nikon E4300";
	temp_camera.calibration_value=8.8363;
	temp_camera.minimum_fc=6;
	temp_camera.maximum_fc=40;
	temp_camera.clibration_date="16-Jul-04";
	temp_camera.owner="Pablo DUREN";
	temp_camera.note="";
	vctCamera.push_back(temp_camera);

	//model11
	temp_camera.CameraName="Cannon PowerShot A75";
	temp_camera.calibration_value=6.5598;
	temp_camera.minimum_fc=5.41;
	temp_camera.maximum_fc=16.2;
	temp_camera.clibration_date="19-Aug-04";
	temp_camera.owner="PEPEL";
	temp_camera.note="Max resolution=2048x1536";
	vctCamera.push_back(temp_camera);

	this->SaveCameras();

}
