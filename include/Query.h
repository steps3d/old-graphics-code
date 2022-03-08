//
// Basic wrapper for OpenGL query objects
//
// Author: Alexey V. Boreskov, <steps3d@gmail.com>, <steps3d@narod.ru>
//

#pragma once
#ifndef	__GL_QUERY__
#define	__GL_QUERY__

#include	<GL/glew.h>

class Query
{
	GLuint	id;
	GLenum	target;
	
								// disable copying
	Query ( const Query& );
	Query& operator = ( const Query& );
	
public:
	Query ();
	~Query ();

	void beginQuery ( GLenum theTarget )
	{
		target = theTarget;
		glBeginQuery ( target, id );
	}

	void endQuery ()
	{
		glEndQuery ( target );
	}

	void beginQueryIndexed ( GLenum theTarget, int index )
	{
		target = theTarget;
		glBeginQuery ( target, id );
	}

	void endQueryIndexed ( int index )
	{
		glEndQueryIndexed ( target, index );
	}

	void	timestamp ()
	{
		target = GL_TIMESTAMP;

		glQueryCounter ( target, id );
	}

	bool	 isReady     () const;		// result is ready ?
	GLint	 getResult   () const;
	GLuint64 getResult64 () const;

	static GLuint	activeQuery ( GLenum theTarget );
	static int	counterBits ( GLenum theTarget );
};

#endif
