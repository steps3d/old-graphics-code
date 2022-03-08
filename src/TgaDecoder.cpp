//
// Class to load images from TGA files
//
// Author: Alexey V. Boreskov <steps3d@gmail.com>, <steps3d@narod.ru>
//

#include    <stdio.h>
#include    <memory.h>
#include    "TgaDecoder.h"
#include    "Data.h"
#include    "TexImage.h"
#include    "TgaHeader.h"

TexImage * TgaDecoder :: load ( Data * data )
{
    TgaHeader * hdr = (TgaHeader *) data -> getPtr ();

    if ( hdr -> imageType != 0 && hdr -> imageType != 1 && hdr -> imageType != 2 &&
         hdr -> imageType != 3 && hdr -> imageType != 9 && hdr -> imageType != 10 &&
         hdr -> imageType != 11 )                    // unsupported types
        return NULL;

    if ( hdr -> pixelSize != 32 && hdr -> pixelSize != 24 && hdr -> pixelSize != 16 && hdr -> pixelSize != 15 && hdr -> pixelSize != 8 )
        return NULL;

    int     width  = hdr -> width;
    int     height = hdr -> height;
    int     offs   = sizeof ( TgaHeader );
    dword   palette [256];
    int     numComponents = 3;

    if ( hdr -> pixelSize  == 32 || (hdr -> pixelSize == 8 && hdr -> colormapEntrySize == 32) )
        numComponents = 4;

    if ( hdr -> idLength != 0 )
        offs += hdr -> idLength;

    byte  * bits = offs + (byte *) data -> getPtr ();

    data -> seekAbs ( offs );

    TexImage * tex = TexImage :: new2D ( width, height, numComponents );
    dword    * buf = new dword [width];

    if ( hdr -> imageType == 1 || hdr -> imageType == 9 )       // color-mapped image
    {
        if ( hdr -> colormapEntrySize == 15 || hdr -> colormapEntrySize == 16 )
        {
            byte  * ptr = (byte *) bits;
            int     a, b;

            for ( int i = 0; i < hdr -> colormapLength; i++ )
            {
                a = *ptr++;
                b = *ptr++;

                palette [i] = makeRgba ( a & 0x1F, ((b & 0x03) << 3) | ((a & 0xE0) >> 5),  (b & 0x7C) >> 2 );
            }

            bits += 2 * hdr -> colormapLength;

            data -> seekCur ( 2 * hdr -> colormapLength );
        }
        else
        if ( hdr -> colormapEntrySize == 24 )
        {
            byte * pal = bits;

            for ( int i = 0; i < hdr -> colormapLength; i++, pal += 3 )
                palette [i] = makeRgba ( pal [2], pal [1], pal [0] );

            bits += 3 * hdr -> colormapLength;

            data -> seekAbs ( offs + 3 * hdr -> colormapLength );
        }
        else
        if ( hdr -> colormapEntrySize == 32 )
        {
            memcpy ( palette, bits, sizeof ( dword ) * hdr -> colormapLength );

            bits += 4 * hdr -> colormapLength;

            data -> seekCur ( 4 * hdr -> colormapLength );
        }
    }
    else                                                            // if not platted, but 8 bpp
    if ( hdr -> colormapEntrySize == 0 && hdr -> pixelSize == 8 )   // grey-scale image
    {
                                                                    // create greyscale identity palette
        for ( int i = 0; i < 256; i++ )
            palette [i] = makeRgba ( i, i, i, i );
    }

    bool    code;
    bool    vertInvert = (hdr -> imageDesc & 0x20) != 0;

    if ( hdr -> imageType >= 1 && hdr -> imageType <= 3 )       // unencoded image
    {
        if ( hdr -> pixelSize == 8 )
            code = loadRGB8  ( data, tex, buf, palette, vertInvert );
        else
        if ( hdr -> pixelSize == 24 )
            code = loadRGB24 ( data, tex, buf, vertInvert );
        else
            code = loadRGB32 ( data, tex, buf, vertInvert );
    }
    else
    if ( hdr -> imageType >= 9 && hdr -> imageType <= 11 )      // RLE image
    {
        if ( hdr -> pixelSize == 8 )
            code = loadRLE8  ( data, tex, buf, palette, vertInvert );
        else
        if ( hdr -> pixelSize == 24 )
            code = loadRLE24 ( data, tex, buf, vertInvert );
        else
            code = loadRLE32 ( data, tex, buf, vertInvert );
    }

    delete buf;

    if ( !code )
    {
        delete tex;

        return NULL;
    }

    return tex;
}

bool    TgaDecoder :: loadRGB8 ( Data * data, TexImage * tex, dword * buf, dword palette [], bool vertInvert )
{
    int width  = tex -> getWidth  ();
    int height = tex -> getHeight ();

    for ( int y = 0; y < height; y++ )
    {
        memset ( buf, '\0', width * sizeof ( dword ) );

        for ( int x = 0; x < width; x++ )
        {
            int val = data -> getByte ();

            if ( val == -1 )
                return false;

            buf [x] = palette [val];
        }

        tex -> putLine ( vertInvert ? height - 1 - y : y, buf );
    }

    return true;
}

