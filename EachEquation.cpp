#include "stdafx.h"
#include ".\eachequation.h"

CEachEquation::CEachEquation(void)
{
	lnP0=0;
	SectionArea=0;
	G=0;

}


CEachEquation::~CEachEquation(void)
{
}

bool CEachEquation::IsHaveThisID(int ID)
{
	for(unsigned int i=0;i<this->vctID.size();i++)
	{
		if(vctID[i]==ID){return true;}
	}
	return false;
}

double CEachEquation::GetPathLength(int VoxelID)
{
	double temp=0;
	for(unsigned int i=0;i<this->vctID.size();i++)
	{
		if(vctID[i]==VoxelID){temp=this->vctPathLength[i];}
	}
	return temp;
}

