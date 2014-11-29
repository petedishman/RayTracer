// Ray.cpp: implementation of the CRay class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RayTracer.h"
#include "RayTracing.h"
#include "Ray.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// The CVector constructor has already initialised the vectors to zero

CRay::CRay()
{
	Dir.Set(0,0,1); // Just initialise the vector to point down the z-axis
	CurrentObjIndex = -1;
}

CRay::~CRay()
{

}

CVector CRay::Evaluate(double Lambda)
{
	CVector result;

	result = Start + Dir * Lambda;

	return result;
}
