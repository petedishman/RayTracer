// World.cpp: implementation of the CWorld class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RayTracer.h"
#include <time.h> // This is just for seeding the random number generator
#include "RayTracing.h"

#include "RayTable.h"
#include "World.h"
#include "RenderProgressDlg.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWorld::CWorld()
{
	FileParser = new CFileParser((CWorld*) this);

	RayArray = NULL;
	ImageFile = NULL;
	PreScaledImage = NULL;

	RestoreRenderOptions();

	FocalLength = 500.0;
	LensRadius = 1.0;

	KeepRendering = TRUE;
}

CWorld::~CWorld()
{
	delete FileParser;

	// save settings to registry
	SaveRenderOptions();

	// just to make sure there's no objects about
	RestoreDefaults();
}


BOOL CWorld::ConstructScene(CString *source_text)
{
	CString input;
	input = *source_text;

	// clean everything up before we read in new stuff
	RestoreDefaults();

	if (FileParser->ParseInput(&input))
	{
		return true;
	}
	else
	{
		// set up the error index so that calling function can highlight error
		ParserErrorIndex = FileParser->ErrorAtIndex;
		return false;
	}
}

void CWorld::RestoreDefaults()
{
	// This function will wipe out all of the stored world
	// information ready for a fresh set or when an error
	// occurs and we want to remove some dodgy data

	// we need to delete all the lights we created
	Light_List.FreeExtra();
	for (int i = 0; i < Light_List.GetSize(); i++)
		delete Light_List[i];

	Light_List.RemoveAll();

	// we need to delete all the objects we created
	Object_List.FreeExtra();
	for (int i = 0; i < Object_List.GetSize(); i++)
		delete Object_List[i];

	Object_List.RemoveAll();

	// return all the other values to their default states
	CameraLocation.Heading.Set(0.0, 0.0, 1.0);
	CameraLocation.Postion.Set(0.0, 0.0, -100.0);
	CameraLocation.Up.Set(0.0, 1.0, 0.0);

	bg_Colour.Set(0.0f, 0.0f, 0.0f);
	AmbientLight.Set(1.0f, 1.0f, 1.0f);

	FieldOfView = 30.0;

	Fog_Distance = 5000.0;

	Image_Height = 0;
	Image_Width = 0;

	KeepRendering = TRUE;
}

int CWorld::GetImageHeight()
{
	return Image_Height;
}

BOOL CWorld::EverythingReady()
{
	// FileParser ensures they're positive
	if (Image_Height != 0 && Image_Width != 0 && Object_List.GetSize() != 0 && Light_List.GetSize() != 0)
		return TRUE;
	else
	{
		HWND hWnd = AfxGetApp()->m_pMainWnd->m_hWnd;

		if (Image_Height == 0)
			MessageBox(hWnd, _T("You must specify a Height for the image"), _T("Error in Input"), MB_OK);
		if (Image_Width == 0)
			MessageBox(hWnd, _T("You must specify a Width for the image"), _T("Error in Input"), MB_OK);
		if (Object_List.GetSize() == 0)
			MessageBox(hWnd, _T("You must specify at least one object"), _T("Error in Input"), MB_OK);
		if (Light_List.GetSize() == 0)
			MessageBox(hWnd, _T("You must specify at least one light"), _T("Error in Input"), MB_OK);

		return FALSE;
	}
}

