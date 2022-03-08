//
// Class to load images from PNG files
//
// Author: Alexey V. Boreskov <steps3d@gmail.com>, <steps3d@narod.ru>
//

#pragma once
#ifndef	__PNG_DECODER__
#define	__PNG_DECODER__

#include	"TextureDecoder.h"

class	PngDecoder : public TextureDecoder
{
public:
	PngDecoder () : TextureDecoder () {}

	virtual	TexImage * load ( Data * data );
};

#endif
