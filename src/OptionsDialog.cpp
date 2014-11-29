// OptionsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "RayTracer.h"
#include "OptionsDialog.h"
#include "Direct.h"		// for the _chdir() function in OnOK()
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionsDialog dialog


COptionsDialog::COptionsDialog(CWnd* pParent /*=NULL*/)
	: CDialog(COptionsDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(COptionsDialog)
	m_bAtmosFogging = FALSE;
	m_iClampColours = -1;
	m_bDOFEffects = FALSE;
	m_bLightAttenuation = FALSE;
	m_iMaxDepth = 0;
	m_OutputDir = _T("");
	m_bStochasticSS = FALSE;
	m_iRaysPerPixel = -1;
	//}}AFX_DATA_INIT
}


void COptionsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionsDialog)
	DDX_Check(pDX, IDC_ATMOSFOGGING, m_bAtmosFogging);
	DDX_Radio(pDX, IDC_CLAMPCOLOURS, m_iClampColours);
	DDX_Check(pDX, IDC_DOFEFFECTS, m_bDOFEffects);
	DDX_Check(pDX, IDC_LIGHTATTEN, m_bLightAttenuation);
	DDX_Text(pDX, IDC_MAXDEPTH, m_iMaxDepth);
	DDV_MinMaxUInt(pDX, m_iMaxDepth, 1, 100);
	DDX_Text(pDX, IDC_OUTPUTDIR, m_OutputDir);
	DDV_MaxChars(pDX, m_OutputDir, 255);
	DDX_Check(pDX, IDC_STOCHASTICSS, m_bStochasticSS);
	DDX_CBIndex(pDX, IDC_SUPERSAMPLING, m_iRaysPerPixel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionsDialog, CDialog)
	//{{AFX_MSG_MAP(COptionsDialog)
	ON_BN_CLICKED(IDC_DOFEFFECTS, OnDOFeffects)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionsDialog message handlers

BOOL COptionsDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CenterWindow();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COptionsDialog::OnDOFeffects() 
{
	CButton wndDOFCheckBox;

	wndDOFCheckBox.Attach (GetDlgItem(IDC_DOFEFFECTS)->m_hWnd);

	if (wndDOFCheckBox.GetCheck())
	{
		// Depth of field has been selected
		// do what needs to be done

		// need to enable Stochastic Supersampling and then disable the control
		// so that the user can't unselect it
		CButton wndSSCheckBox;
		wndSSCheckBox.Attach(GetDlgItem(IDC_STOCHASTICSS)->m_hWnd);
		
		// save the current state of the check box;
		StochasticSS_PrevState = wndSSCheckBox.GetCheck();

		wndSSCheckBox.SetCheck(1);
		wndSSCheckBox.EnableWindow(FALSE);
		wndSSCheckBox.Detach();

		// we also need to ensure that at least 4 RPP is selected;
		CComboBox wndRPPComboBox;
		wndRPPComboBox.Attach(GetDlgItem(IDC_SUPERSAMPLING)->m_hWnd);
		
		if (wndRPPComboBox.GetCurSel() == 0)
			wndRPPComboBox.SetCurSel(1);
		wndRPPComboBox.Detach();

	}
	else
	{
		// Depth of field has been unselected
		// enable the Stochastic Supersampling check box again
		CButton wndSSCheckBox;

		wndSSCheckBox.Attach(GetDlgItem(IDC_STOCHASTICSS)->m_hWnd);

		wndSSCheckBox.EnableWindow(TRUE);
		// set it back to it's previous state
		wndSSCheckBox.SetCheck(StochasticSS_PrevState);
		wndSSCheckBox.Detach();
	}

	wndDOFCheckBox.Detach();
}

void COptionsDialog::OnOK() 
{

	CEdit wndOutputDir;
	wndOutputDir.Attach(GetDlgItem(IDC_OUTPUTDIR)->m_hWnd);
	CString Dir;
	wndOutputDir.GetWindowText(Dir);

	wndOutputDir.Detach();
	if (_tchdir((LPCTSTR) Dir))
	{
		// Directory does not exist
		MessageBox(_T("The specified directory does not exist\n\nPlease close this message box and create the directory\nthen click OK again"),
					_T("Directory Error"), MB_OK);
	}
	else
	{
		CDialog::OnOK();
	}
}
