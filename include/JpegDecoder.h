//
// Class to load images from jpeg files
//
// Author: Alexey V. Boreskov <steps3d@gmail.com>, <steps3d@narod.ru>
//

#ifndef	__JPEG_DECODER__
#define	__JPEG_DECODER__

#include	"TextureDecoder.h"

class	JpegDecoder : public TextureDecoder
{
public:
	JpegDecoder () : TextureDecoder () {}

	virtual	TexImage * load ( Data * data );
};

#endif
