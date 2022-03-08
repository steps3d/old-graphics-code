//
// Class to load images from BMP files
//
// Author: Alex V. Boreskoff <steps3d@gmail.com>, <steps3d@narod.ru>
//

#include	<stdio.h>
#include	<memory.h>
#include	"BmpDecoder.h"
#include	"BmpHeader.h"
#include	"TexImage.h"
#include	"Data.h"
////////////////////////// here go the methods ///////////////////////////////

TexImage * BmpDecoder :: load ( Data * data )
{
	BmpHeader     * hdr     = (BmpHeader *)     data -> getPtr ();
	BmpInfoHeader * infoHdr = (BmpInfoHeader *) data -> getPtr ( sizeof ( BmpHeader ) );

    bool	errorCode = hdr -> type == 0x4D42 && infoHdr -> size == 40 && infoHdr -> planes == 1 &&
                        (infoHdr -> bitCount == 1 || infoHdr -> bitCount == 2 ||
                        infoHdr -> bitCount == 4 || infoHdr -> bitCount == 8 || infoHdr -> bitCount == 24 || infoHdr -> bitCount == 32) &&
                        (infoHdr -> compression == BI_RGB || infoHdr -> compression == BI_RLE4 || infoHdr -> compression == BI_RLE8);

	if ( !errorCode )                    // bad header buf
        return NULL;

    int	numColors = 1 << infoHdr -> bitCount;
    int	width     = infoHdr -> width;
    int	height    = infoHdr -> height;

    RGBQuad * pal = (RGBQuad *)(infoHdr + 1);
	dword	  palette [256];
	int		  numComponents = 3;

	if ( infoHdr -> bitCount == 32 )
		numComponents = 4;

	if ( infoHdr -> bitCount <= 8 )
		for ( int i = 0; i < numColors; i++ )
			palette [i] = makeRgba ( pal [i].red, pal [i].green, pal [i].blue );

	TexImage * tex = TexImage :: new2D ( width, height, numComponents );
	dword    * buf = new dword [width];

	data -> seekAbs ( hdr -> offBits );

    if ( infoHdr -> compression == BI_RGB )
    {
		if ( infoHdr -> bitCount == 1 )		// mono
			errorCode = loadMono ( data, tex, buf, palette );
		else
        if ( infoHdr -> bitCount == 4 )		// 16-colors uncompressed
			errorCode = loadRGB4 ( data, tex, buf, palette );
        else
        if ( infoHdr -> bitCount == 8 )		// 256-colors uncompressed
			errorCode = loadRGB8 ( data, tex, buf, palette );
		else
        if ( infoHdr -> bitCount == 24 )	// True-Color bitmap
            errorCode = loadRGB24 ( data, tex, buf );
		else
		if ( infoHdr -> bitCount == 32 )	// true-color bitmap with alpha-channel
			 errorCode = loadRGB32 ( data, tex, buf );
    }
    else
    if ( infoHdr -> compression == BI_RLE4 )// 16-colors RLE compressed
        errorCode = loadRLE4 ( data, tex, buf, palette );
    else
    if ( infoHdr -> compression == BI_RLE8 )// 256-colors RLE compressed
        errorCode = loadRLE8 ( data, tex, buf, palette );
    else
		errorCode = false;

	delete buf;

    if ( !errorCode )
    {
        delete tex;

        return NULL;
    }

    return tex;
}

bool	BmpDecoder :: loadMono ( Data * data, TexImage * tex, dword * buf, dword palette [] )
{
	int	   width  = tex -> getWidth  ();
	int	   height = tex -> getHeight ();
	byte * ptr    = new byte [width];				// sure it's enough

	int		bytesPerLine = (width + 7) / 8;
	int		delta        = bytesPerLine & 3;		// do DWORD alignment

	if ( delta != 0 )
		bytesPerLine += 4 - delta;

	for ( int y = height - 1; y >= 0; y-- )
	{
		if ( data -> getBytes ( ptr, bytesPerLine ) != bytesPerLine )
		{
			delete ptr;

			return false;
		}

		memset ( buf, '\0', sizeof ( dword ) * width );

		for ( int x = 0; x < width; x++ )
			if ( ptr [x >> 3] & (0x80 >> (x & 7) ) )
				buf [x] = palette [0];
			else
				buf [x] = palette [1];

		tex -> putLine ( height - 1 - y, buf );
	}
		
	delete ptr;

    return true;
}


bool	BmpDecoder :: loadRGB4 ( Data * data, TexImage * tex, dword * buf, dword palette [] )
{
	int	width  = tex -> getWidth  ();
	int	height = tex -> getHeight ();
	int	count, byte;

    for ( int y = height - 1; y >= 0; y-- )
    {
		memset ( buf, '\0', sizeof ( dword ) * width );

		for ( int x = 0; x < width; x++ )
			if ( ( x & 1 ) == 0 )
				if ( ( byte = data -> getByte () ) == -1 )
					return false;
                else
                    buf [x] = palette [byte >> 4];
            else
                buf [x] = palette [byte & 0x0F];

        for ( count = ( width + 1 ) / 2; count % 4; count++ )
			if ( data -> getByte () == -1 )
				return false;

		tex -> putLine ( height - 1 - y, buf );
    }

    return true;
}

bool	BmpDecoder :: loadRGB8 ( Data * data, TexImage * tex, dword * buf, dword palette [] )
{
	int	width  = tex -> getWidth  ();
	int	height = tex -> getHeight ();
	int	count, byte;

    for ( int y = height - 1; y >= 0; y-- )
    {
		memset ( buf, '\0', sizeof ( dword ) * width );

		for ( int x = count = 0; x < width; x++, count++ )
			if ( ( byte = data -> getByte () ) == -1 )
				return false;
            else
                buf [x] = palette [byte];

        for ( ; count % 4; count++ )    // skip remaining bytes
			if ( data -> getByte () == -1 )
				return false;

		tex -> putLine ( height - 1 - y, buf );
    }

    return true;
}