void CWorld::RenderImage()
{
	// This is the main rendering function
	// this is called and it results in the picture being created

	Stats.Set_RenderStart();
	Stats.Set_TotalLights(Light_List.GetSize());
	Stats.Set_TotalObjects(Object_List.GetSize());
	Stats.Set_ImageWidth(Image_Width);
	Stats.Set_ImageHeight(Image_Height);

	Stats.Set_ClampColours(ClampColours);
	Stats.Set_DepthOfField(DepthOfFieldOn);
	Stats.Set_FocalLength(FocalLength);
	Stats.Set_LensRadius(LensRadius);
	Stats.Set_LightAtten(LightAtten);
	Stats.Set_AtmosFogging(AtmosFogging);
	Stats.Set_MaxDepth(MaxDepth);
	Stats.Set_RaysPerPixel(RaysPerPixel);
	Stats.Set_StochasticSS(StochasticSS);

	// Need to create the output filename based off of the input file name
    CString OutputFileName;

	OutputFileName += OutputDir;
	if (OutputFileName.Right(2) != _T("\\"))
		OutputFileName += _T("\\");

	if (InputFileName.Right(4) == _T(".rsd"))
	{
		CString temp;
		temp = InputFileName.Left(InputFileName.GetLength() - 4);
		temp += _T(".tga");
		OutputFileName += temp;
	}
	else
	{
		OutputFileName += InputFileName;
		OutputFileName += _T(".tga");
	}


	// check to see that the file doesn't exist all ready
	CFile File;
	if (File.Open((LPCTSTR) OutputFileName, CFile::modeRead | CFile::shareDenyNone, NULL))
	{
		int result;
		// the file exists
		result = MessageBox(AfxGetApp()->m_pMainWnd->m_hWnd,
			_T("The output file already exists, either rename the existing file now\nor click OK to overwrite it anyway"),
			_T("File Exists"), MB_OK);
		File.Close();
	}

	Stats.Set_OutputFile(OutputFileName);
	Stats.Set_InputFile(InputFileName);

	// Initialise the renderer
	// First we need to set up the projection plane
	CalculateProjectionPlane();

	// we'll initialise the RayArray here too
	RayArray = new CRay[RaysPerPixel];

		// We also need to initialise the Targa surface to write to
	ImageFile = new CTarga;


	// seed random number generator with current time;
	srand((unsigned) time(NULL));

	// we're not fussed about background colour as we will be writing over
	// every pixel anyway
	ImageFile->create_canvas(Image_Width, Image_Height, 0,0,0);

	if (!ClampColours)
	{
		// we need to create an array of CColour's to allow scaling to be performed
		PreScaledImage = new CColour[Image_Width * Image_Height];

	}


	// render first line
	// send message to dialog box
	// repeat

	for (int j = 0; j < Image_Height; j++)
	{
		RenderLine(j);

		RenderingDialog->SendMessage(WM_USER_ONE_LINE_RENDERED, 0, 0);
	}


	// If we're scaling the colours ImageFile doesn't contain anything yet
	// so here we perform the scaling putting the results in to ImageFile.
	if (!ClampColours)
		ScaleColours();


	//MessageBox(AfxGetApp()->m_pMainWnd->m_hWnd, (LPCTSTR) OutputFileName, "Filename is", MB_OK);
	// write out image file
	if (!ImageFile->Write_Targa((LPCTSTR) OutputFileName))
		MessageBox(AfxGetApp()->m_pMainWnd->m_hWnd, _T("Failed to create file"), _T("Filename"), MB_OK);
	else
		MessageBox(AfxGetApp()->m_pMainWnd->m_hWnd, _T("File created successfully"), _T("Filename"), MB_OK);


	// we're done
	// clear up memory allocations
	ClearUpMemoryAllocations();

	// send message saying we're done
	RenderingDialog->SendMessage(WM_USER_RENDER_COMPLETE, 0, 0);

	Stats.Set_RenderDone();

	Stats.WriteStats();

}




// This is the thread function that gets the rendering going
// CRayTracerFileView will call this with AfxBeginThread()
UINT ThreadFunc (LPVOID pParam)
{
	CWorld * WorldPtr;

	WorldPtr = (CWorld *) pParam;

	WorldPtr->RenderImage();

	return 0;
}


void CWorld::CalculateProjectionPlane()
{
	// This function will take the camera position information
	// along with the field of view
	// and calculate where the projection plane is going to be
	// probably also want image height/width
	// as it's probably best if the projection plane has the same
	// aspect ratio that the image has.

	CVector CentreOfPlane;
	double HalfDistPlane;
	CVector DirToEdgeOfPlane;
	double HalfHeightPlane;
	double AspectRatio;

	CVector Edge1, Edge2;

	double CameraToPlane = 1.0;

	CentreOfPlane = CameraLocation.Postion + (CameraLocation.Heading * CameraToPlane);
	HalfDistPlane = CameraToPlane * tan(0.5 * DEG2RAD(FieldOfView));

	DirToEdgeOfPlane = CameraLocation.Up.Cross (CameraLocation.Heading);	// Up x Heading
	// the cross product automatically normalises the returned vector for us

	Edge1 = CentreOfPlane + (DirToEdgeOfPlane * HalfDistPlane);
	// negate the direction;
	DirToEdgeOfPlane = DirToEdgeOfPlane * -1;
	Edge2 = CentreOfPlane + (DirToEdgeOfPlane * HalfDistPlane);

	///////////////////////////////////////////////////////////////
	//
	//
	//		-----------------
	//      |               |
	//      |               |    we now have these 3 points on the plane
	//      *       *       *
	//      |               |
	//      |               |
	//      -----------------
	//
	// Trouble is we're not sure which way round Edge1 and Edge2 go
	// need to figure that out, but they could be anywhere
	//
	// the location all depends on the cross product.
	// with what I've got now (0,1,0)x(0,0,1) = (1,0,0)
	// which would imply edge1 is the one on the right
	// I'm going to go with that until something proves me wrong
	////////////////////////////////////////////////////////////////

	AspectRatio = (double) Image_Width / (double) Image_Height;

	HalfHeightPlane = HalfDistPlane / AspectRatio;

	// if we take a normalised copy of the camera up vector
	// we can get the 4 positions of the plane;

	CVector Up = CameraLocation.Up;
	Up.Normalize();

	ProjectionPlane.TopRight = Edge1 + (Up * HalfHeightPlane);
	ProjectionPlane.TopLeft = Edge2 + (Up * HalfHeightPlane);
	HalfHeightPlane = -HalfHeightPlane;

	ProjectionPlane.BottomRight = Edge1 + (Up * HalfHeightPlane);
	ProjectionPlane.BottomLeft = Edge2 + (Up * HalfHeightPlane);

	// done
}

