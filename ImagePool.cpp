// ImagePool.cpp: implementation of the CImagePool class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Tree Analyser.h"
#include "ImagePool.h"
#include "EachImage.h"
#include ".\imagepool.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// IMPLEMENT_SERIAL (CImagePool, CObject, 1)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


//! A constructor.
/*!
   Default value of the project are setup here.
*/
CImagePool::CImagePool()
: m_mean_leaf_inc(0)
, m_inversion_method(0)
{
	vctImage.empty();
	cellSize_dx=0.2;
	cellSize_dy=0.2;
	cellSize_dz=0.2;
	bounding_x=1;
	bounding_y=1;
	bounding_z=1;
	raypx=3;
	raypy=3;
	m_enlarge=0.05;
	maxTreeHeight=0; minTreeHeight=0;
	maxVHeight=0; minVHeight=0;
	maxDiameter=0; minDiameter=0;
	averageTreeHeight=0; averageVHeight=0; averageDiameter=0;
	m_ifoutputbox=FALSE;
	m_iflogvolume=TRUE;
	m_LimitEle=45;
	
	//setting for leaf area estimation
	m_mean_leaf_area=50;
	gap_option=0;
//	lapx=3;
//	lapy=3;
	m_leaf_distribution=5;
	gpx=100; 
	gpy=100;
	gap_average_option=0;
	m_mean_leaf_inc=57;
	LeafDistributionFile="";
	ZeroGapManage=1; minimumGap=0.001;
	IfSetMaxLAD=1; MaxLAD=30;
	m_leaf_dispersion=1;
	m_leaf_dispersion_option=1;
	m_inversion_method=1;
	EsimatedLeafArea=0;
	ActivePath="";
	fix_pza=17;

	//Variable added for subdivision of PZA
	is_divide_pza=true;
	division_horizontal=2;
	division_vertical=2;

	//setting for LAD computatiion
	ifCalculVprofile=1;
}

CImagePool::~CImagePool()
{

}


//! Add image data to the array.
/*!
*	Last modification: 9-Sep-04.\n
*	Called by:\n
*	-# CImagePool::OpenTextData(CString *fname)
*	-# CTreeAnalyserDlg::OnProjectAddimage()
*/
void CImagePool::PutImage(CEachImage * pImage)
{
	vctImage.push_back(*pImage);
}

void CImagePool::DisplayImageInformation(unsigned int ipic)
{	
	if (ipic>vctImage.size()+1||ipic<0) {MessageBox(NULL,"No specify image","Error",0);}
	else
	{
		vctImage[ipic-1].DisplayImageInformation();
	}

}

CEachImage * CImagePool::GetImage(int imageID)
{
	return &vctImage[imageID];
}

CEachImage * CImagePool::GetImage(CString *picName)
{
		unsigned int i=0;
		while (vctImage[i].GetPictureName()!=*picName && i<vctImage.size())
		{
			i++;
		}
		return &vctImage[i];
}


void CImagePool::ClearData()
{
	vctImage.clear();
}

CString CImagePool::GetPictureFileName(CString picName)
{
		int i=0;
		while (vctImage[i].GetPictureName()!=picName)
		{
			i++;
		}
		return vctImage[i].GetFileName();
}


bool CImagePool::SaveNewFormat(CString *fname)
{
	CFile theFile;
	CFileException e;
	if( !theFile.Open( fname->GetString(), CFile::modeCreate | CFile::modeWrite, &e ) )
	{
		#ifdef _DEBUG
		afxDump << "File could not be opened " << e.m_cause << "\n";
		#endif
		return false;
	}
	CArchive ar(&theFile, CArchive::store);

	//write data

	//1. version marker
	CString ms="TA"; double version=1.20;
	ar<<ms<<version;


	//2. system setting

	//3. normal setting
	ar<<cellSize_dx<<cellSize_dy<<cellSize_dz; //voxel size
	ar<<m_inversion_method; //inversion method
	ar<<m_mean_leaf_area; //mean leaf area
	ar<<this->m_leaf_distribution;//leaf angle distribution type
	ar<<this->LeafDistributionFile; //leaf angle distribution file (if custom was selected)
	ar<<this->m_mean_leaf_inc; //mean leaf inclination (if conical was selected)
	ar<<this->ifCalculVprofile;//chk vprofile
	ar<<m_iflogvolume; //output log
	ar<<m_ifoutputbox; //output VegeSTAR voxels


	//4. Advance setting
	ar<<this->m_LimitEle; //limit elevation (deg)
	ar<<m_enlarge; //enlarge bounding box (%)
	ar<<raypx<<raypy; //beam spacing for volume reconstruction
	ar<<this->IfSetMaxLAD;
	ar<<this->MaxLAD;
	ar<<this->ZeroGapManage; //zero gap management option
	ar<<this->minimumGap; //minimum gap fraction if fix minimum gap is set
	ar<<this->gap_option; //zoning size option
	ar<<gpx<<gpy; //size of zoning if custom zoning is selected
	ar<<this->fix_pza;
	ar<<this->is_divide_pza;
	ar<<this->division_horizontal;
	ar<<this->division_vertical;

	//clumping option is unavailable

	//5. Each image data
	unsigned int imageCount=vctImage.size();
	ar<<imageCount;
 	CEachImage *pWorkImage=new CEachImage;
	for (unsigned int i=0;i<imageCount;i++)
	{
		pWorkImage= &(vctImage[i]); //get data form image pool
		
		ar<<pWorkImage->GetPictureName();
		ar<<pWorkImage->GetFileName();
		ar<<pWorkImage->camera_name;
		ar<<pWorkImage->GetCameraCCDSize();
		ar<<pWorkImage->GetCameraDirection();
		ar<<pWorkImage->GetCameraDistance();
		ar<<pWorkImage->GetCameraFocalLength();
		ar<<pWorkImage->GetCameraHeight();
		ar<<pWorkImage->GetCameraInc();
		ar<<pWorkImage->GetCameraRot();
		ar<<pWorkImage->GetCameraBending();
		ar<<pWorkImage->GetPictureBit();
		ar<<pWorkImage->GetPictureHeight();
		ar<<pWorkImage->GetPictureWidth();
		ar<<pWorkImage->GetCheck();
		ar<<pWorkImage->GetSelectedDirection();

	}
	ar.Close();
	theFile.Close();
	return true;
}//bool CImagePool::SaveNewFormat(CString *fname)

