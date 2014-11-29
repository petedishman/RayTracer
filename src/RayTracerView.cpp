// RayTracerView.cpp : implementation of the CRayTracerFileView class
//

#include "stdafx.h"
#include "RayTracer.h"

#include "RayTracerDoc.h"
#include "RayTracerView.h"
#include "OptionsDialog.h"
#include "TextInsertions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRayTracerFileView

IMPLEMENT_DYNCREATE(CRayTracerFileView, CEditView)

BEGIN_MESSAGE_MAP(CRayTracerFileView, CEditView)
	//{{AFX_MSG_MAP(CRayTracerFileView)
	ON_COMMAND(ID_RENDERIMAGE, OnRenderImage)
	ON_UPDATE_COMMAND_UI(ID_RENDERIMAGE, OnUpdateRenderImage)
	ON_MESSAGE(WM_USER_RENDER_DLG_DESTROYED, OnDialogDestroyed)		// the render progress dialog
	ON_MESSAGE(WM_USER_RENDER_PAUSE, OnRenderPause)					// the render progress dialog
	ON_MESSAGE(WM_USER_RENDER_RESUME, OnRenderResume)				// the render progress dialog
	ON_MESSAGE(WM_USER_TERMINATE_RENDERING, OnRenderCancel)			// the render progress dialog
	ON_COMMAND(ID_RENDER_OPTIONS, OnRenderOptions)
	ON_COMMAND(ID_INSERT_GLOBALSETTINGS, OnInsertGlobalSettings)
	ON_UPDATE_COMMAND_UI(ID_INSERT_GLOBALSETTINGS, OnUpdateInsertGlobalSettings)
	ON_COMMAND(ID_INSERT_BOUNDEDCHECKERBOARD, OnInsertBoundedCheckerBoard)
	ON_UPDATE_COMMAND_UI(ID_INSERT_BOUNDEDCHECKERBOARD, OnUpdateInsertBoundedCheckerBoard)
	ON_COMMAND(ID_INSERT_BOUNDEDPLANE, OnInsertBoundedPlane)
	ON_UPDATE_COMMAND_UI(ID_INSERT_BOUNDEDPLANE, OnUpdateInsertBoundedPlane)
	ON_COMMAND(ID_INSERT_CAMERA, OnInsertCamera)
	ON_UPDATE_COMMAND_UI(ID_INSERT_CAMERA, OnUpdateInsertCamera)
	ON_COMMAND(ID_INSERT_CHECKERBOARD, OnInsertCheckerBoard)
	ON_UPDATE_COMMAND_UI(ID_INSERT_CHECKERBOARD, OnUpdateInsertCheckerBoard)
	ON_COMMAND(ID_INSERT_CYLINDER, OnInsertCylinder)
	ON_UPDATE_COMMAND_UI(ID_INSERT_CYLINDER, OnUpdateInsertCylinder)
	ON_COMMAND(ID_INSERT_INFINITEPLANE, OnInsertInfinitePlane)
	ON_UPDATE_COMMAND_UI(ID_INSERT_INFINITEPLANE, OnUpdateInsertInfinitePlane)
	ON_COMMAND(ID_INSERT_LIGHT, OnInsertLight)
	ON_UPDATE_COMMAND_UI(ID_INSERT_LIGHT, OnUpdateInsertLight)
	ON_COMMAND(ID_INSERT_SPHERE, OnInsertSphere)
	ON_UPDATE_COMMAND_UI(ID_INSERT_SPHERE, OnUpdateInsertSphere)
	ON_UPDATE_COMMAND_UI(ID_RENDER_OPTIONS, OnUpdateRenderOptions)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CEditView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRayTracerFileView construction/destruction

CRayTracerFileView::CRayTracerFileView()
{
	m_pProgressDlg = NULL;

	// try changing the font here
	// Everyone should have Courier surely?
	Font.CreatePointFont(100, _T("Courier"), NULL);

	Rendering = FALSE;
}

