// cVGSline.h: interface for the cVGSline class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CVGSLINE_H__D260349B_0CB4_4652_98B0_B99F87458573__INCLUDED_)
#define AFX_CVGSLINE_H__D260349B_0CB4_4652_98B0_B99F87458573__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class cVGSline
{
public:
	cVGSline();
	~cVGSline();
	void Putobject(unsigned int id) {object=id;}
	void PutEachX(double inex) {eachX=inex;}
	void PutEachY(double iney) {eachY=iney;}
	void PutEachZ(double inez) {eachZ=inez;}

	void PutTranX(double intx) {tranX=intx;}
	void PutTranY(double inty) {tranY=inty;}
	void PutTranZ(double intz) {tranZ=intz;}

	void PutRotX(double inrx) {rotX=inrx;}
	void PutRotY(double inry) {rotY=inry;}
	void PutRotZ(double inrz) {rotZ=inrz;}



	unsigned int Getobject() {return object;}
	double GetEachX() {return eachX;}
	double GetEachY() {return eachY;}
	double GetEachZ() {return eachZ;}

	double GetTranX() {return tranX;}
	double GetTranY() {return tranY;}
	double GetTranZ() {return tranZ;}

	double GetRotX() {return rotX;}
	double GetRotY() {return rotY;}
	double GetRotZ() {return rotZ;}



	bool dupIndex;

	double tranX;
	double tranY;
	double tranZ;
	double rotX;
	double rotY;
	double rotZ;

private:
	unsigned int object;
	double eachX;
	double eachY;
	double eachZ;
	unsigned int G;
};
#endif // !defined(AFX_CVGSLINE_H__D260349B_0CB4_4652_98B0_B99F87458573__INCLUDED_)
