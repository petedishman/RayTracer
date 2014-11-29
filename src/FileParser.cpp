// FileParser.cpp: implementation of the CFileParser class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RayTracer.h"
#include "RayTracing.h"
#include "FileParser.h"
#include "World.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////
//
//  Constructors / Destructors
//
/////////////////////////////////////////////

CFileParser::CFileParser()
{
}

CFileParser::CFileParser(CWorld* ptr)
{
	WorldPtr = ptr;
}

CFileParser::~CFileParser()
{
}

////////////////////////////////////////////////
//
// Lexing functions
//
/////////////////////////////////////////////////


TCHAR CFileParser::GetNextChar()
{
	// reads next character from the input string
	// returning either the character or END (for the end of the string)
	if (index < length)
	{
		// get the next character and increment the index)
		TCHAR temp;

		temp = input[index];
		index++;				// should be able to replace this with return input[index++];
		
		return temp;
	}
	else
	{
		// we've reached the end of input so return END
		return END;
	}
}

void CFileParser::SkipComment()
{
	// a comment starts when a ';' is read in
	// and continues until a new line character is found (or END)
	
	TCHAR temp;

	temp = GetNextChar();

	while (temp != '\n' && temp != END)
		temp = GetNextChar();
}

TCHAR CFileParser::LookAtNextChar()
{
	// this function does much the same as GetNextChar()
	// except it won't move the index value up
	if (index < length)
	{
		return input[index];
	}
	else
	{
		return END;
	}
}

int CFileParser::GetNextToken()
{
	// this function will scan the input string
	// to find the next token for parsing
	// it returns #define's such as T_CAMERA

	CString Value = "";

	TCHAR Current;
	
	while (1)	// this while loop will be exited by one of the below return statements;
	{
		Current = GetNextChar();

		switch (Current)
		{
			// sorry for the ugly hacks, but this'll have to do for now
		case 'a':
		case 'b':
		case 'c':
		case 'd':
		case 'e':
		case 'f':
		case 'g':
		case 'h':
		case 'i':
		case 'j':
		case 'k':
		case 'l':
		case 'm':
		case 'n':
		case 'o':
		case 'p':
		case 'q':
		case 'r':
		case 's':
		case 't':
		case 'u':
		case 'v':
		case 'w':
		case 'x':
		case 'y':
		case 'z':
		case '_':
			{
			// place Current on to a temp string
			// if the next character is one of this set too stick that on the string etc.
			Value += Current;

			TCHAR temp;

			temp = LookAtNextChar();
			while ((temp >= 'a' && temp <= 'z') || temp == '_')   // while it's a valid letter
			{
				Value += GetNextChar();	// put the letter on to the temp string and move the index up one

				temp = LookAtNextChar(); // have a peek at the next char
			}
			
			// when we get here the next letter is not part of the word
			// so we shouldn't really care what it is
			// we can just try and match what we've got

			// match Value against all of our set string literals.
			if (Value == "camera")				return T_CAMERA;
			if (Value == "begin")				return T_BEGIN;
			if (Value == "end")					return T_END;
			if (Value == "pos")					return T_POS;
			if (Value == "up")					return T_UP;
			if (Value == "heading")				return T_HEADING;
			if (Value == "fov")					return T_FOV;
			if (Value == "background")			return T_BACKGROUND;
			if (Value == "width")				return T_WIDTH;
			if (Value == "height")				return T_HEIGHT;
			if (Value == "light")				return T_LIGHT;
			if (Value == "colour")				return T_COLOUR;
			if (Value == "sphere")				return T_SPHERE;
			if (Value == "centre")				return T_CENTRE;
			if (Value == "radius")				return T_RADIUS;
			if (Value == "sr_coeff")			return T_SR_COEFF;
			if (Value == "dr_coeff")			return T_DR_COEFF;
			if (Value == "shininess")			return T_SHININESS;
			if (Value == "reflection")			return T_REFLECTION;
			if (Value == "transparency")		return T_TRANSPARENCY;
			if (Value == "refractive_index")	return T_REFRACTIVE_INDEX;
			if (Value == "ambient_colour")		return T_AMBIENTCOLOUR;
			if (Value == "amb_coeff")			return T_AMB_COEFF;
			if (Value == "focallength")			return T_FOCALLENGTH;
			if (Value == "lensradius")			return T_LENSRADIUS;
			if (Value == "coeff_a")				return T_LA_A;
			if (Value == "coeff_b")				return T_LA_B;
			if (Value == "coeff_c")				return T_LA_C;
			if (Value == "infinite_plane")		return T_INFINITEPLANE;
			if (Value == "plane_normal")		return T_PLANENORMAL;
			if (Value == "distance")			return T_DISTANCE;
			if (Value == "colour_one")			return T_COLOUR1;
			if (Value == "colour_two")			return T_COLOUR2;
			if (Value == "checksize")			return T_CHECKSIZE;
			if (Value == "checkerboard")		return T_CHECKERBOARD;
			if (Value == "cylinder")			return T_CYLINDER;
			if (Value == "axis")				return T_AXIS;
			if (Value == "length")				return T_LENGTH;
			if (Value == "bounded_plane")		return T_BOUNDEDPLANE;
			if (Value == "bounded_checkboard")	return T_BOUNDEDCHECKBOARD;
			if (Value == "fog_distance")		return T_FOGDISTANCE;
			
			// if we haven't returned by now then we don't know what it is
			// so return T_UNKNOWN and let the parser crap out and deal with it
			return T_UNKNOWN;

			// no need for a break statement here with all these returns around!
			}
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		case '.':
		case '-':
			{
			// we can use the CString value to build up the number in much 
			// the same was as is done above
			Value += Current;

			TCHAR temp;

			temp = LookAtNextChar();
			while ((temp >= '0' && temp <= '9') || temp == '.')   // while it's a valid digit
			{
				Value += GetNextChar();	// put the digit on to the temp string and move the index up one

				temp = LookAtNextChar(); // have a peek at the next char
			}
			// this will happily pile any number of digits and .'s on to the string
			// until it finds something else, it's up to the parser to check
			// if it's valid or not.
			// the parser can get the string from the class member Number (a CString)

			Number = Value;
			return T_NUMBER;
			}
		case '(':
			return T_BRA;

		case ')':
			return T_KET;

		case '=':
			return T_EQUALS;

		case ',':
			return T_COMMA;

		case ';':
			// we can call SkipComment() then break
			// which will make us come back round this switch
			SkipComment();
			break;

		case '\n':
		case '\r':
		case '\f':
		case '\t':
		case ' ':
			// we want to skip these so now they've been absorbed loop back round
			break;

		case END:
			return END_OF_INPUT;

		default:
			return T_UNKNOWN;
		}
	}
}