bool CImagePool::OpenNewFormat(CString *fname)
{
	CFile theFile;
	CFileException e;
	if( !theFile.Open( fname->GetString(), CFile::modeRead, &e ) )
	{
		#ifdef _DEBUG
		afxDump << "File could not be opened " << e.m_cause << "\n";
		#endif
		return false;
	}

	CArchive ar(&theFile, CArchive::load);
	CString ms;
	ar>>ms;
	if(ms!="TA"){AfxMessageBox("Selected file is not Tree Analyser file!");return false;}

	if(ar.IsBufferEmpty()){return false;}
	double version=0;
	ar>>version;


	//2. system setting

	//3. normal setting
	ar>>cellSize_dx; ar>>cellSize_dy;ar>>cellSize_dz; //voxel size
	ar>>m_inversion_method; //inversion method
	ar>>m_mean_leaf_area; //mean leaf area
	ar>>this->m_leaf_distribution;//leaf angle distribution type
	ar>>this->LeafDistributionFile;
	ar>>this->m_mean_leaf_inc;
	ar>>this->ifCalculVprofile;//chk vprofile
	ar>>m_iflogvolume; //output log
	ar>>m_ifoutputbox; //output VegeSTAR voxels


	//4. Advance setting
	ar>>this->m_LimitEle; //limit elevation (deg)
	ar>>m_enlarge; //enlarge bounding box
	ar>>raypx; ar>>raypy; //beam spacing for volume reconstruction
	ar>>this->IfSetMaxLAD; //if fixed maximum LAD is set
	ar>>this->MaxLAD; //value of maximum LAD
	ar>>this->ZeroGapManage; //zero gap management option
	ar>>this->minimumGap; //minimum gap fraction if fix minimum gap is set
	ar>>this->gap_option; //zoning size option
	ar>>gpx;ar>>gpy; //size of zoning if custom zoning is selected
	if (version==1.20)
	{
		ar>>fix_pza;
		ar>>this->is_divide_pza;
		ar>>this->division_horizontal;
		ar>>this->division_vertical;
	}

	//clumping option is unavailable


	//5. Each image data
	unsigned int imageCount=0;
	ar>>imageCount;
	for (unsigned int i=0;i<imageCount;i++)
	{
		CEachImage tempImage;
		double tempDouble;
		CString tempString;
		int tempInt;
		bool tempBool;

		ar>>tempString;tempImage.PutPictureName(tempString);
		ar>>tempString;tempImage.PutFileName(tempString);
		ar>>tempString;tempImage.camera_name=tempString;
//		ar>>tempInt;tempImage.PutSelectCamera(tempInt);
		ar>>tempDouble;tempImage.PutCameraCCDSize(tempDouble);
		ar>>tempDouble;tempImage.PutCameraDirection(tempDouble);
		ar>>tempDouble;tempImage.PutCameraDistance(tempDouble);
		ar>>tempDouble;tempImage.PutCameraFocalLength(tempDouble);
		ar>>tempDouble;tempImage.PutCameraHeight(tempDouble);
		ar>>tempDouble;tempImage.PutCameraInc(tempDouble);
		ar>>tempDouble;tempImage.PutCameraRot(tempDouble);
		ar>>tempDouble;tempImage.PutCameraBending(tempDouble);
		ar>>tempInt;tempImage.PutPictureBit(tempInt);
		ar>>tempInt;tempImage.PutPictureHeight(tempInt);
		ar>>tempInt;tempImage.PutPictureWidth(tempInt);
		ar>>tempBool;tempImage.PutCheck(tempBool);
		ar>>tempInt;tempImage.PutSelectedDirection(tempInt);

		vctImage.push_back(tempImage);

	}

	ar.Close();
	theFile.Close();

	//CString str;
	//str.Format("Program=%s  version=%2.2f",ms,version);
	//AfxMessageBox(str);

	return true;
}
/*
bool CImagePool::SaveData(CString *fname)
{
 	CEachImage *pWorkImage=new CEachImage;
	FILE * pFile;
	
	pFile=fopen(*fname,"wt");

	//save setting
	fprintf(pFile,"TA	1.07\n");
	fprintf(pFile,"%i\n",this->m_LimitEle);
	fprintf(pFile,"%6.4f\n",m_enlarge);
	fprintf(pFile,"%i\n",raypx);
	fprintf(pFile,"%i\n",raypy);
	fprintf(pFile,"%i\n",m_ifoutputbox);
	fprintf(pFile,"%i\n",m_iflogvolume);

	fprintf(pFile,"%6.4f\n",cellSize_dx);
	fprintf(pFile,"%6.4f\n",cellSize_dy);
	fprintf(pFile,"%6.4f\n",cellSize_dz);

	//opions for leaf area estimation
//	fprintf(pFile,"%i\n",lapx);
//	fprintf(pFile,"%i\n",lapy);
	fprintf(pFile,"%i\n",gpx);
	fprintf(pFile,"%i\n",gpy);
	fprintf(pFile,"%i\n",gap_average_option);
	fprintf(pFile,"%i\n",m_leaf_distribution);
	fprintf(pFile,"%6.4f\n",this->m_mean_leaf_inc);
	fprintf(pFile,"%s\n",this->LeafDistributionFile);
	fprintf(pFile,"%i\n",ZeroGapManage);
	fprintf(pFile,"%8.6f\n",minimumGap);
	fprintf(pFile,"%i\n",IfSetMaxLAD);
	fprintf(pFile,"%6.2f\n",MaxLAD);
	fprintf(pFile,"%i\n",m_leaf_dispersion_option);
	fprintf(pFile,"%6.4f\n",m_leaf_dispersion);
	fprintf(pFile,"%6.4f\n",m_mean_leaf_area);
	fprintf(pFile,"%i\n",gap_option);
	fprintf(pFile,"%i\n",this->m_inversion_method);
	fprintf(pFile,"%i\n",this->ifCalculVprofile);


	//save data of each image
	for (unsigned int i=0;i<vctImage.size();i++)
	{
		pWorkImage= &(vctImage[i]); //get data form image pool
		
		//store data for each image
		fprintf(pFile,"%s\n",pWorkImage->GetPictureName());
		fprintf(pFile,"%s\n",pWorkImage->GetFileName());
		fprintf(pFile,"%6.2f\n",pWorkImage->GetCameraBending());
		fprintf(pFile,"%6.2f\n",pWorkImage->GetCameraCCDSize());
		fprintf(pFile,"%6.2f\n",pWorkImage->GetCameraDirection());
		fprintf(pFile,"%6.2f\n",pWorkImage->GetCameraDistance());
		fprintf(pFile,"%6.2f\n",pWorkImage->GetCameraFocalLength());
		fprintf(pFile,"%6.2f\n",pWorkImage->GetCameraHeight());
		fprintf(pFile,"%6.2f\n",pWorkImage->GetCameraInc());
		fprintf(pFile,"%6.2f\n",pWorkImage->GetCameraRot());
		fprintf(pFile,"%i\n",pWorkImage->GetPictureBit());
		fprintf(pFile,"%i\n",pWorkImage->GetCheck());
//		fprintf(pFile,"%i\n",pWorkImage->GetSelectCamera());
		fprintf(pFile,"%i\n",pWorkImage->GetSelectedDirection());

//		pWorkImage->DisplayImageInformation();
	}
	fclose(pFile);
	CString ms;
	//ms.Format("%i image saved",iImageCount);
	//MessageBox(NULL,ms,"Tree Analyser",0);
	return 1;
}
*/

