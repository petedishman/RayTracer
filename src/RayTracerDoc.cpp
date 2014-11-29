// RayTracerDoc.cpp : implementation of the CRayTracerDoc class
//

#include "stdafx.h"
#include "RayTracer.h"


#include "RayTracerDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRayTracerDoc

IMPLEMENT_DYNCREATE(CRayTracerDoc, CDocument)

BEGIN_MESSAGE_MAP(CRayTracerDoc, CDocument)
	//{{AFX_MSG_MAP(CRayTracerDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRayTracerDoc construction/destruction

CRayTracerDoc::CRayTracerDoc()
{
	World = new CWorld;

}

CRayTracerDoc::~CRayTracerDoc()
{
	delete World;
}

BOOL CRayTracerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	((CEditView*)m_viewList.GetHead())->SetWindowText(NULL);


	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CRayTracerDoc serialization

void CRayTracerDoc::Serialize(CArchive& ar)
{
	// This is probably full of bugs/issues
	// but it's the bare minimum that'll work.
	// and will do for now
	CEditView* pEditView = (CEditView*)m_viewList.GetHead();

	if (ar.IsStoring())
	{
		// get window text, it'll be in utf-16?
		// save to file as utf-8
		CString docContents;
		pEditView->GetWindowText(docContents);

		CFile* file = ar.GetFile();
	
#ifdef _UNICODE
		CStringA string = CW2A(docContents, CP_UTF8);
		file->Write(string.GetBuffer(), string.GetLength());
#else
		file->Write(docContents.GetBuffer(), docContents.GetLength());
#endif
	}
	else
	{
		// loading
		// read in file as utf-8, convert to utf-16
		// set window text.

		CFile* file = ar.GetFile();
		size_t fileLength = (size_t)file->GetLength();

		void* fileContents = malloc(fileLength);
		file->Read(fileContents, fileLength);

#ifdef _UNICODE
		CStringW docContents = CA2W((char*)fileContents, CP_UTF8);
#else
		CString docContents((char*)fileContents);
#endif;

		pEditView->SetWindowText(docContents);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CRayTracerDoc diagnostics

#ifdef _DEBUG
void CRayTracerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CRayTracerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG
