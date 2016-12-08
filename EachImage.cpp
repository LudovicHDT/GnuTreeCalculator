// EachImage.cpp: implementation of the CEachImage class.
//

#include "stdafx.h"
//#include "Tree Analyser.h"
#include "EachImage.h"
#include ".\eachimage.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEachImage::CEachImage()
{
	m_fname="empty";
	m_distance=5;
	m_direction=0;
	m_height;
	m_bending=0;
	m_rot=0;
	m_inc=0;
	m_pictureWidth=0;
	m_pictureHeight=0;
	m_pictureBit=1;
	camera_name="Not defined";
	camera_cal=1.0;
	m_focalLength=1;
	m_picName="not defined";
	m_check=1;
	m_selectedDirection = 0;

	cam_az=0;
	cam_ele=0;
	cam_radious=0;
	height_of_mid_point=0;
	CameraCoordinate.PutXyz(0,0,0);
	ifMarkedPoint=0;
}

CEachImage::~CEachImage()
{
}
void CEachImage::CalculBasic()
{//all of distance have unit in pixel, angle have unit in radian
	double pii=acos(-1.0);
	double degree_to_pii=pii/180;

	cam_az=m_direction*degree_to_pii;
	cam_ele=m_inc*degree_to_pii;
	cam_rot=m_rot*degree_to_pii;
	cam_radious=CalculCameraDistanceInPixel()*cos(cam_ele);
	height_of_mid_point=CalculCameraDistanceInPixel()*sin(cam_ele);
	cam_x=cam_radious*cos(cam_az);
	cam_y=cam_radious*sin(cam_az);
	CalculCameraCoordinate();
	CalculAnglePerPixel();

}
void CEachImage::DisplayImageInformation()
{
	CString ms;
	ms.Format("Picture name=%s\nFile name=%s\nCamera distance=%5.2f height=%5.2f direction=%5.2f\nCamera az=%5.2f inc=%5.2f rot=%5.2f\nPicture width=%i height=%i bit=%i\nCamera CCD size=%5.2f focal length=%5.2f\n",
		m_picName,
		m_fname,
		m_distance,m_height,m_direction,
		m_bending,m_inc,m_rot,
		m_pictureWidth,m_pictureHeight,m_pictureBit,
		camera_cal,m_focalLength);
	MessageBox(NULL,ms,"Image Property",0);
}
double CEachImage::GetViewAngle() //return veiw angle of camera unit=radians
{
	
	double aa=atan(camera_cal/(m_focalLength*2))*2;
	return aa;
}


double CEachImage::CalculDistancePerpixel()
{

	return CalculImageDiameterInMetre()/CalculImageDiameterInPixel();

}

double CEachImage::CalculImageDiameterInPixel()
{
	return sqrt(pow(double(GetPictureHeight()),2)+pow(double(GetPictureWidth()),2));

}

double CEachImage::CalculCameraDistanceInPixel()
{//distance of camera to image plane in unit pixel
	return (CalculImageDiameterInPixel()/2)/tan(GetViewAngle()/2);

}

double CEachImage::CalculImageDiameterInMetre()
{
	return 2*GetCameraDistance()*tan(GetViewAngle()/2);
}


