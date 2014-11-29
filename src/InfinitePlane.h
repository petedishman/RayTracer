// InfinitePlane.h: interface for the CInfinitePlane class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFINITEPLANE_H__2F458A99_73F2_4F72_BEAA_025E6474EFE6__INCLUDED_)
#define AFX_INFINITEPLANE_H__2F458A99_73F2_4F72_BEAA_025E6474EFE6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



class CInfinitePlane : public CSceneObject  
{
public:
	virtual CVector Normal (CVector *Point, CRay *Ray);
	virtual BOOL Intersect(CRay *Ray, CVector* Point);
	CInfinitePlane();
	virtual ~CInfinitePlane();

	CVector PlaneNormal;
	double DistanceToOrigin;

};

#endif // !defined(AFX_INFINITEPLANE_H__2F458A99_73F2_4F72_BEAA_025E6474EFE6__INCLUDED_)
