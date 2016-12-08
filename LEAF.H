// leaf.h: interface for the leaf class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LEAF_H__53021FB2_303A_4109_9214_EDA01DEFC3AD__INCLUDED_)
#define AFX_LEAF_H__53021FB2_303A_4109_9214_EDA01DEFC3AD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "utils.h"
#include "cPoint.h"


class leaf : public cPoint
{
public:
	
	leaf();
	~leaf();
	void PutID(unsigned int id) {leaf_id=id;}
	void PutLength(double length) {leaf_length=length;}
	void PutWidth(double width) {leaf_width=width;}
	void PutSf(double surface) {leaf_sf=surface;}
	void PutInc(double inc) {leaf_inc=inc;}
	void PutAz(double az) {leaf_az=az;}

	unsigned int GetID() {return leaf_id;}
	double GetLength() {return leaf_length;}
	double GetSurface() {return leaf_sf;}
	double GetInc(){return leaf_inc;}
	double GetAz() {return leaf_az;}
	double GetWidth() {return leaf_width;}
	double GetCosBeta(double beamHeight,double beamAzimuth);


private:
	
	unsigned int leaf_id;
	double leaf_length;
	double leaf_width;
	double leaf_sf;
	double leaf_inc;
	double leaf_az;
	
};

#endif // !defined(AFX_LEAF_H__53021FB2_303A_4109_9214_EDA01DEFC3AD__INCLUDED_)
