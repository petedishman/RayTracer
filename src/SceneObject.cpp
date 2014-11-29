// SceneObject.cpp: implementation of the CSceneObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RayTracer.h"

#include "RayTracing.h"
#include "SceneObject.h"

#include "World.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CSceneObject::CSceneObject()
{
	// Here we will set all the default properties that objects have
	Colour.Set(0.5f, 0.5f, 0.5f);
	DR_Coeff = 0.8;
	SR_Coeff = 0.8;
	Amb_Coeff = 0.2;
	Shininess = 5;
	Reflection.Set(0.0f,0.0f,0.0f);
	Transparency.Set(0.0f,0.0f,0.0f);
	Refractive_Index = 1.0;
}



CColour CSceneObject::Shade(s_ShadingInfo *Info)
{
	// This function will return the shade of the object at the given intersection point

	CColour Ambient, Diffuse, Specular, Total, Diff_Spec;
	CLight CurrentLight;


	INT_PTR NoOfLights = Info->ActiveLights->GetSize();

	InitialiseValues( Info->Intersection, Info->Ray );

	Ambient = CalcAmbientContribution( Info->Ambient );

	for (INT_PTR i = 0; i < NoOfLights; i++)
	{
		int Light_Index = (*(Info->ActiveLights))[i]->Index;
		CurrentLight = *((*(Info->Lights))[Light_Index]);

		// if Light Attenuation is on then we need to scale the intensity of the light
		// by f-att before shading occurs
		if (Info->LightAtten)
			CurrentLight.Colour = CurrentLight.Colour * F_Attenuation;

		// Scale the light intensity by combined transparency of any objects in between here
		// and the light.  If there aren't any in the way then this just multiplys the value
		// by 1.0 so it's not a problem
		CurrentLight.Colour = CurrentLight.Colour * (*(Info->ActiveLights))[i]->Transparency;


		InitialiseValuesPerLight(&CurrentLight, Info->Ray, Info->Intersection);

		Specular  = CalcSpecularContribution (&CurrentLight);
		Diffuse   = CalcDiffuseContribution  (&CurrentLight, &Specular);
		Diff_Spec = Specular + Diffuse;

		// combine the total of this light with the total from the rest of the lights
		Total = Total + Diff_Spec;
	}

	// finally add on the ambient contribution

	Total = Total + Ambient;

	return Total;
}

CVector CSceneObject::GetReflectedDir(CRay *Ray, s_IntersectionInfo *Intersection)
{
	// This function will just provide a base implementation of finding
	// a reflected ray. it just uses the normal R = 2N(N.L) - L form of finding
	// a reflected direction.  Derived classes can override this if they want.

	CVector temp;

	// this might be wrong so we'll try something a little different.
//	temp = (Intersection->Normal *  (2.0 * (fabs(Intersection->Normal.Dot(Ray->Dir))))) - Ray->Dir;



	//temp = (Ray->Dir *  (2 * fabs ( Ray->Dir.Dot(Intersection->Normal) ) ) ) + Ray->Dir;

	// we should be inverting the direction of the Ray to make this work
	// that way we won't need to use the fabs() function as really a ray won't hit a surface and reflect
	// when the angle between the ray and normal is > 90
	//
	// Therefore we'll try inverting Ray->Dir and then using it, that way we can avoid using the
	// fabs() and change it back to a - instead of a +

	CVector RayDir;	// Initialised to (0,0,0)

	RayDir = RayDir - Ray->Dir;

	temp = (Intersection->Normal * (2.0 * (Intersection->Normal.Dot(RayDir)))) - RayDir;


	temp.Normalize();

	return temp;
}

