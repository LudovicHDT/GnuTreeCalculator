#pragma once
#include <vector>

class CEachEquation
{
public:
	CEachEquation(void);
	virtual ~CEachEquation(void);

	std::vector<unsigned int>vctID;
	std::vector<double>vctPathLength;

	double lnP0;
	double SectionArea;
	double G;
	double sumVolume;
	double sumPathLength;
	bool IsHaveThisID(int ID);
	double GetPathLength(int VoxelID);
};
