// EachImage.h: interface for the CEachImage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EACHIMAGE_H__D0A8A18D_CB5F_41A1_9D32_63AE4EC926C2__INCLUDED_)
#define AFX_EACHIMAGE_H__D0A8A18D_CB5F_41A1_9D32_63AE4EC926C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Pixel.h"
#include "cPoint.h"	// Added by ClassView
#include "utils.h"
#include "Plane.h"

/**
* \brief The class CEachImage\n
* This class designed to hold the parameters of each image.
*/
class CEachImage  
{
public:
	CEachImage();
	virtual ~CEachImage();

	void CalculCameraCoordinate();
	cPoint CalculUnitDirection(double pixelx,double pixely);
	cPoint CEachImage::CalculUnitDirection(cPoint StartPoint, cPoint SecondPoint);
	double CalculImageDiameterInMetre();
	double CalculCameraDistanceInPixel();
	double CalculImageDiameterInPixel();
	double CalculDistancePerpixel();
	CPixel  pixTop,pixBottom,pixLeft,pixRight;
	bool ifMarkedPoint;


	//input data variables
	CString GetFileName(){return m_fname;}
	CString *GetpFileName(){return &m_fname;}
	CString GetPictureName(){return m_picName;} 
	double GetCameraDistance(){return m_distance;} //m
	double GetCameraDirection(){return m_direction;} //degree. It is the direction of camera around the tree 0=East
	double GetCameraHeight(){return m_height;} //m
	double GetCameraBending(){return m_bending;} // in deg. 
	double GetCameraRot(){return m_rot;} //deg
	double GetCameraInc(){return m_inc;} //deg
	double GetCameraCCDSize(){return camera_cal;} //mm
	double GetCameraFocalLength(){return m_focalLength;} //mm
	double Get_cam_ele_radians(){return cam_ele;};//return camera elevation in pi
	int GetPictureWidth(){return m_pictureWidth;} //pixel
	int GetPictureHeight(){return m_pictureHeight;} //pixel
	int GetPictureBit(){return m_pictureBit;}
	bool GetCheck(){return m_check;}
	int GetSelectedDirection(){return m_selectedDirection;}
	double GetAnglePerPixel(){return angle_per_pixel;}

	//output data variables
	void PutFileName(CString fname) {m_fname=fname;}
	void PutPictureName(CString picname){m_picName=picname;}
	void PutCameraDistance(double dis){m_distance=dis;}
	void PutCameraDirection(double dir){m_direction=dir;}
	void PutCameraHeight(double height){m_height=height;}
	void PutCameraBending(double az){m_bending=az;}
	void PutCameraInc(double inc){m_inc=inc;}
	void PutCameraRot(double rot){m_rot=rot;}
	void PutPictureWidth(int width){m_pictureWidth=width;}
	void PutPictureHeight(int height){m_pictureHeight=height;}
	void PutCameraFocalLength(double fc){m_focalLength=fc;}
	void PutCameraCCDSize(double cs){camera_cal=cs;}
	void PutPictureBit(int pbit){m_pictureBit=pbit;}
	void PutCheck(bool check){m_check=check;}
	void PutSelectedDirection(int select){m_selectedDirection=select;}

	//Processing
	void DisplayImageInformation();
	double GetViewAngle(); //return veiw angle of camera unit=radians
	void CalculBasic();
	void CalculAnglePerPixel();

	cPoint CameraCoordinate;

private:
	CString	m_fname;
	double	m_distance; //horizontal distance of camrea to the trunk (m)
	double	m_direction; //direction of camera unit in degree (0=East, 90=North);
	double	m_inc; //camera inclination in degree
	double	m_rot; //camrea rotation in degree
	double	m_height; //height of camera to reference point (m)

	double	m_bending;  //camera bending unit in degree (<0=left <0=right);
	int		m_pictureWidth;
	int		m_pictureHeight;
	int		m_pictureBit;

	CString	m_picName;
	bool	m_check;
	int		m_selectedDirection;



	double cam_az,cam_ele,cam_radious,height_of_mid_point,cam_x,cam_y,cam_rot;
	double angle_per_pixel;

public:

	CString camera_name;
	double	camera_cal;
	double	m_focalLength;

	CPlane GetCanopyPlane(int iPlane); //0 for tree height, 1 for diameter
	double GetIntersectRayPlane(int x,int y, cPoint *IntersectedPoint,int iPlane);
	double GetIntersectRayPlane(double x,double y, cPoint *IntersectedPoint,int iPlane);
	cPoint GetCameraLocation(void);
	double CalculDistanceToImagePlaneInMeter(void);
	double CalculSectionArea(int ZoneWidth,double px,double py);
	double CalculSectionAreaRectangle(double zone_size_x,double zone_size_y,double px,double py);
	double CalculDistanceToPixel(double px, double py);
	double CalculGap(double px, double py, int gx, int gy, int GapOption);
	bool ReadImageDimension(CString activePath);
	CString GetRootFileName(CString fullPathName);
	double Get_cam_ele_deg(void);
};

#endif // !defined(AFX_EACHIMAGE_H__D0A8A18D_CB5F_41A1_9D32_63AE4EC926C2__INCLUDED_)
