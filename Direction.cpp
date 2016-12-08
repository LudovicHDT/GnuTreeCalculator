// Direction.cpp: implementation of the CDirection class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Tree Analyser.h"
#include "Direction.h"
#include ".\direction.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDirection::CDirection()
{
	gapfraction=0;
	unitDirection.PutXyz(0,0,0);
	px=0;
	py=0;
	LAD=0;PathLength=0;
	beam_volume=0;
	this->number_pixel=0;
	angle_x=0;
	angle_y=0;
}

CDirection::~CDirection()
{

}

double CDirection::CalculNormalInc(void)
{
	return asin(unitDirection.GetZ()/unitDirection.GetDistance());
}