bool    TgaDecoder :: loadRGB15 ( Data * data, TexImage * tex, dword * buf, bool vertInvert )
{
    int width  = tex -> getWidth  ();
    int height = tex -> getHeight ();
    int a, b;

    for ( int y = 0; y < height; y++ )
    {
        memset ( buf, '\0', width * sizeof ( dword ) );

        for ( int x = 0; x < width; x++ )
        {
            a = data -> getByte ();
            b = data -> getByte ();

            if ( a == -1 || b == -1 )
                return false;

            buf [x] = makeRgba ( a & 0x1F, ((b & 0x03) << 3) | ((a & 0xE0) >> 5), (b & 0x7C) >> 2 );
        }

        tex -> putLine ( vertInvert ? height - 1 - y : y, buf );
    }

    return true;
}

bool    TgaDecoder :: loadRGB24 ( Data * data, TexImage * tex, dword * buf, bool vertInvert )
{
    int width  = tex -> getWidth  ();
    int height = tex -> getHeight ();
    int red, green, blue;

    for ( int y = 0; y < height; y++ )
    {
        memset ( buf, '\0', width * sizeof ( dword ) );

        for ( int x = 0; x < width; x++ )
        {
            blue  = data -> getByte ();
            green = data -> getByte ();
            red   = data -> getByte ();

            if ( blue == -1 || green == -1 || blue == -1 )
                return false;

            buf [x] = makeRgba ( red, green, blue );
        }

        tex -> putLine ( vertInvert ? height - 1 - y : y, buf );
    }

    return true;
}

bool    TgaDecoder :: loadRGB32 ( Data * data, TexImage * tex, dword * buf, bool vertInvert )
{
    int width  = tex -> getWidth  ();
    int height = tex -> getHeight ();
    int red, green, blue, alpha;

    for ( int y = 0; y < height; y++ )
    {
        memset ( buf, '\0', width * sizeof ( dword ) );

        for ( int x = 0; x < width; x++ )
        {
            blue  = data -> getByte ();
            green = data -> getByte ();
            red   = data -> getByte ();
            alpha = data -> getByte ();

            if ( blue == -1 || green == -1 || blue == -1 || alpha == -1 )
                return false;

            buf [x] = makeRgba ( red, green, blue, alpha );
        }

        tex -> putLine ( vertInvert ? height - 1 - y : y, buf );
    }

    return true;
}

bool    TgaDecoder :: loadRLE8 ( Data * data, TexImage * tex, dword * buf, dword palette [], bool vertInvert )
{
    int width  = tex -> getWidth  ();
    int height = tex -> getHeight ();
    int val;
    int packetHeader, packetSize;

    memset ( buf, '\0', width * sizeof ( dword ) );

    for ( int y = 0; y < height; y++ )
    {
        for ( int x = 0; x < width; )
        {
            packetHeader = data -> getByte ();
            packetSize   = (packetHeader & 0x7F) + 1;

            if ( packetHeader == -1 )
                return false;

            if ( packetHeader & 0x80 )              // run-length packet
            {
                val = data -> getByte ();

                if ( val == -1 )
                    return false;

                for ( int c = 0; c < packetSize; c++ )
                {
                    buf [x++] = palette [val];

                    if ( x >= width )
                    {
                        tex -> putLine ( vertInvert ? height - 1 - y : y, buf );

                        memset ( buf, '\0', width * sizeof ( dword ) );

                        x = 0;

                        if ( ++y >= height )
                            return true;
                    }
                }
            }
            else                                    // non run-length packet
            {
                for ( int i = 0; i < packetSize; i++ )
                {
                    val = data -> getByte ();

                    if ( val == -1 )
                        return false;

                    buf [x++] = palette [val];

                    if ( x >= width )
                    {
                        tex -> putLine ( vertInvert ? height - 1 - y : y, buf );

                        memset ( buf, '\0', width * sizeof ( dword ) );

                        x = 0;

                        if ( ++y >= height )
                            return true;
                    }
                }
            }

        }
    }

    return true;
}

