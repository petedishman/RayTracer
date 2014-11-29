// BoundedCheckerBoard.cpp: implementation of the CBoundedCheckerBoard class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RayTracer.h"

#include "RayTracing.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBoundedCheckerBoard::CBoundedCheckerBoard()
{

}

CBoundedCheckerBoard::~CBoundedCheckerBoard()
{

}

BOOL CBoundedCheckerBoard::Intersect(CRay *Ray, CVector* Point)
{
	CVector PointOnPlane;


	// as this planes are restricted to be centered on (0,0,0)
	// we know that all plane intersections will occur at y == 0;
	// then we can do a simple bounds check using the x,z coords

	if (CInfinitePlane::Intersect(Ray, &PointOnPlane))
	{
		// The infinite plane was hit, check if the bounded one was
		if ((PointOnPlane.Get_x() >= (-0.5 * Width)) && (PointOnPlane.Get_x() <= (0.5 * Width)))
		{
			// it's alright in the x direction, now check z
			if ((PointOnPlane.Get_z() >= (-0.5 * Length)) && (PointOnPlane.Get_z() <= (0.5 * Length)))
			{
				// ok in z too
				(*Point) = PointOnPlane;
				return TRUE;
			}
		}
		// else
		return FALSE;
	}
	else
	{
		// the infinite plane wasn't hit.
		return FALSE;
	}
}

