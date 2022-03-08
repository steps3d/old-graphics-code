//
// Class to load images from TGA files
//
// Author: Alexey V. Boreskov <steps3d@gmail.com>, <steps3d@narod.ru>
//

#include    <stdio.h>
#include    <memory.h>

#ifdef	_WIN32
    #include	<fcntl.h>
    #include	<io.h>
    #include	<sys/stat.h>
#else
    #include	<unistd.h>
    #include	<sys/types.h>
    #include	<sys/stat.h>
    #include	<fcntl.h>
    #define	O_BINARY	0
#endif

#include    "TgaEncoder.h"
#include    "TgaHeader.h"

bool	TgaEncoder :: encode ( byte * image, int width, int height, int numComponents, const char * fileName ) const
{
    TgaHeader   hdr;
	int			file  = open ( fileName, O_WRONLY | O_BINARY | O_CREAT | O_TRUNC, S_IWRITE );
	int			numPixels = width * height;
    byte      * pixel = image;
    byte    	temp;

	if ( file == -1 )
		return false;

    memset ( &hdr, '\0', sizeof ( hdr ) );

    hdr.imageType = 2;              // unencoded image
    hdr.width     = width;
    hdr.height    = height;
    hdr.pixelSize = numComponents * 8;

                                            // swap R and B channels
    for ( int i = 0; i < numPixels; i++, pixel += numComponents )
    {
        temp      = pixel [0];
        pixel [0] = pixel [2];
        pixel [2] = temp;
    }
 
    write ( file, &hdr, sizeof ( hdr ) );
    write ( file, image, numPixels * 3 );
    close ( file );
	
	return true;
}