void CWorld::RenderLine(int j)
{
	// This function will loop through all the pixels on this line and render them

	CColour result;

	for (int i = 0; i < Image_Width; i++)
	{
		result = RenderPixel(i, j);

		// depending on whether we're clamping or scaling colours we need to do different things
		// with result.
		if (ClampColours)
		{

			// have a problem here
			// rendering part is written with (0,0) being top left pixel
			// but Targa class is written with (0,0) being bottom left
			// and for good reason too if I remember
			// so I'll put in a small kludge here to get round that and keep
			// both as they are

			ImageFile->Write_Pixel(i, (Image_Height - 1 - j), result.Get_Rbyte(), result.Get_Gbyte(), result.Get_Bbyte());
		}
		else
		{
			// scaling colours
			PreScaledImage[j * Image_Width + i] = result;
		}
	}
}

CColour CWorld::RenderPixel(int i, int j)
{
	// This is a trickier one
	CColour temp;

	// We want to check here every loop that the user hasn't cancelled the process
	// Because we only check every once a pixel there might be a slight delay but nothing
	// too noticeable unless they're doing 100rpps and a massive maxdepth with a really
	// complicated scene.  But it's not that important to have a quick response anyway
	if (!KeepRendering)
	{
		// The User has cancelled it
		// we can call  TerminateRenderingThread() here which will then
		// clear up any memory and stop the thread.
		StopRendering();
	}

	if (DepthOfFieldOn)
	{
		// generate rays with depth of field ray function and average results;
		// Need to generate depth of field style rays then render them as for
		// ordinary rays, can probably happily move the calls to renderray below
		// out of the if statement as that section can be kept identical.

		// This function needs to generate a set of rays and place them in RayArray
		GenerateDOFRays(i, j);
	}
	else
	{
		GenerateRays(i, j);
		// the RayArray will now have the rays to use in it.
	}


	// Render all the different rays and then average their results
	for (int n = 0; n < RaysPerPixel; n++)
	{
		Stats.Inc_PrimaryRays();

		temp = temp + RenderRay(&(RayArray[n]), 0);
	}

	temp = temp * ((double) 1.0 / (double) RaysPerPixel);



	return temp;

}

void CWorld::GenerateRays(int i, int j)
{
	// This function will take a pixel address and then
	// using the Rays per pixel value and the projection
	// plane will calculate the rays needed filling in
	// the RayArray array  (what a great name :-) )

	double x, y;

	CVector AcrossPlane, DownPlane;
	CVector PointOnPlane;

	AcrossPlane = ProjectionPlane.TopRight - ProjectionPlane.TopLeft;
	DownPlane = ProjectionPlane.BottomLeft - ProjectionPlane.TopLeft;
	AcrossPlane.Normalize();
	DownPlane.Normalize();

	double LengthOfPlane, HeightOfPlane;
	LengthOfPlane = (ProjectionPlane.TopRight - ProjectionPlane.TopLeft).Length();
	HeightOfPlane = (ProjectionPlane.BottomLeft - ProjectionPlane.TopLeft).Length();

	// we want to generate rays from the camera position to a point on the projection plane
	double MaxJitter;

	if (StochasticSS)
	{
		switch (RaysPerPixel)
		{
		case ONE_RPP:
			MaxJitter = 0.5;	// probably quite strange using stochastic on 1 rpp
			break;
		case FOUR_RPP:
			MaxJitter = 0.25;
			break;
		case NINE_RPP:
			MaxJitter = 1.0 / 6.0;
			break;
		case SIXTEEN_RPP:
			MaxJitter = 1.0 / 8.0;
			break;
		case FORTYNINE_RPP:
			MaxJitter = 1.0 / 14.0;
			break;
		case HUNDRED_RPP:
			MaxJitter = 1.0 / 20.0;
			break;
		}
	}


	xy_pair temp;

	for (int n = 0; n < RaysPerPixel; n++)
	{
		switch (RaysPerPixel)
		{
		case ONE_RPP:
			temp = one_rpp[n];
			break;
		case FOUR_RPP:
			temp = four_rpp[n];
			break;
		case NINE_RPP:
			temp = nine_rpp[n];
			break;
		case SIXTEEN_RPP:
			temp = sixteen_rpp[n];
			break;
		case FORTYNINE_RPP:
			temp = fortynine_rpp[n];
			break;
		case HUNDRED_RPP:
			temp = hundred_rpp[n];
			break;
		}

		if (StochasticSS)
		{
			temp.x += GetRandomNumber(2.0 * MaxJitter) - MaxJitter;
			temp.y += GetRandomNumber(2.0 * MaxJitter) - MaxJitter;
		}

		x = ((double) i + temp.x) / (double) Image_Width;
		y = ((double) j + temp.y) / (double) Image_Height;

		x *= LengthOfPlane;
		y *= HeightOfPlane;

		PointOnPlane = ProjectionPlane.TopLeft + (AcrossPlane * x) + (DownPlane * y);

		RayArray[n].Start = CameraLocation.Postion;
		RayArray[n].Dir = PointOnPlane - CameraLocation.Postion;
		RayArray[n].Dir.Normalize();
		RayArray[n].CurrentObjIndex = -1;

	}
}