bool CImagePool::OpenTextData(CString *fname)
{
 	CEachImage *pWorkImage=new CEachImage;
	char seps[]   = "\n"; //seperation of each value by line
	char *token;
	char buffer[256];
	FILE *datafile;
	datafile= fopen(*fname,"r");
	int n=0,i=0; //, tempi=0;
	//float tempf=0;
	//double tempd=0;
	double ver=0;
	CString ms;
	if (datafile!=NULL)
	{
		CString filePath=this->GetDirectoryName(*fname);
		CString ss;
		fgets(buffer,255,datafile);
		token = strtok( buffer, "\t" );
		if(token != NULL){ss=token;}
		//check if is Tree Analyser file
		if (ss!="TA") 
		{
			MessageBox(NULL,"Selected file is not Tree Analyser file!","Tree Analyser",0);
			fclose(datafile);
			return 0;
		}
		//check for version of file
		token = strtok( NULL, seps );
		if(token != NULL){ver=atof(token);}
		//ss.Format("version=%6.2f",ver);
		//MessageBox(NULL,ss,"Tree Analyser",0);

		if(ver==1.0 || ver==1.01 || ver==1.02 || ver==1.03 || ver==1.04)
		{
			int tempbool=0;

			fgets(buffer,255,datafile);
			token = strtok( buffer, seps );
			if(token != NULL){m_LimitEle=atoi(token);}

			fgets(buffer,255,datafile);
			token = strtok( buffer, seps );
			if(token != NULL){m_enlarge=atof(token);}

			fgets(buffer,255,datafile);
			token = strtok( buffer, seps );
			if(token != NULL){raypx=atoi(token);}

			fgets(buffer,255,datafile);
			token = strtok( buffer, seps );
			if(token != NULL){raypy=atoi(token);}

			fgets(buffer,255,datafile);
			token = strtok( buffer, seps );
			if(token != NULL){m_ifoutputbox=atoi(token);}

			fgets(buffer,255,datafile);
			token = strtok( buffer, seps );
			if(token != NULL){m_iflogvolume=atoi(token);}



			fgets(buffer,256,datafile);
			token = strtok( buffer, seps );
			if(token != NULL){cellSize_dx=atof(token);}

			fgets(buffer,256,datafile);
			token = strtok( buffer, seps );
			if(token != NULL){cellSize_dy=atof(token);}

			fgets(buffer,256,datafile);
			token = strtok( buffer, seps );
			if(token != NULL){cellSize_dz=atof(token);}

			fgets(buffer,256,datafile);
			token = strtok( buffer, seps );
			if(token != NULL){}
			//if(token != NULL){lapx=atoi(token);}

			fgets(buffer,256,datafile);
			token = strtok( buffer, seps );
			if(token != NULL){}
			//if(token != NULL){lapy=atoi(token);}

			fgets(buffer,256,datafile);
			token = strtok( buffer, seps );
			if(token != NULL){gpx=atoi(token);}

			fgets(buffer,256,datafile);
			token = strtok( buffer, seps );
			if(token != NULL){gpy=atoi(token);}

			fgets(buffer,256,datafile);
			token = strtok( buffer, seps );
			if(token != NULL){gap_average_option=atoi(token);}

			fgets(buffer,256,datafile);
			token = strtok( buffer, seps );
			if(token != NULL){m_leaf_distribution=atoi(token);}

			fgets(buffer,256,datafile);
			token = strtok( buffer, seps );
			if(token != NULL){m_mean_leaf_inc=atof(token);}

			fgets(buffer,256,datafile);
			token = strtok( buffer, seps );
			if(token != NULL){LeafDistributionFile=token;}

			fgets(buffer,256,datafile);
			token = strtok( buffer, seps );
			if(token != NULL){ZeroGapManage=atoi(token);}

			fgets(buffer,256,datafile);
			token = strtok( buffer, seps );
			if(token != NULL){minimumGap=atof(token);}

			fgets(buffer,256,datafile);
			token = strtok( buffer, seps );
			if(token != NULL){tempbool=atoi(token);}
			if (tempbool==1){IfSetMaxLAD=1;} else {IfSetMaxLAD=0;}

			fgets(buffer,256,datafile);
			token = strtok( buffer, seps );
			if(token != NULL){MaxLAD=atof(token);}

			if(ver>1.00)
			{
				fgets(buffer,256,datafile);
				token = strtok( buffer, seps );
				if(token != NULL){m_leaf_dispersion_option=atoi(token);}

				fgets(buffer,256,datafile);
				token = strtok( buffer, seps );
				if(token != NULL){m_leaf_dispersion=atof(token);}
			}
			else {m_leaf_dispersion_option=1;m_leaf_dispersion=1;}

			if(ver==1.02 || ver==1.03 || ver==1.04)
			{
				fgets(buffer,256,datafile);
				token = strtok( buffer, seps );
				if(token != NULL){m_mean_leaf_area=atof(token);}

				fgets(buffer,256,datafile);
				token = strtok( buffer, seps );
				if(token != NULL){gap_option=atoi(token);}
			}
			else 
			{
				m_mean_leaf_area=1;
				gap_option=0;
			}

			if(ver==1.03 || ver==1.04)
			{
				//AfxMessageBox("version 1.03");
				fgets(buffer,256,datafile);
				token = strtok( buffer, seps );	
				if(atoi(token)>1){m_inversion_method=1;} //prevent old version with negtive binomial option
				else {m_inversion_method=atoi(token);}			
			}
			else
			{
				m_inversion_method=0;
			}

			if(ver==1.04)
			{
				//AfxMessageBox("version 1.03");
				fgets(buffer,256,datafile);
				token = strtok( buffer, seps );
				if(token != NULL){tempbool=atoi(token);}
				if (tempbool==1){ifCalculVprofile=1;} else {ifCalculVprofile=0;}
			}
			else
			{
				this->ifCalculVprofile=1;
			}

			i=0;
			while (!feof(datafile))
			{
				fgets(buffer,255,datafile);
					
				token = strtok( buffer, seps );// Establish string and get the first token: 
				if(token != NULL)
				{
					i++;
					switch(i)
					{
					case 1:
						pWorkImage->PutPictureName(token);
						break;
					case 2:
						pWorkImage->PutFileName(token);
						break;
					case 3:
						pWorkImage->PutCameraBending(atof(token));
						break;
					case 4:
						pWorkImage->PutCameraCCDSize(atof(token));
						break;
					case 5:
						pWorkImage->PutCameraDirection(atof(token));
						break;
					case 6:
						pWorkImage->PutCameraDistance(atof(token));
						break;
					case 7:
						pWorkImage->PutCameraFocalLength(atof(token));
						break;
					case 8:
						pWorkImage->PutCameraHeight(atof(token));
						break;
					case 9:
						pWorkImage->PutCameraInc(atof(token));
						break;
					case 10:
						pWorkImage->PutCameraRot(atof(token));
						break;
					case 11:
						pWorkImage->PutPictureBit(atoi(token));
						break;
					case 12:
						atoi(token);
						break;
					case 13:
						//pWorkImage->PutPictureType(atoi(token));
						break;
					case 14:
						atoi(token);
						break;
					case 15:
						if(atoi(token)==1){pWorkImage->PutCheck(1);}
						else {pWorkImage->PutCheck(0);}
						
						break;
					case 16:
						//pWorkImage->PutSelectCamera(atoi(token));
						pWorkImage->camera_name=this->GetOldCameraName(atoi(token));
						break;
					case 17:
						pWorkImage->PutSelectedDirection(atoi(token));
						break;
					}
				}
				if (i==17) 
				{
					n++;i=0;
					//PutImage(pWorkImage);
					if(pWorkImage->ReadImageDimension(filePath)){PutImage(pWorkImage);}
				}
			}
		}//if(ver==1.0 || ver==1.01 || ver==1.02 || ver==1.03 || ver==1.04)


		if(ver==1.07)
		{
			int tempbool=0;

			fgets(buffer,255,datafile);
			token = strtok( buffer, seps );
			if(token != NULL){m_LimitEle=atoi(token);}

			fgets(buffer,255,datafile);
			token = strtok( buffer, seps );
			if(token != NULL){m_enlarge=atof(token);}

			fgets(buffer,255,datafile);
			token = strtok( buffer, seps );
			if(token != NULL){raypx=atoi(token);}

			fgets(buffer,255,datafile);
			token = strtok( buffer, seps );
			if(token != NULL){raypy=atoi(token);}

			fgets(buffer,255,datafile);
			token = strtok( buffer, seps );
			if(token != NULL){m_ifoutputbox=atoi(token);}

			fgets(buffer,255,datafile);
			token = strtok( buffer, seps );
			if(token != NULL){m_iflogvolume=atoi(token);}



			fgets(buffer,256,datafile);
			token = strtok( buffer, seps );
			if(token != NULL){cellSize_dx=atof(token);}

			fgets(buffer,256,datafile);
			token = strtok( buffer, seps );
			if(token != NULL){cellSize_dy=atof(token);}

			fgets(buffer,256,datafile);
			token = strtok( buffer, seps );
			if(token != NULL){cellSize_dz=atof(token);}

			fgets(buffer,256,datafile);
			token = strtok( buffer, seps );
			if(token != NULL){}
//			if(token != NULL){lapx=atoi(token);}

			fgets(buffer,256,datafile);
			token = strtok( buffer, seps );
			if(token != NULL){}
			//if(token != NULL){lapy=atoi(token);}

			fgets(buffer,256,datafile);
			token = strtok( buffer, seps );
			if(token != NULL){gpx=atoi(token);}

			fgets(buffer,256,datafile);
			token = strtok( buffer, seps );
			if(token != NULL){gpy=atoi(token);}

			fgets(buffer,256,datafile);
			token = strtok( buffer, seps );
			if(token != NULL){gap_average_option=atoi(token);}

			fgets(buffer,256,datafile);
			token = strtok( buffer, seps );
			if(token != NULL){m_leaf_distribution=atoi(token);}

			fgets(buffer,256,datafile);
			token = strtok( buffer, seps );
			if(token != NULL){m_mean_leaf_inc=atof(token);}

			fgets(buffer,256,datafile);
			token = strtok( buffer, seps );
			if(token != NULL){LeafDistributionFile=token;}

			fgets(buffer,256,datafile);
			token = strtok( buffer, seps );
			if(token != NULL){ZeroGapManage=atoi(token);}

			fgets(buffer,256,datafile);
			token = strtok( buffer, seps );
			if(token != NULL){minimumGap=atof(token);}

			fgets(buffer,256,datafile);
			token = strtok( buffer, seps );
			if(token != NULL){tempbool=atoi(token);}
			if (tempbool==1){IfSetMaxLAD=1;} else {IfSetMaxLAD=0;}

			fgets(buffer,256,datafile);
			token = strtok( buffer, seps );
			if(token != NULL){MaxLAD=atof(token);}

			fgets(buffer,256,datafile);
			token = strtok( buffer, seps );
			if(token != NULL){m_leaf_dispersion_option=atoi(token);}

			fgets(buffer,256,datafile);
			token = strtok( buffer, seps );
			if(token != NULL){m_leaf_dispersion=atof(token);}

			fgets(buffer,256,datafile);
			token = strtok( buffer, seps );
			if(token != NULL){m_mean_leaf_area=atof(token);}

			fgets(buffer,256,datafile);
			token = strtok( buffer, seps );
			if(token != NULL){gap_option=atoi(token);}

			fgets(buffer,256,datafile);
			token = strtok( buffer, seps );
			if(atoi(token)>1){m_inversion_method=1;} //prevent old version with negtive binomial option
			else {m_inversion_method=atoi(token);}			

			fgets(buffer,256,datafile);
			token = strtok( buffer, seps );
			if(token != NULL){tempbool=atoi(token);}
			if (tempbool==1){ifCalculVprofile=1;} else {ifCalculVprofile=0;}

			i=0;
			while (!feof(datafile))
			{
				fgets(buffer,255,datafile);
					
				token = strtok( buffer, seps );// Establish string and get the first token: 
				if(token != NULL)
				{
					i++;
					switch(i)
					{
					case 1:
						pWorkImage->PutPictureName(token);
						break;
					case 2:
						pWorkImage->PutFileName(token);
						break;
					case 3:
						pWorkImage->PutCameraBending(atof(token));
						break;
					case 4:
						pWorkImage->PutCameraCCDSize(atof(token));
						break;
					case 5:
						pWorkImage->PutCameraDirection(atof(token));
						break;
					case 6:
						pWorkImage->PutCameraDistance(atof(token));
						break;
					case 7:
						pWorkImage->PutCameraFocalLength(atof(token));
						break;
					case 8:
						pWorkImage->PutCameraHeight(atof(token));
						break;
					case 9:
						pWorkImage->PutCameraInc(atof(token));
						break;
					case 10:
						pWorkImage->PutCameraRot(atof(token));
						break;
					case 11:
						pWorkImage->PutPictureBit(atoi(token));
						break;
					case 12:
						//pWorkImage->PutPictureType(atoi(token));
						break;
					case 13:
						if(atoi(token)==1){pWorkImage->PutCheck(1);}
						else {pWorkImage->PutCheck(0);}
						
						break;
					case 14:
						//pWorkImage->PutSelectCamera(atoi(token));
						pWorkImage->camera_name=this->GetOldCameraName(atoi(token));
						break;
					case 15:
						pWorkImage->PutSelectedDirection(atoi(token));
						break;
					}
				}
				if (i==15) 
				{
					n++;i=0;
					if(pWorkImage->ReadImageDimension(filePath)){PutImage(pWorkImage);}
				}
			}
		}//if(ver==1.07)



		fclose(datafile);
	//	MessageBox(NULL,"OK","Test",0);
		return 1;
	} //if (datafile!=NULL)
	else {MessageBox(NULL,"Error reading data file.","Error",0);return 0;}

}//bool CImagePool::OpenTextData(CString *fname)

