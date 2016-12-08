#pragma once
#include "EachCamera.h"
#include <vector>

class CCameraPool
{
public:
	CCameraPool(void);
	~CCameraPool(void);
	std::vector<CEachCamera> vctCamera;

	double GetCalibrationValue(CString CameraName);
	CEachCamera * GetpCamera(CString camera_name);
	bool Cameras_Load();
	void LoadDefault();
	void SaveCameras();
	CString filename;
};
