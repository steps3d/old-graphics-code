//
// Basic wrapper for OpenGL VBO extension
//
// Author: Alexey V. Boreskov, <steps3d@gmail.com>, <steps3d@narod.ru>
//

#ifndef	__VERTEX_BUFFER_CLASS__
#define	__VERTEX_BUFFER_CLASS__

#include	<GL/glew.h>
#include	<string.h>			// for NULL

class	VertexBuffer
{
	GLuint	id;
	GLenum	target;
	bool	ok;
	
public:
	VertexBuffer  ();
	~VertexBuffer ();

	bool	isOk () const
	{
		return id != 0;
	}
	
	GLenum 	getId () const
	{
		return id;
	}
	
	GLenum	currentTarget () const
	{
		return target;
	}
	
	bool	create ();
	void	bind   ( GLenum theTarget );
	void	unbind ();
	
	void	setData    ( unsigned size, const void * ptr, GLenum usage );
	void	setSubData ( unsigned offs, unsigned size, const void * ptr );
	void	getSubData ( unsigned offs, unsigned size, void * ptr );
	void  * map        ( GLenum access );
	bool	unmap      ();

	void	clear ()
	{
		glBufferData ( target, 0, NULL, 0 /*usage*/ );
	}
	
								// for transform feedback or uniform buffers only
	void	bindBase  ( GLenum theTarget, int index );
	void	bindRange ( GLenum theTarget, int index, GLintptr offset, GLsizeiptr size );
	
	static	bool	isSupported ();
};

#endif