int CImagePool::GetImageID(CString *picName)
{
		int i=0;
		while (vctImage[i].GetPictureName()!=*picName)
		{
			i++;
		}
		return i;
}

void CImagePool::ChangeFilename(CString oldname,CString newname)
{
	int ncount=this->GetImageCount();
	for (int i=0;i<ncount;i++)
	{
		if(vctImage[i].GetFileName()==oldname)
		{
			vctImage[i].PutFileName(newname);
			break;
		}
	}
}


void CImagePool::RemoveImage(int ImageID)
{
	vctImage.erase(&vctImage[ImageID]);
}

//! Calculate leaf projection coefficeint (G)
/*!
*	Created:4-May-04 
*	\n Last modification: 4-Oct-04.\n
*	Called by
*	-# CImagePool::CalculMinimumZoneSize(CEachImage *image)
*	-# CTreeAnalyserDlg::EstimateLeafArea()
*	-# CTreeAnalyserDlg::CaculLAD1(void)
*	-# CTreeAnalyserDlg::CalculDirections6(CEachImage *image,int gx,int gy,CString *fname)
*	-# CTreeAnalyserDlg::CalculDirections7(CEachImage *image,int gx,int gy, CEquationPool *eqPool)
*/
double CImagePool::CalculG(double beam_inc_deg)
{
	double G=0;
again:
	switch (m_leaf_distribution)
	{
	case 0: //conical distribution
		G=GetG_conical(beam_inc_deg,this->m_mean_leaf_inc);
		break;
	case 1: //ellipsoid
		break;
	case 2: //erectophile
		G=GetG_Type(beam_inc_deg,2);
		break;
	case 3: //extremophile
		G=GetG_Type(beam_inc_deg,3);
		break;
	case 4: //plagiophile
		G=GetG_Type(beam_inc_deg,4);
		break;
	case 5: //planophile
		G=GetG_Type(beam_inc_deg,5);
		break;
	case 6: //spherical distribution
		G=0.5;
		break;
	case 7: //custom
		if (vctLeafIncDis.size()==0)
		{
			if(!ReadLeafDis(LeafDistributionFile,ActivePath))
			{
				goto again;
			}
		}

		G=GetG_custom(beam_inc_deg);
		break;

	default:
		G=0.5;
	}

	return G;
}

