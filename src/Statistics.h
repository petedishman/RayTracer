// Statistics.h: interface for the CStatistics class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATISTICS_H__80C2BAB3_C82D_4CE0_808C_39EB22AEE2C9__INCLUDED_)
#define AFX_STATISTICS_H__80C2BAB3_C82D_4CE0_808C_39EB22AEE2C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



class CStatistics  
{
public:
	void WriteStats();
	void ResetValues();
	CStatistics();
	virtual ~CStatistics();

	// Counter Increment Functions
	void Inc_TotalRays(void);
	void Inc_PrimaryRays(void);
	void Inc_ReflectedRays(void);
	void Inc_TransmittedRays(void);
	void Inc_TIRRays(void);
	void Inc_ShadowRays(void);
	void Inc_TotalIntersections(void);
	void Inc_ShadedIntersections(void);
	void Inc_MaxDepthExceeded(void);

	// This function will calculate the max depth reached if it is called with the current depth
	// every time RenderRay is entered
	void Set_CurrentDepth(int Depth);

	// Render Time & Date Functions
	void Set_RenderStart(void);
	void Set_RenderDone(void);

	// File Functions
	void Set_InputFile(CString FileName);
	void Set_OutputFile(CString FileName);

	// Rendering Option Functions
	void Set_LightAtten(BOOL On);
	void Set_StochasticSS(BOOL On);
	void Set_AtmosFogging(BOOL On);
	void Set_DepthOfField(BOOL On);
	void Set_ClampColours(BOOL On);
	void Set_FocalLength(double Length);
	void Set_LensRadius(double Radius);
	void Set_MaxDepth(int Max);
	void Set_RaysPerPixel (int RPP);
	void Set_ImageWidth(int Width);
	void Set_ImageHeight(int Height);

	// Scene Detail Functions
	void Set_TotalObjects(INT_PTR Number);
	void Set_TotalLights(INT_PTR Number);

private:
	void Log(CString text);
	int TotalRays;
	int PrimaryRays;
	int ReflectedRays;
	int TransmittedRays;
	int TIRRays;
	int ShadowRays;
	int TotalIntersections;
	int ShadedIntersections;
	int MaxDepthReached;
	int MaxDepthExceeded;

	CTime StartTime;
	CTime EndTime;
	CTimeSpan TimeToRender;
	CString InputFile;
	CString OutputFile;
	
	BOOL LightAtten;
	BOOL StochasticSS;
	BOOL AtmosFogging;
	BOOL DepthOfField;
	BOOL ClampColours;
	double FocalLength;
	double LensRadius;
	int MaxDepth;
	int RaysPerPixel;
	int ImageWidth;
	int ImageHeight;

	INT_PTR TotalLights;
	INT_PTR TotalObjects;

	// LogFile

	CLogFile StatsFile;
};

#endif // !defined(AFX_STATISTICS_H__80C2BAB3_C82D_4CE0_808C_39EB22AEE2C9__INCLUDED_)