//////////////////////////////////////////////
//
// Parsing Functions
//
///////////////////////////////////////////////

int CFileParser::Parse_Error()
{
	// This function will clear out any stored data in CWorld
	// then set the Error point for CWorld to see
	// and then return;

	WorldPtr->RestoreDefaults();

	// Set up the CFileParser variable indicating where the error occured
	ErrorAtIndex = index;


	return PARSER_FAILURE;
}


int CFileParser::ParseInput(CString *source)
{
	input = *source; // these are CString's not C-Style strings so this actually copies the string
					 // it's not just a pointer copy

	// input should be case insensitive so we'll make it all lower case for now
	input.MakeLower();

	input.FreeExtra();
	length = input.GetLength();

	// initialisation
	index = 0;
	NoOfLights = 0;
	NoOfObjects = 0;

	CSceneObject *LastObjectPtr = NULL;
	CLight *LastLightPtr = NULL;
	int LastTransformableObj = 0;	// used because we want to be able to transform the
									// camera and lights as well as objects
									// It will use the ordinary token values as Identifiers

	int CurrentToken;
	int CurrentMode = M_NULL;
	BOOL Done = false;

	while (!Done)
	{
		switch (CurrentMode)
		{
		case M_NULL:
			{
				CurrentToken = GetNextToken();
				switch (CurrentToken)
				{
				case T_CAMERA:
					CurrentMode = M_CAMERA;
					break;
				case T_AMBIENTCOLOUR:
					CurrentMode = M_AMBIENT;
					break;
				case T_FOV:
					CurrentMode = M_FOV;
					break;
				case T_BACKGROUND:
					CurrentMode = M_BACKGROUND;
					break;
				case T_WIDTH:
					CurrentMode = M_WIDTH;
					break;
				case T_HEIGHT:
					CurrentMode = M_HEIGHT;
					break;
				case T_LIGHT:
					CurrentMode = M_LIGHT;
					break;
				case T_SPHERE:
					CurrentMode = M_SPHERE;
					break;
				case T_FOCALLENGTH:
					CurrentMode = M_FOCALLENGTH;
					break;
				case T_LENSRADIUS:
					CurrentMode = M_LENSRADIUS;
					break;
				case T_INFINITEPLANE:
					CurrentMode = M_INFINITEPLANE;
					break;
				case T_CHECKERBOARD:
					CurrentMode = M_CHECKERBOARD;
					break;
				case T_CYLINDER:
					CurrentMode = M_CYLINDER;
					break;
				case T_BOUNDEDPLANE:
					CurrentMode = M_BOUNDEDPLANE;
					break;
				case T_BOUNDEDCHECKBOARD:
					CurrentMode = M_BOUNDEDCHECKBOARD;
					break;
				case T_FOGDISTANCE:
					CurrentMode = M_FOGDISTANCE;
					break;
				case END_OF_INPUT:
					// I think we just need to set Done to true here
					// which will cause the parser to exit successfully
					Done = true;
					break;
				default:
					return Parse_Error();
				}
				break;
			}
		case M_CAMERA:
			{
				// now we want to read in all the camera information
				if (GetNextToken() != T_BEGIN) return Parse_Error();

				int VectorType;
				CVector Vector;
				
				for (int i = 0; i < 3; i++)
				{
					VectorType = GetNextToken();

					if (!ReadVector(Vector))	return Parse_Error();
					
					switch (VectorType)
					{
					case T_POS:
						WorldPtr->CameraLocation.Postion = Vector;
						break;
					case T_UP:
						WorldPtr->CameraLocation.Up = Vector;
						break;
					case T_HEADING:
						WorldPtr->CameraLocation.Heading = Vector;
						break;
					default:
						return Parse_Error();
					}
				}
				
				// now we've read all three vectors
				if (GetNextToken() != T_END) return Parse_Error();

				LastTransformableObj = T_CAMERA;
				LastObjectPtr = NULL;
				LastLightPtr = NULL;

				// then return to M_NULL mode
				CurrentMode = M_NULL;
				break;
			}

		case M_FOV:
			{
				// read in the Field of View information
				double FOV;

				if (!ReadDouble(FOV))	return Parse_Error();
				if (!CheckGTZ(FOV))		return Parse_Error();

				WorldPtr->FieldOfView = FOV;

				// return to M_NULL mode
				CurrentMode = M_NULL;
				break;
			}
		case M_BACKGROUND:
			{
				// read in the background colour information
				CColour Colour;

				if (!ReadColour(Colour))	return Parse_Error();

				// write the information to CWorld
				WorldPtr->bg_Colour = Colour;

				// return to NULL mode
				CurrentMode = M_NULL;
				break;
			}
		case M_WIDTH:
			{
				int width;

				if (!ReadInteger(width))	return Parse_Error();
				if (!CheckGTZ(width))		return Parse_Error();

				// write to CWorld
				WorldPtr->Image_Width = width;

				// return to NULL mode
				CurrentMode = M_NULL;
				break;
			}
		case M_HEIGHT:
			{
				int height;

				if (!ReadInteger(height))	return Parse_Error();
				if (!CheckGTZ(height))		return Parse_Error();

				// write to CWorld
				WorldPtr->Image_Height = height;
				// return to NULL mode
				CurrentMode = M_NULL;
				break;
			}
		case M_FOCALLENGTH:
			{
				double focallength;

				if (!ReadDouble(focallength))	return Parse_Error();
				if (!CheckGTZ(focallength))		return Parse_Error();
				// write to CWorld
				WorldPtr->FocalLength = focallength;

				// return to NULL mode
				CurrentMode = M_NULL;
				break;
			}
		case M_LENSRADIUS:
			{
				double radius;

				if (!ReadDouble(radius))	return Parse_Error();
				if (!CheckGTZ(radius))		return Parse_Error();

				// write to CWorld
				WorldPtr->LensRadius = radius;
				// return to NULL mode
				CurrentMode = M_NULL;
				break;
			}
		case M_AMBIENT:
			{
				// read in the ambient colour information

				CColour Colour;

				if (!ReadColour(Colour))	return Parse_Error();

				// write the information to CWorld

				WorldPtr->AmbientLight = Colour;

				// return to NULL mode
				CurrentMode = M_NULL;
				break;
			}
		case M_LIGHT:
			{
				CVector Vector;
				CColour Colour;
				CLight *light;
				int Token;
				double Coeff1, Coeff2, Coeff3;
				BOOL ColourSet, Coeff1Set, Coeff2Set, Coeff3Set;
				
				ColourSet = Coeff1Set = Coeff2Set = Coeff3Set = FALSE;

				if (GetNextToken() != T_BEGIN)	return Parse_Error();
				if (GetNextToken() != T_POS)	return Parse_Error();
				// position is the only required item for a light
				// so it should also come next
				if (!ReadVector(Vector))	return Parse_Error();

				// at this point the user could either put END, COLOUR or coeff_a, coeff_b etc
				Token = GetNextToken();   

				while (Token != T_END)
				{
					switch (Token)
					{
					case T_COLOUR:
						if (!ReadColour(Colour))	return Parse_Error();
						ColourSet = TRUE;
						break;
					case T_LA_A:	// coeff1
						if (!ReadDouble(Coeff1))	return Parse_Error();
						if (!CheckGTEZ(Coeff1))		return Parse_Error();
						Coeff1Set = TRUE;
						break;
					case T_LA_B:	// coeff2
						if (!ReadDouble(Coeff2))	return Parse_Error();
						if (!CheckGTEZ(Coeff2))		return Parse_Error();
						Coeff2Set = TRUE;
						break;
					case T_LA_C:	// coeff3
						if (!ReadDouble(Coeff3))	return Parse_Error();
						if (!CheckGTEZ(Coeff3))		return Parse_Error();
						Coeff3Set = TRUE;
						break;
					default:
						return Parse_Error();
					}
	
					Token = GetNextToken();
					if (Token == END_OF_INPUT) return Parse_Error();
				}

				// everythings been read now so we can set up the light

				// we can set up the light object now
				// and add it to the list in CWorld

				light = new CLight;
				light->Position = Vector;
				
				if (ColourSet)	light->Colour = Colour;
				if (Coeff1Set)	light->c1 = Coeff1;
				if (Coeff2Set)	light->c2 = Coeff2;
				if (Coeff3Set)	light->c3 = Coeff3;

				WorldPtr->Light_List.Add (light);
				NoOfLights++;

				LastTransformableObj = T_LIGHT;
				LastObjectPtr = NULL;
				LastLightPtr = light;


				// set mode back to NULL
				CurrentMode = M_NULL;
				break;
			}
		case M_SPHERE:
			{
				// read in all the information for a sphere
				// centre and radius are required and must come first
				// in that order

				double rad;
				CVector Vector;
				CSphere *Sphere;

				if (GetNextToken() != T_BEGIN)	return Parse_Error();
				if (GetNextToken() != T_CENTRE)	return Parse_Error();

				if (!ReadVector(Vector))		return Parse_Error();

				// the next token must be T_RADIUS
				if (GetNextToken() != T_RADIUS)	return Parse_Error();
				if (!ReadDouble(rad))			return Parse_Error();
				if (!CheckGTZ(rad))				return Parse_Error();

				// now we get to the optional parameters
				DefaultParameters Settings;

				if(!ReadDefaultParameters(Settings))	return Parse_Error();

				// we've now read in all we're going to including the END token;
				// now we need to create the new sphere object
				// and add it to the list.

				Sphere = new CSphere;
				Sphere->Centre = Vector;
				Sphere->radius = rad;

				ApplyDefaultParameters(Settings, Sphere);

				// add the new sphere to the list and increment the counter
				WorldPtr->Object_List.Add (Sphere);
				NoOfObjects++;

				LastTransformableObj = T_SPHERE;
				LastObjectPtr = Sphere;
				LastLightPtr = NULL;

				// set mode back to null and carry on
				CurrentMode = M_NULL;
				break;
			}
		case M_INFINITEPLANE:
			{
				CVector Normal;
				double Distance;
				CInfinitePlane *Plane;

				if (GetNextToken() != T_BEGIN)	return Parse_Error();
				if (GetNextToken() != T_PLANENORMAL) return Parse_Error();

				if (!ReadVector(Normal)) return Parse_Error();
				if (GetNextToken() != T_DISTANCE) return Parse_Error();
				if (!ReadDouble(Distance)) return Parse_Error();

				DefaultParameters Settings;
				if (!ReadDefaultParameters(Settings))	return Parse_Error();

				Plane = new CInfinitePlane;

				Plane->PlaneNormal = Normal;
				Plane->DistanceToOrigin = Distance;

				ApplyDefaultParameters(Settings, Plane);

				WorldPtr->Object_List.Add (Plane);
				NoOfObjects++;

				LastTransformableObj = T_INFINITEPLANE;
				LastObjectPtr = Plane;
				LastLightPtr = NULL;

				CurrentMode = M_NULL;
				break;
			}
		case M_CHECKERBOARD:
			{
				CVector Normal;
				double Distance;
				double CheckSize;
				CColour Colour1;
				CColour Colour2;
				CCheckerBoard *Plane;

				// Read Token Values
				if (GetNextToken() != T_BEGIN)	return Parse_Error();

				if (GetNextToken() != T_PLANENORMAL) return Parse_Error();
				if (!ReadVector(Normal)) return Parse_Error();

				if (GetNextToken() != T_DISTANCE) return Parse_Error();
				if (!ReadDouble(Distance)) return Parse_Error();

				if (GetNextToken() != T_COLOUR1)	return Parse_Error();
				if (!ReadColour(Colour1))	return Parse_Error();
				
				if (GetNextToken() != T_COLOUR2)	return Parse_Error();
				if (!ReadColour(Colour2))	return Parse_Error();

				if (GetNextToken() != T_CHECKSIZE)	return Parse_Error();
				if (!ReadDouble(CheckSize))	return Parse_Error();
				if (!CheckGTZ(CheckSize))	return Parse_Error();

				DefaultParameters Settings;
				if (!ReadDefaultParameters(Settings))	return Parse_Error();

				// Create the Object and apply the token values to it
				Plane = new CCheckerBoard;

				Plane->PlaneNormal = Normal;
				Plane->DistanceToOrigin = Distance;
				Plane->Colour1 = Colour1;
				Plane->Colour2 = Colour2;
				Plane->CheckSize = CheckSize;

				ApplyDefaultParameters(Settings, Plane);

				WorldPtr->Object_List.Add (Plane);
				NoOfObjects++;

				LastTransformableObj = T_CHECKERBOARD;
				LastObjectPtr = Plane;
				LastLightPtr = NULL;

				CurrentMode = M_NULL;
				break;
			}
		case M_CYLINDER:
			{
				CVector Axis, Centre;
				double cylinderLength, radius;
				CCylinder* Cylinder;

				if (GetNextToken() != T_BEGIN)		return Parse_Error();

				if (GetNextToken() != T_CENTRE)		return Parse_Error();
				if (!ReadVector(Centre))			return Parse_Error();

				if (GetNextToken() != T_AXIS)		return Parse_Error();
				if (!ReadVector(Axis))				return Parse_Error();

				if (GetNextToken() != T_LENGTH)		return Parse_Error();
				if (!ReadDouble(cylinderLength))	return Parse_Error();
				if (!CheckGTZ(cylinderLength))		return Parse_Error();

				if (GetNextToken() != T_RADIUS)		return Parse_Error();
				if (!ReadDouble(radius))			return Parse_Error();
				if (!CheckGTZ(radius))				return Parse_Error();

				DefaultParameters Settings;
				if (!ReadDefaultParameters(Settings))	return Parse_Error();

				Cylinder = new CCylinder;
				
				// Apply cylinder specific settings
				Axis.Normalize();
				Cylinder->Axis = Axis;
				Cylinder->Length = cylinderLength;
				Cylinder->Radius = radius;

				CVector temp;
				temp = temp - Axis;  // get a copy of Axis pointing the other way.

				// User gives centre point we want end point
				Cylinder->StartPoint = Centre + (temp * ( cylinderLength / ((double) 2.0)));

				// Set up the bounding planes for the cylinder
				Cylinder->InitialiseBounds();

				ApplyDefaultParameters(Settings, Cylinder);

				WorldPtr->Object_List.Add (Cylinder);
				NoOfObjects++;

				LastTransformableObj = T_CYLINDER;
				LastObjectPtr = Cylinder;
				LastLightPtr = NULL;

				CurrentMode = M_NULL;
				break;
			}
		case M_BOUNDEDPLANE:
			{
				double Length, Width;
				CBoundedPlane* Plane;

				if (GetNextToken() != T_BEGIN)		return Parse_Error();
				if (GetNextToken() != T_LENGTH)		return Parse_Error();
				if (!ReadDouble(Length))			return Parse_Error();
				if (!CheckGTZ(Length))				return Parse_Error();

				if (GetNextToken() != T_WIDTH)		return Parse_Error();
				if (!ReadDouble(Width))				return Parse_Error();
				if (!CheckGTZ(Width))				return Parse_Error();

				DefaultParameters Settings;

				if (!ReadDefaultParameters(Settings))	return Parse_Error();

				Plane = new CBoundedPlane;

				Plane->PlaneNormal.Set(0.0, 1.0, 0.0);
				Plane->DistanceToOrigin = 0.0;
				Plane->Length = Length;
				Plane->Width = Width;
				
				ApplyDefaultParameters(Settings, Plane);

				WorldPtr->Object_List.Add(Plane);
				NoOfObjects++;

				LastTransformableObj = T_BOUNDEDPLANE;
				LastObjectPtr = Plane;
				LastLightPtr = NULL;

				CurrentMode = M_NULL;
				break;
			}
		case M_BOUNDEDCHECKBOARD:
			{
				double Length, Width;
				CColour Colour1, Colour2;
				double CheckSize;
				CBoundedCheckerBoard* Plane;

				if (GetNextToken() != T_BEGIN)		return Parse_Error();
				if (GetNextToken() != T_LENGTH)		return Parse_Error();
				if (!ReadDouble(Length))			return Parse_Error();
				if (!CheckGTZ(Length))				return Parse_Error();

				if (GetNextToken() != T_WIDTH)		return Parse_Error();
				if (!ReadDouble(Width))				return Parse_Error();
				if (!CheckGTZ(Width))				return Parse_Error();

				if (GetNextToken() != T_COLOUR1)	return Parse_Error();
				if (!ReadColour(Colour1))			return Parse_Error();
				
				if (GetNextToken() != T_COLOUR2)	return Parse_Error();
				if (!ReadColour(Colour2))			return Parse_Error();

				if (GetNextToken() != T_CHECKSIZE)	return Parse_Error();
				if (!ReadDouble(CheckSize))			return Parse_Error();
				if (!CheckGTZ(CheckSize))			return Parse_Error();

				DefaultParameters Settings;

				if (!ReadDefaultParameters(Settings))	return Parse_Error();

				Plane = new CBoundedCheckerBoard;

				Plane->PlaneNormal.Set(0.0, 1.0, 0.0);
				Plane->DistanceToOrigin = 0.0;
				Plane->Length = Length;
				Plane->Width = Width;
				Plane->Colour1 = Colour1;
				Plane->Colour2 = Colour2;
				Plane->CheckSize = CheckSize;
				
				ApplyDefaultParameters(Settings, Plane);

				WorldPtr->Object_List.Add(Plane);
				NoOfObjects++;

				LastTransformableObj = T_BOUNDEDCHECKBOARD;
				LastObjectPtr = Plane;
				LastLightPtr = NULL;

				CurrentMode = M_NULL;
				break;
			}
		case M_FOGDISTANCE:
			{
				double FogDistance;

				if (!ReadDouble(FogDistance))		return Parse_Error();
				if (!CheckGTZ(FogDistance))			return Parse_Error();

				WorldPtr->Fog_Distance = FogDistance;

				CurrentMode = M_NULL;
				break;
			}


		}// end of switch (CurrentMode)

	} // end of while (!Done)

	return PARSER_SUCCESS;
}


