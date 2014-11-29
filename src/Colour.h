// Colour.h: interface for the CColour class.
//
//
// This class will represent all of the colour information in 
// the ray tracer, it is RGB based and will store colour internally
// as three 'float' values each ranging from 0.0 to 1.0
// with 0.0 being black and 1.0 being white
//
// It will be possible to set/get the colour information in either
// 0.0 to 1.0 or 0 to 255 format (BYTE)
// 
// When retrieving the colour it will be clamped to the maximum values
//
//
//
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COLOUR_H__494E93B2_19A7_4FF3_8823_1ABB31E31941__INCLUDED_)
#define AFX_COLOUR_H__494E93B2_19A7_4FF3_8823_1ABB31E31941__INCLUDED_

#include "StdAfx.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



class CColour  
{
public:
	CColour operator* (CColour arg);
	CColour operator* (double arg);
	int     operator== (CColour arg);
	CColour operator+ (CColour arg);
	CColour operator- (CColour arg);
	int		operator!= (CColour arg);

	void Set(BYTE r, BYTE g, BYTE b);
	void Set(float r, float g, float b);

	BYTE Get_Rbyte();
	BYTE Get_Gbyte();
	BYTE Get_Bbyte();
	float Get_R();
	float Get_cR();
	float Get_G();
	float Get_cG();
	float Get_B();
	float Get_cB();

	void Set_R(BYTE r);
	void Set_G(BYTE g);
	void Set_B(BYTE b);
	void Set_R(float r);
	void Set_G(float g);
	void Set_B(float b);

	CColour();
	CColour(BYTE r, BYTE g, BYTE b);
	CColour(float r, float g, float b);
	virtual ~CColour();

private:
	float Clamp_f(float colour);
	float red;
	float green;
	float blue;

};

#endif // !defined(AFX_COLOUR_H__494E93B2_19A7_4FF3_8823_1ABB31E31941__INCLUDED_)
