// Ray.h: interface for the CRay class.
//
//////////////////////////////////////////////////////////////////////
// 
// This class is the representation of a light ray sent through
// the scene
// it will just contain a start position and a direction
// both of which are just CVector's


#if !defined(AFX_RAY_H__D1A5C24B_DC9A_46DD_8862_A5D88CAAAF28__INCLUDED_)
#define AFX_RAY_H__D1A5C24B_DC9A_46DD_8862_A5D88CAAAF28__INCLUDED_

#include "Vector.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CRay  
{
public:
	CVector Evaluate (double Lambda);
	CRay();
	virtual ~CRay();

	CVector Start;
	CVector Dir;
	double  CurrentObjIndex;

};

#endif // !defined(AFX_RAY_H__D1A5C24B_DC9A_46DD_8862_A5D88CAAAF28__INCLUDED_)
