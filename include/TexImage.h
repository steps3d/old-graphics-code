//
// Simple class to hold a texture
// Supported 1/2/3-d textures and cubemaps, mipmaps
//
// Author: Alexey V. Boreskov <steps3d@gmail.com>, <steps3d@narod.ru>
//

#pragma once
#ifndef __TEX_IMAGE__
#define __TEX_IMAGE__

#include <GL/glew.h>
#include "TypeDefs.h"

class   TexImage
{
protected:
    int     width;
    int     height;
    int	    depth;
    int	    numImages;		// for cubemaps is 6
    int     numComponents;
    int     bytesPerChannel;
    int     format;		// GL internal format
    int     type;		// data type used for components
    byte  * data;
    int     numLevels;		// number of mipmaps
    int     blockSize;		// for block-compressed images
    int     imageSize;		// total size of one image (including mipmaps)
    bool    compressed;
    bool    cubemap;
    int     imageType;

    TexImage ( int theWidth, int theHeight, int theDepth, int theNumComponents );
	
public:
	static TexImage * new2D                ( int w, int h, int components, int compType = typeByte ); 
	static TexImage * new2DCompressed      ( int w, int h, int format, int numLevels, int imageSize );
	static TexImage * newCubemap           ( int w, int components, int compType = typeByte ); 
	static TexImage * newCubemapCompressed ( int w, int format, int numLevels, int imageSize );
	static TexImage * new3D                ( int w, int h, int d, int components, int compType = typeByte ); 
	
    ~TexImage ();

	bool	isOk () const
	{
		return width > 0 && height > 0 && depth > 0 && data != NULL;
	}
	
    int getWidth () const
    {
        return width;
    }

    int getHeight () const
    {
        return height;
    }

	int	getDepth () const
	{
		return depth;
	}
	
	int	getImageType () const
	{
		return imageType;
	}
	
    int getNumComponents () const
    {
        return numComponents;
    }

    int getFormat () const
    {
        return format;
    }

    int getNumLevels () const			// get number of mipmpap levels, >= 1
    {
        return numLevels;
    }

    bool    isCompressed () const
    {
        return compressed;
    }

	bool	isCubemap () const
	{
		return cubemap;
	}
	

    byte  * imageData     ( int image = 0, int level = 0 ) const;
	size_t	imageDataSize ( int image = 0, int level = 0 ) const;
	GLenum	getGlType     () const;
	
                                            // store 32-bit RGBA image into texture in a
                                            // specified line, used by loaders
	void    putLine ( int y, dword * bits );
	
	enum	DataType
	{
		typeNone   = 0,
		typeByte   = 1,
		typeHalf   = 2,
		typeFloat  = 3,
		typeUInt16 = 4,
		typeUInt32 = 5,
		typeInt8   = 6,
		typeInt16  = 7,
		typeInt32  = 8
	};
	
	enum	ImageType
	{
		imageNone    = 0,
		image2D      = 1,
		image3D      = 2,
		imageCubemap = 3
							// XXX - arrays ???
	};
	
protected:
	static	size_t	typeSize                 ( int theType );
	void			setFormatFromNumCompType ();
};

inline  dword   makeRgba ( int red, int green, int blue, int alpha = 255 )
{
    return red + (green << 8) + (blue << 16) + (alpha << 24);
}

#endif
