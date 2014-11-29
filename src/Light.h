// Light.h: interface for the CLight class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LIGHT_H__CBE85B62_9857_423F_B28D_D0A820B52434__INCLUDED_)
#define AFX_LIGHT_H__CBE85B62_9857_423F_B28D_D0A820B52434__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLight  
{
public:
	CLight();
	CLight(double x, double y, double z);
	virtual ~CLight();

	CVector Position;
	CColour Colour;

	// constants used in Light attenuation
	double c1;
	double c2;
	double c3;

};

#endif // !defined(AFX_LIGHT_H__CBE85B62_9857_423F_B28D_D0A820B52434__INCLUDED_)
