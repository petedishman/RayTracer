// Sphere.cpp: implementation of the CSphere class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RayTracer.h"
#include "RayTracing.h"
#include "Sphere.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSphere::CSphere()
{

}

CSphere::~CSphere()
{

}

BOOL CSphere::Intersect(CRay *Ray, CVector *Point)
{
	// This function will take the CRay and check to see
	// if it intersects with the sphere, if it does it
	// returns TRUE and the point of intersection, if it
	// doesn't it returns false.

	// do not set the second parameter to NULL when calling this function !!!!!!!

	// The direction of Ray is assumed to be normalised already
	CVector temp;

	temp = Ray->Start - Centre;

	double b = 2.0 * Ray->Dir.Get_x() * temp.Get_x();
	b += 2.0 * Ray->Dir.Get_y() * temp.Get_y();
	b += 2.0 * Ray->Dir.Get_z() * temp.Get_z();

	double c = temp.Get_x() * temp.Get_x();
	c += temp.Get_y() * temp.Get_y();
	c += temp.Get_z() * temp.Get_z();
	c -= radius * radius;

	double determinant = (b * b) - (4 * c);

	// can test on determinant as to whether anything intersects
	// we will count a grazing touch (i.e. determinant = 0)

	if (determinant < 0.0)
	{
		// we don't have an intersection so quit
		return FALSE;
	}
	else if (determinant == 0.0)
	{
		/*
		double lambda;
	
		// a grazing touch
		lambda = -b / (double) 2.0;

		// might as well reuse temp (CVector)
		temp = Ray->Evaluate(lambda);

		// return the point of intersection
		(*Point) = temp;

		return true;
		*/

		return FALSE;
	}

	// else we have two lambda values

	double sqrt_term;

	sqrt_term = sqrt(determinant);

	double lambda1, lambda2;

	lambda1 = (-b + sqrt_term) / (double) 2.0;
	lambda2 = (-b - sqrt_term) / (double) 2.0;
	// if either of the lambda's are negative then that doesn't count as an intersection
	// as that is behind our start term!


	// the lesser of the two lambdas is our intersection point

	double lambda;

	if (lambda1 < 0.0)
	{
		if (lambda2 < 0.0)
			return FALSE;
		else
			lambda = lambda2;
	}
	else // lambda1 > 0
	{
		if (lambda2 < 0.0)
		{
			lambda = lambda1;
		}
		else // both are positive
		{
			if (lambda1 <= lambda2)
				lambda = lambda1;
			else
				lambda = lambda2;
		}
	}

	temp = Ray->Evaluate(lambda);

	(*Point) = temp;

	return TRUE;
}

CVector CSphere::Normal(CVector *Point, CRay *Ray)
{
	// This returns the normalised normal vector from the given point
	// we just assume the point is on the surface.

	CVector result;

	result = (*Point) - Centre;
	result.Normalize();

	return result;
}

