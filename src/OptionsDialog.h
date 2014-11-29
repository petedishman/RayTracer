#if !defined(AFX_OPTIONSDIALOG_H__4D9629FE_E6CE_483A_81AC_515F6842181C__INCLUDED_)
#define AFX_OPTIONSDIALOG_H__4D9629FE_E6CE_483A_81AC_515F6842181C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionsDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COptionsDialog dialog

class COptionsDialog : public CDialog
{
// Construction
public:
	COptionsDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COptionsDialog)
	enum { IDD = IDD_OPTIONSDIALOG };
	BOOL	m_bAtmosFogging;
	int		m_iClampColours;
	BOOL	m_bDOFEffects;
	BOOL	m_bLightAttenuation;
	UINT	m_iMaxDepth;
	CString	m_OutputDir;
	BOOL	m_bStochasticSS;
	int		m_iRaysPerPixel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptionsDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation

private:
	int StochasticSS_PrevState;

protected:

	// Generated message map functions
	//{{AFX_MSG(COptionsDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnDOFeffects();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONSDIALOG_H__4D9629FE_E6CE_483A_81AC_515F6842181C__INCLUDED_)
