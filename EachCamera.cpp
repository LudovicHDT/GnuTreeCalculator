#include "stdafx.h"
#include ".\eachcamera.h"

CEachCamera::CEachCamera(void)
{
	CameraName="Not definded";
	calibration_value=0;
	minimum_fc=0;
	maximum_fc=999;
	clibration_date="Not definded";
	owner="Not definded";
	note="None";
}

CEachCamera::~CEachCamera(void)
{
}
