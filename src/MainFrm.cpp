// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "RayTracer.h"

#include "MainFrm.h"
#include "RayTracerDoc.h"
#include "RayTracerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_ENDSESSION()
	ON_UPDATE_COMMAND_UI(ID_FILE_NEW, OnUpdateFileNew)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPEN, OnUpdateFileOpen)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnUpdateFileSaveAs)
	ON_UPDATE_COMMAND_UI(ID_APP_EXIT, OnUpdateAppExit)
	//}}AFX_MSG_MAP
	// Global help commands
	ON_COMMAND(ID_HELP_FINDER, CFrameWnd::OnHelpFinder)
	ON_COMMAND(ID_HELP, CFrameWnd::OnHelp)
	ON_COMMAND(ID_CONTEXT_HELP, CFrameWnd::OnContextHelp)
	ON_COMMAND(ID_DEFAULT_HELP, CFrameWnd::OnHelpFinder)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	LoadBarState(_T("Version 1.0\\WindowState"));



	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

	

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::OnClose() 
{
	// User can only quit if rendering is not in progress
	if (! ((CRayTracerFileView*)GetActiveView())->GetRenderingState() )
	{
		SaveWindowState();
		SaveBarState(_T("Version 1.0\\WindowState"));
		
		CFrameWnd::OnClose();
	}
}

void CMainFrame::OnEndSession(BOOL bEnding) 
{
	if (bEnding)
	{
		SaveWindowState();
		SaveBarState(_T("Version 1.0\\WindowState"));
	}
	CFrameWnd::OnEndSession(bEnding);

}

void CMainFrame::SaveWindowState()
{
	CString version = _T("Version 1.0\\WindowState");

	CWinApp *App = AfxGetApp();

	WINDOWPLACEMENT wp;
	wp.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(&wp);

	App->WriteProfileInt(version, _T("flags"), wp.flags);
	App->WriteProfileInt(version, _T("showCmd"), wp.showCmd);
	App->WriteProfileInt(version, _T("x1"), wp.rcNormalPosition.left);
	App->WriteProfileInt(version, _T("y1"), wp.rcNormalPosition.top);
	App->WriteProfileInt(version, _T("x2"), wp.rcNormalPosition.right);
	App->WriteProfileInt(version, _T("y2"), wp.rcNormalPosition.bottom);
}


BOOL CMainFrame::RestoreWindowState()
{
    CString version = _T ("Version 1.0\\WindowState");

	CWinApp *App = AfxGetApp();

    WINDOWPLACEMENT wp;
    wp.length = sizeof (WINDOWPLACEMENT);
    GetWindowPlacement (&wp);

    if (((wp.flags =
            App->GetProfileInt (version, _T ("flags"), -1)) != -1) &&
        ((wp.showCmd =
            App->GetProfileInt (version, _T ("showCmd"), -1)) != -1) &&
        ((wp.rcNormalPosition.left =
            App->GetProfileInt (version, _T ("x1"), -1)) != -1) &&
        ((wp.rcNormalPosition.top =
            App->GetProfileInt (version, _T ("y1"), -1)) != -1) &&
        ((wp.rcNormalPosition.right =
            App->GetProfileInt (version, _T ("x2"), -1)) != -1) &&
        ((wp.rcNormalPosition.bottom =
            App->GetProfileInt (version, _T ("y2"), -1)) != -1)) {

        wp.rcNormalPosition.left = min (wp.rcNormalPosition.left,
            ::GetSystemMetrics (SM_CXSCREEN) -
            ::GetSystemMetrics (SM_CXICON));
        wp.rcNormalPosition.top = min (wp.rcNormalPosition.top,
            ::GetSystemMetrics (SM_CYSCREEN) -
            ::GetSystemMetrics (SM_CYICON));

        SetWindowPlacement (&wp);
        return TRUE;
    }
    return FALSE;
}

void CMainFrame::OnUpdateFileNew(CCmdUI* pCmdUI) 
{
	if (((CRayTracerFileView*)GetActiveView())->GetRenderingState())
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

void CMainFrame::OnUpdateFileOpen(CCmdUI* pCmdUI) 
{
	if (((CRayTracerFileView*)GetActiveView())->GetRenderingState())
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

void CMainFrame::OnUpdateFileSave(CCmdUI* pCmdUI) 
{
	if (((CRayTracerFileView*)GetActiveView())->GetRenderingState())
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

void CMainFrame::OnUpdateFileSaveAs(CCmdUI* pCmdUI) 
{
	if (((CRayTracerFileView*)GetActiveView())->GetRenderingState())
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

void CMainFrame::OnUpdateAppExit(CCmdUI* pCmdUI) 
{
	if (((CRayTracerFileView*)GetActiveView())->GetRenderingState())
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}
