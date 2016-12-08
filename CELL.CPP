// cell.cpp: implementation of the cell class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Tree Analyser.h"
#include "cell.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//constructor
cell::cell() 
{
	included_in_canopy=1;
	sum_surface=0;
	cPoint();
	keepit=1;
	haveleaf=0;
	intersect_volume=0;
	maximumLeafArea=9e9;
	ID=0;
	pathLength=0;
	section_area=0; //use for exporting equation to Python binomial model
/* removed to save memory
	each_leaf.clear();
	leaf_number=0;
	connect_border=0;
*/
}

//destructor
cell::~cell(){}


//void cell::PutNumber_of_leaf(int lfno) {leaf_number=lfno;}

//int cell::GetNumber_of_leaf() {return leaf_number;}

/*
void cell::CalculSumSurface() //calculate sum of leaf surface and store value inside this cell
{
	if (leaf_number==0) {sum_surface=0;}
	else
	{
		double temp=0;
		for (unsigned int i=0;i<leaf_number;i++)
		{
			temp+=each_leaf[i].GetSurface();
		}
		sum_surface=temp;
	}
}




void cell::PutLeaf(leaf in_leaf) 
{
	each_leaf.push_back(in_leaf);
	++leaf_number;
}


double cell::G(double inc,double az) //inc and az is direction of light in radians
{
	double temp=0;
	if (leaf_number==0) {temp=0;}
	else
	{
		CalculSumSurface();
		double sumprojection=0;
		for (unsigned int i=0;i<leaf_number;i++)
		{
			sumprojection+= CosBeta(inc,az,each_leaf[i].GetInc(),each_leaf[i].GetAz())*each_leaf[i].GetSurface();
		}
		temp=sumprojection/sum_surface;
	}
	return temp;
}

double cell::G(double inc,double az,unsigned int option) 
//assume random leaf azimuth
{
	if(option==0)
	{ 
		az=az;
	}
		double temp=0;
		if (leaf_number==0) {temp=0;}
		else
		{
			CalculSumSurface();
			double sumprojection=0;
			for (unsigned int i=0;i<leaf_number;i++)
			{
				sumprojection+=CosBeta(inc,each_leaf[i].GetInc())*each_leaf[i].GetSurface();
			}
			temp=sumprojection/sum_surface;
		}
	
	return temp;
}

double cell::CosBeta(double beamHeight,double beamAzimuth,double leafInclination,double leafAzimuth)
{
	double dotProduct=0;
	dotProduct=fabs(cos(leafInclination)*sin(beamHeight)+sin(leafInclination)*cos(beamHeight)*cos(leafAzimuth-beamAzimuth));
	return dotProduct;
}

double cell::CosBeta(double BeamHeight,double LeafInclination)
{

Function for calculate G from leaf inclination when assumed random leaf azimuth distribution
form H. Sinoquet, C. Varlet-Grancher and R. Bonhomme. 1993. Modelling radiative transfer with in homogenous canopies: basic concepts
in C. Varlet-Grancher, R. Bonhomme and H. Sinoquet. Crop Structure and Light Microclimate: Characterization and Application. INRA Editions, Paris.

	const double pii=acos(double(-1));
	double temp=0;
	if (BeamHeight==0){BeamHeight=1e-9;} //prevent cosbeta=0 when beam height=0
	if (BeamHeight>LeafInclination) {temp=cos(LeafInclination)*sin(BeamHeight);}
	else 
	{
		double theta0=acos(-tan(BeamHeight)/tan(LeafInclination));
		temp=cos(LeafInclination)*sin(BeamHeight)*(2*(theta0-tan(theta0))/pii-1);
	}
	return temp;
}
*/
/*
void cell::PutNxyz(int x,int y,int z)
{
	nx=x;ny=y;nz=z;
}*/