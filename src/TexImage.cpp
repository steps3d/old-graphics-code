//
// Simple texture class
//

#include	<assert.h>
#include	<malloc.h>		// ??? do we need it ????
#include    <memory.h>
#include	<string.h>
#include	"TexImage.h"

TexImage :: TexImage ( int theWidth, int theHeight, int theDepth, int theNumComponents )
{
    width         = theWidth;
    height        = theHeight;
	depth         = theDepth;
    numComponents = theNumComponents;
	numImages     = 1;
	numLevels     = 1;
	blockSize     = 0;
	imageSize     = 0;
	imageType     = imageNone;
	type          = typeNone;
    data          = NULL;
    compressed    = false;
	cubemap       = false;
    format        = GL_NONE;
}

TexImage :: ~TexImage ()
{
    if ( data != NULL )
        free ( data );
}

TexImage * TexImage :: new2D ( int w, int h, int components, int compType )
{
	assert ( compType >= typeByte && compType <= typeInt32 );
	
	TexImage * image = new TexImage ( w, h, 1, components );
	
	image -> type          = compType;
	image -> blockSize     = 1;
	image -> numComponents = components;
	image -> imageSize     = image -> width * image -> height * image -> numComponents * typeSize ( image -> type );
	image -> data          = (byte *) calloc ( image -> imageSize, 1 );
	image -> imageType     = image2D;
	
	image -> setFormatFromNumCompType ();
	
	return image;
}

TexImage * TexImage :: new2DCompressed ( int w, int h, int format, int numLevels, int imageSize )
{
	int	components = 0;
	
	switch ( format )
	{
		case GL_COMPRESSED_RGB_S3TC_DXT1_EXT:
			components = 3;
			break;
			
		case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT:
		case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT:
		case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT:
			components = 4;
			break;
			
		default:
			assert ( 0 );
			break;
	}
	
	assert ( components > 0 && components <= 4 );
	
	TexImage * image = new TexImage ( w, h, 1, components );

	image -> format     = format;
	image -> imageSize  = imageSize;
	image -> blockSize  = 4;
	image -> type       = typeByte;
	image -> compressed = true;
	image -> numLevels  = numLevels;
	image -> data       = (byte *) calloc ( image -> imageSize, 1 );
	image -> imageType  = image2D;
	
	return image;
}

TexImage * TexImage :: newCubemap ( int w, int components, int compType )
{
	assert ( compType >= typeByte && compType <= typeInt32 );
	
	TexImage * image = new TexImage ( w, w, 1, components );
	
	image -> type          = compType;
	image -> blockSize     = 1;
	image -> numComponents = components;
	image -> imageSize     = image -> width * image -> height * image -> numComponents * typeSize ( image -> type );
	image -> data          = (byte *) calloc ( image -> imageSize, 6 );
	image -> numImages     = 6;
	image -> imageType     = imageCubemap;
	image -> cubemap       = true;
	
	image -> setFormatFromNumCompType ();
	
	return image;

}

TexImage * TexImage :: newCubemapCompressed ( int w, int format, int numLevels, int imageSize )
{
	int	components = 0;
	
	switch ( format )
	{
		case GL_COMPRESSED_RGB_S3TC_DXT1_EXT:
			components = 3;
			break;
			
		case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT:
		case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT:
		case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT:
			components = 4;
			break;
			
		default:
			assert ( 0 );
			break;
	}
	
	assert ( components > 0 && components <= 4 );
	
	TexImage * image = new TexImage ( w, w, 1, components );

	image -> format     = format;
	image -> imageSize  = imageSize;
	image -> blockSize  = 4;
	image -> type       = typeByte;
	image -> compressed = true;
	image -> numLevels  = numLevels;
	image -> numImages  = 6;
	image -> data       = (byte *) calloc ( image -> imageSize, 6 );
	image -> imageType  = imageCubemap;
	image -> cubemap    = true;
	image -> compressed = true;
	
	return image;
}
TexImage * TexImage :: new3D ( int w, int h, int d, int components, int compType )
{
	assert ( compType >= typeByte && compType <= typeInt32 );
	
	TexImage * image = new TexImage ( w, h, d, components );
	
	image -> type          = compType;
	image -> blockSize     = 1;
	image -> numComponents = components;
	image -> imageSize     = image -> width * image -> height * image -> depth * image -> numComponents * typeSize ( image -> type );
	image -> data          = (byte *) calloc ( image -> imageSize, 1 );
	image -> imageType     = image3D;
	
	image -> setFormatFromNumCompType ();
	
	return image;
}
                                            // store 32-bit RGBA image into texture in a specified line
