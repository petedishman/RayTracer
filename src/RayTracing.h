#if !defined(AFX_RAYTRACING_H__888B88C1_991A_487F_9F72_A81B32F3B734__INCLUDED_)
#define AFX_RAYTRACING_H__888B88C1_991A_487F_9F72_A81B32F3B734__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LogFile.h"
#include "Vector.h"
#include "Colour.h"
#include "Ray.h"
#include "Light.h"
#include "SceneObject.h"
#include "Sphere.h"
#include "InfinitePlane.h"
#include "CheckerBoard.h"
#include "BoundedPlane.h"
#include "Cylinder.h"
#include "BoundedCheckerBoard.h"
#include "Statistics.h"

#define ONE_RPP			1		// These are the only valid numbers that RaysPerPixel
#define FOUR_RPP		4		// should be set to - Won't really ever need more than
#define NINE_RPP		9		// 100, in fact 100's too many
#define SIXTEEN_RPP		16
#define	FORTYNINE_RPP	49
#define HUNDRED_RPP		100

#define PI					(3.1415926535897932384)
#define DEG2RAD(x)			((x*PI)/180.0)
#define RAD2DEG(x)			((x*180.0)/PI)

struct s_IntersectionInfo
{
	int Index;		// index of object in ObjectList
	CVector Point;	// point of intersection on object
	CVector Normal; // Normal vector at point of intersection
};

struct s_ActiveLightsInfo
{
	int Index;			// index of light in LightList
	CColour Transparency;	// combined effect of transparent objects between point and light 0.0 - 1.0
};

struct s_CameraLocation
{
	CVector Postion;
	CVector Up;
	CVector Heading;
};

struct s_ProjectionPlane
{
	CVector TopLeft;
	CVector TopRight;
	CVector BottomLeft;
	CVector BottomRight;
};

typedef CTypedPtrArray<CPtrArray, CSceneObject*>		CObjectArray;
typedef CTypedPtrArray<CPtrArray, CLight*>				CLightArray;
typedef CTypedPtrArray<CPtrArray, s_ActiveLightsInfo*>	CActiveLightArray;


struct s_ShadingInfo
{
	CLightArray *Lights;
	CActiveLightArray *ActiveLights;
	s_IntersectionInfo *Intersection;
	CColour* Ambient;
	CRay* Ray;
	BOOL LightAtten;
	BOOL AtmosFogging;
};

// The refractive index of Air
#define RI_AIR	((double) 1.0)

// This is the global Stats Object.  It's been made global as various different
// classes need it and I don't want to have to pass around pointers all the time
// It will get reused for each new rendering operation that we do.

// Most files include this one so everyone should be able to see it.
extern CStatistics Stats;


#endif // !defined(AFX_RAYTRACING_H__888B88C1_991A_487F_9F72_A81B32F3B734__INCLUDED_)