cPoint CEachImage::CalculUnitDirection(double pixelx, double pixely)
{/* -using unit of pixel while calculation
	-Assume
		-camera height=0
		-image center is on z axis and height of image depend on camera elevation*/


	cPoint cam_point,tempPoint;

	cam_point.PutXyz(this->cam_x,this->cam_y,0);

	//calculation of current location of pixel
	double dx=0,dy=0,dz=0;

	//step1 calculate position of the pixel on the image (x+ on the right,y+ on the top)
	double dpixelx=pixelx-(double(m_pictureWidth)/2);
	double dpixely=(double(m_pictureHeight)/2)-pixely;

	//step2 transform to first 3D position of pixel without any rotation and translation,set camera azimuth=0
	dx=0;
	dy=dpixelx;
	dz=dpixely;
	tempPoint.PutXyz(dx,dy,dz);

	//step3 rotation of point around x axis due to rotation of camera (clockwise)
	//dx does not change
	dy=tempPoint.GetY()*cos(this->cam_rot)-tempPoint.GetZ()*sin(this->cam_rot);
	dz=tempPoint.GetY()*sin(this->cam_rot)+tempPoint.GetZ()*cos(this->cam_rot);
	tempPoint.PutY(dy); tempPoint.PutZ(dz); //update tempPoint

	//step4 rotation around y axis due to elevation of camera
	dx=tempPoint.GetZ()*sin(this->cam_ele);
	//dy does not change
	dz=tempPoint.GetZ()*cos(this->cam_ele);
	tempPoint.PutX(dx); tempPoint.PutZ(dz); //update tempPoint

	//step5 rotation around z axis due to azimuth of camera
	dx=tempPoint.GetX()*cos(this->cam_az)-tempPoint.GetY()*sin(this->cam_az);
	dy=tempPoint.GetX()*sin(this->cam_az)+tempPoint.GetY()*cos(this->cam_az);
	//dz does not change
	tempPoint.PutX(dx); tempPoint.PutY(dy); //update tempPoint

	//step6 translocation of due to height of picture
	// no change in x and y
	dz=tempPoint.GetZ()+this->height_of_mid_point;
	tempPoint.PutZ(dz); //update tempPoint

	return this->CalculUnitDirection(cam_point,tempPoint);
}

cPoint CEachImage::CalculUnitDirection(cPoint StartPoint, cPoint SecondPoint)
{//line equation r=a+l*(b-a)
	cPoint direction;
	direction.PutXyz(SecondPoint.GetX()-StartPoint.GetX(),SecondPoint.GetY()-StartPoint.GetY(),SecondPoint.GetZ()-StartPoint.GetZ());
	double magnitude_of_direction=sqrt(pow(direction.GetX(),2)+pow(direction.GetY(),2)+pow(direction.GetZ(),2));
	cPoint unit_direction;
	unit_direction.PutXyz(direction.GetX()/magnitude_of_direction,direction.GetY()/magnitude_of_direction,direction.GetZ()/magnitude_of_direction);
//	CString ms;
//	ms.Format("Unit x=%6.4f  y=%6.4f z=%6.4f",unit_direction.GetX(),unit_direction.GetY(),unit_direction.GetZ());
//	MessageBox(NULL,ms,"Image Property",0);
	return unit_direction;
}
void CEachImage::CalculCameraCoordinate()
{
	double cameraAngle=(m_direction)*acos(-1.0)/180; //convert pi unit
	double cameray=sin(cameraAngle)*m_distance;
	double camerax=cos(cameraAngle)*m_distance;
	CameraCoordinate.PutXyz(camerax,cameray,m_height);
}

void CEachImage::CalculAnglePerPixel()
{
	angle_per_pixel=this->GetViewAngle()/this->CalculImageDiameterInPixel();
}

CPlane CEachImage::GetCanopyPlane(int iPlane)
{ //get canopy plane defined as A*x+B*y+Cz+D=0 where A2+B2+C2=1 
  //[A B C] is unit vector of normal plane
	//iPlane=0, vertical for tree height
	//iPlane=1, for diameter
	CPlane tplane;
	tplane.d=0;
	

	CalculBasic();
	if (iPlane==1)
	{
		cPoint cam_dir=this->CalculUnitDirection(this->m_pictureWidth/2,this->m_pictureHeight/2);
		tplane.PutX(cam_dir.GetX()*-1); 
		tplane.PutY(cam_dir.GetY()*-1);
		tplane.PutZ(cam_dir.GetZ()*-1);

		//1.find intersected point view direction to z axis (x=0, y=0X
		tplane.d=tplane.GetZ()*(this->m_height+this->m_distance*sin(this->cam_ele))*-1;
		//solve for D
	}
	if (iPlane==0)
	{
		tplane.PutX(cos(cam_az)*-1); //cam_az is direction of camera around the tree
		tplane.PutY(sin(cam_az)*-1);
		tplane.PutZ(0);
		tplane.d=0;
	}

	return tplane;
}

