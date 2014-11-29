// InfinitePlane.cpp: implementation of the CInfinitePlane class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RayTracer.h"
#include "RayTracing.h"
#include "InfinitePlane.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInfinitePlane::CInfinitePlane()
{

}

CInfinitePlane::~CInfinitePlane()
{

}

CVector CInfinitePlane::Normal (CVector *Point, CRay *Ray)
{
	// Infinite planes are going to be 2 sided and therefore we need
	// to return a normal that depends on what side of the plane the ray
	// hits it
	CVector temp, ZeroVec, RayDir;

	temp = PlaneNormal;
	temp.Normalize();

	// before we do this we need to invert the dir of the ray as we
	// have the same old problem as before. i.e. they're both heading
	// in opposite directions so you get larger angles then you'd think

	RayDir = ZeroVec - Ray->Dir;


	// if the dot product of the ray dir and the normal is less than zero
	// then the ray has hit the plane on the otherside so we want to invert
	// the normal and return that instead
	if (RayDir.Dot(temp) < 0.0)
		temp = ZeroVec - temp;

	return temp;
}

BOOL CInfinitePlane::Intersect(CRay *Ray, CVector* Point)
{
	double Divisor;
	double lambda;


	Divisor = Ray->Dir.Dot(PlaneNormal);

	if (Divisor != 0.0)
	{
		lambda = (-(PlaneNormal.Dot(Ray->Start) + DistanceToOrigin)) / Divisor;

		if (lambda > 0.0)
		{
			(*Point) = Ray->Evaluate(lambda);

			return TRUE;
		}
	}
	
	return FALSE;
}
