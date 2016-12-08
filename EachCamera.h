#pragma once

class CEachCamera
{
public:
	CEachCamera(void);
	~CEachCamera(void);

	//varibles
	CString CameraName;
	double calibration_value;
	double minimum_fc,maximum_fc;
	CString clibration_date;
	CString owner;
	CString note;
};