BOOL CFileParser::ReadInteger(int &Value)
{
	// This function will read in an Integer value along with the preceding equals sign
	// placing the result in Value and returning TRUE on success
	// if it fails at any point it returns FALSE

	if (GetNextToken() != T_EQUALS) return FALSE;
	if (GetNextToken() != T_NUMBER) return FALSE;

	Value = _ttoi((LPCTSTR) Number);
	
	return TRUE;
}

BOOL CFileParser::ReadFloat(float &Value)
{
	// same as ReadInteger(...) except for floats

	if (GetNextToken() != T_EQUALS) return FALSE;
	if (GetNextToken() != T_NUMBER) return FALSE;

	Value = (float) _ttof((LPCTSTR) Number);
	
	return TRUE;
}

BOOL CFileParser::ReadDouble(double &Value)
{
	// same as ReadInteger(...) except for doubles 
	
	if (GetNextToken() != T_EQUALS) return FALSE;
	if (GetNextToken() != T_NUMBER) return FALSE;

	Value = _ttof((LPCTSTR) Number);
	
	return TRUE;
}


BOOL CFileParser::ReadColour(CColour &Colour)
{
	// same again, except for colour values

	float r,g,b;

	if (GetNextToken() != T_EQUALS) return FALSE;
	if (GetNextToken() != T_BRA)	return FALSE;
	if (GetNextToken() != T_NUMBER)	return FALSE;

	r = (float) _ttof((LPCTSTR) Number);

	if (GetNextToken() != T_COMMA)	return FALSE;
	if (GetNextToken() != T_NUMBER)	return FALSE;

	g = (float) _ttof((LPCTSTR) Number);

	if (GetNextToken() != T_COMMA)	return FALSE;
	if (GetNextToken() != T_NUMBER)	return FALSE;

	b = (float) _ttof((LPCTSTR) Number);

	if (GetNextToken() != T_KET)	return FALSE;

	Colour.Set(r,g,b);

	if (!CheckColourValid(Colour))	return FALSE;

	return TRUE;
}

