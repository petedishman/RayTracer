// RayTracerView.h : interface of the CRayTracerFileView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RAYTRACERVIEW_H__0A36326D_C310_475C_8E7F_9B8D642B0893__INCLUDED_)
#define AFX_RAYTRACERVIEW_H__0A36326D_C310_475C_8E7F_9B8D642B0893__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RenderProgressDlg.h"		


class CRayTracerFileView : public CEditView
{
protected: // create from serialization only
	CRayTracerFileView();
	DECLARE_DYNCREATE(CRayTracerFileView)


// member variables
public:
	CRenderProgressDlg *m_pProgressDlg;

// Attributes
public:
	CRayTracerDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRayTracerFileView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL GetRenderingState();
	virtual ~CRayTracerFileView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

private:
	CFont Font;
	BOOL Rendering;


protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CRayTracerFileView)
	afx_msg void OnRenderImage();
	afx_msg void OnUpdateRenderImage(CCmdUI* pCmdUI);
	afx_msg LRESULT OnDialogDestroyed (WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRenderPause (WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRenderResume (WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRenderCancel (WPARAM wParam, LPARAM lParam);
	afx_msg void OnRenderOptions();
	afx_msg void OnInsertGlobalSettings();
	afx_msg void OnUpdateInsertGlobalSettings(CCmdUI* pCmdUI);
	afx_msg void OnInsertBoundedCheckerBoard();
	afx_msg void OnUpdateInsertBoundedCheckerBoard(CCmdUI* pCmdUI);
	afx_msg void OnInsertBoundedPlane();
	afx_msg void OnUpdateInsertBoundedPlane(CCmdUI* pCmdUI);
	afx_msg void OnInsertCamera();
	afx_msg void OnUpdateInsertCamera(CCmdUI* pCmdUI);
	afx_msg void OnInsertCheckerBoard();
	afx_msg void OnUpdateInsertCheckerBoard(CCmdUI* pCmdUI);
	afx_msg void OnInsertCylinder();
	afx_msg void OnUpdateInsertCylinder(CCmdUI* pCmdUI);
	afx_msg void OnInsertInfinitePlane();
	afx_msg void OnUpdateInsertInfinitePlane(CCmdUI* pCmdUI);
	afx_msg void OnInsertLight();
	afx_msg void OnUpdateInsertLight(CCmdUI* pCmdUI);
	afx_msg void OnInsertSphere();
	afx_msg void OnUpdateInsertSphere(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRenderOptions(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in RayTracerView.cpp
inline CRayTracerDoc* CRayTracerFileView::GetDocument()
   { return (CRayTracerDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RAYTRACERVIEW_H__0A36326D_C310_475C_8E7F_9B8D642B0893__INCLUDED_)
