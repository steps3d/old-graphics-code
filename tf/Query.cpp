
#include	"Query.h"

Query :: Query ()
{
	target = 0;
	
	glGenQueries ( 1, &id );
}

Query :: ~Query ()
{
	glDeleteQueries ( 1, &id );
}

bool	 Query :: isReady () const
{
	GLint	ready;

	glGetQueryObjectiv ( id, GL_QUERY_RESULT_AVAILABLE, &ready );

	return ready != 0;
}

GLint	 Query :: getResult () const
{
	GLint result;

	glGetQueryObjectiv ( id, GL_QUERY_RESULT, &result );

	return result;
}

GLuint64 Query :: getResult64 () const
{
	GLuint64 result;

	glGetQueryObjectui64v ( id, GL_QUERY_RESULT, &result );

	return result;
}
GLuint	Query :: activeQuery ( GLenum theTarget )
{
	GLint	active;

	glGetQueryiv ( theTarget, GL_CURRENT_QUERY, &active );

	return active;
}

int	Query :: counterBits ( GLenum theTarget )
{
	GLint	bits;

	glGetQueryiv ( theTarget, GL_QUERY_COUNTER_BITS, &bits );

	return bits;
}
