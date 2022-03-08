
#include	"Sync.h"

Sync :: Sync ()
{
	sync = 0;
}

Sync :: ~Sync ()
{
	destroy ();
}

void Sync :: destroy ()
{
	if ( sync )
		glDeleteSync ( sync );

	sync = 0;
}

void Sync :: setFence ( GLenum condition, GLbitfield flags )
{
	sync = glFenceSync ( condition, flags );
}

GLenum Sync :: clientWait ( GLuint64 timeout, GLbitfield flags )
{
	return glClientWaitSync ( sync, flags, timeout );
}

void   Sync :: serverWait ( GLuint64 timeout, GLbitfield flags )
{
	glWaitSync ( sync, flags, timeout );
}

GLint64	Sync :: maxServerTimeout ()
{
	GLint64 maxTimeout;

	glGetInteger64v ( GL_MAX_SERVER_WAIT_TIMEOUT, &maxTimeout );

	return maxTimeout;
}