CColour CWorld::RenderRay(CRay *Ray, int Depth)
{
	// This is the recursive part of the ray tracer
	// we will call this recursively until we reach
	// the max depth or don't need to go any further

	CColour Result;
	CColour ZeroColour;
	s_ShadingInfo ShadingInformation;
	s_IntersectionInfo Intersection;
	int NoOfLightsVisible;

	// we can count the number of rays by how many times this function gets called.

	Stats.Inc_TotalRays();

	Stats.Set_CurrentDepth(Depth);

	if (Depth < MaxDepth)
	{
		// Render
		// Find closest intersection of ray with all objects
		// Find which lights are visible at that point
		// Calculate colour of point from those lights
		if (GetNearestIntersection(Ray, &Intersection))
		{
			NoOfLightsVisible = GetActiveLights(&Intersection);

			// Build data structure to pass to shade function
			ShadingInformation.Lights = &Light_List;
			ShadingInformation.ActiveLights = &ActiveLight_List;
			ShadingInformation.Intersection = &Intersection;
			ShadingInformation.Ambient = &AmbientLight;
			ShadingInformation.Ray = Ray;
			ShadingInformation.LightAtten = LightAtten;
			ShadingInformation.AtmosFogging = AtmosFogging;




			// now we can tell the object to shade it self
			Result = Object_List[Intersection.Index]->Shade(&ShadingInformation);


			// we're not in air so we should scale the shaded colour by the transparency I reckon
			if (Ray->CurrentObjIndex != -1)
				Result = Result * Object_List[Intersection.Index]->Transparency;


			// We're inside the object so we want to negate the normal to get the following
			// calculations correct.  We don't negate the normal for ordinary shading as
			// ordinary shading is for calculating the effect of lights on the surfaces not inside
			// them
			CVector ZeroVec;

			if (Ray->CurrentObjIndex != -1)
				Intersection.Normal = ZeroVec - Intersection.Normal;


			// at this point we can now generate the reflective ray and render it
			// recursively with RenderRay(Ray, Depth + 1)

			// we only do a reflective ray if the object is reflective

			if ((Object_List[Intersection.Index]->Reflection != ZeroColour))
			{
				CColour Refl_Colour;
				CRay	ReflectedRay;



				// Create the reflected ray
				ReflectedRay.Start = Intersection.Point;
				ReflectedRay.Dir = Object_List[Intersection.Index]->GetReflectedDir(Ray, &Intersection);

				// Nudge the ray along a little to avoid it hitting the object it came from
				ReflectedRay.Start = ReflectedRay.Evaluate(0.0005);

				Stats.Inc_ReflectedRays();

				Refl_Colour = RenderRay(&ReflectedRay, Depth + 1);

				Refl_Colour = Refl_Colour * Object_List[Intersection.Index]->Reflection;

				Result = Result +  Refl_Colour;

			}
			// now we need to spawn refracted rays

			if (Object_List[Intersection.Index]->Transparency != ZeroColour)
			{
				CColour Refr_Colour;
				CVector RefractedRayDir;
				CRay	RefractedRay;

				// We'll call GetRefractedDir with Ray containing the current objects RI
				if (Object_List[Intersection.Index]->GetRefractedDir(Ray, &Intersection, &RefractedRayDir))
				{
					// Create the refracted ray
					RefractedRay.Start = Intersection.Point;
					RefractedRay.Dir = RefractedRayDir;



//					if (Ray->CurrentRI == RI_AIR)
//						RefractedRay.CurrentRI = Object_List[Intersection.Index]->Refractive_Index;
//					else
//						RefractedRay.CurrentRI = RI_AIR;

					// If we're in air the new ray is in the object, if we're in the object
					// the new ray is in air as that's all we're allowing.
					if (Ray->CurrentObjIndex == -1)
						RefractedRay.CurrentObjIndex = Intersection.Index;
					else
						RefractedRay.CurrentObjIndex = -1;


					// Nudge the ray just a little bit
					RefractedRay.Start = RefractedRay.Evaluate(0.0005);

					Stats.Inc_TransmittedRays();

					Refr_Colour = RenderRay(&RefractedRay, Depth + 1);

					Refr_Colour = Refr_Colour * Object_List[Intersection.Index]->Transparency;

					Result = Result + Refr_Colour;
				}
				else
				{
					// we might need to trace the totally internally reflected ray actually?????
					RefractedRay.Start = Intersection.Point;
					RefractedRay.Dir = RefractedRayDir;

					// This ray should have the refractive index of the object as TIR rays should
					// always be internal
					RefractedRay.CurrentObjIndex = Intersection.Index;

					// Nudge the ray just a little bit
					RefractedRay.Start = RefractedRay.Evaluate(0.0005);

					Stats.Inc_TIRRays();

					Refr_Colour = RenderRay(&RefractedRay, Depth + 1);

					Refr_Colour = Refr_Colour * Object_List[Intersection.Index]->Transparency;

					Result = Result + Refr_Colour;

				}

			}
		}
		else
		{
			// we didn't get an intersection
			Result = bg_Colour;
		}

	}
	else
	{
		Stats.Inc_MaxDepthExceeded();

		// return black
		Result.Set(0.0f, 0.0f, 0.0f);
	}

	// we can apply fogging here if it is turned on

	if (AtmosFogging)
	{
		if (Result != bg_Colour)
		{
			// we got an intersection so apply fogging to it.
			// since the fog colour is the bg_colour there's no point applying fog
			// if it's already that colour plus it might mean there wasn't an intersection.

			double scale_factor;

			double Distance_To_Point;
			Distance_To_Point = (CameraLocation.Postion - Intersection.Point).Length();

			scale_factor = Distance_To_Point / Fog_Distance;

			// clamp that to [0..1]
			if (scale_factor < 0.0) scale_factor = 0.0;
			if (scale_factor > 1.0) scale_factor = 1.0;


			Result = (Result * (1 - scale_factor)) + (bg_Colour * scale_factor);


		}
	}

	return Result;

}

