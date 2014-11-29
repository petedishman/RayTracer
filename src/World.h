// World.h: interface for the CWorld class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WORLD_H__0998C970_3FE0_48BA_93F1_A400F4FAA112__INCLUDED_)
#define AFX_WORLD_H__0998C970_3FE0_48BA_93F1_A400F4FAA112__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//forward declaration for FileParser.h
class CWorld;

#include "FileParser.h"
#include "Targa.h"
#include "LogFile.h"
#include "Colour.h"	// Added by ClassView



// This is the thread function that gets the rendering going
// CRayTracerFileView will call this with AfxBeginThread()
UINT ThreadFunc (LPVOID pParam);



class CWorld  
{
public:
	CString InputFileName;
	void RenderImage();
	BOOL EverythingReady();
	int GetImageHeight();
	void RestoreDefaults();
	BOOL ConstructScene(CString *source_text);
	CWorld();
	virtual ~CWorld();

	int ParserErrorIndex;
	CDialog* RenderingDialog;

	BOOL DepthOfFieldOn;
	int RaysPerPixel;
	BOOL StochasticSS;
	int MaxDepth;	
	BOOL LightAtten;
	BOOL AtmosFogging;
	CString OutputDir;
	BOOL ClampColours;
	double FocalLength;
	double LensRadius;
	BOOL KeepRendering;


private:
	void RestoreRenderOptions();
	void SaveRenderOptions();
	void StopRendering();
	void ClearUpMemoryAllocations();
	void ScaleColours();
	double GetRandomNumber(double Max);
	void GenerateDOFRays(int i , int j);
	int GetActiveLights(s_IntersectionInfo *Info);
	BOOL GetNearestIntersection(CRay *Ray, s_IntersectionInfo *Info);
	CColour RenderRay(CRay *Ray, int Depth);
	void GenerateRays(int i, int j);
	CColour RenderPixel(int i, int j);
	void RenderLine(int j);
	void CalculateProjectionPlane();
	
	//
	CObjectArray Object_List;
	CLightArray Light_List;
	CActiveLightArray ActiveLight_List;

	CTarga *ImageFile;

	CFileParser* FileParser;
	
	CRay* RayArray;	// only using an ordinary array as we know everything about this
					// one and have no need for advanced array type stuff.

	CColour* PreScaledImage;

	s_ProjectionPlane ProjectionPlane;
	s_CameraLocation CameraLocation;
	CColour bg_Colour;
	CColour AmbientLight;
	double FieldOfView;
	int Image_Width;
	int Image_Height;

	double Fog_Distance;

	// this is to allow the fileparser to set up the private world data without
	// needing a lot of extra pointless functions
	friend class CFileParser;
};





#endif // !defined(AFX_WORLD_H__0998C970_3FE0_48BA_93F1_A400F4FAA112__INCLUDED_)
