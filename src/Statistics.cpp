// Statistics.cpp: implementation of the CStatistics class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RayTracer.h"
#include "RayTracing.h"	// includes "Statistics.h" in here

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

////////////////////////////////////////////////////////////
//
// Global Defn
//
// This is the global Stats collecting class,
// There is an extern defn for it in RayTracing.h" so
// everyone can see it
CStatistics Stats;



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStatistics::CStatistics()
{
	ResetValues();
	StatsFile.Initialise(_T("Statistics.log"), APPEND);
	StatsFile.EnableAutoLineEnd(false);
	StatsFile.EnableFileFlush(false);
	StatsFile.EnableTimeLogging(false);
}

CStatistics::~CStatistics()
{

}

// Counter Increment Functions
void CStatistics::Inc_TotalRays(void)
{
	TotalRays++;
}
void CStatistics::Inc_PrimaryRays(void)
{
	PrimaryRays++;
}
void CStatistics::Inc_ReflectedRays(void)
{
	ReflectedRays++;
}
void CStatistics::Inc_TransmittedRays(void)
{
	TransmittedRays++;
}
void CStatistics::Inc_TIRRays(void)
{
	TIRRays++;
}
void CStatistics::Inc_ShadowRays(void)
{
	ShadowRays++;
}
void CStatistics::Inc_TotalIntersections(void)
{
	TotalIntersections++;
}
void CStatistics::Inc_ShadedIntersections(void)
{
	ShadedIntersections++;
}
void CStatistics::Inc_MaxDepthExceeded(void)
{
	MaxDepthExceeded++;
}


void CStatistics::Set_CurrentDepth(int Depth)
{
	if (Depth > MaxDepthReached)
		MaxDepthReached = Depth;
}


// Render Time & Date Functions
void CStatistics::Set_RenderStart(void)
{
	StartTime = CTime::GetCurrentTime();
}
void CStatistics::Set_RenderDone(void)
{
	EndTime = CTime::GetCurrentTime();
}

// File Functions
void CStatistics::Set_InputFile(CString FileName)
{
	// CString automagically strcpy's this for us
	InputFile = FileName;
}
void CStatistics::Set_OutputFile(CString FileName)
{
	OutputFile = FileName;
}

// Rendering Option Functions
void CStatistics::Set_LightAtten(BOOL On)
{
	LightAtten = On;
}
void CStatistics::Set_StochasticSS(BOOL On)
{
	StochasticSS = On;
}
void CStatistics::Set_AtmosFogging(BOOL On)
{
	AtmosFogging = On;
}
void CStatistics::Set_DepthOfField(BOOL On)
{
	DepthOfField = On;
}
void CStatistics::Set_ClampColours(BOOL On)
{
	ClampColours = On;
}
void CStatistics::Set_FocalLength(double Length)
{
	FocalLength = Length;
}
void CStatistics::Set_LensRadius(double Radius)
{
	LensRadius = Radius;
}
void CStatistics::Set_MaxDepth(int Max)
{
	MaxDepth = Max;
}
void CStatistics::Set_RaysPerPixel (int RPP)
{
	RaysPerPixel = RPP;
}
void CStatistics::Set_ImageWidth(int Width)
{
	ImageWidth = Width;
}
void CStatistics::Set_ImageHeight(int Height)
{
	ImageHeight = Height;
}

// Scene Detail Functions
void CStatistics::Set_TotalObjects(INT_PTR Number)
{
	TotalObjects = Number;
}
void CStatistics::Set_TotalLights(INT_PTR Number)
{
	TotalLights = Number;
}

void CStatistics::ResetValues()
{
	TotalRays =
		PrimaryRays =
		ReflectedRays =
		TransmittedRays =
		TIRRays =
		ShadowRays =
		TotalIntersections =
		ShadedIntersections =
		MaxDepthReached =
		MaxDepthExceeded =
		MaxDepth =
		RaysPerPixel =
		ImageWidth =
		ImageHeight = 0;

	TotalLights =
	TotalObjects = 0L;

	InputFile = "";
	OutputFile = "";
		
	LightAtten =
	StochasticSS =
	AtmosFogging =
	DepthOfField =
	ClampColours = FALSE;

	FocalLength =
	LensRadius = 0.0;
}


void CStatistics::Log(CString text)
{
	StatsFile.Log((LPCTSTR) text);
}

