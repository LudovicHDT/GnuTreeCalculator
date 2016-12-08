#if !defined(MY_FUNCTION)
#define MY_FUNCTION

#include <math.h>
#include <stdlib.h>
#include <time.h>


int iseed=(unsigned)time( NULL );
const double pii=acos(double(-1));

double RandomInterval(double min, double max)
{
	srand( (unsigned)time( NULL ) );
	//srand( iseed ); //set the random seed by time
	double x=0, interval=max-min;
	//for (int i=1; i<=2000; i++) {x=rand();} // to void that get the same first of random value
	//iseed=rand();
	x=rand();//get random value
	x=x/32767; // chance to value between 0-1;
	return min+x*interval;
}

double mod(double x,double y)
{
	int i = int(x/y);
	double result=x-i*y;
	return result;
}

double CosBeta(double beamHeight,double beamAzimuth,double leafInclination,double leafAzimuth)
{
	double dotProduct=0;
	dotProduct=fabs(cos(leafInclination)*sin(beamHeight)+sin(leafInclination)*cos(beamHeight)*cos(leafAzimuth-beamAzimuth));
	return dotProduct;
}

double CosBeta(double BeamHeight,double LeafInclination)
{
/*
Function for calculate G from leaf inclination when assumed random leaf azimuth distribution
form H. Sinoquet, C. Varlet-Grancher and R. Bonhomme. 1993. Modelling radiative transfer with in homogenous canopies: basic concepts
in C. Varlet-Grancher, R. Bonhomme and H. Sinoquet. Crop Structure and Light Microclimate: Characterization and Application. INRA Editions, Paris.
*/
	//double pii=acos(-1);
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

void swap (double *v1,double *v2)
{
	double temp= *v1;
	*v1=*v2;
	*v2=temp;
}

#endif