BOOL CWorld::GetNearestIntersection(CRay *Ray, s_IntersectionInfo *Info)
{
	// This function will take the given ray and intersect it
	// with all the objects in the object list looking for the closest
	// intersection
	// if it finds one it fills in the Info object and returns true
	// otherwise it returns false;

	CVector PointOfIntersection;


	s_IntersectionInfo CurrentHit;
	double CurrentLength;
	double length;

	bool foundany = false;

	bool FoundAtLeastOne = false;

	for (int i = 0; i < Object_List.GetSize(); i++)
	{
		if (Object_List[i]->Intersect(Ray, &PointOfIntersection))
		{
			// we have a hit
			FoundAtLeastOne = true;

			Stats.Inc_TotalIntersections();

			// find length of this ray
			length = (PointOfIntersection - Ray->Start).Length();

			if (!foundany)
			{
				CurrentHit.Index = i;
				CurrentHit.Point = PointOfIntersection;
				CurrentLength = length;
				foundany = true;
			}
			else
			{
				// we have found one before
				if (length < CurrentLength)
				{
					CurrentHit.Index = i;
					CurrentHit.Point = PointOfIntersection;
					CurrentLength = length;
				}
			}

		}
	}

	if (FoundAtLeastOne)
	{
		// I think this is the only place that the normal function is called

		Stats.Inc_ShadedIntersections();

		// Get the normal at the point
		CurrentHit.Normal = Object_List[CurrentHit.Index]->Normal(&(CurrentHit.Point), Ray);

		// return the intersection info
		(*Info) = CurrentHit;
	}

	return FoundAtLeastOne;
}

