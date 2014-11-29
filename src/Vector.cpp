// Vector.cpp: implementation of the CVector class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RayTracer.h"
#include "Vector.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVector::CVector()
{
	x = y = z = 0.0;
}

CVector::CVector(double new_x, double new_y, double new_z)
{
	x = new_x;
	y = new_y;
	z = new_z;
}

CVector::~CVector()
{

}

//////////////////////////////////////////////////////////////////////
// Member retrieval / setting
//////////////////////////////////////////////////////////////////////

void CVector::Set(double new_x, double new_y, double new_z)
{
	x = new_x;
	y = new_y;
	z = new_z;
}

double CVector::Get_x(void)
{
	return x;
}

double CVector::Get_y(void)
{
	return y;
}

double CVector::Get_z(void)
{
	return z;
}


//////////////////////////////////////////////////////////////////////
// Vector math operations (+ operator overloads)
//////////////////////////////////////////////////////////////////////

void CVector::Normalize()
{
	// Normalise the vector to unit length

	double mag;

	mag = Length();

	x /= mag;
	y /= mag;
	z /= mag;
}

double CVector::Length()
{
	// calculate the length of the vector from (0,0,0) to (x,y,z)

	return sqrt(x*x + y*y + z*z);

}

CVector CVector::operator+ (CVector arg)
{
	CVector temp;

	temp.x = x + arg.x;
	temp.y = y + arg.y;
	temp.z = z + arg.z;

	return temp;
}

CVector CVector::operator -(CVector arg)
{
	CVector temp;

	temp.x = x - arg.x;
	temp.y = y - arg.y;
	temp.z = z - arg.z;

	return temp;
}

double CVector::Dot(CVector arg)
{
	// This function will do this.arg  i.e. this dotted with arg.

	return (x * arg.x) + (y * arg.y) + (z * arg.z);
}

CVector CVector::Cross(CVector arg)
{
	// This function will perform the cross product of the two vectors
	CVector temp;

	temp.x = (y * arg.z)  - (z * arg.y);
	temp.y = (-x * arg.z) + (z * arg.x);
	temp.z = (x * arg.y)  - (y * arg.x);

	temp.Normalize();

	return temp;
}

CVector CVector::operator* (double arg)
{
	// this function scales the vector by a number
	// must be used CVector*double;

	CVector temp;
	temp.x = x * arg;
	temp.y = y * arg;
	temp.z = z * arg;

	return temp;
}
