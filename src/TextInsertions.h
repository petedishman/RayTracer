// TextInsertions.h

// This file will just contain some strings that are to be used
// by the insert menu for inserting default text in to a scene file
// only the view class is probably going to need it.

// It's in a separate file just to keep things tidy.


// NOTE: These strings are to be put in to a windows CEditCtrl therefore
//       they need to use \r\n in order to give a newline

const CString strGlobalSettings = 
"Width  = 400\r\n\
Height = 400\r\n\
\r\n\
FOV = 30.0\r\n\
\r\n\
Background = (0.0, 0.0, 0.0)\r\n\
Fog_Distance = 500.0\r\n\
\r\n\
Ambient_Colour = (1.0, 1.0, 1.0)\r\n\
\r\n\
FocalLength = 100.0\r\n\
LensRadius  = 1.0\r\n\
\r\n\
\r\n\
Camera\r\n\
 Begin\r\n\
  Pos     = (0.0, 0.0, -100.0)\r\n\
  Heading = (0.0, 0.0, 1.0)\r\n\
  Up      = (0.0, 1.0, 0.0)\r\n\
 End\r\n\
\r\n\
\r\n";

const CString strCamera = 
"Camera\r\n\
 Begin\r\n\
  Pos     = (0.0, 0.0, -100.0)\r\n\
  Heading = (0.0, 0.0, 1.0)\r\n\
  Up      = (0.0, 1.0, 0.0)\r\n\
 End\r\n\
\r\n\
\r\n";

const CString strLight = 
"Light\r\n\
 Begin\r\n\
  Pos    = (0.0, 20.0, -100.0)\r\n\
  Colour = (1.0, 1.0, 1.0)\r\n\
 End\r\n\
\r\n\
\r\n";

const CString strSphere = 
"Sphere\r\n\
 Begin\r\n\
  Centre       = (0.0, 0.0, 0.0)\r\n\
  Radius       = 10.0\r\n\
  Colour       = (1.0, 0.0, 0.0)\r\n\
  DR_Coeff     = 0.8\r\n\
  Amb_Coeff    = 0.2\r\n\
  SR_Coeff     = 0.7\r\n\
  Shininess    = 10\r\n\
  Reflection   = (0.0, 0.0, 0.0)\r\n\
  Transparency = (0.0, 0.0, 0.0)\r\n\
  Refractive_Index = 1.0\r\n\
 End\r\n\
\r\n\
\r\n";

const CString strInfinitePlane = 
"Infinite_Plane\r\n\
 Begin\r\n\
  Plane_Normal = (0.0, 1.0, 0.0)\r\n\
  Distance     = 10.0\r\n\
  Colour       = (1.0, 1.0, 0.0)\r\n\
  DR_Coeff     = 0.8\r\n\
  Amb_Coeff    = 0.2\r\n\
  SR_Coeff     = 0.7\r\n\
  Shininess    = 10\r\n\
  Reflection   = (0.0, 0.0, 0.0)\r\n\
  Transparency = (0.0, 0.0, 0.0)\r\n\
  Refractive_Index = 1.0\r\n\
 End\r\n\
\r\n\
\r\n";

const CString strBoundedPlane = 
"Bounded_Plane\r\n\
 Begin\r\n\
  Length       = 40.0\r\n\
  Width        = 20.0\r\n\
  Colour       = (1.0, 1.0, 0.0)\r\n\
  DR_Coeff     = 0.8\r\n\
  Amb_Coeff    = 0.2\r\n\
  SR_Coeff     = 0.7\r\n\
  Shininess    = 10\r\n\
  Reflection   = (0.0, 0.0, 0.0)\r\n\
  Transparency = (0.0, 0.0, 0.0)\r\n\
  Refractive_Index = 1.0\r\n\
 End\r\n\
\r\n\
\r\n";

const CString strCheckerBoard = 
"CheckerBoard\r\n\
 Begin\r\n\
  Plane_Normal = (0.0, 1.0, 0.0)\r\n\
  Distance     = 10.0\r\n\
  Colour_One   = (1.0, 0.0, 0.0)\r\n\
  Colour_Two   = (1.0, 1.0, 0.0)\r\n\
  CheckSize    = 10.0\r\n\
  DR_Coeff     = 0.8\r\n\
  Amb_Coeff    = 0.2\r\n\
  SR_Coeff     = 0.7\r\n\
  Shininess    = 10\r\n\
  Reflection   = (0.0, 0.0, 0.0)\r\n\
  Transparency = (0.0, 0.0, 0.0)\r\n\
  Refractive_Index = 1.0\r\n\
 End\r\n\
\r\n\
\r\n";


const CString strBoundedCheckerBoard = 
"Bounded_CheckBoard\r\n\
 Begin\r\n\
  Length       = 40.0\r\n\
  Width	       = 25.0\r\n\
  Colour_One   = (1.0, 0.0, 0.0)\r\n\
  Colour_Two   = (1.0, 1.0, 0.0)\r\n\
  CheckSize    = 10.0\r\n\
  DR_Coeff     = 0.8\r\n\
  Amb_Coeff    = 0.2\r\n\
  SR_Coeff     = 0.7\r\n\
  Shininess    = 10\r\n\
  Reflection   = (0.0, 0.0, 0.0)\r\n\
  Transparency = (0.0, 0.0, 0.0)\r\n\
  Refractive_Index = 1.0\r\n\
 End\r\n\
\r\n\
\r\n";

const CString strCylinder = 
"Cylinder\r\n\
 Begin\r\n\
  Centre       = (0.0, 0.0, 0.0)\r\n\
  Axis         = (1.0, 0.0, 0.2)\r\n\
  Length       = 20.0\r\n\
  Radius       = 5.0\r\n\
  Colour       = (1.0, 1.0, 0.0)\r\n\
  DR_Coeff     = 0.8\r\n\
  Amb_Coeff    = 0.2\r\n\
  SR_Coeff     = 0.7\r\n\
  Shininess    = 10\r\n\
  Reflection   = (0.0, 0.0, 0.0)\r\n\
  Transparency = (0.0, 0.0, 0.0)\r\n\
  Refractive_Index = 1.0\r\n\
 End\r\n\
\r\n\
\r\n";




