//
// Basic description of the OIpenGL texture format
//
// Author: Alexey V. Boreskov, <steps3d@narod.ru>, <steps3s@gmail.com>
//

#include	<stdio.h>
#include	"TexFormat.h"

TexFormat :: TexFormat () 
{
	format         = 0;
	internalFormat = 0;
	numComponents  = 0;
	bitsPerChannel = 0;
}

TexFormat :: TexFormat ( GLenum theFormat, GLenum theIntFormat )
{
	format         = theFormat;
	internalFormat = theIntFormat;

	if ( internalFormat == GL_COMPRESSED_RED_RGTC1 || internalFormat == GL_COMPRESSED_SIGNED_RED_RGTC1 )
	{
		numComponents  = 1;
		bitsPerChannel = 0;
		
		return;
	}
	
	if ( internalFormat == GL_COMPRESSED_RG_RGTC2 || internalFormat == GL_COMPRESSED_SIGNED_RG_RGTC2 )
	{
		numComponents  = 2;
		bitsPerChannel = 0;
		
		return;
	}
	
	if ( internalFormat == GL_COMPRESSED_RGB_S3TC_DXT1_EXT )
	{
		numComponents  = 3;
		bitsPerChannel = 0;
		
		return;
	}
	
	if ( internalFormat == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT || internalFormat == GL_COMPRESSED_RGBA_S3TC_DXT3_EXT || internalFormat == GL_COMPRESSED_RGBA_S3TC_DXT5_EXT )
	{
		numComponents  = 4;
		bitsPerChannel = 0;
		
		return;
	}
	
	switch ( format )
	{
		case GL_RED:
		case GL_GREEN:
		case GL_BLUE:
		case GL_RED_INTEGER:
		case GL_GREEN_INTEGER:
		case GL_BLUE_INTEGER:
		case GL_DEPTH_COMPONENT:
		
			numComponents = 1;
			break;
			
		case GL_RG:
		case GL_RG_INTEGER:
			numComponents = 2;
			break;
			
		case GL_RGB:
		case GL_RGB_INTEGER:
		case GL_BGR:
		case GL_BGR_INTEGER:
			numComponents = 3;
			break;
			
		case GL_RGBA:
		case GL_RGBA_INTEGER:
		case GL_BGRA:
		case GL_BGRA_INTEGER:
			numComponents = 4;
			break;
			
		default:
			numComponents = 0;			// ???
			break;
	}
	
	switch ( internalFormat )
	{
		case GL_R8:
		case GL_R8I:
		case GL_R8UI:
		case GL_R8_SNORM:
		case GL_RG8:
		case GL_RG8_SNORM:
		case GL_RG8I:
		case GL_RG8UI:
		case GL_RGB8:
		case GL_RGB8I:
		case GL_RGB8UI:
		case GL_RGBA8:
		case GL_RGBA8I:
		case GL_RGBA8UI:
		case GL_SRGB8:
		case GL_SRGB8_ALPHA8:
		
			bitsPerChannel = 8;
			break;
			
		case GL_R16:
		case GL_R16I:
		case GL_R16UI:
		case GL_R16F:
		case GL_R16_SNORM:
		case GL_RG16:
		case GL_RG16_SNORM:
		case GL_RG16I:
		case GL_RG16UI:
		case GL_RG16F:
		case GL_RGB16:
		case GL_RGB16_SNORM:
		case GL_RGBA16:
		case GL_RGBA16I:
		case GL_RGBA16UI:
		case GL_RGBA16F:
		case GL_RGBA16_SNORM:
			bitsPerChannel = 16;
			break;
			
		case GL_R32I:
		case GL_R32UI:
		case GL_R32F:
		case GL_RG32I:
		case GL_RG32UI:
		case GL_RG32F:
		case GL_RGBA32I:
		case GL_RGBA32UI:
		case GL_RGBA32F:
			bitsPerChannel = 32;
			break;
	
		case GL_DEPTH_COMPONENT16:
				bitsPerChannel = 16;
				break;

		case GL_DEPTH_COMPONENT24:
				bitsPerChannel = 24;
				break;

		case GL_DEPTH_COMPONENT32:
				bitsPerChannel = 32;
				break;

		default:
			break;
	}
}

bool	TexFormat :: isPacked () const
{
	return internalFormat == GL_R3_G3_B2  || internalFormat == GL_RGB5_A1 ||
	       internalFormat == GL_RGB10_A2 || internalFormat == GL_RGB9_E5;
}

