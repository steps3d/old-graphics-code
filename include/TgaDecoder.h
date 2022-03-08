//
// Class to load images from TGA files
//
// Author: Alexey V. Boreskov <steps3d@gmail.com>, <steps3d@narod.ru>
//

#ifndef	__TGA_DECODER__
#define	__TGA_DECODER__

#include	"TextureDecoder.h"
#include	"TypeDefs.h"

class	TgaDecoder : public TextureDecoder
{
public:
	TgaDecoder () : TextureDecoder () {}

	virtual	TexImage * load ( Data * data );

private:
	bool	loadRGB8  ( Data * data, TexImage * tex, dword * buf, dword palette [], bool vertInvert );
	bool	loadRGB15 ( Data * data, TexImage * tex, dword * buf, bool vertInvert );
	bool	loadRGB24 ( Data * data, TexImage * tex, dword * buf, bool vertInvert );
	bool	loadRGB32 ( Data * data, TexImage * tex, dword * buf, bool vertInvert );
	bool	loadRLE8  ( Data * data, TexImage * tex, dword * buf, dword palette [], bool vertInvert );
	bool	loadRLE15 ( Data * data, TexImage * tex, dword * buf, bool vertInvert );
	bool	loadRLE24 ( Data * data, TexImage * tex, dword * buf, bool vertInvert );
	bool	loadRLE32 ( Data * data, TexImage * tex, dword * buf, bool vertInvert );
};

#endif
