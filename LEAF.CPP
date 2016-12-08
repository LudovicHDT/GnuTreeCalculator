// leaf.cpp: implementation of the leaf class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Tree Analyser.h"
#include "leaf.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

leaf::leaf()
{
}


leaf::~leaf()
{
}


double leaf::GetCosBeta(double beamHeight,double beamAzimuth)
{
	double dotProduct=0;
	dotProduct=fabs(cos(leaf_inc)*sin(beamHeight)+sin(leaf_inc)*cos(beamHeight)*cos(leaf_az-beamAzimuth));
	return dotProduct;

}