CRayTracerFileView::~CRayTracerFileView()
{
	// as we don't let the window close in COGLWindow
	// we have to make sure we destroy it here
	// or maybe this is the wrong place.
	Font.DeleteObject();
}

BOOL CRayTracerFileView::PreCreateWindow(CREATESTRUCT& cs)
{

	BOOL bPreCreated = CEditView::PreCreateWindow(cs);
	cs.style &= ~(ES_AUTOHSCROLL|WS_HSCROLL);	// Enable word-wrapping

	return bPreCreated;
}

/////////////////////////////////////////////////////////////////////////////
// CRayTracerFileView drawing

void CRayTracerFileView::OnDraw(CDC* pDC)
{
	CRayTracerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
}

/////////////////////////////////////////////////////////////////////////////
// CRayTracerFileView printing

BOOL CRayTracerFileView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default CEditView preparation
	return CEditView::OnPreparePrinting(pInfo);
}

void CRayTracerFileView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// Default CEditView begin printing.
	CEditView::OnBeginPrinting(pDC, pInfo);
}

void CRayTracerFileView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// Default CEditView end printing
	CEditView::OnEndPrinting(pDC, pInfo);
}

/////////////////////////////////////////////////////////////////////////////
// CRayTracerFileView diagnostics

#ifdef _DEBUG
void CRayTracerFileView::AssertValid() const
{
	CEditView::AssertValid();
}

void CRayTracerFileView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}