int CWorld::GetActiveLights(s_IntersectionInfo *Info)
{
	// This function will use the given information about
	// an intersection to check which lights are visible from
	// that point
	// it then fills in the array ActiveLight_List which the object
	// can then use to light itself with
	// if objects are in the way of the light but are transparent
	// then that is factored in to the info

	// the return value is the number of lights added to the list




	CRay Ray;
	bool Blocked;
	CVector PointOfIntersection;
	s_ActiveLightsInfo *LightInfo;
	double DistanceToLight;
	int count = 0;

	CColour transparency;
	CColour ZeroColour;

	// First off we want to clear out the old information from the active light list
	ActiveLight_List.FreeExtra();
	for (int n = 0; n < ActiveLight_List.GetSize(); n++)
		delete ActiveLight_List[n];
	ActiveLight_List.RemoveAll();



	Ray.Start = Info->Point;

	for (int i = 0; i < Light_List.GetSize(); i ++)
	{
		// we need to generate a ray from the point to each light in turn
		// then take that ray and see if any objects get in the way of it (noting if they're transparent)
		Ray.Dir = Light_List[i]->Position - Ray.Start;
		DistanceToLight = Ray.Dir.Length();
		Ray.Dir.Normalize();

		// If at this point dirtolight dotted with surface normal is not in range 0.0 to 1.0
		// then we ignore the light definitely
		double DotResult;
		DotResult = Ray.Dir.Dot(Info->Normal);
		// if this value is not in the range then we can't see the light
		// so... there's no point doing any of the below, the light is not visible ??



		// NOTE:
		//
		// When testing for the dot product to be in the desired range, if we use 0.0 and 1.0
		// we sometimes get what appear to be rounding errors (?) on the edges of spheres
		// these usually show up as near-white pixels as on that pixel there is a massive amount of
		// specular reflection from a light and on the pixel next to it the light is no longer visible
		// due to this dot product so the pixel ends up being significantly darker
		// needless to say this looks horrible.
		//
		// it may be necessary to change this value as 0.05 seems a little high but effective nonetheless.


		if (DotResult >= 0.05 )
		{


			transparency.Set(1.0f,1.0f,1.0f);		// ie. we want 100% of this lights light at the moment.
			Blocked = false;

			// we don't want to check for intersections with the object that this ray starts from !!!!!!

			for (int j = 0; j < Object_List.GetSize(); j++)
			{
				// we now need to check that ray against each object except itself
				if (j != Info->Index)
				{
					Stats.Inc_ShadowRays();

					if (Object_List[j]->Intersect(&Ray, &PointOfIntersection))
					{
						// this object is not necessarily in the way we need to check that the object
						// is actually before the light
						// i.e.   we have      pointofinterest.........object.......light
						//
						// rather than    pointofinterest.......light.......object

						// if the distance to PointOfIntersection from Ray.start is less than the distance to
						// the light then the object does block it otherwise it doesn't

						if ((PointOfIntersection - Ray.Start).Length() < DistanceToLight)
						{
							// this object is in the way
							// if it isn't transparent at all then we can stop
							// if it is transparent we have to note the transparency then check all the other objects
							if (Object_List[j]->Transparency == ZeroColour)
							{
								// object is not transparent so light is blocked
								Blocked = true;

								//MessageBox(NULL, "We're are getting lights blocked by objects", "Debug", MB_OK);

								break;
							}
							else
							{
								// object is transparent so include it's transparency
								transparency = transparency * Object_List[j]->Transparency;
							}
						}
						// else the light is in front of this object so we should count it
					}
				}
			} // end of for loop through objects

			// when we get here either the light is not visible and Blocked equals true
			// or the light is visible (possibly through transparent objects) and Blocked equals false
			if (!Blocked)
			{

				// the light is visible so we want to add it to the active lights list
				LightInfo = new s_ActiveLightsInfo;
				LightInfo->Index = i;
				LightInfo->Transparency = transparency;

				ActiveLight_List.Add(LightInfo);

				count++;
			}

		}
//		MessageBox(NULL, "The light was blocked due to the dot product going \nout of range (ray direction and normal)", "Debug", MB_OK);

		// if it is blocked we don't care and we just carry on checking the next one
	} // end of for loop through lights

	return count;	// number of lights added to list
}


