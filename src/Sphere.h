// Sphere.h: interface for the CSphere class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPHERE_H__64122019_EFFB_431A_91D3_56C115BA3B29__INCLUDED_)
#define AFX_SPHERE_H__64122019_EFFB_431A_91D3_56C115BA3B29__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CSphere : public CSceneObject  
{
public:
	virtual CVector Normal (CVector *Point, CRay *Ray);
	virtual BOOL Intersect(CRay *Ray, CVector* Point);
	CSphere();
	virtual ~CSphere();

	CVector Centre;
	double radius;
};

#endif // !defined(AFX_SPHERE_H__64122019_EFFB_431A_91D3_56C115BA3B29__INCLUDED_)