double CImagePool::GetG_conical(double beam_inc_deg,double mean_leaf_inc_deg,int integration_category)
{//from Varlet-Grancher C., R. Bonhomme and H. Sinoquet 1993 page 210 eq.6
	double G=0;
	double pii=acos(double(-1));
	cPoint beamnor,leafnor;

	double leaf_inc_radian=mean_leaf_inc_deg*pii/180;
	double beam_inc_radian=beam_inc_deg*pii/180;

	double beam_az=0;
	beamnor.PutX(cos(beam_inc_radian)*cos(beam_az)); 
	beamnor.PutY(cos(beam_inc_radian)*sin(beam_az));
	beamnor.PutZ(sin(beam_inc_radian));

	
	double d_integration_cat=2*pii/integration_category;

	//integration process
	for (int i=0;i<integration_category;i++)
	{
		double curr_az=i*d_integration_cat+d_integration_cat/2;
		leafnor.PutX(cos(leaf_inc_radian+pii/2)*cos(curr_az+pii));
		leafnor.PutY(cos(leaf_inc_radian+pii/2)*sin(curr_az+pii));
		leafnor.PutZ(sin(leaf_inc_radian+pii/2));
		G+=abs(beamnor*leafnor)*d_integration_cat;
		
	}
	return G/(2*pii);
}


