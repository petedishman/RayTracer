// RayTracerDoc.h : interface of the CRayTracerDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RAYTRACERDOC_H__68B32878_011A_4B2A_AF62_B3CCCCDD16F9__INCLUDED_)
#define AFX_RAYTRACERDOC_H__68B32878_011A_4B2A_AF62_B3CCCCDD16F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RayTracing.h"
#include "World.h"


class CRayTracerDoc : public CDocument
{
protected: // create from serialization only
	CRayTracerDoc();
	DECLARE_DYNCREATE(CRayTracerDoc)

// Attributes
public:
	CWorld* World;
	CWinThread *RenderingThread;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRayTracerDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRayTracerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CRayTracerDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RAYTRACERDOC_H__68B32878_011A_4B2A_AF62_B3CCCCDD16F9__INCLUDED_)