bool	TexFormat :: isCompressed () const
{
	return internalFormat == GL_COMPRESSED_RED_RGTC1 ||
	       internalFormat == GL_COMPRESSED_SIGNED_RED_RGTC1 ||
		   internalFormat == GL_COMPRESSED_RG_RGTC2 ||
		   internalFormat == GL_COMPRESSED_SIGNED_RG_RGTC2 ||
   		   internalFormat == GL_COMPRESSED_RGB_S3TC_DXT1_EXT ||
		   internalFormat == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT ||
		   internalFormat == GL_COMPRESSED_RGBA_S3TC_DXT3_EXT ||
		   internalFormat == GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
}

bool	TexFormat :: isColor () const
{
	return format == GL_RED || format == GL_RG || format == GL_RGB || format == GL_RGBA || format == GL_BGR || format == GL_BGRA ||
		   format == GL_RED_INTEGER || format == GL_GREEN_INTEGER || format == GL_BLUE_INTEGER || format == GL_RG_INTEGER ||
		   format == GL_RGB_INTEGER || format == GL_RGBA_INTEGER;
}

bool	TexFormat :: isColorFloat () const
{
	if ( !isColor () )
		return false;
		
	return internalFormat == GL_R16F || internalFormat == GL_R32F ||
	       internalFormat == GL_RG16F || internalFormat == GL_RG32F ||
		   internalFormat == GL_RGB16F || internalFormat == GL_RGB32F ||
		   internalFormat == GL_RGBA16F || internalFormat == GL_RGBA32F;
}

bool	TexFormat :: isColorInt () const
{
	if ( !isColor () )
		return false;
		
	return internalFormat == GL_R8I    || internalFormat == GL_R16I    || internalFormat == GL_R32I ||
	       internalFormat == GL_RG8I   || internalFormat == GL_RG16I   || internalFormat == GL_RG32I ||
		   internalFormat == GL_RGB8I  || internalFormat == GL_RGB16I  || internalFormat == GL_RGB32I ||
		   internalFormat == GL_RGBA8I || internalFormat == GL_RGBA16I || internalFormat == GL_RGBA32I;
}

bool	TexFormat :: isColorUint () const
{
	if ( !isColor () )
		return false;
		
	return internalFormat == GL_R8UI    || internalFormat == GL_R16UI    || internalFormat == GL_R32UI ||
	       internalFormat == GL_RG8UI   || internalFormat == GL_RG16UI   || internalFormat == GL_RG32UI ||
		   internalFormat == GL_RGB8UI  || internalFormat == GL_RGB16UI  || internalFormat == GL_RGB32UI ||
		   internalFormat == GL_RGBA8UI || internalFormat == GL_RGBA16UI || internalFormat == GL_RGBA32UI;
}

bool	TexFormat :: isColorNormalized () const
{
	if ( !isColor () )
		return false;
		
	return internalFormat == GL_R8    || internalFormat == GL_R16   ||
		   internalFormat == GL_RG8   || internalFormat == GL_RG16  ||
		   internalFormat == GL_RGB8  || internalFormat == GL_RGB16 ||
		   internalFormat == GL_RGBA8 || internalFormat == GL_RGBA16;
}

bool	TexFormat :: isColorSignedNormalized () const
{
	if ( !isColor () )
		return false;
		
	return internalFormat == GL_R8_SNORM    || internalFormat == GL_R16_SNORM   ||
		   internalFormat == GL_RG8_SNORM   || internalFormat == GL_RG16_SNORM  ||
		   internalFormat == GL_RGB8_SNORM  || internalFormat == GL_RGB16_SNORM ||
		   internalFormat == GL_RGBA8_SNORM || internalFormat == GL_RGBA16_SNORM;
}