//! A function to give the name of the leaf distribution for the output log file.
/*!
*	Created : \n
*	Last modification: 13-Oct-04\n
*	Called by:\n
*	-# CTreeAnalyserDlg::EstimateLeafArea()
*	-# CTreeAnalyserDlg::CalculLAD1
*/
CString CImagePool::GetLeafDistributionName(void)
{
		CString leaf_dis;
		switch (m_leaf_distribution)
		{
		case 0: //conical distribution
			leaf_dis.Format("Conical_MA=%2.2f",this->m_mean_leaf_inc);
			break;
		case 1: //ellipsoid
			leaf_dis="Ellipsoid";
			break;
		case 2: //erectophile
			leaf_dis="Erectophile";
			break;
		case 3: //extremophile
			leaf_dis="Extremophile";
			break;
		case 4: //plagiophile
			leaf_dis="Plagiophile";
			break;
		case 5: //planophile
			leaf_dis="Planophile";
			break;
		case 6: //spherical distribution
			leaf_dis="Spherical";
			break;
		case 7: //custom
			leaf_dis.Format("Custom=%s",this->LeafDistributionFile);
			break;

		default:
			leaf_dis="Spherical";
		}
	return leaf_dis;
}



double CImagePool::GetG_custom(double beam_inc_deg)
{
	double sumG=0;
	if (vctLeafIncDis.size()>0)
	{
		int NumCat=vctLeafIncDis.size();
		double CatSize=90/NumCat;
		if(beam_inc_deg<0){beam_inc_deg=abs(beam_inc_deg);}
		if(beam_inc_deg>90){beam_inc_deg=beam_inc_deg-int(beam_inc_deg/90)*90;}

		for(int i=0;i<NumCat;i++)
		{
			double leaf_inc=CatSize*i+CatSize/2;
			sumG+=this->GetG_conical(beam_inc_deg,leaf_inc)*vctLeafIncDis[i];
		}
	}
	else {sumG=0;}
//end_process:;
	return sumG;
}