bool	BmpDecoder :: loadRGB24 ( Data * data, TexImage * tex, dword * buf )
{
	int	width  = tex -> getWidth  ();
	int	height = tex -> getHeight ();
	int	count;
    int	red, green, blue;

    for ( int y = height - 1; y >= 0; y-- )
    {
		memset ( buf, '\0', sizeof ( dword ) * width );

		for ( int x = count = 0; x < width; x++, count += 3 )
        {
			if ( ( blue = data -> getByte () ) == -1 )
				return false;

            if ( ( green = data -> getByte () ) == -1 )
                return false;

            if ( ( red = data -> getByte () ) == -1 )
                return false;

            buf [x] = makeRgba ( red, green, blue );
        }

        for ( ; count % 4; count++ )    // skip remaining bytes
			if ( data -> getByte () == -1 )
				return false;

		tex -> putLine ( height - 1 - y, buf );
    }

    return true;
}

bool	BmpDecoder :: loadRGB32 ( Data * data, TexImage * tex, dword * buf )
{
	int	width  = tex -> getWidth  ();
	int	height = tex -> getHeight ();
    int	red, green, blue, alpha;

    for ( int y = height - 1; y >= 0; y-- )
    {
		memset ( buf, '\0', sizeof ( dword ) * width );

		for ( int x = 0; x < width; x++ )
        {
			if ( ( blue = data -> getByte () ) == -1 )
				return false;

            if ( ( green = data -> getByte () ) == -1 )
                return false;

            if ( ( red = data -> getByte () ) == -1 )
                return false;

			if ( ( alpha = data -> getByte () ) == -1 )
				return false;

            buf [x] = makeRgba ( red, green, blue, alpha );
        }

		tex -> putLine ( height - 1 - y, buf );
    }

    return true;
}

bool	BmpDecoder :: loadRLE4 ( Data * data, TexImage * tex, dword * buf, dword palette [] )
{
	int	width  = tex -> getWidth  ();
	int	height = tex -> getHeight ();
	int	y      = height - 1;
    int x      = 0;
    int count, byte;

	memset ( buf, '\0', sizeof ( dword ) * width );

    for ( ; ; )
    {
		if ( ( count = data -> getByte () ) == -1 )
			return false;
        else
        if ( count == 0 )
        {
			if ( ( count = data -> getByte () ) == -1 )
				return false;
            else
            if ( count == 0 )       // end of line
            {
				tex -> putLine ( height - 1 - y, buf );

				memset ( buf, '\0', sizeof ( dword ) * width );

				y--;
                x = 0;
            }
            else
            if ( count == 1 )       // 0, 1 == end of RLE buf
				break;
            else
            if ( count == 2 )       // 0, 2 == delta record
            {
				tex -> putLine ( height - 1 - y, buf );

				memset ( buf, '\0', sizeof ( dword ) * width );

				y -= data -> getByte ();
                x += data -> getByte ();
            }
            else                    // start of an unencoded block
            {
				for ( int i = 0; i < count; i += 2, x += 2 )
                {
					if ( ( byte = data -> getByte () ) == -1 )
						return false;

                    buf [x]     = palette [byte >> 4];
                    buf [x + 1] = palette [byte & 0x0F];
                }

                if ( ( count / 2 ) & 1 )
					if ( data -> getByte () == -1 )
						return false;
            }
        }
        else                    // RLE decoded record
        {
			if ( ( byte = data -> getByte () ) == -1 )
				return false;

            for ( int i = 0; i < count; i++, x++ )
				if ( i & 1 )
					buf [x] = palette [byte & 0x0F];
                else
					buf [x] = palette [byte >> 4];
        }
    }

    return true;
}

bool	BmpDecoder :: loadRLE8 ( Data * data, TexImage * tex, dword * buf, dword palette [] )
{
	int	width  = tex -> getWidth  ();
	int	height = tex -> getHeight ();
	int	y      = height - 1;
    int x      = 0;
	int count, byte;

	memset ( buf, '\0', sizeof ( dword ) * width );

    for ( ; ; )
    {
		if ( ( count = data -> getByte () ) == -1 )
			return false;
        else
        if ( count == 0 )
        {
			if ( ( count = data -> getByte () ) == -1 )
				return false;
            else
            if ( count == 0 )       // end of line
            {
				tex -> putLine ( height - 1 - y, buf );

				memset ( buf, '\0', sizeof ( dword ) * width );

				y--;
                x = 0;
            }
            else
            if ( count == 1 )       // 0, 1 == end of RLE buf
				break;
            else
            if ( count == 2 )       // 0, 2 == delta record
            {
				tex -> putLine ( height - 1 - y, buf );

				memset ( buf, '\0', sizeof ( dword ) * width );

				y -= data -> getByte ();
                x += data -> getByte ();
            }
            else                    // start of an unencoded block
            {
				for ( int i = 0; i < count; i++, x ++ )
					if ( ( byte = data -> getByte () ) == -1 )
						return false;
                    else
						buf [x] = palette [byte];

                if ( count & 1 )
					if ( data -> getByte () == -1 )
						return false;
            }
        }
        else                    // RLE decoded record
        {
			if ( ( byte = data -> getByte () ) == -1 )
				return false;

            for ( int i = 0; i < count; i++, x++ )
				buf [x] = palette [byte];
        }
	}

    return true;
}