string	TexFormat :: description () const
{
				// treat special cases
	if ( internalFormat == GL_R3_G3_B2 )
		return "Packed normalized RGB format 3:3:2";
		
	if ( internalFormat == GL_RGB5_A1 )
		return "Packed normalized RGBA format 5:5:5:1";
		
	if (  internalFormat == GL_RGB10_A2 )
		return "Packed normalized RGBA format 10:10:10:2";
		
	if ( internalFormat == GL_RGB9_E5 )
		return "Packed RGB9_E5 floating point with shared 5-bit exponent";
		
	if ( internalFormat == GL_COMPRESSED_RED_RGTC1 )
		return "Normalized Red Compressed RGTC1";
		
	if ( internalFormat == GL_COMPRESSED_SIGNED_RED_RGTC1 )
		return "Signed Normalized Red Compressed RGBTC1";
		
	if ( internalFormat == GL_COMPRESSED_RG_RGTC2 )
		return "Normalized RG Compressed RGTC2";
		
	if ( internalFormat == GL_COMPRESSED_SIGNED_RG_RGTC2 )
		return "Signed Normalized RG Compressed RGTC2";
		
   	if ( internalFormat == GL_COMPRESSED_RGB_S3TC_DXT1_EXT )
		return "Normalized RGB Compressed DXT1";
		
	if ( internalFormat == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT )
		return "Normalized RGBA Compressed DXT1";
		
	if ( internalFormat == GL_COMPRESSED_RGBA_S3TC_DXT3_EXT )
		return "Normalized RGBA Compressed DXT3";
		
	if ( internalFormat == GL_COMPRESSED_RGBA_S3TC_DXT5_EXT )
		return "Normalized RGBA Compressed DXT5";
		
	if ( !isOk () )
		return "Bad format";
		
	string	res;
	char	str [80];
	static const char * comps [] = { "R", "RG", "RGB", "RGBA" };
		
	if ( isColor () )
		res = "Color ";
	else
	if ( isDepth () )
		res = "Depth ";
	else
	if ( isDepthStencil () )
		res = "Depth-Stencil ";
	else 
		res = "Unknown type ";

	if ( isColorNormalized () )
		res += "Normalized ";
		
	res += comps [numComponents - 1];
	
	if ( isColorInt () )
		res += "Integer ";
	else
	if ( isColorUint () )
		res += "Unsigned Integer ";
		
	sprintf ( str, "BPC = %d", bitsPerChannel );
	
	res += str;
	
	return res;
}

TexFormat TexFormat :: intFormat ( int theNumChannels, int theNumBits )
{
	TexFormat	fmt;
	
	if ( theNumChannels < 1 || theNumChannels > 4 || (theNumBits != 8 && theNumBits != 16 && theNumBits != 32) )
	{
		fmt.numComponents  = 0;		// set invalid values so isOk () return false
		fmt.bitsPerChannel = 0;
		
		return fmt;
	}
	
	fmt.numComponents  = theNumChannels;
	fmt.bitsPerChannel = theNumBits;

	switch ( fmt.numComponents )
	{
		case 1:
			fmt.format = GL_RED_INTEGER;
			if ( fmt.bitsPerChannel == 8 )
				fmt.internalFormat = GL_R8I;
			else
			if ( fmt.bitsPerChannel == 16 )
				fmt.internalFormat = GL_R16I;
			else
				fmt.internalFormat = GL_R32I;
			break;
			
		case 2:
			fmt.format = GL_RG_INTEGER;
			if ( fmt.bitsPerChannel == 8 )
				fmt.internalFormat = GL_RG8I;
			else
			if ( fmt.bitsPerChannel == 16 )
				fmt.internalFormat = GL_RG16I;
			else
				fmt.internalFormat = GL_RG32I;
			break;
			
		case 3:
			fmt.format = GL_RGB_INTEGER;
			if ( fmt.bitsPerChannel == 8 )
				fmt.internalFormat = GL_RGB8I;
			else
			if ( fmt.bitsPerChannel == 16 )
				fmt.internalFormat = GL_RGB16I;
			else
				fmt.internalFormat = GL_RGB32I;
			break;
			
		case 4:
			fmt.format = GL_RGBA_INTEGER;
 			if ( fmt.bitsPerChannel == 8 )
				fmt.internalFormat = GL_RGBA8I;
			else
			if ( fmt.bitsPerChannel == 16 )
				fmt.internalFormat = GL_RGBA16I;
			else
				fmt.internalFormat = GL_RGBA32I;
			break;
	}
	
	return fmt;
}
 
