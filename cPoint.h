// cPoint.h: interface for the cPoint class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CPOINT_H__695CC5D7_B6C1_4E98_9875_34217F6C2F02__INCLUDED_)
#define AFX_CPOINT_H__695CC5D7_B6C1_4E98_9875_34217F6C2F02__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "utils.h"


class cPoint
{
public:
	//constructors
	cPoint();
	cPoint(double inx,double iny,double inz);

	//destructor
	~cPoint();
	
	//operators
	double operator* (const cPoint &);
	cPoint operator* (const double &);

	void PutXyz(double px,double py,double pz); // {x=px;y=py;z=pz;}
	void PutX(double inx) {x=inx;}
	void PutY(double iny) {y=iny;}
	void PutZ(double inz) {z=inz;}
	double GetX() {return x;}
	double GetY() {return y;}
	double GetZ() {return z;}
	double GetDistance() {return sqrt(pow(x,2)+pow(y,2)+pow(z,2));}
//	void PrintPoint() {printf("x=%6.4f	y=%6.4f	z=%6.4f\n",x,y,z);}


private:
	double x,y,z;

};

#endif // !defined(AFX_CPOINT_H__695CC5D7_B6C1_4E98_9875_34217F6C2F02__INCLUDED_)
