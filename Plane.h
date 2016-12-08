#pragma once
#include "cpoint.h"

class CPlane :
	public cPoint
{
public:
	CPlane(void);
	~CPlane(void);
	double d;
};