TexFormat TexFormat :: uintFormat  ( int theNumChannels, int theNumBits )
{
	TexFormat	fmt;
	
	if ( theNumChannels < 1 || theNumChannels > 4 || (theNumBits != 8 && theNumBits != 16 && theNumBits != 32) )
	{
		fmt.numComponents  = 0;		// set invalid values so isOk () return false
		fmt.bitsPerChannel = 0;
		
		return fmt;
	}
	
	fmt.numComponents  = theNumChannels;
	fmt.bitsPerChannel = theNumBits;

	switch ( fmt.numComponents )
	{
		case 1:
			fmt.format = GL_RED_INTEGER;
			if ( fmt.bitsPerChannel == 8 )
				fmt.internalFormat = GL_R8UI;
			else
			if ( fmt.bitsPerChannel == 16 )
				fmt.internalFormat = GL_R16UI;
			else
				fmt.internalFormat = GL_R32UI;
			break;
			
		case 2:
			fmt.format = GL_RG_INTEGER;
			if ( fmt.bitsPerChannel == 8 )
				fmt.internalFormat = GL_RG8UI;
			else
			if ( fmt.bitsPerChannel == 16 )
				fmt.internalFormat = GL_RG16UI;
			else
				fmt.internalFormat = GL_RG32UI;
			break;
			
		case 3:
			fmt.format = GL_RGB_INTEGER;
			if ( fmt.bitsPerChannel == 8 )
				fmt.internalFormat = GL_RGB8UI;
			else
			if ( fmt.bitsPerChannel == 16 )
				fmt.internalFormat = GL_RGB16UI;
			else
				fmt.internalFormat = GL_RGB32UI;
			break;
			
		case 4:
			fmt.format = GL_RGBA_INTEGER;
 			if ( fmt.bitsPerChannel == 8 )
				fmt.internalFormat = GL_RGBA8UI;
			else
			if ( fmt.bitsPerChannel == 16 )
				fmt.internalFormat = GL_RGBA16UI;
			else
				fmt.internalFormat = GL_RGBA32UI;
			break;
	}
	
	return fmt;
}

TexFormat TexFormat :: floatFormat ( int theNumChannels, int theNumBits )
{
	TexFormat	fmt;
	
	if ( theNumChannels < 1 || theNumChannels > 4 || (theNumBits != 16 && theNumBits != 32) )
	{
		fmt.numComponents  = 0;		// set invalid values so isOk () return false
		fmt.bitsPerChannel = 0;
		
		return fmt;
	}
	
	fmt.numComponents    = theNumChannels;
	fmt.bitsPerChannel = theNumBits;

	switch ( fmt.numComponents )
	{
		case 1:
			fmt.format = GL_RED;
			if ( fmt.bitsPerChannel == 16 )
				fmt.internalFormat = GL_R16F;
			else
				fmt.internalFormat = GL_R32F;
			break;
			
		case 2:
			fmt.format = GL_RG;
			if ( fmt.bitsPerChannel == 16 )
				fmt.internalFormat = GL_RG16F;
			else
				fmt.internalFormat = GL_RG32F;
			break;
			
		case 3:
			fmt.format = GL_RGB;
			if ( fmt.bitsPerChannel == 16 )
				fmt.internalFormat = GL_RGB16F;
			else
				fmt.internalFormat = GL_RGB32F;
			break;
			
		case 4:
			fmt.format = GL_RGBA;
			if ( fmt.bitsPerChannel == 16 )
				fmt.internalFormat = GL_RGBA16F;
			else
				fmt.internalFormat = GL_RGBA32F;
			break;
	}
	
	return fmt;
}

TexFormat TexFormat :: normFormat ( int theNumChannels, int theNumBits )
{
	TexFormat	fmt;
	
	if ( theNumChannels < 1 || theNumChannels > 4 || (theNumBits != 8 && theNumBits != 16) )
	{
		fmt.numComponents  = 0;		// set invalid values so isOk () return false
		fmt.bitsPerChannel = 0;
		
		return fmt;
	}
	
	fmt.numComponents  = theNumChannels;
	fmt.bitsPerChannel = theNumBits;

	switch ( fmt.numComponents )
	{
		case 1:
			fmt.format = GL_RED;
 			if ( fmt.bitsPerChannel == 8 )
				fmt.internalFormat = GL_R8;
			else
			if ( fmt.bitsPerChannel == 16 )
				fmt.internalFormat = GL_R16;
				
			break;
			
		case 2:
			fmt.format = GL_RG;
 			if ( fmt.bitsPerChannel == 8 )
				fmt.internalFormat = GL_RG8;
			else
			if ( fmt.bitsPerChannel == 16 )
				fmt.internalFormat = GL_RG16;
				
			break;
			
		case 3:
			fmt.format = GL_RGB;
 			if ( fmt.bitsPerChannel == 8 )
				fmt.internalFormat = GL_RGB8;
			else
			if ( fmt.bitsPerChannel == 16 )
				fmt.internalFormat = GL_RGB16;

			break;
			
		case 4:
			fmt.format = GL_RGBA;
 			if ( fmt.bitsPerChannel == 8 )
				fmt.internalFormat = GL_RGBA8;
			else
			if ( fmt.bitsPerChannel == 16 )
				fmt.internalFormat = GL_RGBA16;
				
			break;
	}
	
	return fmt;
}

