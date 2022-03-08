
#include	<stdio.h>
#include	<map>
#include	<string>
#include	<list>
#include	"Resources.h"
#include	"Data.h"
#include	"TexImage.h"
#include    "Texture.h"
#include    "BmpDecoder.h"
#include    "TgaDecoder.h"
#include    "DdsDecoder.h"
#include    "JpegDecoder.h"
#include    "PngDecoder.h"
#include	"stringUtils.h"
#include	"FileSystem.h"
#include	"ZipFileSystem.h"

using namespace std;

bool	doDdsFlip    = true;	// whether loader should vertically flip dds image
bool	doNotDdsFlip = false;	// whether loader should vertically flip not-dds (bmp/png/tga/jpg) images

list <FileSystem *>             fileSystems;			// where we can take resources
map  <string, TextureDecoder *>	decoders;				// how to decode resource into textures

static	void	initResources ()
{
	static	bool	inited = false;

	if ( inited )
		return;

	addDecoder ( "bmp",  new BmpDecoder  );
	addDecoder ( "dds",  new DdsDecoder  );
	addDecoder ( "jpg",  new JpegDecoder );
	addDecoder ( "jpeg", new JpegDecoder );
	addDecoder ( "png",  new PngDecoder  );
	addDecoder ( "tga",  new TgaDecoder  );

	inited = true;
}

Data * getFile ( const string& fileName )
{
	initResources ();								// it's safe to place it anywhere

    if ( fileExist ( fileName ) )
        return new Data ( fileName );

    Data * data;

    for ( list <FileSystem *> :: iterator it = fileSystems.begin (); it != fileSystems.end (); ++it )
        if ( ( data = (*it) -> getFile ( fileName ) ) != NULL )
            return data;
			
    return NULL;
}

bool    addFileSystem ( FileSystem * fileSystem )
{
    if ( !fileSystem -> isOk () )
    {
        delete fileSystem;

        return false;
    }

    fileSystems.push_back ( fileSystem );

    return true;
}


bool    addZipFileSystem ( const string& fileName )
{
    return addFileSystem ( new ZipFileSystem ( fileName ) );
}

/*
bool    addRarFileSystem ( const char * fileName )
{
    return addFileSystem ( new RarFileSystem ( fileName ) );
}
*/

bool	addSearchPath ( const string& path )
{
	return addFileSystem ( new Folder ( path ) );
}


bool	addDecoder ( const string& ext, TextureDecoder * decoder )
{
	string									   extLower = stringLower ( ext );
	map <string, TextureDecoder *> :: iterator it       = decoders.find ( extLower );

	if ( it != decoders.end () )
		return false;

	decoders [extLower] = decoder;

	return true;
}

TexImage * loadTexImage ( const string& fileName )
{
    string	   ext   = stringLower ( getExtension ( fileName ) );
	Data     * data  = getFile ( fileName );
	TexImage * image = NULL;
	
    if ( data == NULL || data -> getLength () < 1 )
    {
        fprintf ( stderr, "Cannot load \"%s\"\n", fileName.c_str () );

        return NULL;
    }

	map <string, TextureDecoder *> :: iterator it = decoders.find ( ext );

	if ( it != decoders.end () )
		image = it -> second -> load ( data );

    if ( image == NULL )
        fprintf ( stderr, "No loader for %s !!!\n", fileName.c_str () );

	if ( !image -> isOk () )
	{
        fprintf ( stderr, "Incorrect image %s !!!\n", fileName.c_str () );
		
		delete image;
		
		image = NULL;
	}
	
    delete data;

    return image;
}

