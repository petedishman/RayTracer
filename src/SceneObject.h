// SceneObject.h: interface for the CSceneObject class.
//
//////////////////////////////////////////////////////////////////////
//
// This class is an abstract base class that will form the root
// of the inheritance tree for all of the objects that can appear
// in the scene (such as spheres, cones, planes, etc)
// it will define some pure virtual functions plus some other
// data members that all objects will have.
//



#if !defined(AFX_SCENEOBJECT_H__66D089C4_6D4C_46F8_BD09_D0C75976E218__INCLUDED_)
#define AFX_SCENEOBJECT_H__66D089C4_6D4C_46F8_BD09_D0C75976E218__INCLUDED_


// These are here so that this will compile, anything that doesn't use world.h
// won't like it but we can get round that.
class CLight;
struct s_ActiveLightsInfo;
struct s_IntersectionInfo;
struct s_ShadingInfo;
typedef CTypedPtrArray<CPtrArray, CLight*>				CLightArray;
typedef CTypedPtrArray<CPtrArray, s_ActiveLightsInfo*>	CActiveLightArray;


#include "Colour.h"	// Added by ClassView


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSceneObject  
{
public:
	CSceneObject();
	
	// Pure Virtual Functions.  These must be overridden in derived classes
	virtual BOOL Intersect(CRay *Ray, CVector *Point) = 0;
	virtual CVector Normal(CVector *Point, CRay *Ray) = 0;


	virtual CVector GetReflectedDir(CRay *Ray, s_IntersectionInfo *Intersection);
	virtual BOOL GetRefractedDir(CRay *Ray, s_IntersectionInfo *Intersection, CVector* Result);
	virtual CColour Shade(s_ShadingInfo *Info);


	CColour Colour;
	double	DR_Coeff;
	double	SR_Coeff;
	double  Amb_Coeff;
	int		Shininess;
	CColour	Reflection;
	CColour	Transparency;
	double	Refractive_Index;

	// Most data members will go here actually,
	// especially all the ones to do with lighting and reflection/refraction

	// The derived classes will just provide overrides and size data for each
	// particular object

protected:
	virtual CColour CalcDiffuseContribution(CLight *Light, CColour *SpecularAmount);
	virtual CColour CalcSpecularContribution(CLight* Light);
	virtual void InitialiseValues(s_IntersectionInfo *Intersection, CRay *IncomingRay);
	virtual void InitialiseValuesPerLight(CLight* Light, CRay* IncomingRay, s_IntersectionInfo *Intersection);
	virtual CColour CalcAmbientContribution(CColour *Ambient);

	CVector DirToLight;
	double NdotL;
	CVector ViewerDir;
	double RdotV;
	CVector NormalVec;
	double DistanceToLight;
	double F_Attenuation;
};

#endif // !defined(AFX_SCENEOBJECT_H__66D089C4_6D4C_46F8_BD09_D0C75976E218__INCLUDED_)
