// Pixel.h: interface for the CPixel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PIXEL_H__8D2A4D22_926C_4A6D_94AA_A588EE70C0EF__INCLUDED_)
#define AFX_PIXEL_H__8D2A4D22_926C_4A6D_94AA_A588EE70C0EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPixel  
{
public:
	int x,y,ColorValue;
	CPixel();
	CPixel(int px,int py,int value=16777215){x=px;y=py;ColorValue=value;}
	virtual ~CPixel();
};

#endif // !defined(AFX_PIXEL_H__8D2A4D22_926C_4A6D_94AA_A588EE70C0EF__INCLUDED_)