BOOL CFileParser::ReadVector(CVector &Vector)
{
	// guess what this does ?
	double x,y,z;

	if (GetNextToken() != T_EQUALS) return FALSE;
	if (GetNextToken() != T_BRA)	return FALSE;
	if (GetNextToken() != T_NUMBER)	return FALSE;

	x = _ttof((LPCTSTR) Number);

	if (GetNextToken() != T_COMMA)	return FALSE;
	if (GetNextToken() != T_NUMBER)	return FALSE;

	y = _ttof((LPCTSTR)Number);

	if (GetNextToken() != T_COMMA)	return FALSE;
	if (GetNextToken() != T_NUMBER)	return FALSE;

	z = _ttof((LPCTSTR)Number);

	if (GetNextToken() != T_KET)	return FALSE;

	Vector.Set(x,y,z);

	return TRUE;
}

BOOL CFileParser::ReadDefaultParameters(DefaultParameters &DefParm)
{
	// This function will read in all of the default parameters
	// that every object in the scene has.
	//
	// It fills in the given DefaultParameters struct with all of the
	// information and which ones it set (as these are all optional parameters)
	//
	// It returns TRUE if everything was read correctly otherwise
	// it will return FALSE the calling procedure should then call Parse_Error()

	// This function should be called once all of the required parameters for 
	// an object have been read in it expects the first token to be either T_END
	// or one of the starting tokens for an optional component.

	// NOTE: This function will read the last END token in as well so it should
	// be the last bit of parsing for each object

	// First thing to do is to ensure that the BOOLs in DefParm are all set to FALSE

	DefParm.Amb_CoeffSet = 
	DefParm.ColourSet = 
	DefParm.DR_CoeffSet = 
	DefParm.Refractive_IndexSet = 
	DefParm.ReflectionSet = 
	DefParm.ShininessSet = 
	DefParm.SR_CoeffSet = 
	DefParm.TransparencySet = FALSE;

	int Token;

	Token = GetNextToken();

	while (Token != T_END)
	{
		switch (Token)
		{
		case T_COLOUR:
			{
				if (!ReadColour(DefParm.Colour))	return FALSE;
				DefParm.ColourSet = TRUE;
				break;
			}
		case T_SR_COEFF:
			{
				if (!ReadDouble(DefParm.SR_Coeff))	return FALSE;
				if (!RangeCheck(DefParm.SR_Coeff, 0.0, 1.0)) return FALSE;
				DefParm.SR_CoeffSet = TRUE;
				break;
			}
		case T_DR_COEFF:
			{
				if (!ReadDouble(DefParm.DR_Coeff))	return FALSE;
				if (!RangeCheck(DefParm.DR_Coeff, 0.0, 1.0)) return FALSE;
				DefParm.DR_CoeffSet = TRUE;
				break;
			}
		case T_AMB_COEFF:
			{
				if (!ReadDouble(DefParm.Amb_Coeff))	return FALSE;
				if (!RangeCheck(DefParm.Amb_Coeff, 0.0, 1.0)) return FALSE;
				DefParm.Amb_CoeffSet = TRUE;
				break;
			}
		case T_SHININESS:
			{
				if (!ReadInteger(DefParm.Shininess))	return FALSE;
				if (!CheckGTEZ(DefParm.Shininess))		return FALSE;
				DefParm.ShininessSet = TRUE;
				break;
			}
		case T_REFLECTION:
			{
				if (!ReadColour(DefParm.Reflection))	return FALSE;
				DefParm.ReflectionSet = TRUE;
				break;
			}
		case T_TRANSPARENCY:
			{
				if (!ReadColour(DefParm.Transparency))	return FALSE;
				DefParm.TransparencySet = TRUE;
				break;
			}
		case T_REFRACTIVE_INDEX:
			{
				if (!ReadDouble(DefParm.Refractive_Index))	return FALSE;
				if (!CheckGTZ(DefParm.Refractive_Index))	return FALSE;
				DefParm.Refractive_IndexSet = TRUE;
				break;
			}
		default:
			return FALSE;
			break;
		}

		Token = GetNextToken();

		if (Token == END_OF_INPUT) return FALSE;
	}

	return TRUE;

}