TexFormat TexFormat :: snormFormat ( int theNumChannels, int theNumBits )
{
	TexFormat	fmt;
	
	if ( theNumChannels < 1 || theNumChannels > 4 || (theNumBits != 8 && theNumBits != 16) )
	{
		fmt.numComponents  = 0;		// set invalid values so isOk () return false
		fmt.bitsPerChannel = 0;
		
		return fmt;
	}
	
	fmt.numComponents  = theNumChannels;
	fmt.bitsPerChannel = theNumBits;

	switch ( fmt.numComponents )
	{
		case 1:
			fmt.format = GL_RED;
 			if ( fmt.bitsPerChannel == 8 )
				fmt.internalFormat = GL_R8_SNORM;
			else
			if ( fmt.bitsPerChannel == 16 )
				fmt.internalFormat = GL_R16_SNORM;
				
			break;
			
		case 2:
			fmt.format = GL_RG;
 			if ( fmt.bitsPerChannel == 8 )
				fmt.internalFormat = GL_RG8_SNORM;
			else
			if ( fmt.bitsPerChannel == 16 )
				fmt.internalFormat = GL_RG16_SNORM;
				
			break;
			
		case 3:
			fmt.format = GL_RGB;
 			if ( fmt.bitsPerChannel == 8 )
				fmt.internalFormat = GL_RGB8_SNORM;
			else
			if ( fmt.bitsPerChannel == 16 )
				fmt.internalFormat = GL_RGB16_SNORM;
				
			break;
			
		case 4:
			fmt.format = GL_RGBA;
 			if ( fmt.bitsPerChannel == 8 )
				fmt.internalFormat = GL_RGBA8_SNORM;
			else
			if ( fmt.bitsPerChannel == 16 )
				fmt.internalFormat = GL_RGBA16_SNORM;
				
			break;
	}
	
	return fmt;
}
 
TexFormat TexFormat :: srgbFormat ()
 {
	TexFormat	fmt;
	
	fmt.format         = GL_RGB;
	fmt.internalFormat = GL_SRGB8;
	fmt.numComponents  = 3;
	fmt.bitsPerChannel = 8;
	
	return fmt;
}

TexFormat TexFormat :: srgbaFormat ()
{
	TexFormat	fmt;
	
	fmt.format         = GL_RGBA;
	fmt.internalFormat = GL_SRGB8_ALPHA8;
	fmt.numComponents  = 4;
	fmt.bitsPerChannel = 8;
	
	return fmt;
}

TexFormat TexFormat :: depthNormalized ( int theNumBits )
{
	TexFormat	fmt;
	
	fmt.format         = GL_DEPTH_COMPONENT;
	fmt.numComponents  = 1;
	fmt.bitsPerChannel = theNumBits;

	if ( theNumBits == 16 )
		fmt.internalFormat = GL_DEPTH_COMPONENT16;
	else
	if ( theNumBits == 24 )
		fmt.internalFormat = GL_DEPTH_COMPONENT24;
	else
	if ( theNumBits == 32 )
		fmt.internalFormat = GL_DEPTH_COMPONENT32;
	
	return fmt;
}

TexFormat TexFormat :: depthFloat ()
{
	TexFormat	fmt;
	
	fmt.format         = GL_DEPTH_COMPONENT;
	fmt.numComponents  = 1;
	fmt.bitsPerChannel = 32;
	fmt.internalFormat = GL_DEPTH_COMPONENT32F;
	
	return fmt;
}

TexFormat TexFormat :: depthStenil24_8 ()
{
	TexFormat	fmt;
	
	fmt.format         = GL_DEPTH_STENCIL;
	fmt.internalFormat = GL_DEPTH24_STENCIL8;
	fmt.numComponents  = 2;
	fmt.bitsPerChannel = 0;
	
	return fmt;
}

TexFormat TexFormat :: depthStencilFloat ()
{
	TexFormat	fmt;
	
	fmt.format         = GL_DEPTH_STENCIL;
	fmt.internalFormat = GL_DEPTH32F_STENCIL8;
	fmt.numComponents  = 2;
	fmt.bitsPerChannel = 0;
	
	return fmt;
}