bool CImagePool::ReadLeafDis(CString dis_file,CString CurrentPath)
{
	//AfxMessageBox("OK");
 	//CEachImage *pWorkImage=new CEachImage;
	char *token;
	char buffer[256];
	FILE *datafile;
	bool round=0,result=0;
	vctLeafIncDis.clear();
try_again:;
	datafile= fopen(dis_file,"r");
	CString ms;
	double temp;
	if (datafile!=NULL)
	{
		vctLeafIncDis.resize(0);
		CString ss;
		fgets(buffer,255,datafile);
		token = strtok( buffer, " ,\t\n" );
		while( token != NULL )
		{
			temp=atof(token);
			vctLeafIncDis.push_back(temp);
			token = strtok( NULL," ,\t\n" );
		}

		fclose(datafile);

		//check sum of leaf inclination
		int NumCat=vctLeafIncDis.size();
		double sum_inc=0;
		for (int i=0;i<NumCat;i++)
		{
			sum_inc+=vctLeafIncDis[i];
		}
		if(sum_inc<0.99 || sum_inc>1.01)
		{
			ms.Format("Sum of leaf inclination fraction=%6.4f\nThis value should be=1.00.",sum_inc);
			MessageBox(NULL,ms,"Tree Analyser",0);
		}
			//ms.Format("Number of category found=%i\nSum=%6.4f",NumCat,sum_inc);
			//MessageBox(NULL,ms,"Tree Analyser",0);
		if(round==1){this->LeafDistributionFile=dis_file;}
		result=1;
	} //if (datafile!=NULL)
	else //if datafile is NULL
	{
		if(round==0)
		{
			CString ofile=dis_file;
			dis_file=CurrentPath+GetFileName(dis_file);
			//this->LeafDistributionFile=dis_file;
			round=1;
			//ms.Format("Error reading leaf distribution file.\n%s",distribution_file);
			//MessageBox(NULL,ms,"Tree Analyser",0);
			goto try_again;
			
		}
		CString ms;
		ms.Format("Error reading leaf angle distribution file.\nReset leaf angle distribution to 'Planophile'.");
		MessageBox(NULL,ms,"Tree Analyser",0);
		m_leaf_distribution=5;
		result=0;
	}
	return result;
}

CString CImagePool::GetFileName(CString fullPathName)
{
	CString result="error";
	int total=fullPathName.GetLength();
	char a='a';
	int i=total;
	while (i>0)
	{
		a=fullPathName.GetAt(i);
		if (a=='\\') {result=fullPathName.Right(total-i-1);break;}
		i--;
	}
	return result;
}

double CImagePool::CalculMinimumZoneSize(CEachImage *image)
{//MinimumZoneSize^2=minimum zone area
	image->CalculBasic();
	//step 1 get distance (in meter) of camera to the center of image plane
	double DistanceToImage=image->CalculDistanceToImagePlaneInMeter();

	//setp 2 Calcul G projected normal to the camera
	double pii=acos(double(-1));
	//double cele_deg=image->Get_cam_ele()*180/pii;
	double G=this->CalculG(image->Get_cam_ele_deg());

	//step 3 calculate projected leaf size for current leaf area assuming square leaf
	double ProjectedLeafArea=this->GetMeanLeafArea()*G;
	double ProjectedLeafSize=sqrt(ProjectedLeafArea);

	//step 4 compute minimum zone size
	double MinimumZoneSize=(ProjectedLeafSize*image->CalculCameraDistanceInPixel())/DistanceToImage;
	return MinimumZoneSize;
}

int CImagePool::CalculOptimalZoneSize(CEachImage *image)
{//return optimal zonesize (dx) in pixel
	double dx=CalculMinimumZoneSize(image);
	double dx2=dx*dx*17; //optimal zone size = 17*PZA
	return int(ceil(sqrt(dx2)));
	//int tt=int(ceil(sqrt(dx2)));
	//CString ms;
	//ms.Format("tt=%i",tt);
	//AfxMessageBox(ms);
	//return int(sqrt(dx2))+1;
}

int CImagePool::CalculGivenZoneSize(CEachImage *image,double given_pza)
{//return zonesize (dx) in pixel for given PZA
	double dx=CalculMinimumZoneSize(image);
	double dx2=dx*dx*given_pza; 
	return int(ceil(sqrt(dx2)));
}

//! A function to calcualte the optimal zone size (unit in pixel).
/*!
*	Created : 22-Sep-04\n
*	Last modification: 22-Sep-04.\n
*	Called by:\n
*	-# CTreeAnalyserDlg::Test1()
*	-# CTreeAnalyserDlg::EstimateLeafArea()
*	-# CTreeAnalyserDlg::CaculLAD1(void)
*	-# CTreeAnalyserDlg::SolvingEquation()
*/
/*
int CImagePool::CalculOptimalZoneSize(CEachImage *image)
{
	double temp=CalculMinimumZoneSize(image)*4;
	if(mod(temp,1.0)==0){return int(temp);}
	else {return int(temp)+1;}
}
*/
//! A function to give the name of the model for the output log file.
/*!
*	Created : \n
*	Last modification: 13-Oct-04\n
*	Called by:\n
*	-# CTreeAnalyserDlg::EstimateLeafArea()
*	-# CTreeAnalyserDlg::CalculLAD1
*/
CString CImagePool::GetModelName(void)
{
	CString model_name;
	if(m_inversion_method==0){model_name.Format("Binom+_LeafArea=%4.2f",this->m_mean_leaf_area);}
	if(m_inversion_method==1){model_name="Beer";}
	return model_name;
}