void CWorld::GenerateDOFRays(int i, int j)
{
	// The first thing we want to do is generate a ray
	// from the eyepos straight through the centre of the
	// pixel, this will be our primary ray along which we'll
	// find the focal point for this set of rays.

	double x,y;

	CVector AcrossPlane, DownPlane, PointOnPlane;

	AcrossPlane = ProjectionPlane.TopRight - ProjectionPlane.TopLeft;
	DownPlane = ProjectionPlane.BottomLeft - ProjectionPlane.TopLeft;
	AcrossPlane.Normalize();
	DownPlane.Normalize();

	double LengthOfPlane, HeightOfPlane;
	LengthOfPlane = (ProjectionPlane.TopRight - ProjectionPlane.TopLeft).Length();
	HeightOfPlane = (ProjectionPlane.BottomLeft - ProjectionPlane.TopLeft).Length();

	// Generate primary ray through middle of pixel square.

	x = ((double) i + (double) 0.5) / (double) Image_Width;
	y = ((double) j + (double) 0.5) / (double) Image_Height;

	x *= LengthOfPlane;
	y *= HeightOfPlane;

	PointOnPlane = ProjectionPlane.TopLeft + (AcrossPlane * x) + (DownPlane * y);

	CRay PrimaryRay;
	PrimaryRay.Start = CameraLocation.Postion;
	PrimaryRay.Dir = PointOnPlane - CameraLocation.Postion;
	PrimaryRay.Dir.Normalize();

	CVector FocalPoint;

	FocalPoint = PrimaryRay.Evaluate(FocalLength);

	// Now we have the focal point we need to calculate the jittered positions for the other
	// rays to start from.

	// as we're using a square lens for now we can use the ray position tables from the ordinary
	// supersampling, however these are tables of offsets with 0,0 being the top left
	// we want 0,0 being the middle so we need to subtract 0.5 from all of them first

	xy_pair CurrentPoint;
	double MaxJitter;	// The maximum amount we can jitter each subpixel ray point to keep it in it's
						// little square

	switch (RaysPerPixel)
	{
	case ONE_RPP:
		// don't need it for this
		break;
	case FOUR_RPP:
		MaxJitter = 0.25;
		break;
	case NINE_RPP:
		MaxJitter = 1.0 / 6.0;
		break;
	case SIXTEEN_RPP:
		MaxJitter = 1.0 / 8.0;
		break;
	case FORTYNINE_RPP:
		MaxJitter = 1.0 / 14.0;
		break;
	case HUNDRED_RPP:
		MaxJitter = 1.0 / 20.0;
		break;
	}

	for (int n = 0; n < RaysPerPixel; n++)
	{
		switch (RaysPerPixel)
		{
		case ONE_RPP:
			CurrentPoint = one_rpp[n];	// doesn't make sense to do this for 1RPP, check above ver of this func
			break;
		case FOUR_RPP:
			CurrentPoint = four_rpp[n];
			break;
		case NINE_RPP:
			CurrentPoint = nine_rpp[n];
			break;
		case SIXTEEN_RPP:
			CurrentPoint = sixteen_rpp[n];
			break;
		case FORTYNINE_RPP:
			CurrentPoint = fortynine_rpp[n];
			break;
		case HUNDRED_RPP:
			CurrentPoint = hundred_rpp[n];
			break;
		}

		// subtract 0.5 off as mentioned above
		CurrentPoint.x -= 0.5;
		CurrentPoint.y -= 0.5;

		// Jitter the position in x and y by +/- MaxJitter

		CurrentPoint.x += GetRandomNumber(2.0 * MaxJitter) - MaxJitter;
		CurrentPoint.y += GetRandomNumber(2.0 * MaxJitter) - MaxJitter;

		// Scale the point by the lensradius, as we're using a square lens here, we'll take
		// lensradius to be the height/width of the lens

		CurrentPoint.x *= LensRadius;
		CurrentPoint.y *= LensRadius;

		// now we need to create the ray start point by getting a point that is y up from
		// the eyepos and x to the right of the eyepos.

		// to get the direction to the right of eyepos we need to do Heading X Up.

		CVector Up, Across;

		Up = CameraLocation.Up;
		Across = Up.Cross(CameraLocation.Heading);
		Across.Normalize();

		CVector RayStart;

		// Create the new ray
		RayStart = CameraLocation.Postion + (Across * CurrentPoint.x) + (Up * CurrentPoint.y);

		RayArray[n].Start = RayStart;
		RayArray[n].Dir = FocalPoint - RayStart;
		RayArray[n].Dir.Normalize();
		RayArray[n].CurrentObjIndex = -1;


	}


}


double CWorld::GetRandomNumber(double Max)
{
	// Return a random floating point number in the range (0.0..Max)

	return ((double) rand() / (double) RAND_MAX) * Max;
}

