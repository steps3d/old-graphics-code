//
// Basic wrapper for OpenGL sync object
//
// Author: Alexey V. Boreskov, <steps3d@gmail.com>, <steps3d@narod.ru>
//

#pragma once
#ifndef	__SYNC_OBJECT__
#define	__SYNC_OBJECT__

#include	<GL/glew.h>

class Sync
{
	GLsync	sync;
	
								// disable copying
	Sync ( const Sync& );
	Sync& operator = ( const Sync& );
	
public:
	Sync  ();
	~Sync ();	

	bool isOk () const
	{
		return sync != 0;
	}

	void destroy ();

	void setFence ( GLenum condition = GL_SYNC_GPU_COMMANDS_COMPLETE, 
			GLbitfield flags = 0 );

	GLenum clientWait ( GLuint64 timeout, GLbitfield flags = GL_SYNC_FLUSH_COMMANDS_BIT );
	void   serverWait ( GLuint64 timeout = GL_TIMEOUT_IGNORED, GLbitfield flags = 0 );

	static GLint64	maxServerTimeout ();
};

#endif
