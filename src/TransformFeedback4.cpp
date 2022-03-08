//
// Wrapper for transform feedback object for GL 4.1
//
// Author: Alexey V. Boreskov, <steps3d@narod.ru>, <steps3s@gmail.com>
//

#include	"TransformFeedback4.h"

int TransformFeedback4 :: maxInterlavedAttribs ()
{
	int maxSize;
		
	glGetIntegerv ( GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS, &maxSize );
		
	return maxSize;
}

int TransformFeedback4 :: maxSeparateAttribs ()
{
	int maxSize;
	
	glGetIntegerv ( GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS, &maxSize );
		
	return maxSize;
}

int TransformFeedback4 :: maxBuffers ()
{
	int	maxBuffers;

	glGetIntegerv ( GL_MAX_TRANSFORM_FEEDBACK_BUFFERS, &maxBuffers );
		
	return maxBuffers;
}

