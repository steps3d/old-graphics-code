//
// Basic description of the OpenGL texture format
//
// Author: Alexey V. Boreskov, <steps3d@narod.ru>, <steps3s@gmail.com>
//

#pragma once
#ifndef	__TEX_FORMAT__
#define	__TEX_FORMAT__

#include	<string>
#include	<GL/glew.h>
#include	"gl3.h"

using namespace std;

class Texture;

class	TexFormat
{
	GLenum	format;
	GLenum	internalFormat;
	int		numComponents;
	int		bitsPerChannel;
	
	TexFormat ();
	
public:
	TexFormat ( GLenum theFormat, GLenum theIntFormat );
	
	GLenum	getFormat () const
	{
		return format;
	}
	
	GLenum	getInternalFormat () const
	{
		return internalFormat;
	}
	
	int	getNumComponents () const
	{
		return numComponents;
	}
	
	bool	isOk () const
	{
		return numComponents > 0 && numComponents < 5;
	}
	
	bool	isPacked     () const;		// whether it is a bit-packet format
	bool	isCompressed () const;	// whether it is a compressed (DXTC or RGTC) format
	bool	isColor      () const;
	bool	isColorFloat () const;
	bool	isColorInt   () const;
	bool	isColorUint  () const;
	
	bool	isColorNormalized       () const;
	bool	isColorSignedNormalized () const;
		
	bool	isDepth () const
	{
		return format == GL_DEPTH_COMPONENT;
	}
		
	bool	isDepthStencil () const
	{
		return format == GL_DEPTH_STENCIL;
	}
		
	string	description () const;	// get text description of the format
	
									// create specific color formats
	static TexFormat intFormat   ( int theNumChannels, int theNumBits = 8 );
	static TexFormat uintFormat  ( int theNumChannels, int theNumBits = 8 );
	static TexFormat floatFormat ( int theNumChannels, int theNumBits = 16 );
	static TexFormat normFormat  ( int theNumChannels, int theNumBits = 8 );
	static TexFormat snormFormat ( int theNumChannels, int theNumBits = 8 );
	static TexFormat srgbFormat  ();
	static TexFormat srgbaFormat ();
	
									// create specific depth formats
	static TexFormat depthNormalized ( int theNumBits );
	static TexFormat depthFloat     ();
	
									// create specific packed depth-stencil formats
	static TexFormat depthStenil24_8 ();
	static TexFormat depthStencilFloat ();
	
	friend class Texture;
};

#endif
