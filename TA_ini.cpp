#include "stdafx.h"
#include ".\ta_ini.h"

IMPLEMENT_SERIAL( CTA_ini, CObject, 1 )

CTA_ini::CTA_ini(void)
{
}

CTA_ini::~CTA_ini(void)
{
	vctLastFile.clear();
}

CTA_ini::CTA_ini(CString programpath)
{
	program_path=programpath;
	CurrentPath=programpath;
	vctLastFile.resize(0);
}

void CTA_ini::Serialize( CArchive& archive )
{
    // call base class function first
    // base class is CObject in this case
    CObject::Serialize( archive );

    // now do the stuff for our specific class
    if( archive.IsStoring() )
	{
        //archive << m_name << m_number;
		CString ms="Storing file "+this->program_path+"TA.ini";
		//AfxMessageBox(ms);
	}
    else
	{
        //archive >> m_name >> m_number;
		CString ms="Loading file "+this->program_path+"TA.ini";
		//AfxMessageBox(ms);
	}
}

void CTA_ini::ReadIni()
{
	CString filename=this->program_path+"TA.ini";
	CFile theFile;
	CFileException fileException;
	if ( !theFile.Open( filename.GetString(), CFile::modeRead, &fileException ))
	{
		TRACE( "Can't open the INI file %s, error = %u\n",
		filename.GetString(), fileException.m_cause );
	}
	CArchive storefile(&theFile, CArchive::load);
	Serialize(storefile);

	storefile.Close();
	theFile.Close();
}//void CTA_ini::ReadIni()

void CTA_ini::WriteIni()
{
	CString filename=this->program_path+"TA.ini";
	CFile theFile;
	CFileException fileException;
	if ( !theFile.Open( filename.GetString(), CFile::modeCreate | 
                 CFile::modeWrite, &fileException ) )
	{
		TRACE( "Can't wirte the INI file %s, error = %u\n",
		filename.GetString(), fileException.m_cause );
	}

	CArchive storefile(&theFile, CArchive::store);
	Serialize(storefile);

	storefile.Close();
	theFile.Close();
}//void CTA_ini::ReadIni()