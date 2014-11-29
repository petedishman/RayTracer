// BoundedCheckerBoard.h: interface for the CBoundedCheckerBoard class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BOUNDEDCHECKERBOARD_H__40A0E71F_CEC4_4403_855C_314E964308FE__INCLUDED_)
#define AFX_BOUNDEDCHECKERBOARD_H__40A0E71F_CEC4_4403_855C_314E964308FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBoundedCheckerBoard : public CCheckerBoard  
{
public:
	CBoundedCheckerBoard();
	virtual ~CBoundedCheckerBoard();
	virtual BOOL Intersect(CRay *Ray, CVector* Point);

	double Length;
	double Width;

};

#endif // !defined(AFX_BOUNDEDCHECKERBOARD_H__40A0E71F_CEC4_4403_855C_314E964308FE__INCLUDED_)
