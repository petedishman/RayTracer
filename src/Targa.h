// Targa.h: interface for the CTarga class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TARGA_H__80BD295B_0830_448E_A710_404649FB5BAB__INCLUDED_)
#define AFX_TARGA_H__80BD295B_0830_448E_A710_404649FB5BAB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// includes for the targa file class

#include <iostream>
#include <fstream>
#include <memory.h>


// File structs for the targa format

typedef unsigned char BYTE;

struct TGA_Header
{
	BYTE offset00;
	BYTE offset01;
	BYTE ImageType;
	BYTE offset03;
	BYTE offset04;
	BYTE offset05;
	BYTE offset06;
	BYTE offset07;
	BYTE offset08;
	BYTE offset09;
	BYTE offset10;
	BYTE offset11;
	BYTE Width_lo;
	BYTE Width_hi;
	BYTE Height_lo;
	BYTE Height_hi;
	BYTE BitDepth;
	BYTE offset17;
};




class CTarga
{
public:
	void Write_Pixel(int x, int y, BYTE red, BYTE green, BYTE blue);
	void create_canvas(int w, int h, BYTE bg_red, BYTE bg_green, BYTE bg_blue);
	bool Write_Targa(const TCHAR* filename);
	CTarga();
	virtual ~CTarga();

private:
	BYTE temp_red;
	BYTE temp_green;
	BYTE temp_blue;
	TGA_Header Header_Info;

	int height;
	int width;
	BYTE* canvas;
	bool canvas_created;

	std::ofstream outfile;
};

#endif // !defined(AFX_TARGA_H__80BD295B_0830_448E_A710_404649FB5BAB__INCLUDED_)