void    TexImage :: putLine ( int y, dword * bits )
{
    if ( y < 0 || y >= height )
        return;

    int    offs = y * width * numComponents * typeSize ( type );
    byte * ptr  = data + offs;

    if ( numComponents == 4 )               // RGBA image
        memcpy ( ptr, bits, 4 * width );
    else
    if ( numComponents == 3 )               // RGB image
    {
        byte * src = (byte *) bits;

        for ( int i = 0; i < width; i++, src += 4 )
        {
            *ptr++ = src [0];
            *ptr++ = src [1];
            *ptr++ = src [2];
        }
    }
    else
    if ( numComponents == 1 )               // greyscale image
    {
        for ( int i = 0; i < width ; i++, bits++ )
            *ptr++ = *(byte *) bits;
    }
}

/*
bool    TexImage :: upload ( int target, bool mipmap )
{
    if ( target == GL_TEXTURE_1D )
    {
        if ( mipmap )
            gluBuild1DMipmaps ( target, getNumComponents (), getWidth (),
                                getFormat (), GL_UNSIGNED_BYTE, getData () );
        else
            glTexImage1D      ( target, 0, getNumComponents (), getWidth (), 0,
                                getFormat (), GL_UNSIGNED_BYTE, getData () );
    }
    else
    {
        if ( mipmap )
            gluBuild2DMipmaps ( target, getNumComponents (), getWidth (), getHeight (),
                                getFormat (), GL_UNSIGNED_BYTE, getData () );
        else
            glTexImage2D      ( target, 0, getNumComponents (), getWidth (), getHeight (), 0,
                                getFormat (), GL_UNSIGNED_BYTE, getData () );
    }

    return true;
}
*/

size_t	TexImage :: typeSize ( int theType )
{
	switch ( theType )
	{
		case typeByte:
			return 1;
			
		case typeHalf:
			return 2;
			
		case typeFloat:
			return 4;
			
		case typeUInt16:
			return 2;
			
		case typeUInt32:
			return 4;
			
		case typeInt8:
			return 1;
			
		case typeInt16:
			return 2;
			
		case typeInt32:
			return 4;
			
		default:
			break;
	}
	
	assert ( 0 );
	
	return 0;
}
	
void	TexImage :: setFormatFromNumCompType ()
{
	static	int	f1 [9] = { GL_NONE, GL_R8,    GL_R16F,    GL_R32F,    GL_R16UI,    GL_R32UI,    GL_R8I,    GL_R16I,    GL_R32I    };
	static	int	f2 [9] = { GL_NONE, GL_RG8,   GL_RG16F,   GL_RG32F,   GL_RG16UI,   GL_RG32UI,   GL_RG8I,   GL_RG16I,   GL_RG32I   };
	static	int	f3 [9] = { GL_NONE, GL_RGB8,  GL_RGB16F,  GL_RGB32F,  GL_RGB16UI,  GL_RGB32UI,  GL_RGB8I,  GL_RGB16I,  GL_RGB32I  };
	static	int	f4 [9] = { GL_NONE, GL_RGBA8, GL_RGBA16F, GL_RGBA32F, GL_RGBA16UI, GL_RGBA32UI, GL_RGBA8I, GL_RGBA16I, GL_RGBA32I };
	
	switch ( numComponents )
	{
		case 1:
			if ( type >= 0 && type < sizeof ( f1 ) / sizeof ( f1 [0] ) )
				format = f1 [type];
			break;
			
		case 2:
			if ( type >= 0 && type < sizeof ( f1 ) / sizeof ( f1 [0] ) )
				format = f2 [type];
			break;
			
		case 3:
			if ( type >= 0 && type < sizeof ( f1 ) / sizeof ( f1 [0] ) )
				format = f3 [type];
			break;
			
		case 4:
			if ( type >= 0 && type < sizeof ( f1 ) / sizeof ( f1 [0] ) )
				format = f4 [type];
			break;
			
		default:
			break;
	}
}

