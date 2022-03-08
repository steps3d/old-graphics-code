//
// Vertex Array Object wrapper
// Author: Alexey V. Boreskov, <steps3d@gmail.com>, <steps3d@narod.ru>
//

#pragma once
#ifndef	__VERTEX_ARRAY_OBJECT__
#define	__VERTEX_ARRAY_OBJECT__

#include	<GL/glew.h>

class	VertexArray
{
	GLuint	id;
	
public:
	VertexArray ()
	{
		id = 0;
	}
	
	~VertexArray ()
	{
		glDeleteVertexArrays ( 1, &id );
	}
	
	bool	isOk () const
	{
		return glIsVertexArray ( id );
	}
	
	GLuint 	getId () const
	{
		return id;
	}
	
	void	create ()
	{
		glGenVertexArrays ( 1, &id );
	}
	
	void	bind ()
	{
		glBindVertexArray ( id );
	}
	
	void	unbind ()
	{
		glBindVertexArray ( 0 );
	}
};

#endif
