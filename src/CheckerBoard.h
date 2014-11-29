// CheckerBoard.h: interface for the CCheckerBoard class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHECKERBOARD_H__DA0E5304_B3F7_4896_9180_D3E2FAA92344__INCLUDED_)
#define AFX_CHECKERBOARD_H__DA0E5304_B3F7_4896_9180_D3E2FAA92344__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCheckerBoard : public CInfinitePlane  
{
public:
	CCheckerBoard();
	virtual ~CCheckerBoard();

	CColour Colour1;
	CColour Colour2;
	double CheckSize;

private:
	virtual void InitialiseValues(s_IntersectionInfo *Intersection, CRay *IncomingRay);

};

#endif // !defined(AFX_CHECKERBOARD_H__DA0E5304_B3F7_4896_9180_D3E2FAA92344__INCLUDED_)
