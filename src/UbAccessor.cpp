//
// Simple wraper to set values into UBO
//
// Author: Alexey V. Boreskov, <steps3d@narod.ru>, <steps3s@gmail.com>
//

#include	<stdlib.h>
#include	"UbAccessor.h"
#include	"Program.h"
#include	"VertexBuffer.h"
#include	"vec2.h"
#include	"vec3.h"
#include	"vec4.h"
#include	"mat2.h"
#include	"mat3.h"
#include	"mat4.h"

UbAccessor :: UbAccessor  ()
{
	program     = 0;
	blockIndex  = 0;
	numUniforms = 0;
	blockSize   = 0;
	uniforms    = NULL;
	ptr         = NULL;
}
UbAccessor :: ~UbAccessor ()
{
	delete uniforms;
	
	if ( ptr != NULL )
		free ( ptr );
}

									// init and allocate data
bool	UbAccessor :: create ( const Program& p, const char * blockName )
{
	program     = p.getProgram ();
	blockIndex  = p.indexForUniformBlock ( blockName );
	blockSize   = p.uniformBlockSize ( blockIndex );
	numUniforms = p.uniformBlockActiveUniforms ( blockIndex );
	ptr         = (byte *) calloc ( blockSize, 1 );
	
	if ( !isOk () )
		return false;
		
	uniforms = new VarInfo [numUniforms];
	
	return true;
}
									// init mapping, can call setters
bool	UbAccessor :: updateBuffer   ( VertexBuffer& buffer )
{
	buffer.setData ( blockSize, ptr, GL_WRITE_ONLY );
	
	return true;
}
									// setters, init VarInfo on first access
bool	UbAccessor :: setVector ( const char * name, const vec2& v )
{
	if ( !isOk () || ptr == NULL )
		return false;
		
	int	i = locateName ( name );
	
	if ( i < 0 || i >= numUniforms )
		return false;
		
	if ( uniforms [i].type != GL_FLOAT_VEC2 )		// illegal data type
		return false;
				
	float * p = (float *)(ptr + uniforms [i].offset);
			
	p [0] = v.x;
	p [1] = v.y;
			
	return true;
}

bool	UbAccessor :: setVector ( const char * name, const vec3& v )
{
	if ( !isOk () || ptr == NULL )
		return false;
		
	int	i = locateName ( name );
	
	if ( i < 0 || i >= numUniforms )
		return false;
		
	if ( uniforms [i].type != GL_FLOAT_VEC3 )		// illegal data type
		return false;
				
	float * p = (float *)(ptr + uniforms [i].offset);
			
	p [0] = v.x;
	p [1] = v.y;
	p [2] = v.z;
			
	return true;
}

bool	UbAccessor :: setVector ( const char * name, const vec4& v )
{
	if ( !isOk () || ptr == NULL )
		return false;
		
	int	i = locateName ( name );
	
	if ( i < 0 || i >= numUniforms )
		return false;
		
	if ( uniforms [i].type != GL_FLOAT_VEC4 )		// illegal data type
		return false;
				
	float * p = (float *)(ptr + uniforms [i].offset);
			
	p [0] = v.x;
	p [1] = v.y;
	p [2] = v.z;
	p [3] = v.w;
			
	return true;
}

bool	UbAccessor :: setMatrix ( const char * name, const mat2& v )
{
	if ( !isOk () || ptr == NULL )
		return false;
		
	int	i = locateName ( name );
	
	if ( i < 0 || i >= numUniforms )
		return false;
		
	if ( uniforms [i].type != GL_FLOAT_MAT2 )		// illegal data type
		return false;
				
	float       * p       = (float *)(ptr + uniforms [i].offset);
	int		      stride  = uniforms [i].matrixStride;
	const float * m       = v.data ();					// is row-major

	if ( uniforms [i].isRowMajor )
	{
		p [0] = m [0];
		p [1] = m [1];
		
		p = (float *)(ptr + uniforms [i].offset + stride);
		
		p [0] = m [2];
		p [1] = m [3];	
	}
	else											// column-major
	{
		p [0] = m [0];
		p [1] = m [2];
		
		p = (float *)(ptr + uniforms [i].offset + stride);
		
		p [0] = m [1];
		p [1] = m [3];
	}
						
	return true;
}