CRayTracerDoc* CRayTracerFileView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRayTracerDoc)));
	return (CRayTracerDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRayTracerFileView message handlers


void CRayTracerFileView::OnRenderImage() 
{

	// This function may be small but this is the one where
	// all of the rendering and parsing work is started from

	// It starts by getting the current text in the window and
	// parsing it, if successful the object/light lists will have
	// been built, if parsing fails this function reports the error.
	
	// once parsing is complete this function creates the render progress
	// dialog and then starts the rendering thread going
	// once the thread has begun execution this function exits.


	CString FileText;
	GetWindowText(FileText);

	if (!GetDocument()->World->ConstructScene(&FileText))
	{
		// parsing failed so highlight the error position
		CEdit& theEdit = GetEditCtrl();
		int Line_With_Error, LineStart, LineEnd;

		Line_With_Error = theEdit.LineFromChar( GetDocument()->World->ParserErrorIndex );
		LineStart = theEdit.LineIndex(Line_With_Error);
		LineEnd = LineStart + theEdit.LineLength(Line_With_Error) - 1;

		theEdit.SetSel(LineStart, LineEnd, false);

		MessageBox(_T("An error occured whilst reading the file!\nCheck the highlighted area for errors then try again."), _T("Parsing Error"), MB_OK);
	}
	else
	{
		// need to check we have everything we need at this point
		// we need to get the World class to check it's ready to
		// render i.e. it has a width and height and at least one object/one light.

		if (GetDocument()->World->EverythingReady())
		{
			// create a dialog box,
			// start the thread running
			// thread sends messages to dialog box saying "I've done a line"
			// dialog box can pause and resume the thread and get it to stop

			if (m_pProgressDlg != NULL)
				m_pProgressDlg->SetFocus();	// don't think this will ever happen
			else
			{
				m_pProgressDlg = new CRenderProgressDlg(this);
	
				// pass in the number of lines so that the progress control can initialise itself.
				m_pProgressDlg->m_nNoOfLines = GetDocument()->World->GetImageHeight();

				m_pProgressDlg->Create(IDD_RENDERPROGRESS);
				m_pProgressDlg->ShowWindow(SW_SHOW);

				GetDocument()->World->RenderingDialog = m_pProgressDlg;
			}

			// Pass in the input file name
			GetDocument()->World->InputFileName = GetDocument()->GetTitle();

			// now we can start the thread !
			//(CRayTracerApp *) AfxGetApp;

			// Start the thread and pass it a pointer to the World
			GetDocument()->RenderingThread = AfxBeginThread(ThreadFunc, GetDocument()->World);
			Rendering = TRUE;
			

		}


	}

	
}

void CRayTracerFileView::OnUpdateRenderImage(CCmdUI* pCmdUI) 
{
	if (Rendering)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

LRESULT CRayTracerFileView::OnDialogDestroyed (WPARAM wParam, LPARAM lParam)
{
	m_pProgressDlg = NULL;
	//MessageBox(_T("message handler"), _T("test"), MB_OK);
	
	// We won't say we've stopped rendering until the dialog box has gone.
	Rendering = FALSE;
	return 0;
}

LRESULT CRayTracerFileView::OnRenderPause (WPARAM wParam, LPARAM lParam)
{
	// pause the rendering thread
	GetDocument()->RenderingThread->SuspendThread();

	return 0;
}
LRESULT CRayTracerFileView::OnRenderResume (WPARAM wParam, LPARAM lParam)
{
	// resume the paused rendering thread

	GetDocument()->RenderingThread->ResumeThread();

	return 0;
}

LRESULT CRayTracerFileView::OnRenderCancel (WPARAM wParam, LPARAM lParam)
{
	// cancel the thread by setting CWorld::KeepRendering to False
	// that'll cause it to stop rendering and destroy the thread.
	GetDocument()->World->KeepRendering = FALSE;

	return 0;
}

void CRayTracerFileView::OnRenderOptions() 
{
	COptionsDialog dlg;

	dlg.m_bAtmosFogging = GetDocument()->World->AtmosFogging;
	dlg.m_bDOFEffects = GetDocument()->World->DepthOfFieldOn;
	dlg.m_bLightAttenuation = GetDocument()->World->LightAtten;
	dlg.m_bStochasticSS = GetDocument()->World->StochasticSS;
	
	if (GetDocument()->World->ClampColours)
	{
		dlg.m_iClampColours = 0;
	}
	else
	{
		dlg.m_iClampColours = 1;
	}

	switch (GetDocument()->World->RaysPerPixel)
	{
	case ONE_RPP:
		dlg.m_iRaysPerPixel = 0;
		break;
	case FOUR_RPP:
		dlg.m_iRaysPerPixel = 1;
		break;
	case NINE_RPP:
		dlg.m_iRaysPerPixel = 2;
		break;
	case SIXTEEN_RPP:
		dlg.m_iRaysPerPixel = 3;
		break;
	case FORTYNINE_RPP:
		dlg.m_iRaysPerPixel = 4;
		break;
	case HUNDRED_RPP:
		dlg.m_iRaysPerPixel = 5;
		break;
	}
	dlg.m_iMaxDepth = GetDocument()->World->MaxDepth;
	dlg.m_OutputDir = GetDocument()->World->OutputDir;

	if (dlg.DoModal() == IDOK)
	{
		GetDocument()->World->AtmosFogging = dlg.m_bAtmosFogging;
		GetDocument()->World->DepthOfFieldOn = dlg.m_bDOFEffects;
		GetDocument()->World->LightAtten = dlg.m_bLightAttenuation;
		GetDocument()->World->StochasticSS = dlg.m_bStochasticSS;

		if (dlg.m_iClampColours == 0)
		{
			GetDocument()->World->ClampColours = TRUE;
		}
		else
		{
			GetDocument()->World->ClampColours = FALSE;
		}


		switch (dlg.m_iRaysPerPixel)
		{
		case 0:
			GetDocument()->World->RaysPerPixel = ONE_RPP;
			break;
		case 1:
			GetDocument()->World->RaysPerPixel = FOUR_RPP;
			break;
		case 2:
			GetDocument()->World->RaysPerPixel = NINE_RPP;
			break;
		case 3:
			GetDocument()->World->RaysPerPixel = SIXTEEN_RPP;
			break;
		case 4:
			GetDocument()->World->RaysPerPixel = FORTYNINE_RPP;
			break;
		case 5:
			GetDocument()->World->RaysPerPixel = HUNDRED_RPP;
			break;
		}
		
		GetDocument()->World->MaxDepth = dlg.m_iMaxDepth;
		GetDocument()->World->OutputDir = dlg.m_OutputDir;
	}
}

void CRayTracerFileView::OnInsertGlobalSettings() 
{
	// here we want to get hold of the edit control and insert text
	// at the current selection point
	//
	// the text is in strGlobalSettings
	CEdit& Edit = GetEditCtrl();

	Edit.ReplaceSel((LPCTSTR) strGlobalSettings, TRUE);
	
}

void CRayTracerFileView::OnUpdateInsertGlobalSettings(CCmdUI* pCmdUI) 
{
	if (Rendering)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

void CRayTracerFileView::OnInsertBoundedCheckerBoard() 
{
	CEdit& Edit = GetEditCtrl();

	Edit.ReplaceSel((LPCTSTR) strBoundedCheckerBoard, TRUE);
}

void CRayTracerFileView::OnUpdateInsertBoundedCheckerBoard(CCmdUI* pCmdUI) 
{
	if (Rendering)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

void CRayTracerFileView::OnInsertBoundedPlane() 
{
	CEdit& Edit = GetEditCtrl();

	Edit.ReplaceSel((LPCTSTR) strBoundedPlane, TRUE);
}

void CRayTracerFileView::OnUpdateInsertBoundedPlane(CCmdUI* pCmdUI) 
{
	if (Rendering)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

void CRayTracerFileView::OnInsertCamera() 
{
	CEdit& Edit = GetEditCtrl();

	Edit.ReplaceSel((LPCTSTR) strCamera, TRUE);
}

void CRayTracerFileView::OnUpdateInsertCamera(CCmdUI* pCmdUI) 
{
	if (Rendering)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

void CRayTracerFileView::OnInsertCheckerBoard() 
{
	CEdit& Edit = GetEditCtrl();

	Edit.ReplaceSel((LPCTSTR) strCheckerBoard, TRUE);
}

void CRayTracerFileView::OnUpdateInsertCheckerBoard(CCmdUI* pCmdUI) 
{
	if (Rendering)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

void CRayTracerFileView::OnInsertCylinder() 
{
	CEdit& Edit = GetEditCtrl();

	Edit.ReplaceSel((LPCTSTR) strCylinder, TRUE);
}

void CRayTracerFileView::OnUpdateInsertCylinder(CCmdUI* pCmdUI) 
{
	if (Rendering)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

void CRayTracerFileView::OnInsertInfinitePlane() 
{
	CEdit& Edit = GetEditCtrl();

	Edit.ReplaceSel((LPCTSTR) strInfinitePlane, TRUE);
}

void CRayTracerFileView::OnUpdateInsertInfinitePlane(CCmdUI* pCmdUI) 
{
	if (Rendering)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

void CRayTracerFileView::OnInsertLight() 
{
	CEdit& Edit = GetEditCtrl();

	Edit.ReplaceSel((LPCTSTR) strLight, TRUE);
}

void CRayTracerFileView::OnUpdateInsertLight(CCmdUI* pCmdUI) 
{
	if (Rendering)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

void CRayTracerFileView::OnInsertSphere() 
{
	CEdit& Edit = GetEditCtrl();

	Edit.ReplaceSel((LPCTSTR) strSphere, TRUE);
}

void CRayTracerFileView::OnUpdateInsertSphere(CCmdUI* pCmdUI) 
{
	if (Rendering)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

void CRayTracerFileView::OnInitialUpdate() 
{
	CEditView::OnInitialUpdate();
	
	SetFont(&Font, TRUE);
}

void CRayTracerFileView::OnUpdateRenderOptions(CCmdUI* pCmdUI) 
{
	if (Rendering)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

BOOL CRayTracerFileView::GetRenderingState()
{
	return Rendering;
}
