// Cylinder.cpp: implementation of the CCylinder class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RayTracer.h"
#include "RayTracing.h"
#include "Cylinder.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCylinder::CCylinder()
{

}

CCylinder::~CCylinder()
{

}

BOOL CCylinder::Intersect(CRay *Ray, CVector *Point)
{
	// For now this is effectively an infinite cylinder need to do
	// bounded plane checks at each end too.

	// These aren't going to have good names as they're straight out
	// of my notes ok.

	CVector Result;

	double a,b,c;	// as in equation of a quadratic;

	CVector SminusA;
	double DdotB, SminusA_dotB, DdotS, DdotA, SdotS, AdotS, AdotA;

	SminusA = Ray->Start - StartPoint;

	DdotB = Ray->Dir.Dot(Axis);

	SminusA_dotB = SminusA.Dot(Axis);

	DdotS = Ray->Dir.Dot(Ray->Start);

	DdotA = Ray->Dir.Dot(StartPoint);

	SdotS = Ray->Start.Dot(Ray->Start);

	AdotS = StartPoint.Dot(Ray->Start);

	AdotA = StartPoint.Dot(StartPoint);


	a = (DdotB * DdotB) - 1.0;

	b = 2.0 * ( ( SminusA_dotB * DdotB ) - (DdotS - DdotA) );

	c = (SminusA_dotB * SminusA_dotB) - SdotS + (2.0 * AdotS) - AdotA + (Radius * Radius);

	double determinant;

	determinant = (b * b) - (4 * a * c);

	if (determinant < 0.0)
	{
		// no intersection

		// shouldn't return here as ray could potentially be going straight down the centre of the cylinder
		return FALSE;
	}
	else if (determinant == 0.0)
	{
		// glancing hit
		//return FALSE;
	}
	// else we got two solutions

	// a direct hit

	determinant = sqrt(determinant);

	double lambda1, lambda2;

	lambda1 = (-b + determinant) / ((double) 2.0 * a);
	lambda2 = (-b - determinant) / ((double) 2.0 * a);

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

	// at this point we have the nearest lambda value for the intersection of an infinite length cylinder
	// first we need to check that that is in the right range, we then also need to check against the two
	// bounding planes for this object (checking it does actually hit the plane in the right place)
	// finally we need to return the lowest lambda out of a possible choice of 3, actually no the most you
	// could get is two. as it couldn't hit the cylinder wall and both edges.


	// need to calculate beta from the cylinder intersection.
	double beta;

	beta = ((Ray->Evaluate(lambda)) - StartPoint).Dot(Axis);

	// if beta is < 0 we ignore it, if beta > length we also ignore it.

	// This will be true if we want to include the cylinder lambda in our tests later
	BOOL CylinderHit = (beta >= 0.0) && (beta <= Length);
	double CylinderLambda;
	if (CylinderHit)
		CylinderLambda = lambda;

	// now we want to check if it hit the first plane in the right spot.

	// check for intersection with whole plane

	BOOL FirstPlaneHit;
	double FirstPlaneLambda;
	double divisor = Ray->Dir.Dot(FirstPlaneNormal);

	if (divisor != 0.0)
	{
		FirstPlaneLambda = (-(FirstPlaneDistance + (Ray->Start.Dot(FirstPlaneNormal)))) / divisor;

		if (FirstPlaneLambda > 0.0)
		{
			// now we need to tell if this point is within the necessary bounds
			CVector PointOnFirstPlane = Ray->Evaluate(FirstPlaneLambda);

			if ( (PointOnFirstPlane - StartPoint).Length() < Radius )
				FirstPlaneHit = TRUE;
			else
				FirstPlaneHit = FALSE;
		}
		else
			FirstPlaneHit = FALSE;
	}
	else
	{
		FirstPlaneHit = FALSE;
	}

	// now we need to check if it hit the second plane.
	BOOL SecondPlaneHit;
	double SecondPlaneLambda;
	divisor = Ray->Dir.Dot(SecondPlaneNormal);

	if (divisor != 0.0)
	{
		SecondPlaneLambda = (-(SecondPlaneDistance + (Ray->Start.Dot(SecondPlaneNormal)))) / divisor;

		if (SecondPlaneLambda > 0.0)
		{
			// now we need to tell if this point is within the necessary bounds
			CVector PointOnSecondPlane = Ray->Evaluate(SecondPlaneLambda);

			if ( (PointOnSecondPlane - EndPoint).Length() < Radius )
				SecondPlaneHit = TRUE;
			else
				SecondPlaneHit = FALSE;
		}
		else
			SecondPlaneHit = FALSE;
	}
	else
	{
		SecondPlaneHit = FALSE;
	}


	// now we should know exactly what we hit
	// so we want to return the shortest lambda value.

	// check we hit something
	if ( (!CylinderHit) && (!FirstPlaneHit) && (!SecondPlaneHit))
		return FALSE;

	double MinLambda = 99999999999999.99;

	if (CylinderHit && (CylinderLambda < MinLambda))
		MinLambda = CylinderLambda;
	if (FirstPlaneHit && (FirstPlaneLambda < MinLambda))
		MinLambda = FirstPlaneLambda;
	if (SecondPlaneHit && (SecondPlaneLambda < MinLambda))
		MinLambda = SecondPlaneLambda;


	Result = Ray->Evaluate(MinLambda);

	(*Point) = Result;

	return TRUE;

}


CVector CCylinder::Normal(CVector *Point, CRay *Ray)
{

	CVector Result;
	CVector Diagonal;
	double Mod_Diagonal;

	double Beta;

	Diagonal = (*Point) - StartPoint;
	Mod_Diagonal = Diagonal.Length();
	Diagonal.Normalize();

	Beta = Mod_Diagonal * Axis.Dot(Diagonal);

	Result = (*Point) - (StartPoint + (Axis * Beta));

	// can we cheat and say that if the length of result is almost the radius return Result as the normal
	// if the length is less then the radius then we're at one of the end points so return  +/- Axis
	// but which one, could pick depending on distance of point from startpoint.
	// i.e if distance from startpoint was also less then radius return +Axis else return -Axis

	double tempLength = Result.Length();

	if (tempLength >= (Radius - 0.005))
	{
		// length is pretty much the radius so we're on the curved part of the cylinder, return what we've got
		Result.Normalize();
		return Result;
	}
	//else we've hit one of the edges

	CVector ZeroVec;

	if (Beta <= 0.005)
	{
		// we've hit the first plane
		Result = ZeroVec - FirstPlaneNormal;
		Result.Normalize();
		return Result;
	}
	// else we've hit the other plane

	Result = ZeroVec - SecondPlaneNormal;
	Result.Normalize();
	return Result;

	// TODO: this probably won't work for internal rays, but it might do.

}

void CCylinder::InitialiseBounds()
{
	// This function will initialise the values needed for doing
	// finite length cylinder testing.

	CVector ZeroVec;

	FirstPlaneNormal = Axis;
	FirstPlaneDistance =  -(FirstPlaneNormal.Dot(StartPoint));

	EndPoint = StartPoint + (Axis * Length);

	SecondPlaneNormal = ZeroVec - FirstPlaneNormal;

	SecondPlaneDistance = -(SecondPlaneNormal.Dot(EndPoint));
}
