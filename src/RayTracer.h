// RayTracer.h : main header file for the RAY TRACER application
//

#if !defined(AFX_RAYTRACER_H__719B88C1_991A_487F_9F72_A81B32F3B734__INCLUDED_)
#define AFX_RAYTRACER_H__719B88C1_991A_487F_9F72_A81B32F3B734__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CRayTracerApp:
// See RayTracer.cpp for the implementation of this class
//

class CRayTracerApp : public CWinApp
{
public:
	CRayTracerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRayTracerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CRayTracerApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RAYTRACER_H__719B88C1_991A_487F_9F72_A81B32F3B734__INCLUDED_)
