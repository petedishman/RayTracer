#if !defined(AFX_RENDERPROGRESSDLG_H__22E6ADAB_50B9_46D0_9F6C_CC6AC5716CC2__INCLUDED_)
#define AFX_RENDERPROGRESSDLG_H__22E6ADAB_50B9_46D0_9F6C_CC6AC5716CC2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RenderProgressDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRenderProgressDlg dialog

class CRenderProgressDlg : public CDialog
{
// Construction
public:
	CRenderProgressDlg(CWnd* pParent = NULL);   // standard constructor

	CWnd *ParentView;
	int m_nNoOfLines;
	bool RenderingInProgress;
	bool Paused;
// Dialog Data
	//{{AFX_DATA(CRenderProgressDlg)
	enum { IDD = IDD_RENDERPROGRESS };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRenderProgressDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRenderProgressDlg)
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnOneLineRendered (WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRenderComplete (WPARAM wParam, LPARAM lParam);
	afx_msg void OnPauseButton();
	afx_msg void OnResumeButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RENDERPROGRESSDLG_H__22E6ADAB_50B9_46D0_9F6C_CC6AC5716CC2__INCLUDED_)