GLenum	TexImage :: getGlType () const
{
	switch ( type )
	{
		case typeByte:
			return GL_UNSIGNED_BYTE;
			
		case typeHalf:
			return GL_HALF_FLOAT;
			
		case typeFloat:
			return GL_FLOAT;
			
		case typeUInt16:
			return GL_UNSIGNED_SHORT;
			
		case typeUInt32:
			return GL_UNSIGNED_INT;
			
		case typeInt8:
			return GL_BYTE;
			
		case typeInt16:
			return GL_SHORT;
			
		case typeInt32:
			return GL_INT;
			
		default:
			break;
	}
	
	return GL_NONE;
}

byte  * TexImage :: imageData ( int image, int level ) const
{
	if ( image < 0 || image >= numImages || level < 0 || level >= numLevels )
		return NULL;
		
	if ( level == 0 )
		return data + image * imageSize;
		
	int	w    = width;
	int	h    = height;
	int	offs = 0;
		
	if ( !compressed )	
	{
		int	s    = numComponents * typeSize ( type );
		int	bytesPerLine;
		
		for ( int i = 1; i < level; i++ )
		{
			w /= 2;
			h /= 2;

			if ( w < 1 )
				w = 1;

			if ( h < 1 )
				h = 1;

			bytesPerLine = w * s;

			if ( (bytesPerLine & 3) != 0 )                // do dword alignment
				bytesPerLine += 4 - (bytesPerLine & 3);
				
			offs += bytesPerLine * h;
		}
		
		if ( offs < imageSize )
			return data + image * imageSize + offs;
			
		return NULL;
	}

	int	blockSize, size;
                                            // get number of bytes per block
    if ( format == GL_COMPRESSED_RGB_S3TC_DXT1_EXT || format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT )
        blockSize = 8;
    else
        blockSize = 16;

    for ( int i = 0; i < level; i++ )
    {
        size  = ((w+3)/4) * ((h+3)/4) * blockSize;
        offs += size;
		
        if ( (w = w / 2) == 0 )
            w = 1;

        if ( (h = h / 2)== 0 )
            h = 1;
    }

	if ( offs < imageSize )
		return data + image * imageSize + offs;
		
	return NULL;
}

size_t	TexImage :: imageDataSize ( int image, int level ) const
{
	int	w = width;
	int	h = height;
	int	size, blockSize;
	
	if ( !compressed )	
	{
		int	s    = numComponents * typeSize ( type );
		int	bytesPerLine;
		
		for ( int i = 0; i <= level; i++ )
		{
			bytesPerLine = w * s;

			if ( (bytesPerLine & 3) != 0 )                // do dword alignment
				bytesPerLine += 4 - (bytesPerLine & 3);
				
			size = bytesPerLine * h;
			
			if ( (w = w / 2) == 0 )
				w = 1;

			if ( (h = h / 2)== 0 )
				h = 1;
		}
		
		return size;
	}
                                            // get number of bytes per block
    if ( format == GL_COMPRESSED_RGB_S3TC_DXT1_EXT || format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT )
        blockSize = 8;
    else
        blockSize = 16;

    for ( int i = 0; i <= level; i++ )
    {
        size = ((w+3)/4) * ((h+3)/4) * blockSize;
		
		if ( (w = w / 2) == 0 )
			w = 1;

		if ( (h = h / 2)== 0 )
			h = 1;
    }

	return size;
}
