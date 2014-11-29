// Colour.cpp: implementation of the CColour class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RayTracer.h"
#include "Colour.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CColour::CColour()
{
	red = green = blue = 0.0;
}

CColour::~CColour()
{

}

CColour::CColour(BYTE r, BYTE g, BYTE b)
{
	// If the colour is given in BYTE form then
	// it can only be in the range 0 to 255

	red   = r / 255.0f;
	green = g / 255.0f;
	blue  = b / 255.0f;
}

CColour::CColour(float r, float g, float b)
{
	// As we're constructing a new colour here
	// I think we should clamp the colour values to 0.0 to 1.0 just in case

	red   = Clamp_f(r);
	green = Clamp_f(g);
	blue  = Clamp_f(b);
}


float CColour::Get_R()
{
	// Get colour unclamped
	return red;
}

float CColour::Get_cR()
{
	// Get colour clamped
	return Clamp_f (red);
}

float CColour::Get_G()
{
	// Get colour unclamped
	return green;
}

float CColour::Get_cG()
{
	// Get colour clamped
	return Clamp_f (green);
}

float CColour::Get_B()
{
	// Get colour unclamped
	return blue;
}

float CColour::Get_cB()
{
	// Get colour clamped
	return Clamp_f (blue);
}


float CColour::Clamp_f(float colour)
{
	if (colour < 0.0)
		return 0.0;
	if (colour > 1.0)
		return 1.0;

	return colour;
}

BYTE CColour::Get_Rbyte()
{
	return (BYTE) (Clamp_f(red) * 255.0);
}

BYTE CColour::Get_Gbyte()
{
	return (BYTE) (Clamp_f(green) * 255.0);
}

BYTE CColour::Get_Bbyte()
{
	return (BYTE) (Clamp_f(blue) * 255.0);
}

void CColour::Set_R(BYTE r)
{
	red = r / 255.0f;
}

void CColour::Set_G(BYTE g)
{
	green = g / 255.0f;
}

void CColour::Set_B(BYTE b)
{
	blue = b / 255.0f;
}

void CColour::Set_R(float r)
{
	red = r;
}

void CColour::Set_G(float g)
{
	green = g;
}

void CColour::Set_B(float b)
{
	blue = b;
}

void CColour::Set(float r, float g, float b)
{
	red = r;
	green = g;
	blue = b;
}

void CColour::Set(BYTE r, BYTE g, BYTE b)
{
	red = r / 255.0f;
	green = g / 255.0f;
	blue = b / 255.0f;
}

CColour CColour::operator +(CColour arg)
{
	CColour temp;

	temp.red   = red + arg.red;
	temp.green = green + arg.green;
	temp.blue  = blue + arg.blue;

	return temp;
}

CColour CColour::operator -(CColour arg)
{
	// This might come in handy for lighting // diffuse - specular and all that

	CColour temp;

	temp.red   = red - arg.red;
	temp.green = green - arg.green;
	temp.blue  = blue - arg.blue;

	return temp;
}

CColour CColour::operator* (double arg)
{
	// this will multiply all the colour components by the scalar value given

	CColour temp;

	temp.red   = (float) (red * arg);
	temp.green = (float) (green * arg);
	temp.blue  = (float) (blue * arg);

	return temp;
}

CColour CColour::operator *(CColour arg)
{
	CColour temp;

	temp.red   = red * arg.red;
	temp.green = green * arg.green;
	temp.blue  = blue * arg.blue;

	return temp;
}

int CColour::operator ==(CColour arg)
{
	return ((red == arg.red) && (green == arg.green) && (blue == arg.blue));
}

int	CColour::operator!= (CColour arg)
{
	return !((*this) == arg);
}