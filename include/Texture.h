//
// Texture wrapper class for OpenGL 3.3 and higher
//
// Author: Alexey V. Boreskov
//

#pragma once
#ifndef __TEXTURE_WRAPPER__
#define __TEXTURE_WRAPPER__

#include    <GL/glew.h>
#include    <string>
#include	"TexFormat.h"

using namespace std;

class	TexImage;
class	vec4;

class	Texture
{
	GLuint		id;
	GLenum		target;
	TexFormat	fmt;
	int			width, height, depth;
	bool		cubemap;					// is it a cubemap
	bool		compressed;					// is it compressed
	bool		autoMipmaps;				// load mipmaps or create if cannot load
	int		unit;					// texture unit texture is bound to
	
								// disable copying
	Texture ( const Texture& );
	Texture& operator = ( const Texture& );
	
public:
	Texture  ( bool theAutoMipmaps = true );
	~Texture ();
	
	bool	isOk () const
	{
		return id != 0 && target != 0 && width > 0 && height > 0 && depth > 0;
	}
	
	bool	getAutoMipmaps () const
	{
		return autoMipmaps;
	}
	
	int	getId () const
	{
		return id;
	}
	
	int	getWidth () const
	{
		return width;
	}
	
	int	getHeight () const
	{
		return height;
	}
	
	int	getDepth () const
	{
		return depth;
	}
	
	bool	isCubemap () const
	{
		return cubemap;
	}
	
	bool	isCompressed () const
	{
		return compressed;
	}
	
	GLenum	getTarget () const
	{
		return target;
	}

	const TexFormat& getFormat () const
	{
		return fmt;
	}
	
	void	destroy ();
							// create empty texture with given properties
	bool	create1D        ( int theWidth, GLenum fmt, GLenum intFmt );
	bool	create2D        ( int theWidth, int theHeight, GLenum fmt, GLenum intFmt );
	bool	createRectangle ( int theWidth, int theHeight, GLenum fmt, GLenum intFmt );
	bool	create3D        ( int theWidth, int theHeight, int theDepth, GLenum fmt, GLenum intFmt );
	bool	createCubemap   ( int theWidth, GLenum fmt, GLenum intFmt );
	bool	createDepthRect ( int theWidth, int theHeight, GLenum fmt = GL_DEPTH_COMPONENT, GLenum intFmt = GL_DEPTH_COMPONENT );
	
							// create empty texture arrays
	bool	createArray1D ( int theWidth, int numSlices, GLenum fmt, GLenum intFmt );
	bool	createArray2D ( int theWidth, int theHeight, int numSlices, GLenum fmt, GLenum intFmt );
	
	bool	load1D        ( const char * fileName );
	bool	load2D        ( const char * fileName );
	bool	loadRectangle ( const char * fileName );
	bool	load3D        ( const char * fileName );
	bool	loadCubemap   ( const char * fileName );
	bool	loadCubemap   ( const char * f1, const char * f2, const char * f3, const char * f4, const char * f5, const char * f6 );
	
							// load cubemap from a list of , or ; separated list of names
	bool	loadCubemapFromNames ( const char * fileNames );
	
	bool	load1DRaw   ( int theWidth, GLenum fmt, GLenum intFmt, GLenum dataType, const void * theData );
	bool	load2DRaw   ( int theWidth, int theHeight, GLenum fmt, GLenum intFmt, GLenum dataType, const void * theData );
	bool	loadRectRaw ( int theWidth, int theHeight, GLenum fmt, GLenum intFmt, GLenum dataType, const void * theData );
	bool	load3DRaw   ( int theWidth, int theHeight, int theDepth, GLenum fmt, GLenum intFmt, GLenum dataType, const void * theData );
	
	bool	saveAsTga   ( const char * fileName );
	
	void	bind   ( int texUnit = 0 );
	void	unbind ();
	
	void	buildMipmaps  ();
	void	setSwizzle     ( GLenum red, GLenum green, GLenum blue, GLenum alpha );
	void	setBaseLevel   ( int level );	// ??? add ivar and getter ???
	void	setMaxLevel    ( int level );
	void	setBorderColor ( const vec4& color );
	
	static int	maxSize ();

protected:
	void	setParamsFromTexImage ( TexImage * image, GLenum theTarget );

};

#endif
