// RenderProgressDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RayTracer.h"
#include "RenderProgressDlg.h"
#include "Mmsystem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRenderProgressDlg dialog


CRenderProgressDlg::CRenderProgressDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRenderProgressDlg::IDD, pParent)
{
	ParentView = pParent;
	//{{AFX_DATA_INIT(CRenderProgressDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CRenderProgressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRenderProgressDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRenderProgressDlg, CDialog)
	//{{AFX_MSG_MAP(CRenderProgressDlg)
	ON_MESSAGE(WM_USER_ONE_LINE_RENDERED, OnOneLineRendered)
	ON_MESSAGE(WM_USER_RENDER_COMPLETE, OnRenderComplete)
	ON_BN_CLICKED(IDC_PAUSEBUTTON, OnPauseButton)
	ON_BN_CLICKED(IDC_RESUMEBUTTON, OnResumeButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRenderProgressDlg message handlers

void CRenderProgressDlg::PostNcDestroy() 
{
	CDialog::PostNcDestroy();

	ParentView->SendMessage(WM_USER_RENDER_DLG_DESTROYED, 0, 0);
	delete this;
}

void CRenderProgressDlg::OnCancel() 
{
	// Before we destroy the window we need to stop the renderer
	// and maybe let it do a bit of cleaning up first
	// otherwise it will try to send us a message which won't work very
	// well if we've been destroyed
	if (RenderingInProgress)
	{
		// rendering thread is still in progress so we need to get it to
		// stop

		if (MessageBox(_T("Are you sure you want to cancel rendering?"), _T("Confirmation"), MB_ICONSTOP | MB_YESNO) == IDYES)
		{
			// If rendering is paused then we need to resume it in order
			// to be able to terminate it as the thread has to terminate itself.
			if (Paused)
				ParentView->SendMessage(WM_USER_RENDER_RESUME, 0, 0);

			// This will result in the thread being terminated
			ParentView->SendMessage(WM_USER_TERMINATE_RENDERING, 0, 0);
			RenderingInProgress = FALSE;

			ShowWindow(SW_HIDE);
			MessageBox(_T("Rendering has been cancelled, no image was written to disk"), _T("Cancelled"), MB_OK);
			// now we just need to destroy the window.
			DestroyWindow();
		}


	}
	else
	{
		// the rendering thread has quit so we can happily destroy the
		// window without any side effects
		DestroyWindow();	// this has a 'delete this' call in it.
	}
}

BOOL CRenderProgressDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//ShowWindow(SW_SHOW);
	CenterWindow();
	
	CProgressCtrl*	pProgressBar = (CProgressCtrl*) GetDlgItem(IDC_PROGRESS);
	pProgressBar->SetRange(0, m_nNoOfLines );
	pProgressBar->SetStep(1);
	pProgressBar->SetPos(0);

	RenderingInProgress = true;
	Paused = false;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


LRESULT CRenderProgressDlg::OnOneLineRendered (WPARAM wParam, LPARAM lParam)
{
	// This message will be received when another line has been rendered and we want to
	// step up the progress bar.

//	MessageBox(_T("1 more line has been rendered"), _T("test"), MB_OK);

	CProgressCtrl*	pProgressBar = (CProgressCtrl*) GetDlgItem(IDC_PROGRESS);
	pProgressBar->StepIt();

	return 0;
}

LRESULT CRenderProgressDlg::OnRenderComplete (WPARAM wParam, LPARAM lParam)
{
	// Here we want to change the render dialog to state we're complete and
	// just give an OK button

	RenderingInProgress = false;

	::PlaySound(_T("SystemDefault"), NULL, SND_ALIAS | SND_ASYNC);

	GetDlgItem(IDC_TEXTLABEL)->SetWindowText(_T("Rendering complete!"));
	GetDlgItem(IDCANCEL)->SetWindowText(_T("OK"));
	GetDlgItem(IDC_RESUMEBUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_PAUSEBUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_PROGRESS)->EnableWindow(FALSE);

	return 0;
}

void CRenderProgressDlg::OnPauseButton() 
{
	// pause the rendering thread
	ParentView->SendMessage(WM_USER_RENDER_PAUSE, 0, 0);

	GetDlgItem(IDC_PAUSEBUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_RESUMEBUTTON)->EnableWindow(TRUE);
	GetDlgItem(IDC_TEXTLABEL)->SetWindowText(_T("Rendering paused"));
	Paused = true;
	
}

void CRenderProgressDlg::OnResumeButton() 
{
	ParentView->SendMessage(WM_USER_RENDER_RESUME, 0, 0);

	GetDlgItem(IDC_PAUSEBUTTON)->EnableWindow(TRUE);
	GetDlgItem(IDC_RESUMEBUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_TEXTLABEL)->SetWindowText(_T("Rendering..."));
	Paused = false;
}