void CFileParser::ApplyDefaultParameters(DefaultParameters Settings, CSceneObject *Object)
{
	if (Settings.ColourSet)				Object->Colour				= Settings.Colour;
	if (Settings.DR_CoeffSet)			Object->DR_Coeff			= Settings.DR_Coeff;
	if (Settings.SR_CoeffSet)			Object->SR_Coeff			= Settings.SR_Coeff;
	if (Settings.ShininessSet)			Object->Shininess			= Settings.Shininess;
	if (Settings.ReflectionSet)			Object->Reflection			= Settings.Reflection;
	if (Settings.TransparencySet)		Object->Transparency		= Settings.Transparency;
	if (Settings.Refractive_IndexSet)	Object->Refractive_Index	= Settings.Refractive_Index;
	if (Settings.Amb_CoeffSet)			Object->Amb_Coeff			= Settings.Amb_Coeff;
}

BOOL CFileParser::CheckColourValid(CColour Colour)
{
	float r = Colour.Get_R(), g = Colour.Get_G(), b = Colour.Get_B();
	
	return (RangeCheck(r, 0.0f, 1.0f) && 
		    RangeCheck(g, 0.0f, 1.0f) &&
		    RangeCheck(b, 0.0f, 1.0f));
}

BOOL CFileParser::RangeCheck(float value, float min, float max)
{
	return ((value >= min) && (value <= max));
}


BOOL CFileParser::RangeCheck(double value, double min, double max)
{
	return ((value >= min) && (value <= max));
}

BOOL CFileParser::CheckGTZ(double value)
{
	return (value > 0.0);
}

BOOL CFileParser::CheckGTZ(int value)
{
	return (value > 0);
}

BOOL CFileParser::CheckGTEZ(int value)
{
	return (value >= 0);
}

BOOL CFileParser::CheckGTEZ(double value)
{
	return (value >= 0.0);
}
