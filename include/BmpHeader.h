//
// Windows BMP file header definition
//

#pragma once
#ifndef	__BMP_HEADER__
#define	__BMP_HEADER__

#include	"TypeDefs.h"

										// BMP specific constants and structs
#undef	BI_RGB
#undef	BI_RLE8
#undef	BI_RLE4

#define BI_RGB  0
#define BI_RLE8 1
#define BI_RLE4 2

#pragma pack (push, 1)					// save current pack, set 1-byte packing

										// structs of BMP file
struct  BmpHeader
{
	uint16	type;                   	// type of file, must be 'BM'
	dword	size;                   	// size of file in bytes
    uint16	reserved1, reserved2;
	dword	offBits;                	// offset from this header to actual data
};

struct  BmpInfoHeader
{
	dword	size;                   	// sizeof of this header
	dword	width;                  	// width of bitmap in pixels
	dword	height;                 	// height of bitmap in pixels
	uint16	planes;                 	// # of planes
	uint16	bitCount;               	// bits per pixel
	dword	compression;            	// type of compression, BI_RGB - no compression
	dword	sizeImage;              	// size of image in bytes
	dword	xPelsPerMeter;          	// hor. resolution of the target device
	dword	yPelsPerMeter;          	// vert. resolution
	dword	clrUsed;
	dword	clrImportant;
};

struct  RGBQuad
{
	byte	blue;
	byte	green;
	byte	red;
	byte	reserved;
};

#pragma	pack (pop)

#endif
