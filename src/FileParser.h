// FileParser.h: interface for the CFileParser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEPARSER_H__8FBB5B56_F6CE_48BE_B44C_58564E06B041__INCLUDED_)
#define AFX_FILEPARSER_H__8FBB5B56_F6CE_48BE_B44C_58564E06B041__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////
//
// class   CFileParser
//
/////////////////////////////////////

// constant defines used in lexing/parsing

// used to mark the end of the input string
#define END						0

// lexing tokens	
#define T_CAMERA				256
#define T_BEGIN					257
#define T_END					258
#define T_POS					259
#define T_UP					260
#define T_HEADING				261
#define T_FOV					262
#define T_BACKGROUND			263
#define T_WIDTH					264
#define T_HEIGHT				265
#define T_LIGHT					266
#define T_COLOUR				267
#define T_SPHERE				268
#define T_CENTRE				269
#define T_RADIUS				270
#define T_SR_COEFF				271
#define T_DR_COEFF				272
#define T_SHININESS				273
#define T_REFLECTION			274
#define T_TRANSPARENCY			275
#define T_REFRACTIVE_INDEX		276
#define T_BRA					277
#define T_KET					278
#define T_SEMICOLON				279
#define T_EQUALS				280
#define T_COMMA					281
#define T_NUMBER				282
#define T_AMBIENTCOLOUR			283
#define T_AMB_COEFF				284
#define T_FOCALLENGTH			285
#define T_LENSRADIUS			286
#define T_LA_A					287	// light attenuation factor c1
#define T_LA_B					288	// light attenuation factor c2
#define T_LA_C					289	// light attenuation factor c3
#define T_INFINITEPLANE			290
#define T_PLANENORMAL			291
#define T_DISTANCE				292
#define T_COLOUR1				293
#define T_COLOUR2				294
#define T_CHECKSIZE				295
#define T_CHECKERBOARD			296
#define T_CYLINDER				297
#define T_LENGTH				298
#define T_AXIS					299
#define T_BOUNDEDPLANE			300
#define T_BOUNDEDCHECKBOARD		301
#define T_FOGDISTANCE			302


#define T_UNKNOWN				500
#define END_OF_INPUT			501


// Parser modes
#define M_CAMERA			600
#define M_FOV				601
#define M_BACKGROUND		602
#define M_WIDTH				603
#define M_HEIGHT			604
#define M_LIGHT				605
#define M_SPHERE			606
#define M_NULL				607
#define M_AMBIENT			608
#define M_FOCALLENGTH		609
#define M_LENSRADIUS		610
#define M_INFINITEPLANE		611
#define M_CHECKERBOARD		612
#define M_CYLINDER			613
#define M_BOUNDEDPLANE		614
#define M_BOUNDEDCHECKBOARD 615
#define M_FOGDISTANCE		616

// Parser Exit status
#define PARSER_SUCCESS	1
#define PARSER_FAILURE	0

typedef struct
{
	CColour Colour;				BOOL	ColourSet;
	double SR_Coeff;			BOOL	SR_CoeffSet;
	double DR_Coeff;			BOOL	DR_CoeffSet;
	double Amb_Coeff;			BOOL	Amb_CoeffSet;
	CColour Reflection;			BOOL	ReflectionSet;
	CColour Transparency;		BOOL	TransparencySet;
	double Refractive_Index;	BOOL	Refractive_IndexSet;
	int Shininess;				BOOL	ShininessSet;
} DefaultParameters;

class CWorld;

class CFileParser
{
public:
	CFileParser();
	CFileParser(CWorld* ptr);
	~CFileParser();
	CWorld *WorldPtr;

	int ParseInput(CString *source);

	int ErrorAtIndex;

private:
	BOOL CheckGTEZ(double value);
	BOOL CheckGTEZ(int value);
	BOOL CheckGTZ(int value);
	BOOL CheckGTZ(double value);
	BOOL RangeCheck(double value, double min, double max);
	BOOL RangeCheck(float value, float min, float max);
	BOOL CheckColourValid(CColour Colour);
	void ApplyDefaultParameters(DefaultParameters Settings, CSceneObject* Object);
	BOOL ReadDefaultParameters(DefaultParameters& DefParm);
	BOOL ReadVector(CVector& Vector);
	BOOL ReadFloat(float& Value);
	BOOL ReadDouble(double& Value);
	BOOL ReadColour(CColour& Colour);
	BOOL ReadInteger(int& Value);
	CString input;
	CString Number;
	int index;
	int length;
	TCHAR GetNextChar();
	void SkipComment();
	TCHAR LookAtNextChar();
	int GetNextToken();
	int Parse_Error();
	int NoOfLights;
	int NoOfObjects;
};

#endif // !defined(AFX_FILEPARSER_H__8FBB5B56_F6CE_48BE_B44C_58564E06B041__INCLUDED_)
