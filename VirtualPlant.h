// VirtualPlant.h: interface for the CVirtualPlant class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VIRTUALPLANT_H__EAB78DBF_A9D3_40B8_B7B4_E3E0894F221B__INCLUDED_)
#define AFX_VIRTUALPLANT_H__EAB78DBF_A9D3_40B8_B7B4_E3E0894F221B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "utils.h"
#include "cell.h"
#include "cVGSline.h"

/**
* \brief The class cVirtualPlant\n
* This class designed to hold all of the virtual plant objects
*/

class cVirtualPlant :public CObject
{
	typedef std::vector<cell> VctCell; //definition of vector for cell
	typedef std::vector<cVGSline> VctVgsLine;

public:
	int lifecell;
	void ResetKeepIt();
	void ClearHaveLeaf();
	unsigned int begin_nz;
	int Reconstruct(int nx,int ny,int nz,double dx,double dy,double dz,cPoint startPoint);
	//constructor
	cVirtualPlant();
	cVirtualPlant(CEdit *StatusBar,CEdit *Output);
	cVirtualPlant(int num_x,int num_y,int num_z);

	//destructor
	~cVirtualPlant();

	//input functions
	bool PutCell(int inx,int iny,int inz,cell incell);
	bool PutCell(int vector_id,cell incell);
	bool PutDxyz(double ddx,double ddy,double ddz) {dx=ddx;dy=ddy;dz=ddz;return 1;}
	bool PutMaxmin(double dxmin,double dymin,double dzmin,double dxmax,double dymax,double dzmax);
	bool PutNxyz(int inx,int iny, int inz){nx=inx;ny=iny;nz=inz;return 1;}

	//get stored data functions
	double GetCanopyVolume(){return GetTotal_cells()*dx*dy*dz;}
	unsigned int GetTotal_cells() {return acell.size();}
	int GetNx() {return nx;}
	int GetNy() {return ny;}
	int GetNz() {return nz;}
	double GetMinMax(bool max_min, short int xyz); //(0=min 1=max,1=x 2=y 3=z)
	cell GetCell(int inx,int iny,int inz); //nx, ny and nz start form 1
	cell *GetCellPointer(int inx,int iny,int inz); //nx, ny and nz start form 1
	cell GetCell(int vector_id) {return acell[vector_id];}
	double Getdx(){return dx;}
	double Getdy(){return dy;}
	double Getdz(){return dz;}
	double GetCellVolume(){return dx*dy*dz;}

	//process functions
	double CalculVLAD();
	double CalculMeanLAD();
	bool Resize(unsigned int num_x,unsigned int num_y,unsigned int num_z);
	bool Resize(unsigned int num_cell);
	short int RayCellIntersect(cPoint *ray_origin,cPoint *ray_direction,cell *incell); //1=ray hit 0=missed -1=ray behind the box
	short int RayCellIntersect(cPoint *ray_origin,cPoint *ray_direction,cell *incell,double *tin,double *tout); //1=ray hit 0=missed -1=ray behind the box
	short int RayCanopyIntersect(cPoint *ray_origin,cPoint *ray_direction,double *tin,double *tout); //1=ray hit 0=missed -1=ray behind the box
	unsigned int ConvertIDtoNxyz(unsigned int inID,short int option);
	void ExportCellVGS(CString fname, unsigned int optionShape,unsigned int CellType);
	double GetDiameter();
	double GetCenter(int optionxyz);
	VctCell acell; //create vecter of cells
	VctVgsLine each_line; //create vector of VegeSTAR data line
	void Reset();
	void ResetVoxelPathLength();
	void RandomLeafArea(void);//random switching leaf area between voxels

	/* remove to save memory
	void ConsierCellType();
	bool PutLeaftoCell(int inx,int iny,int inz,leaf in_leaf);
	void UpdateLeafArea();
	unsigned int CountCell(bool option); //count empty or non-empty cell;
	unsigned int CountLeaf();
	bool Consider4();
*/

private:
	//data variables
	bool pack_index; //index for pack data
	int GetCellVectorID(int nx,int ny,int nz);
	int nx,ny,nz; //total cells=nx*ny*nz
	double dx,dy,dz;
	double xmin,ymin,zmin,xmax,ymax,zmax;
	cPoint canopy_center;
	cPoint start_point;


	//internal processes
	void PrintSkeleton(CString fname,unsigned shape, unsigned int cellType);
	void PutSkeleton(int incell);
	bool CompareLine(cVGSline *iline1,cVGSline *iline2);
	void PutBox(double inx,double iny, double inz,double indx,double indy,double indz);
	void PutBox(int icell);

	double mod(double x,double y) {	int i = int(x/y);double result=x-i*y;return result;}

	//display message functions
	int *strOutput_line;
	CEdit *pStatusBar;
	CEdit *pOutput;
	CString ms;
	void AddDisplayText(CString txt);
	void AddDisplayText(CString *txt);
	void DisplayStatus(CString inputText);
	void DisplayStatus();

public:
	int AddCell(cell incell);
	bool IsLeafInVolume(leaf in_leaf);
	void ClearIntersectVolume(void);
	void ShareLeafToVoxel(double InputArea);
	void OutputVerticalLAD(CString fname, double range);
	double GetTotalLeafArea(void);
	void DisplayCanopyProperties(void);
	double CalculP0(cPoint *ray_origin,cPoint *ray_direction);
	void CalculVerticalProfileLeafArea(void);
	void CalculVerticalProfileVolume(void);
	double CalculCellDistance(int cell1,int cell2); //calculate distance between 2 cells
	void ClearSumsurface(void); //clear surface for every cells
	void CalculBasic(void);// Recalculate max min
	void ExportRATPvoxel(CString filename);
	void SaveGridParameters(CString filename);
	void ResetVoxelID(void);
	void ExportPlantGL(CString filename);
};

#endif // !defined(AFX_VIRTUALPLANT_H__EAB78DBF_A9D3_40B8_B7B4_E3E0894F221B__INCLUDED_)
