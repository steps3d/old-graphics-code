//
//
//

#pragma once
#ifndef	__RESOURCES__
#define	__RESOURCES__

#include	<string>

using namespace std;

class	Data;
class	TextureDecoder;
class	TexImage;
class	FileSystem;

Data     * getFile      ( const string& fileName );
bool	   addDecoder   ( const string& ext, TextureDecoder * decoder );
TexImage * loadTexImage ( const string& fileName );

							// add file system
bool		addFileSystem ( FileSystem * fileSystem );

							// add new search path for resources
bool		addSearchPath ( const string& path );

							// add zip file source
bool		addZipFileSystem ( const string& fileName );

extern	bool	doDdsFlip;			// whether loader should vertically flip dds image
//extern	bool	doNotDdsFlip;	// whether loader should vertically flip not-dds (bmp/png/tga/jpg) images

#endif