bool	UbAccessor :: setMatrix ( const char * name, const mat3& v )
{
	if ( !isOk () || ptr == NULL )
		return false;
		
	int	i = locateName ( name );
	
	if ( i < 0 || i >= numUniforms )
		return false;
		
	if ( uniforms [i].type != GL_FLOAT_MAT3 )		// illegal data type
		return false;
				
	float       * p       = (float *)(ptr + uniforms [i].offset);
	int		      stride  = uniforms [i].matrixStride;
	const float * m       = v.data ();					// is row-major

	if ( uniforms [i].isRowMajor )
	{
		p [0] = m [0];
		p [1] = m [1];
		p [2] = m [2];
		
		p = (float *)(ptr + uniforms [i].offset + stride);
		
		p [0] = m [3];
		p [1] = m [4];	
		p [2] = m [5];	
		
		p = (float *)(ptr + uniforms [i].offset + 2*stride);
		
		p [0] = m [6];
		p [1] = m [7];	
		p [2] = m [8];	
	}
	else											// column-major
	{
		p [0] = m [0];
		p [1] = m [3];
		p [2] = m [6];
		
		p = (float *)(ptr + uniforms [i].offset + stride);
		
		p [0] = m [1];
		p [1] = m [4];
		p [2] = m [7];
		
		p = (float *)(ptr + uniforms [i].offset + 2*stride);
		
		p [0] = m [2];
		p [1] = m [5];
		p [2] = m [8];
	}	
			
	return true;
}

bool	UbAccessor :: setMatrix ( const char * name, const mat4& v )
{
	if ( !isOk () || ptr == NULL )
		return false;
		
	int	i = locateName ( name );
	
	if ( i < 0 || i >= numUniforms )
		return false;
		
	if ( uniforms [i].type != GL_FLOAT_MAT4 )		// illegal data type
		return false;
				
	float       * p       = (float *)(ptr + uniforms [i].offset);
	int		      stride  = uniforms [i].matrixStride;
	const float * m       = v.data ();					// is row-major

	if ( uniforms [i].isRowMajor )
	{
		p [0] = m [0];
		p [1] = m [1];
		p [2] = m [2];
		p [3] = m [3];
		
		p = (float *)(ptr + uniforms [i].offset + stride);
		
		p [0] = m [4];
		p [1] = m [5];	
		p [2] = m [6];	
		p [3] = m [7];
		
		p = (float *)(ptr + uniforms [i].offset + 2*stride);
		
		p [0] = m [8];
		p [1] = m [9];	
		p [2] = m [10];	
		p [3] = m [11];
		
		p = (float *)(ptr + uniforms [i].offset + 3*stride);
		
		p [0] = m [12];
		p [1] = m [13];	
		p [2] = m [14];	
		p [3] = m [15];
	}
	else											// column-major
	{
		p [0] = m [0];
		p [1] = m [4];
		p [2] = m [8];
		p [3] = m [12];
		
		p = (float *)(ptr + uniforms [i].offset + stride);
		
		p [0] = m [1];
		p [1] = m [5];
		p [2] = m [9];
		p [3] = m [13];
		
		p = (float *)(ptr + uniforms [i].offset + 2*stride);
		
		p [0] = m [2];
		p [1] = m [6];
		p [2] = m [10];
		p [3] = m [14];
		
		p = (float *)(ptr + uniforms [i].offset + 3*stride);
		
		p [0] = m [3];
		p [1] = m [7];
		p [2] = m [11];
		p [3] = m [15];
	}
			
	return true;
}

