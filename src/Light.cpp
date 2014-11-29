// Light.cpp: implementation of the CLight class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RayTracer.h"
#include "RayTracing.h"
#include "Light.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLight::CLight()
{
	Colour.Set(1.0f, 1.0f, 1.0f);
	c1 = 1.0;
	c2 = 0.0;
	c3 = 0.0;
}

CLight::CLight(double x, double y, double z)
{
	Colour.Set(1.0f, 1.0f, 1.0f);
	Position.Set(x, y, z);
	c1 = 1.0;
	c2 = 0.0;
	c3 = 0.0;
}

CLight::~CLight()
{

}
