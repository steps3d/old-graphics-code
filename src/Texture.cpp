//
// Texture wrapper class for OpenGL 3.3 and higher
// Requires DevIL
//
// Author: Alexey V. Boreskov
//

#include    <stdlib.h>
#include	"Texture.h"
#include	"TexImage.h"
#include	"Resources.h"
#include	"glUtilities.h"
#include	"stringUtils.h"
#include	"TgaEncoder.h"
#include	"vec4.h"

Texture :: Texture ( bool theAutoMipmaps )
{
	id          = 0;
	width       = 0;
	height      = 0;
	depth       = 0;
	target      = 0;
	cubemap     = false;
	compressed  = false;
	autoMipmaps = theAutoMipmaps;
	unit        = -1;
}

Texture :: ~Texture ()
{
	destroy ();
}

void	Texture :: destroy ()
{
	if ( id == 0 )
		return;

	glDeleteTextures ( 1, &id );

	id          = 0;
	width       = 0;
	height      = 0;
	depth       = 0;
	target      = 0;
	cubemap     = false;
	compressed  = false;
	unit        = -1;
}

bool	Texture :: create1D ( int theWidth, GLenum theFormat, GLenum theIntFmt )		// use TexFormat as input
{
	target         = GL_TEXTURE_1D;
	width          = theWidth;
	fmt            = TexFormat ( theFormat, theIntFmt );
	height         = 1;
	depth          = 1;	
	
	glGenTextures   ( 1, &id );
    glBindTexture   ( target, id );
    glPixelStorei   ( GL_UNPACK_ALIGNMENT, 1 );                  // set 1-byte alignment
    glTexParameteri ( target, GL_TEXTURE_WRAP_S, GL_REPEAT );    // set default params for texture
    glTexParameteri ( target, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri ( target, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri ( target, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexImage1D    ( target, 0, getFormat ().getInternalFormat (), width, 0, getFormat ().getFormat (), GL_UNSIGNED_BYTE, NULL );
    glBindTexture   ( target, 0 );

	return true;
}

bool	Texture :: create2D ( int theWidth, int theHeight, GLenum theFormat, GLenum theIntFmt )
{
	target         = GL_TEXTURE_2D;
	fmt            = TexFormat ( theFormat, theIntFmt );
	width          = theWidth;
	height         = theHeight;
	depth          = 1;	
	
	glGenTextures   ( 1, &id );
    glBindTexture   ( target, id );
    glPixelStorei   ( GL_UNPACK_ALIGNMENT, 1 );                  // set 1-byte alignment
    glTexParameteri ( target, GL_TEXTURE_WRAP_S, GL_REPEAT );    // set default params for texture
    glTexParameteri ( target, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri ( target, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri ( target, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexImage2D    ( target, 0, getFormat ().getInternalFormat (), width, height, 0, getFormat ().getFormat (), GL_UNSIGNED_BYTE, NULL );
    glBindTexture   ( target, 0 );

	return true;
}

bool	Texture :: createRectangle ( int theWidth, int theHeight, GLenum theFormat, GLenum theIntFmt )
{
	target         = GL_TEXTURE_RECTANGLE;
	fmt            = TexFormat ( theFormat, theIntFmt );
	width          = theWidth;
	height         = theHeight;
	depth          = 1;	
	autoMipmaps    = false;
	
	glGenTextures   ( 1, &id );
    glBindTexture   ( target, id );
    glTexParameteri ( target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );     // set default params for texture
    glTexParameteri ( target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    glPixelStorei   ( GL_UNPACK_ALIGNMENT, 1 );                  // set 1-byte alignment
    glTexImage2D    ( target, 0, getFormat ().getInternalFormat (), width, height, 0, getFormat ().getFormat (), GL_UNSIGNED_BYTE, NULL );
    glBindTexture   ( target, 0 );

	return true;
}

bool	Texture :: createDepthRect ( int theWidth, int theHeight, GLenum theFormat, GLenum theIntFmt )
{
	target         = GL_TEXTURE_RECTANGLE;
	fmt            = TexFormat ( theFormat, theIntFmt );
	width          = theWidth;
	height         = theHeight;
	depth          = 1;	
	autoMipmaps    = false;
	
	glGenTextures   ( 1, &id );
    glBindTexture   ( target, id );
    glTexParameteri ( target, GL_TEXTURE_WRAP_S, GL_CLAMP );     // set default params for texture
    glTexParameteri ( target, GL_TEXTURE_WRAP_T, GL_CLAMP );
	glTexParameteri ( GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri ( GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexImage2D    ( target, 0, getFormat ().getInternalFormat (), width, height, 0, getFormat ().getFormat (), GL_UNSIGNED_BYTE, NULL );
    glBindTexture   ( target, 0 );

	return true;
}

bool	Texture :: create3D ( int theWidth, int theHeight, int theDepth, GLenum theFormat, GLenum theIntFmt )
{
	target         = GL_TEXTURE_3D;
	fmt            = TexFormat ( theFormat, theIntFmt );
	width          = theWidth;
	height         = theHeight;
	depth          = theDepth;	
	
	glGenTextures   ( 1, &id );
    glBindTexture   ( target, id );
    glPixelStorei   ( GL_UNPACK_ALIGNMENT, 1 );                         // set 1-byte alignment
    glTexParameteri ( target, GL_TEXTURE_WRAP_S, GL_REPEAT );    // set default params for texture
    glTexParameteri ( target, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri ( target, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri ( target, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexImage3D    ( target, 0, getFormat ().getInternalFormat (), width, height, depth, 0, getFormat ().getFormat (), GL_UNSIGNED_BYTE, NULL );
    glBindTexture   ( target, 0 );

	return true;
}

bool	Texture :: createCubemap ( int theWidth, GLenum theFormat, GLenum theIntFmt )
{
	target         = GL_TEXTURE_2D;
	fmt            = TexFormat ( theFormat, theIntFmt );
	width          = theWidth;
	height         = theWidth;
	depth          = 1;	
	
	glGenTextures   ( 1, &id );
    glBindTexture   ( target, id );
    glPixelStorei   ( GL_UNPACK_ALIGNMENT, 1 );                         // set 1-byte alignment
    glTexParameteri ( target, GL_TEXTURE_WRAP_S, GL_REPEAT );    // set default params for texture
    glTexParameteri ( target, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri ( target, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri ( target, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexImage2D    ( target, 0, getFormat ().getInternalFormat (), width, height, 0, getFormat ().getFormat (), GL_UNSIGNED_BYTE, NULL );
    glBindTexture   ( target, 0 );

	return true;
}

									// create empty texture arrays
bool	Texture :: createArray1D ( int theWidth, int numSlices, GLenum theFormat, GLenum theIntFmt )
{
	target         = GL_TEXTURE_1D_ARRAY;
	fmt            = TexFormat ( theFormat, theIntFmt );
	width          = theWidth;
	height         = numSlices;
	depth          = 1;	
	
	glGenTextures   ( 1, &id );
    glBindTexture   ( target, id );
    glPixelStorei   ( GL_UNPACK_ALIGNMENT, 1 );                         // set 1-byte alignment
    glTexParameteri ( target, GL_TEXTURE_WRAP_S, GL_REPEAT );    // set default params for texture
    glTexParameteri ( target, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri ( target, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri ( target, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexImage2D    ( target, 0, getFormat ().getInternalFormat (), width, height, 0, getFormat ().getFormat (), GL_UNSIGNED_BYTE, NULL );
    glBindTexture   ( target, 0 );

	return true;
}

bool	Texture :: createArray2D ( int theWidth, int theHeight, int numSlices, GLenum theFormat, GLenum theIntFmt )
{
	target         = GL_TEXTURE_2D_ARRAY;
	fmt            = TexFormat ( theFormat, theIntFmt );
	width          = theWidth;
	height         = theHeight;
	depth          = numSlices;	
	
	glGenTextures   ( 1, &id );
    glBindTexture   ( target, id );
    glPixelStorei   ( GL_UNPACK_ALIGNMENT, 1 );                         // set 1-byte alignment
    glTexParameteri ( target, GL_TEXTURE_WRAP_S, GL_REPEAT );    // set default params for texture
    glTexParameteri ( target, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri ( target, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri ( target, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexImage3D    ( target, 0, getFormat ().getInternalFormat (), width, height, depth, 0, getFormat ().getFormat (), GL_UNSIGNED_BYTE, NULL );
    glBindTexture   ( target, 0 );

	return true;
}

bool	Texture :: load1D ( const char * fileName )
{
	TexImage * image = loadTexImage ( fileName );
	
	if ( image == NULL )
		return false;
		
	if ( image -> getImageType () != TexImage :: image2D || image -> getHeight () != 1 )
	{
		delete image;
		
		return false;
	}
	
	setParamsFromTexImage ( image, GL_TEXTURE_1D );
	
	glGenTextures   ( 1, &id );
    glBindTexture   ( target, id );
    glPixelStorei   ( GL_UNPACK_ALIGNMENT, 1 );                  // set 1-byte alignment
    glTexParameteri ( target, GL_TEXTURE_WRAP_S, GL_REPEAT );    // set default params for texture

	if ( !image -> isCompressed () )			// not compressed image
	{
		glTexImage1D ( target, 0, getFormat ().getInternalFormat (), width, 0, getFormat ().getFormat (), image -> getGlType (), image -> imageData () );
		
		if ( autoMipmaps )
			glGenerateMipmap ( target );
	}
	else
	{
		int	mipmaps = image -> getNumLevels ();
		int	w       = width;
		int	h       = height;
		byte * ptr  = image -> imageData ( 0,  0 );
		
		glTexParameteri  ( target, GL_TEXTURE_MAX_LEVEL, mipmaps - 1 );
		
		for ( int i = 0; i < mipmaps; i++ )
		{
			int	size = image -> imageDataSize ( 0, i );
			
			glCompressedTexImage1D ( target, i, getFormat ().getInternalFormat (), w, 0, size, ptr );
			
			if ( ( w = w / 2 ) < 1 )
				w = 1;
			
			if ( ( h = h / 2 ) < 1 )
				h = 1;
				
			ptr += size;
		}
	}
	
    if ( autoMipmaps )
    {
        glTexParameteri ( target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
        glTexParameteri ( target, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    }
    else
    {
        glTexParameteri ( target, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri ( target, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    }
	
    glBindTexture  ( target, 0 );

	delete image;
	
	return true;
}

bool	Texture :: load2D ( const char * fileName )
{
	TexImage * image = loadTexImage ( fileName );
	
	if ( image == NULL )
		return false;
		
	if ( image -> getImageType () != TexImage :: image2D )
	{
		delete image;
		
		return false;
	}
	
	setParamsFromTexImage ( image, GL_TEXTURE_2D );
	
	glGenTextures   ( 1, &id );
    glBindTexture   ( target, id );
    glPixelStorei   ( GL_UNPACK_ALIGNMENT, 1 );                         // set 1-byte alignment
    glTexParameteri ( target, GL_TEXTURE_WRAP_S, GL_REPEAT );    // set default params for texture
    glTexParameteri ( target, GL_TEXTURE_WRAP_T, GL_REPEAT );

	if ( !image -> isCompressed () )			// not compressed image
	{
		glTexImage2D ( target, 0, getFormat ().getInternalFormat (), width, height, 0, getFormat ().getFormat (), image -> getGlType (), image -> imageData () );	
		
		if ( autoMipmaps )
			glGenerateMipmap ( target );
	}
	else
	{
		int	mipmaps = image -> getNumLevels ();
		int	w       = width;
		int	h       = height;
		byte * ptr  = image -> imageData ( 0,  0 );
		
		glTexParameteri  ( target, GL_TEXTURE_MAX_LEVEL, mipmaps - 1 );
		
		for ( int i = 0; i < mipmaps; i++ )
		{
			int	size = image -> imageDataSize ( 0, i );
			
			glCompressedTexImage2D ( target, i, getFormat ().getInternalFormat (), w, h, 0, size, ptr );
						
			if ( ( w = w / 2 ) < 1 )
				w = 1;
			
			if ( ( h = h / 2 ) < 1 )
				h = 1;
				
			ptr += size;
		}		
	}
	
    if ( autoMipmaps )
    {
        glTexParameteri ( target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
        glTexParameteri ( target, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    }
    else
    {
        glTexParameteri ( target, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri ( target, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    }
	
    glBindTexture  ( target, 0 );

	delete image;
	
	return true;
}

bool	Texture :: loadRectangle ( const char * fileName )
{
	TexImage * image = loadTexImage ( fileName );
	
	if ( image == NULL )
		return false;
		
	if ( image -> getImageType () != TexImage :: image2D )
	{
		delete image;
		
		return false;
	}
	
	autoMipmaps = false;
	
	setParamsFromTexImage ( image, GL_TEXTURE_RECTANGLE );
	
	glGenTextures   ( 1, &id );
    glBindTexture   ( target, id );
    glPixelStorei   ( GL_UNPACK_ALIGNMENT, 1 );                  // set 1-byte alignment
    glTexParameteri ( target, GL_TEXTURE_WRAP_S, GL_REPEAT );    // set default params for texture
    glTexParameteri ( target, GL_TEXTURE_WRAP_T, GL_REPEAT );

	if ( !image -> isCompressed () )			// not compressed image
	{
		glTexImage2D ( target, 0, getFormat ().getInternalFormat (), width, height, 0, getFormat ().getFormat (), image -> getGlType (), image -> imageData () );	
	}
	else
	{
/*
		int	mipmaps = 1;						// no mipmaps for rect textures
		int	w       = width;
		int	h       = height;
		byte * ptr  = image -> imageData ( 0,  0 );
		
//		glTexParameteri  ( target, GL_TEXTURE_MAX_LEVEL, mipmaps - 1 );
		
		for ( int i = 0; i < mipmaps; i++ )
		{
			int	size = image -> imageDataSize ( 0, i );
			
			glCompressedTexImage2D ( target, i, getFormat ().getInternalFormat (), w, h, 0, size, ptr );
						
			if ( ( w = w / 2 ) < 1 )
				w = 1;
			
			if ( ( h = h / 2 ) < 1 )
				h = 1;
				
			ptr += size;
		}
*/
		delete image;
		
		return false;
	}
	
    glTexParameteri ( target, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri ( target, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glBindTexture   ( target, 0 );

	delete image;
	
	return true;
}

bool	Texture :: load3D ( const char * fileName )
{
	TexImage * image = loadTexImage ( fileName );
	
	if ( image == NULL )
		return false;
		
	if ( image -> getImageType () != TexImage :: image3D )
	{
		delete image;
		
		return false;
	}
	
	setParamsFromTexImage ( image, GL_TEXTURE_3D );
	
	glGenTextures   ( 1, &id );
    glBindTexture   ( target, id );
    glPixelStorei   ( GL_UNPACK_ALIGNMENT, 1 );                         // set 1-byte alignment
    glTexParameteri ( target, GL_TEXTURE_WRAP_S, GL_REPEAT );    // set default params for texture
    glTexParameteri ( target, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri ( target, GL_TEXTURE_WRAP_R, GL_REPEAT );

	if ( !image -> isCompressed () )			// not compressed image
	{
		glTexImage3D ( target, 0, getFormat ().getInternalFormat (), width, height, depth, 0, getFormat ().getFormat (), image -> getGlType (), image -> imageData () );	

		if ( autoMipmaps )
			glGenerateMipmap ( target );
	}
	else
	{
		int	mipmaps = image -> getNumLevels ();
		int	w       = width;
		int	h       = height;
		int	d       = depth;
		byte * ptr  = image -> imageData ( 0,  0 );
		
//		glTexParameteri  ( target, GL_TEXTURE_MAX_LEVEL, mipmaps - 1 );
		
		for ( int i = 0; i < mipmaps; i++ )
		{
			int	size = image -> imageDataSize ( 0, i );
			
			glCompressedTexImage3D ( target, i, getFormat ().getInternalFormat (), w, h, d, 0, size, ptr );

			if ( ( w = w / 2 ) < 1 )
				w = 1;
			
			if ( ( h = h / 2 ) < 1 )
				h = 1;
				
			if ( ( d = d / 2 ) < 1 )
				d = 1;
				
			ptr += size;
		}
	}
	
    if ( autoMipmaps )
    {
        glTexParameteri ( target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
        glTexParameteri ( target, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    }
    else
    {
        glTexParameteri ( target, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri ( target, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    }
	
    glBindTexture  ( target, 0 );

	delete image;
	
	return true;
}

bool	Texture :: loadCubemap ( const char * fileName )
{
	TexImage * image = loadTexImage ( fileName );
	
	if ( image == NULL )
		return false;
		
	if ( image -> getImageType () != TexImage :: imageCubemap )
	{
		delete image;
		
		return false;
	}
	
	setParamsFromTexImage ( image, GL_TEXTURE_CUBE_MAP );
	
	glGenTextures   ( 1, &id );
    glBindTexture   ( target, id );
    glPixelStorei   ( GL_UNPACK_ALIGNMENT, 1 );                         // set 1-byte alignment
    glTexParameterf ( target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameterf ( target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    glTexParameteri ( target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );

	if ( !image -> isCompressed () )			// not compressed image
	{
		for ( int i = 0; i < 6; i++ )
			glTexImage2D ( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, getFormat ().getInternalFormat (), width, height, 0, 
			               getFormat ().getFormat (), image -> getGlType (), image -> imageData ( i ) );	
			
		if ( autoMipmaps )
			glGenerateMipmap ( target );
	}
	else
	{
		int	mipmaps = image -> getNumLevels ();
		
		for ( int i = 0; i < 6; i++ )
		{		
			int	w       = width;
			int	h       = height;
			byte * ptr  = image -> imageData ( i,  0 );
			
			for ( int j = 0; j < mipmaps; j++ )
			{
				int	size = image -> imageDataSize ( i, j );
			
				glCompressedTexImage2D ( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, j, getFormat ().getInternalFormat (), w, h, 0, size, ptr );
				
				if ( ( w = w / 2 ) < 1 )
					w = 1;
				
				if ( ( h = h / 2 ) < 1 )
					h = 1;
					
				ptr += size;
			}		
		}
	}
	
    if ( autoMipmaps )
    {
        glTexParameteri ( target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
        glTexParameteri ( target, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    }
    else
    {
        glTexParameteri ( target, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri ( target, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    }
	
    glBindTexture  ( target, 0 );

	delete image;
	
	return true;
}

bool	Texture :: loadCubemapFromNames ( const char * fileNames )
{
	const char * seps = ",;";
	Tokenizer	 tok ( fileNames, seps );
	string		 names [6];
	
	for ( int i = 0; i < 6; i++ )
	{
		names [i] = tok.next ();
		
		if ( names [i].empty () )
			return false;
	}
	
	return loadCubemap ( names [0].c_str (), names [1].c_str (), names [2].c_str (), names [3].c_str (), 
	                     names [4].c_str (), names [5].c_str () );
}

bool	Texture :: loadCubemap ( const char * f1, const char * f2, const char * f3, const char * f4, const char * f5, const char * f6 )
{
	TexImage * image [6];

	image [0] = loadTexImage ( f1 );
	image [1] = loadTexImage ( f2 );
	image [2] = loadTexImage ( f3 );
	image [3] = loadTexImage ( f4 );
	image [4] = loadTexImage ( f5 );
	image [5] = loadTexImage ( f6 );
	
	for ( int i = 0; i < 6; i++ )
		if ( image [i] == NULL || image [i] -> getImageType () != TexImage :: image2D || image [i] -> getWidth () != image [i] -> getHeight () )
		{
			for ( int j = 0; j < 6; j++ )
				delete image [j];
				
			return false;
		}
	
	for ( int i = 1; i < 6; i++ )
		if ( image [i] -> getWidth () != image [0] -> getWidth () || image [i] -> getFormat () != image [0] -> getFormat () )
		{
			for ( int j = 0; j < 6; j++ )
				delete image [j];
				
			return false;
		}
		
	setParamsFromTexImage ( image [0], GL_TEXTURE_CUBE_MAP );
	
	glGenTextures   ( 1, &id );
    glBindTexture   ( target, id );
    glPixelStorei   ( GL_UNPACK_ALIGNMENT, 1 );                         // set 1-byte alignment
    glTexParameteri ( target, GL_TEXTURE_WRAP_S, GL_REPEAT );    // set default params for texture
    glTexParameteri ( target, GL_TEXTURE_WRAP_T, GL_REPEAT );

	if ( !image [0] -> isCompressed () )			// not compressed image
	{
		for ( int i = 0; i < 6; i++ )
			glTexImage2D ( GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB + i, 0, getFormat ().getInternalFormat (), width, height, 0, 
			               getFormat ().getFormat (), image [i] -> getGlType (), image [i] -> imageData () );	
			
		if ( autoMipmaps )
			glGenerateMipmap ( target );			
	}
	else
	{
		int	mipmaps = image [0] -> getNumLevels ();
		
//		glTexParameteri  ( target, GL_TEXTURE_MAX_LEVEL, mipmaps - 1 );

		for ( int i = 0; i < 6; i++ )
		{		
			int	w       = width;
			int	h       = height;
			byte * ptr  = image [i] -> imageData ( i, 0 );
			
			for ( int j = 0; j < mipmaps; j++ )
			{
				int	size = image [i] -> imageDataSize ( 0, j );
				
				glCompressedTexImage2D ( GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB + i, j, getFormat ().getInternalFormat (), w, h, 0, size, ptr );
				
				if ( ( w = w / 2 ) < 1 )
					w = 1;
				
				if ( ( h = h / 2 ) < 1 )
					h = 1;
					
				ptr += size;
			}		
		}
	}
	
    if ( autoMipmaps )
    {
        glTexParameteri ( target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
        glTexParameteri ( target, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    }
    else
    {
        glTexParameteri ( target, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri ( target, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    }
	
    glBindTexture  ( target, 0 );

	for ( int j = 0; j < 6; j++ )
		delete image [j];
	
	return true;
}

bool	Texture :: load1DRaw ( int theWidth, GLenum theFormat, GLenum theIntFmt, GLenum dataType, const void * theData )
{
	target         = GL_TEXTURE_1D;
	fmt            = TexFormat ( theFormat, theIntFmt );
	width          = theWidth;
	height         = 1;
	depth          = 1;	
	
	glGenTextures   ( 1, &id );
    glBindTexture   ( target, id );
	glGenTextures   ( 1, &id );
    glBindTexture   ( target, id );
    glPixelStorei   ( GL_UNPACK_ALIGNMENT, 1 );                  // set 1-byte alignment
    glTexParameteri ( target, GL_TEXTURE_WRAP_S, GL_REPEAT );    // set default params for texture
    glTexParameteri ( target, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri ( target, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri ( target, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexImage1D    ( target, 0, getFormat ().getInternalFormat (), width, 0, getFormat ().getFormat (), dataType, theData );
    glBindTexture   ( target, 0 );

	return true;
}

bool	Texture :: load2DRaw ( int theWidth, int theHeight, GLenum theFormat, GLenum theIntFmt, GLenum dataType, const void * theData )
{
	target         = GL_TEXTURE_2D;
	fmt            = TexFormat ( theFormat, theIntFmt );
	width          = theWidth;
	height         = theHeight;
	depth          = 1;	
	
	glGenTextures   ( 1, &id );
    glBindTexture   ( target, id );
    glPixelStorei   ( GL_UNPACK_ALIGNMENT, 1 );                  // set 1-byte alignment
    glTexParameteri ( target, GL_TEXTURE_WRAP_S, GL_REPEAT );    // set default params for texture
    glTexParameteri ( target, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri ( target, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri ( target, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexImage2D    ( target, 0, getFormat ().getInternalFormat (), width, height, 0, getFormat ().getFormat (), dataType, theData );
    glBindTexture   ( target, 0 );
	
	return true;
}

bool	Texture :: loadRectRaw ( int theWidth, int theHeight, GLenum theFormat, GLenum theIntFmt, GLenum dataType, const void * theData )
{
	target         = GL_TEXTURE_RECTANGLE;
	fmt            = TexFormat ( theFormat, theIntFmt );
	width          = theWidth;
	height         = theHeight;
	depth          = 1;	
	autoMipmaps    = false;
	
	glGenTextures   ( 1, &id );
    glBindTexture   ( target, id );
    glPixelStorei   ( GL_UNPACK_ALIGNMENT, 1 );                  // set 1-byte alignment
    glTexParameteri ( target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );    // set default params for texture
    glTexParameteri ( target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    glTexParameteri ( target, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri ( target, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexImage2D    ( target, 0, getFormat ().getInternalFormat (), width, height, 0, getFormat ().getFormat (), dataType, theData );
    glBindTexture   ( target, 0 );
	
	return true;
}

bool	Texture :: load3DRaw ( int theWidth, int theHeight, int theDepth, GLenum theFormat, GLenum theIntFmt, GLenum dataType, const void * theData )
{
	target         = GL_TEXTURE_3D;
	fmt            = TexFormat ( theFormat, theIntFmt );
	width          = theWidth;
	height         = theHeight;
	depth          = theDepth;	
	
	glGenTextures   ( 1, &id );
    glBindTexture   ( target, id );
    glPixelStorei   ( GL_UNPACK_ALIGNMENT, 1 );                  // set 1-byte alignment
    glTexParameteri ( target, GL_TEXTURE_WRAP_S, GL_REPEAT );    // set default params for texture
    glTexParameteri ( target, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri ( target, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri ( target, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexImage3D    ( target, 0, getFormat ().getInternalFormat (), width, height, depth, 0, getFormat ().getFormat (), dataType, theData );
    glBindTexture   ( target, 0 );

	return true;
}

bool	Texture :: saveAsTga ( const char * fileName )
{
	GLuint	boundTex;
	
	glGetIntegerv ( GL_TEXTURE_BINDING_2D, (int *) &boundTex );
	
	int		numPixels     = width * height;
	int		numComponents = 4;
	
	if ( getFormat ().getFormat () == GL_RGB || getFormat ().getFormat () == GL_BGR )
		numComponents = 3;
		
	byte  * buffer = (byte *) calloc ( numPixels * numComponents, 1 );
	
	if ( buffer == NULL )
		return NULL;
		
	if ( boundTex != id )
		glBindTexture ( target, id );
		
	glGetTexImage ( target, 0, numComponents == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, buffer );
	
	if ( boundTex != id )
		glBindTexture ( target, boundTex );
		
	TgaEncoder	encoder;
	bool		result = encoder.encode ( buffer, width, height, numComponents, fileName );

	free  ( buffer );
	
	return result;
}

void	Texture :: bind ( int texUnit )
{
	unit = texUnit;				// store the unit

	glActiveTexture ( GL_TEXTURE0 + texUnit );
	glBindTexture   ( target, id );
}

void	Texture :: unbind ()
{
	if ( unit != -1 )
	{
		glActiveTexture ( GL_TEXTURE0 + unit );
		glBindTexture ( target, 0 );

		unit = -1;			// not bound
	}
}

void	Texture :: buildMipmaps ()
{
	glTexParameteri  ( target, GL_TEXTURE_BASE_LEVEL, 0 );
	glTexParameteri  ( target, GL_GENERATE_MIPMAP, GL_TRUE );
	glGenerateMipmap ( target );						// should be bound
}

void	Texture :: setSwizzle ( GLenum red, GLenum green, GLenum blue, GLenum alpha )
{
	int	swizzle [4] = { red, green, blue, alpha };

	glTexParameteriv ( target, GL_TEXTURE_SWIZZLE_RGBA, swizzle );
}

int		Texture :: maxSize ()
{
	int	maxSize;
	
	glGetIntegerv ( GL_MAX_TEXTURE_SIZE, &maxSize );
	
	return maxSize;
}

void	Texture :: setParamsFromTexImage ( TexImage * image, GLenum theTarget )
{
	width      = image -> getWidth  ();
	height     = image -> getHeight ();
	depth      = image -> getDepth  ();
	fmt.format = GL_RGBA;
	target     = theTarget;
	
	if ( image -> getNumComponents () == 1 )
		fmt.format = GL_RED;
	else
	if ( image -> getNumComponents () == 2 )
		fmt.format = GL_RG;
	else
	if ( image -> getNumComponents () == 3 )
		fmt.format = GL_RGB;
		
	fmt = TexFormat ( fmt.format, image -> getFormat () );
		
}

void	Texture :: setBaseLevel ( int level )
{
	glTexParameteri ( target, GL_TEXTURE_BASE_LEVEL, level );
}

void	Texture :: setMaxLevel ( int level )
{
	glTexParameteri ( target, GL_TEXTURE_MAX_LEVEL, level );
}

void	Texture :: 	setBorderColor ( const vec4& color )
{
	glTexParameterfv ( target, GL_TEXTURE_BORDER_COLOR, color );
}

