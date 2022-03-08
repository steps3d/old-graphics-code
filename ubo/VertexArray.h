//
// Vertex Array Object wrapper
// Author: Alexey V. Boreskov, <steps3d@gmail.com>, <steps3d@narod.ru>
//

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
		glBindVertexArray ( id );
	}
};

#endif
