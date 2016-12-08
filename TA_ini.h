#pragma once
#include "utils.h"

class CTA_ini:public CObject
{
public:
	//constructor
	CTA_ini(void);
	CTA_ini(CString programpath);

	//destructor
	virtual ~CTA_ini(void);

	DECLARE_SERIAL( CTA_ini );
	void Serialize( CArchive& archive );

	//variables
	CString program_path;
	double last_cam_distance;
	double last_cam_height;
	double last_fc;
	int LastCamera;
	CString CurrentPath;
	CString OutputLocation;
	std::vector<CString> vctLastFile;

	//functions
	void ReadIni();
	void WriteIni();
};
