//
// Class to load images from DDS files
//
// Author: Alexey V. Boreskov <steps3d@gmail.com>, <steps3d@narod.ru>
//

#pragma once
#ifndef	__DDS_DECODER__
#define	__DDS_DECODER__

#include	"TextureDecoder.h"

struct	DDS_HEADER;

class	DdsDecoder : public TextureDecoder
{
public:
	DdsDecoder () : TextureDecoder () {}

	virtual	TexImage * load  ( Data * data );

protected:
	TexImage * loadCubemap ( Data * data, const DDS_HEADER& ddsd );
	TexImage * load3D      ( Data * data, const DDS_HEADER& ddsd );
	
	bool	loadUncompressed8BitRGBData   ( Data * data, const DDS_HEADER& ddsd, int component, int bytesPerLine, byte * ptr );
	bool	loadUncompressed8BitAlphaData ( Data * data, const DDS_HEADER& ddsd, int bytesPerLine, byte * ptr );
	bool	loadUncompressed16BitRGBData  ( Data * data, const DDS_HEADER& ddsd, int bytesPerLine, byte * ptr );
	
	bool 	loadUncompressed          ( Data * data, const DDS_HEADER& ddsd, int numComponents, byte * image );
	TexImage * loadUncompressed8BitRGB   ( Data * data, const DDS_HEADER& ddsd, int component );
	TexImage * loadUncompressed8BitAlpha ( Data * data, const DDS_HEADER& ddsd );
	TexImage * loadUncompressed16BitRGB  ( Data * data, const DDS_HEADER& ddsd );
	void	  skipMipmaps               ( Data * data, int w, int h, int bytesPerLine, int cnt, int mipmapsCount );
};

#endif
