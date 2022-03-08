//
// Wrapper for transform feedback object for GL 4.1
//
// Author: Alexey V. Boreskov, <steps3d@narod.ru>, <steps3s@gmail.com>
//

#pragma  once
#include	<GL/glew.h>

class TransformFeedback4
{
	GLuint	id;
	
	TransformFeedback4 ( const TransformFeedback4& );
	TransformFeedback4& operator = ( const TransformFeedback4& );
	
public:
	TransformFeedback4 ()
	{
		id = 0;
	}
	
	~TransformFeedback4 ()
	{
		destroy ();
	}
	
	bool	isOk () const
	{
		return id != 0;
	}
	
	void	destroy ()
	{
		if ( id != 0 )
			glDeleteTransformFeedbacks ( 1, &id );

		id = 0;
	}

	void	create ()
	{
		glGenTransformFeedbacks ( 1, &id );
	}
	
	void	bind ()
	{
		glBindTransformFeedback ( GL_TRANSFORM_FEEDBACK, id );
	}
	
	void	unbind ()
	{
		glBindTransformFeedback ( GL_TRANSFORM_FEEDBACK, 0 );
	}
	
	bool	begin ( GLenum primitiveMode  )
	{
		if ( !isOk () ) 
			return false;
			
		glBeginTransformFeedback ( primitiveMode );
		
		return true;
	}
	
	void	end ()
	{
		glEndTransformFeedback ();
	}
	
	void	pause ()
	{
		glPauseTransformFeedback ();
	}
	
	void	resume ()
	{
		glResumeTransformFeedback ();
	}
	
	void draw ( GLenum mode )
	{
		glDrawTransformFeedback ( mode, id );
	}
	
	void	drawStream ( GLenum mode, GLuint stream )
	{
		glDrawTransformFeedbackStream ( mode, id, stream );
	}
	
	static int maxInterlavedAttribs ();
	static int maxSeparateAttribs   ();
	static int maxBuffers           ();
};
