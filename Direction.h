// Direction.h: interface for the CDirection class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIRECTION_H__118ABCEF_8AF5_4DC5_9AF7_E643AE7AAC62__INCLUDED_)
#define AFX_DIRECTION_H__118ABCEF_8AF5_4DC5_9AF7_E643AE7AAC62__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "cPoint.h"

class CDirection
{
public:
	double gapfraction;
	double LAD,PathLength;
	double beam_volume,section_area;
	double number_pixel;
	cPoint unitDirection; //cPoint
	double px,py; //number of pixel on the picture
	double angle_x,angle_y;

	CDirection();
	virtual ~CDirection();

	double CalculNormalInc(void);
};

#endif // !defined(AFX_DIRECTION_H__118ABCEF_8AF5_4DC5_9AF7_E643AE7AAC62__INCLUDED_)
