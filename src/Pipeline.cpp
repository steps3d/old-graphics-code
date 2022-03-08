//
// Wrapper for OpenGL pipeline object
//
// Author: Alexey V. Boreskov, <steps3d@gmail.com>, <steps3d@narod.ru>
//


#include	<stdlib.h>			// for malloc
#include	"Pipeline.h"
#include	"Program.h"
#include	"glUtilities.h"

Pipeline :: Pipeline ()
{
	id = 0;
}

Pipeline :: ~Pipeline ()
{
}

void	Pipeline :: destroy ()
{
	if ( id != 0 )
		glDeleteProgramPipelines ( 1, &id );

	id = 0;
}

bool	Pipeline :: create ()
{
	glGenProgramPipelines ( 1, &id );
	
	return id != 0;
}

bool	Pipeline :: bind ()
{
	if ( id == 0 )
		return false;
		
	glBindProgramPipeline ( id );
	
	return true;
}

bool	Pipeline :: unbind ()
{
	glBindProgramPipeline ( 0 );
	
	return true;
}

bool	Pipeline :: setStage ( GLbitfield mask, Program * program )
{
	if ( id == 0 || program == NULL || !program -> isOk () )
		return false;
		
	glUseProgramStages ( id, mask, program -> getProgram () );
	
	return true;
}

bool	Pipeline :: setAllStages ( Program * program )
{
	return setStage ( GL_ALL_SHADER_BITS, program );
}

bool	Pipeline :: setActiveProgram ( Program * program )
{
	if ( id == 0 || program == NULL || !program -> isOk () )
		return false;
		
	glActiveShaderProgram ( id, program -> getProgram () );
	
	return true;
}

bool	Pipeline :: validate ()
{
	GLint	status;
	
	glValidateProgramPipeline ( id );
	glGetProgramPipelineiv    ( id, GL_VALIDATE_STATUS, &status );
	
	return status != 0;
}

string	Pipeline :: getLog () const
{
	int		len;
	string	log;
	
	glGetProgramPipelineiv ( id, GL_INFO_LOG_LENGTH, &len );
	
	if ( len < 1 )
		return "";
		
	char * buf = (char *) malloc ( len + 1 );
	
	glGetProgramPipelineInfoLog ( id, len+1, NULL, buf );
	
	log = buf;
	
	free ( buf );
	
	return log;
}

int 	Pipeline :: active ()		// id of active pipeline object
{
	int	pipeline;
	
	glGetIntegerv ( GL_PROGRAM_PIPELINE_BINDING, &pipeline );
	
	return pipeline;
}
