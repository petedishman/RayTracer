// BoundedPlane.h: interface for the CBoundedPlane class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BOUNDEDPLANE_H__3FAF41F3_4911_437F_9A9D_D0EB67D1B631__INCLUDED_)
#define AFX_BOUNDEDPLANE_H__3FAF41F3_4911_437F_9A9D_D0EB67D1B631__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// This is a bounded plane class, it's currently a fairly restricted class in that
// the plane is always on the xz plane centred about the origin


class CBoundedPlane : public CInfinitePlane  
{
public:
	CBoundedPlane();
	virtual ~CBoundedPlane();
	virtual BOOL Intersect(CRay *Ray, CVector* Point);

	double Length;
	double Width;
};

#endif // !defined(AFX_BOUNDEDPLANE_H__3FAF41F3_4911_437F_9A9D_D0EB67D1B631__INCLUDED_)