BOOL CSceneObject::GetRefractedDir(CRay *Ray, s_IntersectionInfo *Intersection, CVector* Result)
{
	// If Total Internal Reflection (TIR) occurs then we should ignore the refracted ray
	// therefore if TIR occurs this function will return false and not fill in Result,
	// if TIR does not occur we will return true and fill in result.

	// See Project Log book for the derivation of all this.
	// There's a possibility I may have to change a sign somewhere so look at that first
	// if there's a problem

	CVector temp;
	double IdotN;
	double RI_comb;
	double sqrt_term;

	CVector Normal;
	CVector temp_for_minus_op;
	CVector RayDir;
	CVector ZeroVec;

	RayDir = ZeroVec - Ray->Dir;
	Normal = Intersection->Normal;

	/*if (Intersection->Entering)
		RI_comb = 1.0 / Refractive_Index;
	else
		RI_comb = Refractive_Index;
		*/

	// If the ray was previously in air then we're entering else we're leaving
	if (Ray->CurrentObjIndex == -1)
		RI_comb = RI_AIR / Refractive_Index;
	else
		RI_comb = Refractive_Index / RI_AIR;

	IdotN = RayDir.Dot(Normal);

	sqrt_term = 1 - (RI_comb * RI_comb) + ((RI_comb * RI_comb) * (IdotN * IdotN));

	if (sqrt_term < 0.0)
	{
		// TIR has occured
		temp = GetReflectedDir(Ray, Intersection);
		(*Result) = temp;
		return FALSE;
	}

	sqrt_term = sqrt(sqrt_term);

	temp = (Normal * ((RI_comb * IdotN) - sqrt_term))  -  (RayDir * RI_comb);

	temp.Normalize();
	(*Result) = temp;

	return TRUE;
}

CColour CSceneObject::CalcAmbientContribution(CColour *Ambient)
{
	CColour Result;

	Result.Set_R( Ambient->Get_R() * (float) Amb_Coeff * Colour.Get_R() );
	Result.Set_G( Ambient->Get_G() * (float) Amb_Coeff * Colour.Get_G() );
	Result.Set_B( Ambient->Get_B() * (float) Amb_Coeff * Colour.Get_B() );

	return Result;
}


//////////////////////////////////////////////////////////////////
//
// Note: although this can be overridden the base class version
//		 i.e. this version should probably always be called
//
//////////////////////////////////////////////////////////////////

void CSceneObject::InitialiseValuesPerLight(CLight* Light, CRay* IncomingRay, s_IntersectionInfo *Intersection)
{
	// This function will initialise any useful values needed for shading the object
	//
	// It is called once for every light

	DirToLight = Light->Position - Intersection->Point;
	DistanceToLight = DirToLight.Length();
	DirToLight.Normalize();


	double Divisor = Light->c1 + Light->c2 * DistanceToLight + Light->c3 * (DistanceToLight * DistanceToLight);

	if (Divisor <= 0.0)
		F_Attenuation = 1.0;
	else
	{
		F_Attenuation = 1.0 / Divisor;
		if (F_Attenuation > 1.0)
			F_Attenuation = 1.0;
	}


	NdotL = NormalVec.Dot(DirToLight);

	if (NdotL < 0.0)
		NdotL = 0.0;	// Just to make sure, should already have been clamped before we get here

	RdotV = ((NormalVec * (2.0 * NdotL)) - DirToLight).Dot(ViewerDir);

}

//////////////////////////////////////////////////////////////////
//
// Note: although this can be overridden the base class version
//		 i.e. this version should probably always be called
//
//////////////////////////////////////////////////////////////////

void CSceneObject::InitialiseValues(s_IntersectionInfo *Intersection, CRay *IncomingRay)
{
	// This function can be used to initialise any values that don't need to change for
	// every light
	// it will be called before any shading is done.

	CVector Zero;	// the vector (0,0,0)

	ViewerDir = Zero - IncomingRay->Dir;
	ViewerDir.Normalize();	// This isn't probably necessary but we'll do it just to make absolutely sure

	NormalVec = Intersection->Normal;
}


CColour CSceneObject::CalcSpecularContribution(CLight *Light)
{
	double RdotV_To_n;
	CColour Result;	// initialised to (0,0,0)

	if (RdotV > 0.0)
	{
		RdotV_To_n = pow(RdotV, Shininess);

		double Coeff = SR_Coeff * RdotV_To_n;

		Result.Set_R( (float) (Light->Colour.Get_R() * Coeff) );
		Result.Set_G( (float) (Light->Colour.Get_G() * Coeff) );
		Result.Set_B( (float) (Light->Colour.Get_B() * Coeff) );
	}

	return Result;
}

CColour CSceneObject::CalcDiffuseContribution(CLight *Light, CColour *SpecularAmount)
{
	CColour Result;

	double Coeff = DR_Coeff * NdotL;

	Result.Set_R( (float) ((Light->Colour.Get_R() - SpecularAmount->Get_R()) * Coeff * Colour.Get_R()) );
	Result.Set_G( (float) ((Light->Colour.Get_G() - SpecularAmount->Get_G()) * Coeff * Colour.Get_G()) );
	Result.Set_B( (float) ((Light->Colour.Get_B() - SpecularAmount->Get_B()) * Coeff * Colour.Get_B()) );

	return Result;
}