bool	UbAccessor :: setBool   ( const char * name, bool value )
{
	if ( !isOk () || ptr == NULL )
		return false;
		
	int	i = locateName ( name );
	
	if ( i < 0 || i >= numUniforms )
		return false;
		
	if ( uniforms [i].type != GL_UNSIGNED_INT )		// illegal data type
		return false;
				
	* (GLuint *)(ptr + uniforms [i].offset) = (GLuint) value;
			
	return true;
}

bool	UbAccessor :: setInt    ( const char * name, int value )
{
	if ( !isOk () || ptr == NULL )
		return false;
		
	int	i = locateName ( name );
	
	if ( i < 0 || i >= numUniforms )
		return false;
		
	if ( uniforms [i].type != GL_UNSIGNED_INT )		// illegal data type
		return false;
				
	* (int *)(ptr + uniforms [i].offset) = value;	// XXX: 32 bits
			
	return true;
}

bool	UbAccessor :: setUint   ( const char * name, GLuint value )
{
	if ( !isOk () || ptr == NULL )
		return false;
		
	int	i = locateName ( name );
	
	if ( i < 0 || i >= numUniforms )
		return false;
		
	if ( uniforms [i].type != GL_UNSIGNED_INT )		// illegal data type
		return false;
				
	* (GLuint *)(ptr + uniforms [i].offset) = value;
			
	return true;
}

bool	UbAccessor :: setFloat  ( const char * name, float value )
{
	if ( !isOk () || ptr == NULL )
		return false;
		
	int	i = locateName ( name );
	
	if ( i < 0 || i >= numUniforms )
		return false;
		
	if ( uniforms [i].type != GL_FLOAT )		// illegal data type
		return false;
				
	* (float *)(ptr + uniforms [i].offset) = value;
			
	return true;
}

				// locates the name or fills data for new uniform with given name
int		UbAccessor :: locateName ( const char * name )
{
	int	i;
	
	for ( i = 0; i < numUniforms && !uniforms [i].name.empty (); i++ )
		if ( uniforms [i].name == name )
			return i;
		
	if ( i >= numUniforms )
		return -1;
		
				// get data for this uniform
	uniforms [i].name = name;

	glGetUniformIndices   ( program, 1, &name, (GLuint *)&uniforms [i].index );
	glGetActiveUniformsiv ( program, 1, (GLuint *)&uniforms [i].index, GL_UNIFORM_SIZE,          (GLint *)&uniforms [i].size         );
	glGetActiveUniformsiv ( program, 1, (GLuint *)&uniforms [i].index, GL_UNIFORM_OFFSET,        (GLint *)&uniforms [i].offset       );
	glGetActiveUniformsiv ( program, 1, (GLuint *)&uniforms [i].index, GL_UNIFORM_TYPE,          (GLint *)&uniforms [i].type         );
	
	if ( uniforms [i].size > 0 )			// an array
		glGetActiveUniformsiv ( program, 1, (GLuint *)&uniforms [i].index, GL_UNIFORM_ARRAY_STRIDE,  (GLint *)&uniforms [i].arrayStride  );
	else
		uniforms [i].arrayStride = 0;
	
	if ( uniforms [i].type == GL_FLOAT_MAT2 || uniforms [i].type == GL_FLOAT_MAT3 || uniforms [i].type == GL_FLOAT_MAT4 )
	{
		glGetActiveUniformsiv ( program, 1, (GLuint *)&uniforms [i].index, GL_UNIFORM_MATRIX_STRIDE, (GLint *)&uniforms [i].matrixStride );
		glGetActiveUniformsiv ( program, 1, (GLuint *)&uniforms [i].index, GL_UNIFORM_IS_ROW_MAJOR,  (GLint *)&uniforms [i].isRowMajor   );
	}
	else							// no sense for non-matrix types
	{
		uniforms [i].matrixStride = 0;
		uniforms [i].isRowMajor   = 0;
	}
	
	return i;
}
