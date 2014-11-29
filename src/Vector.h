// Vector.h: interface for the CVector class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VECTOR_H__36282E09_0725_4B6B_ABB0_C77F3140A3DF__INCLUDED_)
#define AFX_VECTOR_H__36282E09_0725_4B6B_ABB0_C77F3140A3DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CVector  
{
	// Initialisation
public:
	CVector ();
	CVector (double new_x, double new_y, double new_z);
	virtual ~CVector();

	// Member variables
private:
	double x;	// I think we should use doubles as they will give more precision which is good
	double y;	// speed isn't that big an issue so it should be alright
	double z;

	// Member functions
public:
	void	Set		(double new_x, double new_y, double new_z);
	double	Get_x	();
	double	Get_y	();
	double	Get_z	();
	CVector	Cross	(CVector arg);
	double	Dot		(CVector arg);
	double	Length	();
	void	Normalize();

	// Operator Overloads
	CVector operator-(CVector arg);
	CVector operator+ (CVector arg);
	CVector operator* (double arg);
};

#endif // !defined(AFX_VECTOR_H__36282E09_0725_4B6B_ABB0_C77F3140A3DF__INCLUDED_)
