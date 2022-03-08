//
// Class to load images from BMP files
//
// Author: Alexey V. Boreskov <steps3d@gmail.com>, <steps3d@narod.ru>
//

#pragma once
#ifndef	__BMP_DECODER__
#define	__BMP_DECODER__

#include	"TextureDecoder.h"
#include	"TypeDefs.h"

class	BmpDecoder : public TextureDecoder
{
public:
	BmpDecoder () : TextureDecoder () {}

	virtual	TexImage * load ( Data * data );

private:
	bool	loadMono  ( Data * data, TexImage * tex, dword * buf, dword palette [] );
	bool	loadRGB4  ( Data * data, TexImage * tex, dword * but, dword palette [] );
	bool	loadRGB8  ( Data * data, TexImage * tex, dword * buf, dword palette [] );
	bool	loadRGB24 ( Data * data, TexImage * tex, dword * buf );
	bool	loadRGB32 ( Data * data, TexImage * tex, dword * buf );
	bool	loadRLE4  ( Data * data, TexImage * tex, dword * buf, dword palette [] );
	bool	loadRLE8  ( Data * data, TexImage * tex, dword * buf, dword palette [] );
};

#endif
