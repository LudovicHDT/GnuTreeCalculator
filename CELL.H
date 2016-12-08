// cell.h: interface for the cell class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CELL_H__695AB3D6_03A2_4193_8150_68242ECC18C9__INCLUDED_)
#define AFX_CELL_H__695AB3D6_03A2_4193_8150_68242ECC18C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "utils.h"
#include "cPoint.h"
#include "leaf.h"

class cell  :public cPoint
{
public: 
	cell();
	~cell();
	bool keepit,haveleaf;
	int ID;
	double pathLength;
	unsigned short int included_in_canopy; //-1=not assign 0=not included 1=included
	double intersect_volume;
	double sum_surface;
	double section_area;

	double GetSumSurface() {return sum_surface;}
	unsigned short int GetCanopyCellIndex() {return included_in_canopy;}
	unsigned short int GetType() { return type;}
	void PutType(unsigned short int intype) { type=intype;}
	double maximumLeafArea;




private:
	//unsigned int nx,ny,nz; // as cell ID
	unsigned short int type; 
	/* cell type 
	0=empty cell 
	1=non-empty 
	2=empty cell inside canopy (all of 6 direction have non-empty cell)
	3=empty cell that fill canopy to be convex */


};
#endif // !defined(AFX_CELL_H__695AB3D6_03A2_4193_8150_68242ECC18C9__INCLUDED_)
