//
// Abstract class to load texture images from specific file formats
//
// Author: Alexey V. Boreskov <steps3d@gmail.com>, <steps3d@narod.ru>
//

#pragma once
#ifndef	__TEXTURE_DECODER__
#define	__TEXTURE_DECODER__

class	Data;
class	TexImage;

class	TextureDecoder
{
public:
	TextureDecoder () {}
	virtual ~TextureDecoder () {}

	virtual	TexImage * load ( Data * data ) = 0;
};

#endif