double CEachImage::GetIntersectRayPlane(int x,int y, cPoint *IntersectedPoint,int iPlane)
{//return 0 ray is parallel
	//chek if ray is parallel
	this->CalculBasic();
	cPoint RayDirection=this->CalculUnitDirection(x,y);
	CPlane inPlane=this->GetCanopyPlane(iPlane);

	double dotproduct=RayDirection.GetX()*inPlane.GetX()+RayDirection.GetY()*inPlane.GetY()+RayDirection.GetZ()*inPlane.GetZ();
	if (dotproduct!=0)
	{
		//solve for t
		cPoint campoint=this->GetCameraLocation();
		double t=(inPlane.GetX()*campoint.GetX()+inPlane.GetY()*campoint.GetY()+inPlane.GetZ()*campoint.GetZ()+inPlane.d)*-1;
		t=t/dotproduct;
		IntersectedPoint->PutX(campoint.GetX()+RayDirection.GetX()*t);
		IntersectedPoint->PutY(campoint.GetY()+RayDirection.GetY()*t);
		IntersectedPoint->PutZ(campoint.GetZ()+RayDirection.GetZ()*t);
	}

	return dotproduct;
}

cPoint CEachImage::GetCameraLocation(void)
{//unit in meter
	this->CalculBasic();
	cPoint tpoint;
	tpoint.PutXyz(this->m_distance*cos(this->cam_az),this->m_distance*sin(this->cam_az),this->m_height);
	return tpoint;
}

/*
int CEachImage::CalculMinimumZoneSize(double LeafArea)
{
	this->CalculBasic();
	//step 1 get distance (in meter) of camera to the center of image plane
	double DistanceToImage=this->CalculDistanceToImagePlaneInMeter();

	//step 2 calculate leaf size for current leaf area assuming square leaf
	double LeafSize=sqrt(LeafArea);

	//setp 3 convert leaf size to zone size
	double MinimumZoneSize=(LeafSize/DistanceToImage)*this->CalculCameraDistanceInPixel();
	return int(MinimumZoneSize)+1;
}*/

double CEachImage::CalculDistanceToImagePlaneInMeter(void)
{

	return this->m_distance/cos(cam_ele);
}

double CEachImage::CalculSectionArea(int ZoneWidth,double px,double py)
{
	//step 1 find the angle between two side from dot product
	cPoint a,b;
	a=CalculUnitDirection(px-double(ZoneWidth)/2,py);
	b=CalculUnitDirection(px+double(ZoneWidth)/2,py);
	double angle_x=acos(abs(a*b)); //aa is dot product or cosbeta

	//step 2 find distance from camera to the pixel from ray-plane intersection
	double distance_to_pixel=CalculDistanceToPixel(px,py);

	//step 3 calcul section size
	double section_size=2*distance_to_pixel*tan(angle_x/2);

	return pow(section_size,2); //assume square section
}

double CEachImage::CalculSectionAreaRectangle(double zone_x,double zone_y,double px,double py)
{
	//step 1 find the angle between two side from dot product
	cPoint a,b;
	a=CalculUnitDirection(px-zone_x/2,py);
	b=CalculUnitDirection(px+zone_x/2,py);
	double angle_x=acos(abs(a*b)); //aa is dot product or cosbeta

	a=CalculUnitDirection(py-zone_y/2,px);
	b=CalculUnitDirection(py+zone_y/2,px);
	double angle_y=acos(abs(a*b)); //aa is dot product or cosbeta

	//step 2 find distance from camera to the pixel from ray-plane intersection
	double distance_to_pixel=CalculDistanceToPixel(px,py);

	//step 3 calcul section size
	double section_size_x=2*distance_to_pixel*tan(angle_x/2);
	double section_size_y=2*distance_to_pixel*tan(angle_y/2);

	return section_size_x*section_size_y;
}


