// Cylinder.h: interface for the CCylinder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CYLINDER_H__5D864F27_2FB2_4AC2_812C_15EA82A0D476__INCLUDED_)
#define AFX_CYLINDER_H__5D864F27_2FB2_4AC2_812C_15EA82A0D476__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CCylinder : public CSceneObject  
{
public:
	CCylinder();
	virtual ~CCylinder();
	virtual BOOL Intersect(CRay *Ray, CVector *Point);
	virtual CVector Normal(CVector *Point, CRay *Ray);
	void InitialiseBounds();

	// Cylinder parameters
	CVector Axis;
	double Length;
	double Radius;
	CVector StartPoint;

private:
	CVector FirstPlaneNormal;
	CVector SecondPlaneNormal;
	double FirstPlaneDistance;
	double SecondPlaneDistance;

	CVector EndPoint;
};

#endif // !defined(AFX_CYLINDER_H__5D864F27_2FB2_4AC2_812C_15EA82A0D476__INCLUDED_)
