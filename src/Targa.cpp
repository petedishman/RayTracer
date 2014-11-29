// Targa.cpp: implementation of the CTarga class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Targa.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTarga::CTarga()
{
	// Set all the header info to 0 first
	memset(&Header_Info, 0, sizeof(Header_Info));

	// Set up the bits that won't change

	Header_Info.BitDepth = 24;  // 24 bit, will be shown as 0x18
	Header_Info.ImageType = 2;  // 24 bit uncompressed image

	canvas_created = false;
	canvas = NULL;
}

CTarga::~CTarga()
{
	if (canvas)
		delete[] canvas;

}

bool CTarga::Write_Targa(const TCHAR *filename)
{
	// Warning, this function won't perform any filename checking at all
	// and it won't even check to see if the filename already exists.
	// it will happily write over any filename you give it.


	if (!canvas_created)
		return false;

	// Let's open the file for output, binary, and replace mode (implied)
	outfile.open(filename, std::ios::out | std::ios::binary);

	if (!outfile)
	{
		// couldn't open the file for some reason so quit
		return false;
	}

	// get width and height down to 2 bytes long, caller is responsible for
	// ensuring this is possible with no loss
	BYTE hi, lo;

	// do the width first
	lo = width;
	hi = width >> 8;
	Header_Info.Width_lo = lo;
	Header_Info.Width_hi = hi;

	lo = height;
	hi = height >> 8;
	Header_Info.Height_lo = lo;
	Header_Info.Height_hi = hi;

	outfile.write((const char*) &Header_Info, sizeof(Header_Info));


	// that's all the header info done, now we want to write the image data

	outfile.write((const char *)canvas, width * height * 3);

	outfile.close();

	return true;

}

void CTarga::create_canvas(int w, int h, BYTE bg_red, BYTE bg_green, BYTE bg_blue)
{
	// allocate mem for the image, stored as bytes so need the 3 there
	canvas = new BYTE [w*h*3];

	canvas_created = true;

	width = w;
	height = h;

	// now set the background color

	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			Write_Pixel(x,y,bg_red, bg_green, bg_blue);
		}
	}


}

void CTarga::Write_Pixel(int x, int y, BYTE red, BYTE green, BYTE blue)
{
	canvas[3*(x+y*width)    ] = blue;
	canvas[3*(x+y*width) + 1] = green;
	canvas[3*(x+y*width) + 2] = red;
}