void CWorld::ScaleColours()
{
	// This function needs to find the highest colour value
	// in PreScaledImage[] and then scale all of the other colours
	// so that they all fit in to the range 0..1

	float highest = 0.0;
	CColour Current;
	// find the highest value
	for (int n = 0; n < Image_Width*Image_Height; n++)
	{
		Current = PreScaledImage[n];

		if (Current.Get_R() > highest)
			highest = Current.Get_R();
		if (Current.Get_G() > highest)
			highest = Current.Get_G();
		if (Current.Get_B() > highest)
			highest = Current.Get_B();

	}

	CString output;
	output.Format(_T("The highest colour value is : %f"), highest);

	MessageBox(NULL, (LPCTSTR) output, _T("Test"), MB_OK);


	float scale_value = 1.0f / highest;

	for (int y = 0; y < Image_Height; y++)
		for (int x = 0; x < Image_Width; x++)
		{
			// we don't want to scale the colours unless some have overflowed
			// to scale the colours we need to divide all of them by highest
			if (highest > 1.0)
				PreScaledImage[y * Image_Width + x] = PreScaledImage[y * Image_Width + x] * scale_value;

			BYTE r, g, b;
			r = PreScaledImage[y * Image_Width + x].Get_Rbyte();
			g = PreScaledImage[y * Image_Width + x].Get_Gbyte();
			b = PreScaledImage[y * Image_Width + x].Get_Bbyte();

			ImageFile->Write_Pixel(x, (Image_Height - 1 - y), r, g, b);


		}
}

void CWorld::ClearUpMemoryAllocations()
{
	// This function will clear out any possible memory allocations that have been
	// made it will be called when rendering is cancelled and when rendering ends
	// naturally.

	if (RayArray != NULL)
	{
		delete[] RayArray;
		RayArray = NULL;
	}

	if (ImageFile != NULL)
	{
		delete ImageFile;
		ImageFile = NULL;
	}

	if ((!ClampColours) && (PreScaledImage != NULL))
	{
		delete[] PreScaledImage;
		PreScaledImage = NULL;
	}

	ActiveLight_List.FreeExtra();
	for (int n = 0; n < ActiveLight_List.GetSize(); n++)
		delete ActiveLight_List[n];
	ActiveLight_List.RemoveAll();

	RestoreDefaults();
}

void CWorld::StopRendering()
{
	// This function needs to clear up any memory allocations and
	// then terminate the thread.

	ClearUpMemoryAllocations();

	// this function also deletes the thread object from memory so
	// we don't need to worry about freeing up that bit of memory
	AfxEndThread(0);

}

void CWorld::SaveRenderOptions()
{
	CWinApp *App = AfxGetApp();

	CString version = _T("version 1.0\\Render");

	App->WriteProfileInt(version, _T("Light Attenuation"), LightAtten);
	App->WriteProfileInt(version, _T("Atmos. Fogging"), AtmosFogging);
	App->WriteProfileInt(version, _T("Depth Of Field"), DepthOfFieldOn);
	App->WriteProfileInt(version, _T("StochasticSS"), StochasticSS);
	App->WriteProfileInt(version, _T("Clamp Colours"), ClampColours);
	App->WriteProfileInt(version, _T("Max Depth"), MaxDepth);
	App->WriteProfileInt(version, _T("Rays Per Pixel"), RaysPerPixel);
	App->WriteProfileString(version, _T("Output Dir"), OutputDir);

}

void CWorld::RestoreRenderOptions()
{
	CWinApp *App = AfxGetApp();

	CString version = _T("version 1.0\\Render");

	int LA, AF, DF, SS, CC, MD, RP;
	CString OD;

	if (
		( (LA = App->GetProfileInt(version, _T("Light Attenuation"), -1)) != -1) &&
		( (AF = App->GetProfileInt(version, _T("Atmos. Fogging"), -1)) != -1) &&
		( (DF = App->GetProfileInt(version, _T("Depth Of Field"), -1)) != -1) &&
		( (SS = App->GetProfileInt(version, _T("StochasticSS"), -1)) != -1) &&
		( (CC = App->GetProfileInt(version, _T("Clamp Colours"), -1)) != -1) &&
		( (MD = App->GetProfileInt(version, _T("Max Depth"), -1)) != -1) &&
		( (RP = App->GetProfileInt(version, _T("Rays Per Pixel"), -1)) != -1))
	{
		// they all had values so now we can use them
		DepthOfFieldOn = DF;
		RaysPerPixel = RP;
		MaxDepth = MD;
		StochasticSS = SS;
		LightAtten = LA;
		AtmosFogging = AF;
		ClampColours = CC;
	}
	else
	{
		// use bog standard default values
		DepthOfFieldOn = FALSE;
		RaysPerPixel = ONE_RPP;
		MaxDepth = 10;
		StochasticSS = FALSE;
		LightAtten = FALSE;
		AtmosFogging = FALSE;
		ClampColours = TRUE;
	}



	if ((OD = App->GetProfileString(version, _T("Output Dir"))) != "")
	{
		// got a good value, use it
		OutputDir = OD;
	}
	else
	{
		// use a default value
		OutputDir = "C:\\My Document\\";
	}
}
