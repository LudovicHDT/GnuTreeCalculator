// ImagePool.h: interface for the CImagePool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAGEPOOL_H__F7B7C61E_2CFF_4838_BDBF_A002EFB727CD__INCLUDED_)
#define AFX_IMAGEPOOL_H__F7B7C61E_2CFF_4838_BDBF_A002EFB727CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "EachImage.h"
#include "utils.h"
/**
* \brief The class CImagePool\n
* This class designed to hold all of the project settings and image objects.
*/
class CImagePool:public CObject
{
//	DECLARE_SERIAL (CImagePool);
public:
	BOOL	m_ifoutputbox;
	BOOL	m_iflogvolume;
	int raypx,raypy; //beam size for volume reconstruction
	double bounding_x,bounding_y,bounding_z; //bounding box unit m
	double cellSize_dx,cellSize_dy,cellSize_dz; //cell size unit m;
	double maxTreeHeight, minTreeHeight;
	double maxVHeight, minVHeight;
	double maxDiameter, minDiameter;
	double averageTreeHeight,averageVHeight,averageDiameter;
	double	m_enlarge;
	int m_LimitEle;
	
	// add for leaf area calulation
	int m_leaf_distribution;
	int gpx,gpy,gap_average_option; //size of zone to calculate gap fraction
	int gap_option; //0=auto 1=custom 2=fix pza
	double fix_pza;
	int m_leaf_dispersion_option;
	double m_leaf_dispersion;
	double m_mean_leaf_area;
	void ChangeFilename(CString oldname,CString newname);

	//add for leaf LAD computation
	BOOL ifCalculVprofile;

	void RemoveImage(int ImageID);
	int GetImageID(CString * picName);

	//constructor
	CImagePool();

	//destructor
	virtual ~CImagePool();

	int GetImageCount(){return vctImage.size();}
	CString GetPictureFileName(CString picName);

	void PutImage(CEachImage * pImage);

	//Process function
	void DisplayImageInformation(unsigned int ipic);
	CEachImage * GetImage(int imageID);
	CEachImage * GetImage(CString *picName);
	void ClearData();
	bool OpenTextData(CString *fname);
	//bool SaveData(CString *fname);
	bool SaveNewFormat(CString *fname);
	bool OpenNewFormat(CString *fname);
	std::vector<CEachImage> vctImage;

private:

public:
	double CalculG(double beam_inc_deg);
	double GetG_conical(double beam_inc_deg, double mean_leaf_inc_deg,int integration_category=180);
	double GetG_custom(double beam_inc_deg);
	double GetG_Type(double beam_inc_deg,int type);
	double m_mean_leaf_inc;
	CString GetLeafDistributionName(void);
	CString LeafDistributionFile;
	bool ReadLeafDis(CString distribution_file,CString CurrentPath);
	std::vector<double> vctLeafIncDis;
	double minimumGap;
	int ZeroGapManage;
	bool IfSetMaxLAD;
	double MaxLAD;
	CString GetFileName(CString fullPathName);
	double GetMeanLeafArea() {return m_mean_leaf_area/10000;}
	double CalculMinimumZoneSize(CEachImage *image);
	int CalculGivenZoneSize(CEachImage *image,double given_pza);
	int CalculOptimalZoneSize(CEachImage *image);//size of dx
	int m_inversion_method;
	CString GetModelName(void);
	CString ActivePath;
	double EsimatedLeafArea;
	CString GetDirectoryName(CString fullPathName);
	double CheckFile(CString filename);
	double CalculSamplingArea(CEachImage *image,int pixel_dx,int pixel_dy);
	double mod(double x,double y) {	int i = int(x/y);double result=x-i*y;return result;}
	CString GetOldCameraName(int selected_number);

	//Variable added for subdivision of PZA
	BOOL is_divide_pza;
	int division_horizontal;
	int division_vertical;

};

#endif // !defined(AFX_IMAGEPOOL_H__F7B7C61E_2CFF_4838_BDBF_A002EFB727CD__INCLUDED_)