double CEachImage::CalculDistanceToPixel(double px, double py)
{// distance to specified pixel in meter
	cPoint xPoint,camPoint; //to be intersected point
	GetIntersectRayPlane(px,py,&xPoint,1);
	camPoint=this->GetCameraLocation();
	return sqrt(pow((xPoint.GetX()-camPoint.GetX()),2)+pow((xPoint.GetY()-camPoint.GetY()),2)+pow((xPoint.GetZ()-camPoint.GetZ()),2));
}

double CEachImage::GetIntersectRayPlane(double x,double y, cPoint *IntersectedPoint,int iPlane)
{//return 0 ray is parallel
	//chek if ray is parallel
	this->CalculBasic();
	cPoint RayDirection=this->CalculUnitDirection(x,y);
	CPlane inPlane=this->GetCanopyPlane(iPlane);

	double dotproduct=RayDirection.GetX()*inPlane.GetX()+RayDirection.GetY()*inPlane.GetY()+RayDirection.GetZ()*inPlane.GetZ();
	if (dotproduct!=0)
	{
		//solve for t
		cPoint campoint=this->GetCameraLocation();
		double t=(inPlane.GetX()*campoint.GetX()+inPlane.GetY()*campoint.GetY()+inPlane.GetZ()*campoint.GetZ()+inPlane.d)*-1;
		t=t/dotproduct;
		IntersectedPoint->PutX(campoint.GetX()+RayDirection.GetX()*t);
		IntersectedPoint->PutY(campoint.GetY()+RayDirection.GetY()*t);
		IntersectedPoint->PutZ(campoint.GetZ()+RayDirection.GetZ()*t);
	}

	return dotproduct;
}


double CEachImage::CalculGap(double px, double py, int gx, int gy, int GapOption)
{//px,py is location of current pixel
	//gx and gy is the sampling size
	//GapOption 0 for fixed zone, 1 for gliding zone

	double gapfraction=0;
	CString ms;
	if(GapOption==0)
	{//fixed zone
		CImage ctrlImage;
		// Load the selected bitmap file
		HRESULT loadresule= ctrlImage.Load(GetFileName());
		if (FAILED(loadresule))
		{
			ms.Format("Error reading file '%s'\r\nSelected file may not existed or not be the bitmap file.",GetFileName());
			AfxMessageBox(ms);
			goto skipimage;
		}//if (FAILED(loadresule))
		else //if load image OK
		{
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
			gapfraction=1-sumblack/total_pixel;
		}//else //if load image OK
	}//if(gap_average_option==0)
skipimage:;
	return gapfraction;
}


//! A function to read image width and height form specified filename. If file not found in the specified then search in the active path. If not found then show error message.
/*!
*	return 1 if success.\n
*	return 0 if fail.\n
*	Last modification: 9-Sep-04.\n
*	Called by:\n
*	-# CImagePool::OpenTextData(CString *fname).
*/
bool CEachImage::ReadImageDimension(CString activePath)
{
	int round=0;
	CImage ctrlImage;

again:;
	HRESULT loadresule= ctrlImage.Load(this->m_fname);
	if (FAILED(loadresule))
	{
		if(round==0)
		{
			round++;
			m_fname=activePath + GetRootFileName(m_fname);
			goto again;
		}
		CString ms;
		ms.Format("Error reading file %s\nSelected file may not existed or not be the bitmap file.",m_fname);
		AfxMessageBox(ms);
		return 0;
	}
	else //if load image OK
	{
		this->m_pictureHeight =ctrlImage.GetHeight();
		this->m_pictureWidth =ctrlImage.GetWidth();
		this->m_pictureBit=ctrlImage.GetBPP();
		ctrlImage.Detach();
		//ctrlImage.Destroy();
		return 1;
	}

}


//! A function to get only the file name form full path name.
/*!
*	Last modification: 9-Sep-04.\n
*	Called by:\n
*	-# CEachImage::ReadImageDimension(CString activePath)
*/
CString CEachImage::GetRootFileName(CString fullPathName)
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
double CEachImage::Get_cam_ele_deg(void)
{
	double pii=acos(-1.0);
	return this->cam_ele*180/pii;
}
