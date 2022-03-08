//
// Simple wraper to set values into UBO
//
// Author: Alexey V. Boreskov, <steps3d@narod.ru>, <steps3s@gmail.com>
//

#pragma once
#ifndef	__UBO_ACCESSOR__
#define	__UBO_ACCESSOR__

#include	<string>
#include	<GL/glew.h>
#include	"TypeDefs.h"

class	vec2;
class	vec3;
class	vec4;
class	mat2;
class	mat3;
class	mat4;

using namespace std;

class	Program;
class	VertexBuffer;

class	UbAccessor
{
	GLuint			program;
	GLuint			blockIndex;
	int				numUniforms;				// number of active uniforms
	int				blockSize;
	byte          * ptr;						// pre-allocated buffer to hold the data to copy
	
	struct VarInfo								// info about a uniform
	{
		string	name;
		int		index;
		int		offset;
		int		type;
		int		size;
		int		arrayStride;
		int		matrixStride;
		bool	isRowMajor;
	};
	
	VarInfo * uniforms;						// array of data for every uniform
	
public:
	UbAccessor  ();
	~UbAccessor ();
	
	bool	isOk () const
	{
		return program != 0 && numUniforms > 0 && ptr != NULL;
	}
										// init and allocate data
	bool	create ( const Program&, const char * blockName );
	
										// copy data to buffer
	bool	updateBuffer   ( VertexBuffer& buf );
	
	
										// setters, init VarInfo on first access
	bool	setVector ( const char * name, const vec2& v );
	bool	setVector ( const char * name, const vec3& v );
	bool	setVector ( const char * name, const vec4& v );
	
	bool	setMatrix ( const char * name, const mat2& v );
	bool	setMatrix ( const char * name, const mat3& v );
	bool	setMatrix ( const char * name, const mat4& v );
	
	bool	setBool   ( const char * name, bool value );
	bool	setInt    ( const char * name, int value );
	bool	setUint   ( const char * name, GLuint value );
	bool	setFloat  ( const char * name, float value );
	
private:
										// locate name in cache or add to cache
	int		locateName ( const char * name );
};

#endif

