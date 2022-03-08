//
// Class to save images to TGA files
//
// Author: Alexey V. Boreskov <steps3d@gmail.com>, <steps3d@narod.ru>
//

#pragma once
#ifndef	__TGA_ENCODER__
#define	__TGA_ENCODER__

#include	"TypeDefs.h"

class	TgaEncoder
{
public:
	TgaEncoder () {}

	bool	encode ( byte * image, int w, int h, int numComponents, const char * fileName ) const;
};

#endif