double CImagePool::GetG_Type(double beam_inc_deg,int type)
{
	double pii=acos(double(-1));
	double sumG=0;

	double CatSize=1;
	int NumCat=int(90/CatSize);
	if(beam_inc_deg<0){beam_inc_deg=abs(beam_inc_deg);}
	if(beam_inc_deg>90){beam_inc_deg=beam_inc_deg-int(beam_inc_deg/90)*90;}

	for(int i=0;i<NumCat;i++)
	{
		double leaf_inc_deg=CatSize*i+CatSize/2;
		double leaf_inc_radian=leaf_inc_deg*pii/180;
		double g=9e9;

		//from Varlet-Grancher C., R. Bonhomme and H. Sinoquet 1993 page 141
		if(type==2){g=2/pii*(1-cos(leaf_inc_radian*2))*((CatSize/90)*(pii/2));} //erectophile
		if(type==3){g=2/pii*(1+cos(leaf_inc_radian*4))*((CatSize/90)*(pii/2));} //extremophile
		if(type==4){g=2/pii*(1-cos(leaf_inc_radian*4))*((CatSize/90)*(pii/2));} //plagiophile
		if(type==5){g=2/pii*(1+cos(leaf_inc_radian*2))*((CatSize/90)*(pii/2));} //planophile
		sumG+=this->GetG_conical(beam_inc_deg,leaf_inc_deg)*g;
	}
	return sumG;
}


//! A function to get directory name form full path name.
/*!
*	Last modification: 9-Sep-04.\n
*	Called by:\n
*	-# CImagePool::OpenTextData(CString *fname).
*/
CString CImagePool::GetDirectoryName(CString fullPathName)
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
}//CString CImagePool::GetDirectoryName(CString fullPathName)


//! A function to check the input file before open it into memory.
/*!
*	return 0 if file does not exist.\n
*	return number of version if it is TA file\n
*	return -1 if the file is not TA file\n
*
*	\n Last modification: 9-Sep-04.\n
*	Called by:
*	-# CTreeAnalyserDlg::Openproject()
*	-# CTreeAnalyserDlg::Openproject(CString *fname)
*/
double CImagePool::CheckFile(CString filename)
{
	char seps[]   = "\n"; //seperation of each value by line
	char *token;
	char buffer[256];
	FILE *datafile;
	datafile= fopen(filename,"r");
	if (datafile==NULL){return 0;}
	else
	{
		CString ss;
		fgets(buffer,255,datafile);
		token = strtok( buffer, "\t" );
		if(token != NULL){ss=token;}
		//check if is Tree Analyser file
		if (ss=="TA") 
		{
			token = strtok( NULL, seps );
			double ver;
			if(token != NULL){ver=atof(token);}
			fclose(datafile);
			return ver;
		}
		else
		{
			fclose(datafile);
			CFile theFile;
			CFileException e;
			if( !theFile.Open( filename, CFile::modeRead, &e ) )
			{
				#ifdef _DEBUG
				afxDump << "File could not be opened " << e.m_cause << "\n";
				#endif
				return false;
			}

			CArchive ar(&theFile, CArchive::load);
			CString ms;
			ar>>ms;
			if(ms!="TA"){AfxMessageBox("Selected file is not Tree Analyser file!");return -1;}

			if(ar.IsBufferEmpty()){return -1;}
			else
			{
				double version=-1;
				ar>>version;
				ar.Close();
				theFile.Close();
				//ms.Format("Version=%2.2f",version);
				//AfxMessageBox(ms);
				return version;
			}
			return -1;
		}

	}
}//int CImagePool::CheckFile(CString filename)



//! A function to calculate sampling size (cm2) projected on camera direction) at the middle of image plane.
/*!
*	Created: 18-Sep-04
*	\n Last modification: 22-Sep-04.\n
*	Called by:
*	-# CTreeAnalyserDlg::EstimateLeafArea()
*	-# CTreeAnalyserDlg::CaculLAD1(void)
*/
double CImagePool::CalculSamplingArea(CEachImage *image,int pixel_dx,int pixel_dy)
{
	image->CalculBasic();
	//step 1 get distance (in meter) of camera to the center of image plane
	double DistanceToImage=image->CalculDistanceToImagePlaneInMeter();

	//step2 calcul side x and y
	double side_x=DistanceToImage*(double(pixel_dx)/double(image->CalculCameraDistanceInPixel()));
	double side_y=DistanceToImage*(double(pixel_dy)/double(image->CalculCameraDistanceInPixel()));

	//step3 calcul sample area in cm
	return side_x*side_y*10000; //return value in cm2
}
CString CImagePool::GetOldCameraName(int selected_number)
{
	CString camera_name="";
	switch (selected_number)
	{
	case 1: //Casio QV-3500EX
		camera_name="Casio QV-3500EX";
		break;	
	case 2: //Epson PhotoPC 3100Z
		camera_name="Epson PhotoPC 3100Z";
		break;
	case 3: //Fuji FinePix1400
		camera_name="Fuji FinePix1400";
		break;
	case 4: //Minolta Dimage 7i
		camera_name="Minolta Dimage 7i";
		break;	
	case 5: //Nikon CoolPix885
		camera_name="Nikon CoolPix885";
		break;	
	case 6: //Nikon CoolPix4500
		camera_name="Nikon CoolPix4500";
		break;	
	case 7: //Olympus C2020Z
		camera_name="Olympus C2020Z";
		break;	
	case 8: //Sony CyberShot DSC-P50
		camera_name="Sony CyberShot DSC-P50";
		break;	
	case 9: //Nikon E4300
		camera_name="Nikon E4300";
		break;	
	case 10: //Cannon PowerShot A75
		camera_name="Cannon PowerShot A75";
		break;		
	case 11: //Nikon E995
		camera_name="Nikon E995";
		break;		
	}
	return camera_name;
}
