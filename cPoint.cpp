// cPoint.cpp: implementation of the cPoint class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Tree Analyser.h"
#include "cPoint.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//constructor
cPoint::cPoint() {x=0; y=0; z=0;}
cPoint::cPoint(double inx,double iny,double inz) { x=inx;y=iny;z=inz;}


//destructor
cPoint::~cPoint() {}

//operators
double cPoint::operator *(const cPoint &P){return (x*P.x+y*P.y+z*P.z);}
cPoint cPoint::operator *(const double &D){return cPoint(x*D,y*D,z*D);}


void cPoint::PutXyz(double px,double py,double pz) {x=px;y=py;z=pz;}