bool    TgaDecoder :: loadRLE15 ( Data * data, TexImage * tex, dword * buf, bool vertInvert )
{
    int     width  = tex -> getWidth  ();
    int     height = tex -> getHeight ();
    int     a, b;
    int     packetHeader, packetSize;
    dword   color;

    memset ( buf, '\0', width * sizeof ( dword ) );

    for ( int y = 0; y < height; y++ )
    {
        for ( int x = 0; x < width; )
        {
            packetHeader = data -> getByte ();
            packetSize   = (packetHeader & 0x7F) + 1;

            if ( packetHeader == -1 )
                return false;

            if ( packetHeader & 0x80 )              // run-length packet
            {
                a = data -> getByte ();
                b = data -> getByte ();

                if ( a == -1 || b == -1 )
                    return false;

                color = makeRgba ( a & 0x1F, ((b & 0x03) << 3) + ((a & 0xE0) >> 5), (b & 0x7C) >> 2 );

                for ( int c = 0; c < packetSize; c++ )
                {
                    buf [x++] = color;

                    if ( x >= width )
                    {
                        tex -> putLine ( vertInvert ? height - 1 - y : y, buf );

                        memset ( buf, '\0', width * sizeof ( dword ) );

                        x = 0;

                        if ( ++y >= height )
                            return true;
                    }
                }
            }
            else                                    // non run-length packet
            {
                for ( int i = 0; i < packetSize; i++ )
                {
                    a = data -> getByte ();
                    b = data -> getByte ();

                    if ( a == -1 || b == -1 )
                        return false;

                    buf [x++] = makeRgba ( a & 0x1F, ((b & 0x03) << 3) + ((a & 0xE0) >> 5), (b & 0x7C) >> 2 );

                    if ( x >= width )
                    {
                        tex -> putLine ( vertInvert ? height - 1 - y : y, buf );

                        memset ( buf, '\0', width * sizeof ( dword ) );

                        x = 0;

                        if ( ++y >= height )
                            return true;
                    }
                }
            }

        }
    }

    return true;
}

bool    TgaDecoder :: loadRLE24 ( Data * data, TexImage * tex, dword * buf, bool vertInvert )
{
    int     width  = tex -> getWidth  ();
    int     height = tex -> getHeight ();
    int     packetHeader, packetSize;
    int     red, green, blue;
    dword   color;

    memset ( buf, '\0', width * sizeof ( dword ) );

    for ( int y = 0; y < height; y++ )
    {
        for ( int x = 0; x < width; )
        {
            packetHeader = data -> getByte ();
            packetSize   = (packetHeader & 0x7F) + 1;

            if ( packetHeader == -1 )
                return false;

            if ( packetHeader & 0x80 )              // run-length packet
            {
                blue  = data -> getByte ();
                green = data -> getByte ();
                red   = data -> getByte ();

                if ( red == -1 || green == -1 || blue == -1 )
                    return false;

                color = makeRgba ( red, green, blue );

                for ( int c = 0; c < packetSize; c++ )
                {
                    buf [x++] = color;

                    if ( x >= width )               // wrapped on next scan line
                    {
                        tex -> putLine ( vertInvert ? height - 1 - y : y, buf );

                        memset ( buf, '\0', width * sizeof ( dword ) );

                        x = 0;

                        if ( ++y >= height )
                            return true;
                    }
                }
            }
            else                                    // non run-length packet
            {
                for ( int i = 0; i < packetSize; i++ )
                {
                    blue  = data -> getByte ();
                    green = data -> getByte ();
                    red   = data -> getByte ();

                    if ( red == -1 || green == -1 || blue == -1 )
                        return false;

                    buf [x++] = makeRgba ( red, green, blue );

                    if ( x >= width )
                    {
                        tex -> putLine ( vertInvert ? height - 1 - y : y, buf );

                        memset ( buf, '\0', width * sizeof ( dword ) );

                        x = 0;

                        if ( ++y >= height )
                            return true;
                    }
                }
            }

        }
    }

    return true;
}

bool    TgaDecoder :: loadRLE32 ( Data * data, TexImage * tex, dword * buf, bool vertInvert )
{
    int     width  = tex -> getWidth  ();
    int     height = tex -> getHeight ();
    int     packetHeader, packetSize;
    int     red, green, blue, alpha;
    dword   color;

    for ( int y = 0; y < height; y++ )
    {
        for ( int x = 0; x < width; )
        {
            packetHeader = data -> getByte ();
            packetSize   = (packetHeader & 0x7F) + 1;

            if ( packetHeader == -1 )
                return false;

            if ( packetHeader & 0x80 )              // run-length packet
            {
                blue  = data -> getByte ();
                green = data -> getByte ();
                red   = data -> getByte ();
                alpha = data -> getByte ();

                if ( red == -1 || green == -1 || blue == -1 || alpha == -1 )
                    return false;

                color = makeRgba ( red, green, blue, alpha );

                for ( int c = 0; c < packetSize; c++ )
                {
                    buf [x++] = color;

                    if ( x >= width )
                    {
                        tex -> putLine ( vertInvert ? height - 1 - y : y, buf );

                        memset ( buf, '\0', width * sizeof ( dword ) );

                        x = 0;

                        if ( ++y >= height )
                            return true;
                    }
                }
            }
            else                                    // non run-length packet
            {
                for ( int i = 0; i < packetSize; i++ )
                {
                    blue  = data -> getByte ();
                    green = data -> getByte ();
                    red   = data -> getByte ();
                    alpha = data -> getByte ();

                    if ( red == -1 || green == -1 || blue == -1 || alpha == -1 )
                        return false;

                    buf [x++] = makeRgba ( red, green, blue, alpha );

                    if ( x >= width )
                    {
                        tex -> putLine ( vertInvert ? height - 1 - y : y, buf );

                        memset ( buf, '\0', width * sizeof ( dword ) );

                        x = 0;

                        if ( ++y >= height )
                            return true;
                    }
                }
            }

        }
    }

    return true;
}
