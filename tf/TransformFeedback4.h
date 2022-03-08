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
	
public:
	TransformFeedback4 ()
	{
		id = 0;
	}
	
	~TransformFeedback4 ()
	{
		if ( id != 0 )
			glDeleteTransformFeedbacks ( 1, &id );
	}
	
	bool	isOk () const
	{
		return id != 0;
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
};