void CStatistics::WriteStats()
{
	// This is the function that will get called at the end of rendering
	// so that we can write all of the data to the log file.
	CString DashedLine = "===================================================================\n";

	CString temp;

	LONGLONG hour;
	int day, month, year, minute, seconds;

	TimeToRender = EndTime - StartTime;

	day = StartTime.GetDay();
	month = StartTime.GetMonth();
	year = StartTime.GetYear();
	hour = StartTime.GetHour();
	minute = StartTime.GetMinute();

	temp.Format(_T("= Date: %02d/%02d/%4d\tTime: %02d:%02d\n"), day, month, year, hour, minute);

	Log(DashedLine);
	Log(temp);
	Log(_T("=\n"));

	temp.Format(_T("= Input File: %s\n"), (LPCTSTR) InputFile);
	Log(temp);

	temp.Format(_T("= Output File: %s\n"), (LPCTSTR) OutputFile);
	Log(temp);

	Log(_T("=\n"));

	hour = StartTime.GetHour();
	minute = StartTime.GetMinute();
	seconds = StartTime.GetSecond();

	temp.Format(_T("= Start Time: %02d:%02d:%02d\t\t"), hour, minute, seconds);
	Log(temp);

	hour = EndTime.GetHour();
	minute = EndTime.GetMinute();
	seconds = EndTime.GetSecond();

	temp.Format(_T("End Time: %02d:%02d:%02d\n"), hour, minute, seconds);
	Log(temp);

	hour = TimeToRender.GetTotalHours();
	minute = TimeToRender.GetMinutes();
	seconds = TimeToRender.GetSeconds();

	temp.Format(_T("= Rendering Time: %02d:%02d:%02d\t(%dhr(s) %d minutes and %d seconds)\n"), hour, minute, seconds, hour, minute, seconds);
	Log(temp);

	Log(DashedLine);
	Log(_T("\n"));
	Log(_T("Scene Consisted of:\n"));
	Log(_T("===================\n"));
	temp.Format(_T("%d Objects\n"), TotalObjects);
	Log(temp);
	temp.Format(_T("%d Lights\n"), TotalLights);
	Log(temp);

	Log(_T("\n"));
	Log(_T("Rendering Options:\n"));
	Log(_T("==================\n"));
	Log(_T("Image Size................."));
	temp.Format(_T("%dx%d\n"), ImageWidth, ImageHeight);
	Log(temp);

	temp.Format(_T("Rays Per Pixel.............%d\n"), RaysPerPixel);
	Log(temp);

	Log(_T("Stochastic Supersampling..."));
	if (StochasticSS) 
		Log(_T("On\n"));
	else
		Log(_T("Off\n"));

	Log(_T("Depth Of Field............."));
	if (DepthOfField)
		Log(_T("On\n"));
	else
		Log(_T("Off\n"));

	temp.Format(_T("Max Trace Depth............%d\n"), MaxDepth);
	Log(temp);

	Log(_T("Light Attenuation.........."));
	if (LightAtten)
		Log(_T("On\n"));
	else
		Log(_T("Off\n"));

	Log(_T("Atmospheric Fogging........"));
	if (AtmosFogging)
		Log(_T("On\n"));
	else
		Log(_T("Off\n"));

	Log(_T("\n"));

	if (ClampColours)
		Log(_T("Colour Values were clamped to 1.0\n"));
	else
		Log(_T("Colour Values were scaled to range 0.0 to 1.0\n"));

	Log(_T("\n"));
	Log(_T("Rendering Stats:\n"));
	Log(_T("================\n"));
	temp.Format(_T("Total Rays............%d\n"), TotalRays);
	Log(temp);
	temp.Format(_T("  Primary Rays........%d\n"), PrimaryRays);
	Log(temp);
	temp.Format(_T("  Reflected Rays......%d\n"), ReflectedRays);
	Log(temp);
	temp.Format(_T("  Transmitted Rays....%d\n"), TransmittedRays);
	Log(temp);
	temp.Format(_T("  TIR Rays............%d\n"), TIRRays);
	Log(temp);

	Log(_T("\n"));

	temp.Format(_T("ShadowRays............%d\n"), ShadowRays);
	Log(temp);

	Log(_T("\n"));

	temp.Format(_T("Total Intersections...%d\n"), TotalIntersections);
	Log(temp);
	temp.Format(_T("Shaded Intersections..%d\n"), ShadedIntersections);
	Log(temp);
	temp.Format(_T("Max Depth Reached.....%d\n"), MaxDepthReached);
	Log(temp);
	temp.Format(_T("Max Depth Exceeded....%d times\n"), MaxDepthExceeded);
	Log(temp);
	
	Log(_T("\n"));
	Log(DashedLine);
	Log(_T("\n"));

	// just to make sure all output goes out now
	StatsFile.EnableFileFlush(true);
	Log(_T("\n"));
	StatsFile.EnableFileFlush(false);	// don't need it on all the time

	// once we've written the stats to file we can reset all the values

	ResetValues();
}
