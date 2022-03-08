//
// Class to encapsulate GLSL program and shader objects and working with them
//
// Author: Alex V. Boreskoff <steps3d@gmail.com>, <steps3d@narod.ru>
//

#include    "vec3.h"
#include    "vec2.h"
#include    "vec4.h"
#include	"mat2.h"
#include	"mat3.h"
#include    "mat4.h"
#include    "Data.h"
#include	"glUtilities.h"
#include    "Program.h"
#include	<malloc.h>
#include	<map>
#include	"stringUtils.h"

Program :: Program ()
{
    program           = 0;
    vertexShader      = 0;
    fragmentShader    = 0;
	geometryShader    = 0;
	tessControlShader = 0;
	tessEvalShader    = 0;
    ok                = false;
	linkRequired      = true;
	separate          = false;
}

Program :: ~Program ()
{
    destroy ();
}

bool	Program :: loadProgram ( const string& fileName )
{
	Data data ( fileName );
	
	if ( !data.isOk () || data.getLength () < 1 )
	{
		log += "Error loading shader from ";
		log += fileName;
		log += "\n";
		
		return false;
	}

	return loadProgram ( &data );
}

bool	Program :: loadProgram ( Data * data )
{
	glGetError ();		// clear OpenGL error

	struct Chunk
	{
		const char * name;
		GLenum		 type;
		int			 start, end;
	};
	
	Chunk	info [] = 
	{
		{ "vertex",      GL_VERTEX_SHADER,       0, 0 },
		{ "fragment",    GL_FRAGMENT_SHADER,     0, 0 },
		{ "geometry",    GL_GEOMETRY_SHADER,     0, 0 },
		{ "tesscontrol", GL_TESS_CONTROL_SHADER, 0, 0 },
		{ "tesseval",    GL_TESS_EVALUATION_SHADER, 0, 0 }
	};
	
	string	s;
	int		start = 0, end = 0;
	int		type = -1;
	
	log += "Started loading composite shader from ";
	log += data -> getFileName ();
	log += "\n";

	while ( data -> getString ( s, '\n' ) )
	{
													// not a start of a new shader
		if ( s.empty () || s.length () < 2 || s [0] != '-' || s [1] != '-' )
		{
			end = data -> getPos ();

			continue;
		}
			
		s = stringLower ( stringTrim ( s.substr ( 2 ) ) );
		
		if ( start < end && type != -1 )			// there is shader
		{
			info [type].start = start;
			info [type].end   = end;
		}
		
		start = data -> getPos ();
		end   = start;
		type  = -1;
		
		for ( size_t i = 0; i < sizeof ( info ) / sizeof ( info [0] ); i++ )
			if ( s == info [i].name )
			{
				type = i;
				break;
			}
	}
	
	if ( start < end && type != -1 )				// process last shader
	{
		info [type].start = start;
		info [type].end   = end;
		start             = data -> getPos ();
		end               = start;
	}
	
						// now load all chunks
	for ( size_t i = 0; i < sizeof ( info ) / sizeof ( info [0] ); i++ )
		if ( info [i].start < info [i].end )
		{
			Data	d ( data -> getPtr ( info [i].start ), info [i].end - info [i].start );
		
			if ( !loadShaderOfType ( &d, info [i].type ) )
				return false;
		}
	
	return relink ();
}

bool	Program :: loadProgramFromString ( const string& source )
{
	Data	data ( (void *)source.c_str (), source.length () + 1 );		// XXX - m.b. strdup ?
	
	return loadProgram ( &data );
}

						// load source and compile it into existing shader
bool    Program :: loadShader ( GLuint shader, Data * data )
{
    const char * body = (const char *) data -> getPtr ( 0 );
    GLint		 len  = data -> getLength ();
    GLint        compileStatus;

    glShaderSource ( shader, 1, &body,  &len );

                                        // compile the shader
    glCompileShader ( shader );

    if ( !checkGlError() )              // check for OpenGL errors
        return false;

    glGetShaderiv ( shader, GL_COMPILE_STATUS, &compileStatus );

    loadShaderLog ( shader );

    return compileStatus != 0;
}

						// create and load shader of specifi type
bool	Program :: loadShaderOfType ( const char * fileName, GLenum type )
{
	Data * data = new Data ( fileName );
	
	if ( !data -> isOk () || data -> getLength () < 1 )
	{
		log += "Error loading shader from ";
		log += fileName;
		log += "\n";
		
		delete data;
		
		return false;
	}
	
	bool	result = loadShaderOfType ( data, type );
	
	delete data;
	
	return result;
}

						// create and load shader of specifi type
bool	Program :: loadShaderOfType ( Data * data, GLenum type )
{
    if ( program == 0 )
        program = glCreateProgram ();

                                // check for errors
    if ( !checkGlError () )
        return false;

    GLuint	shader = glCreateShader ( type );
	
	switch ( type )
	{
		case GL_VERTEX_SHADER:
			log += "Loading vertex shader.\n";
			vertexShader = shader;
			break;	
			
		case GL_FRAGMENT_SHADER:
			log += "Loading fragment shader.\n";
			fragmentShader = shader;
			break;	
			
		case GL_GEOMETRY_SHADER:
			log += "Loading geometry shader.\n";
			geometryShader = shader;
			break;	
			
		case GL_TESS_CONTROL_SHADER:
			log += "Loading tesselation control shader.\n";
			tessControlShader = shader;
			break;	
			
		case GL_TESS_EVALUATION_SHADER:
			log += "Loading tesselation evaluation shader.\n";
			tessEvalShader = shader;
			break;	
					
		default:
			log += "Unknown shader type.\n";
			return false;
	}
	
						// try to load and compile it
	if ( !loadShader ( shader, data ) )
		return false;
		
	glAttachShader ( program, shader );

	linkRequired = true;
	
	return true;
}

bool    Program :: loadShaders ( const char * vertexFileName, const char * fragmentFileName )
{
    ok = false;

    Data * vertexData = new Data ( vertexFileName );

	if ( vertexData == NULL )
	{
		log += "Cannot open \"";
		log += vertexFileName;
		log += "\"\n";
	}
	else
    if ( !vertexData -> isOk () || vertexData -> isEmpty () )
    {
    	log += "Error loading vertex shader ";
		log += vertexFileName;
		log += "\n";

    	delete vertexData;

		vertexData = NULL;
    }

    Data * fragmentData = new Data ( fragmentFileName );

	if ( fragmentData == NULL )
	{
		log += "Cannot open \"";
		log += fragmentFileName;
		log += "\"\n";
	}
	else
    if ( !fragmentData -> isOk () || fragmentData -> isEmpty () )
    {
    	log += "Error loading fragment shader ";
		log += fragmentFileName;
		log += "\n";

    	delete fragmentData;

    	fragmentData = NULL;
    }
	
    bool	result = loadShaders ( vertexData, fragmentData );

	if ( fragmentData == NULL && fragmentFileName != NULL )		// file was specified but failed to open
		result = false;
		
    delete vertexData;
    delete fragmentData;

    return result;
}

bool    Program :: loadShaders ( Data * vertexShaderData, Data * fragmentShaderData )
{
    ok = false;
                                // create a vertex shader object and a fragment shader object
    if ( vertexShaderData != NULL )
    {
	    log += "Loading vertex shader\n";
		
		if ( !loadShaderOfType ( vertexShaderData, GL_VERTEX_SHADER ) )
			return false;
	}

    if ( fragmentShaderData != NULL )
    {
	    log += "Loading fragment shader\n";
		
		if ( !loadShaderOfType ( fragmentShaderData, GL_FRAGMENT_SHADER ) )
			return false;
    }

	return relink ();
}

bool	Program :: loadSeparate ( GLenum type, Data * data )
{
	if ( !data -> isOk () || data -> getLength () < 1 )
	{
		log += "Error loading separate shader\n";
		
		return false;
	}

		// now create an array of zero-terminated strings
		
    const char * body  = (const char *) data -> getPtr ( 0 );
    GLint		 len   = data -> getLength ();
	char       * buf   = (char *) malloc ( len + 1 );
	
	memcpy ( buf, body, len );
	buf [len] = '\0';
	
	program      = glCreateShaderProgramv ( type, 1, (const char **)&buf );
	separate     = true;
	linkRequired = false;
	ok           = true;
	
	loadProgramLog ( program );

	free ( buf );
	
	return program != 0;
}

bool	Program :: loadSeparate ( GLenum type, const string& fileName )
{
    Data * data = new Data ( fileName );

	if ( data == NULL )
	{
		log += "Cannot open \"";
		log += fileName;
		log += "\"\n";
		
		return false;
	}

	bool	res = loadSeparate ( type, data );
	
	delete data;
	
	return res;
}

bool	Program :: getLinkStatus () const
{
    GLint   linked;

    glGetProgramiv ( program, GL_LINK_STATUS, &linked );

    return linked != 0;
}

bool	Program :: relink ()
{
    GLint   linked;

    log += "Linking programs\n";

	if ( GLEW_ARB_get_program_binary )
		glProgramParameteri ( program, GL_PROGRAM_BINARY_RETRIEVABLE_HINT, GL_TRUE );
	
                                // link the program object and print out the info log
    glLinkProgram ( program );

	if ( !checkGlError () )     // check for errors
	    return false;

    glGetProgramiv ( program, GL_LINK_STATUS, &linked );

    loadProgramLog ( program );

    if ( !linked )
        return false;
	
    log += "Link successfull\n";

	linkRequired = false;
	
    return ok = true;
}

Data  * Program :: getBinary ( GLenum * binaryFormat ) const
{
	if ( program == 0 || !getLinkStatus () )
		return NULL;
		
	if ( !GLEW_ARB_get_program_binary )
		return NULL;

	GLint   binaryLength;
	void  * binary;

	glGetProgramiv ( program, GL_PROGRAM_BINARY_LENGTH, &binaryLength );
	
	if ( binaryLength < 1 )
		return NULL;
		
    binary = (GLvoid * )malloc ( binaryLength );
	
	if ( binary == NULL )
		return NULL;
		
    glGetProgramBinary ( program, binaryLength, NULL, binaryFormat, binary );
		
	return new Data ( binary, binaryLength );
}

bool	Program :: saveBinary ( const char * fileName, GLenum * binaryFormat ) const
{
	Data * data = getBinary ( binaryFormat );
	
	if ( data == NULL )
		return false;
		
	data -> saveToFile ( fileName );
	
	delete data;
	
	return true;
}

bool	Program :: loadBinary ( Data * data, GLenum binaryFormat )
{
	if ( program == 0 )
		return false;
		
	if ( !GLEW_ARB_get_program_binary )
		return false;

	if ( data == NULL || data -> getLength () < 1 )
		return false;
		
	glProgramBinary ( program, binaryFormat, data -> getPtr (), data -> getLength () );

	return getLinkStatus ();
}

bool	Program :: loadBinary ( const char * fileName, GLenum binaryFormat )
{
	if ( !GLEW_ARB_get_program_binary )
		return false;

	Data * data = new Data ( fileName );
	bool   res  = loadBinary ( data, binaryFormat );
	
	delete data;
	
	return res;
}

void    Program :: bind ()
{
    glUseProgram ( program );
}

void    Program :: unbind ()
{
    glUseProgram ( 0 );
}

bool    Program :: checkGlError ()
{
    bool    retCode = true;
	const char * err = getGlErrorString ();

    if ( err == NULL )
    	return retCode;

	if ( err != NULL )
	{
		glError = err;
		log    += glError;
	}
	
    return false;
}

void    Program :: destroy ()
{
    glDeleteProgram ( program           );                  // it will also detach shaders
    glDeleteShader  ( vertexShader      );
    glDeleteShader  ( fragmentShader    );
	glDeleteShader  ( geometryShader    );
	glDeleteShader  ( tessControlShader );
	glDeleteShader  ( tessEvalShader    );

    program           = 0;
    vertexShader      = 0;
    fragmentShader    = 0;
	geometryShader    = 0;
	tessControlShader = 0;
	tessEvalShader    = 0;
    ok                = false;
	linkRequired      = true;
}

void    Program :: loadProgramLog ( GLuint object )
{
    GLint       logLength     = 0;
    GLsizei     charsWritten  = 0;
    GLcharARB   buffer [2048];
    GLcharARB * infoLog;

    glGetProgramiv ( object, GL_INFO_LOG_LENGTH, &logLength );

    if ( !checkGlError() )          // check for OpenGL errors
        return;

    if ( logLength < 1 )
        return;
                                    // try to avoid allocating buffer
    if ( logLength > (int) sizeof ( buffer ) )
    {
        infoLog = (GLcharARB*) malloc ( logLength );

        if ( infoLog == NULL )
        {
            log = "ERROR: Could not allocate log buffer";

            return;
        }
    }
    else
        infoLog = buffer;

    glGetProgramInfoLog ( object, logLength, &charsWritten, infoLog );

    log += infoLog;

    if ( infoLog != buffer )
        free ( infoLog );
}

void    Program :: loadShaderLog ( GLuint object )
{
    GLint       logLength     = 0;
    GLsizei     charsWritten  = 0;
    GLcharARB   buffer [2048];
    GLcharARB * infoLog;

    glGetShaderiv ( object, GL_INFO_LOG_LENGTH, &logLength );

    if ( !checkGlError() )          // check for OpenGL errors
        return;

    if ( logLength < 1 )
        return;
                                    // try to avoid allocating buffer
    if ( logLength > (int) sizeof ( buffer ) )
    {
        infoLog = (GLcharARB*) malloc ( logLength );

        if ( infoLog == NULL )
        {
            log = "ERROR: Could not allocate log buffer";

            return;
        }
    }
    else
        infoLog = buffer;

    glGetShaderInfoLog ( object, logLength, &charsWritten, infoLog );

    log += infoLog;

    if ( infoLog != buffer )
        free ( infoLog );
}

bool    Program :: setUniformVector ( const char * name, const vec4& value )
{
    int loc = glGetUniformLocation ( program, name );

    if ( loc < 0 )
        return false;

    glUniform4fvARB ( loc, 1, value );

    return true;
}

bool    Program :: setUniformVector  ( int loc, const vec4& value )
{
    glUniform4fv ( loc, 1, value );

    return true;
}

bool    Program :: setUniformVector ( const char * name, const vec3& value )
{
    int loc = glGetUniformLocation ( program, name );

    if ( loc < 0 )
        return false;

    glUniform3fvARB ( loc, 1, value );

    return true;
}

bool    Program :: setUniformVector  ( int loc, const vec3& value )
{
    glUniform3fv ( loc, 1, value );

    return true;
}

bool    Program :: setUniformVector ( const char * name, const vec2& value )
{
    int loc = glGetUniformLocation ( program, name );

    if ( loc < 0 )
        return false;

    glUniform2fv ( loc, 1, value );

    return true;
}

bool    Program :: setUniformVector  ( int loc, const vec2& value )
{
    glUniform2fv ( loc, 1, value );

    return true;
}

bool    Program :: setUniformFloat ( const char * name, float value )
{
    int loc = glGetUniformLocation ( program, name );

    if ( loc < 0 )
        return false;

    glUniform1f ( loc, value );

    return true;
}

bool    Program :: setUniformFloat ( int loc, float value )
{
    glUniform1f ( loc, value );

    return true;
}

bool    Program :: setUniformInt ( const char * name, int value )
{
    int loc = glGetUniformLocation ( program, name );

    if ( loc < 0 )
        return false;

    glUniform1i ( loc, value );

    return true;
}

bool    Program :: setUniformInt ( int loc, int value )
{
    glUniform1i ( loc, value );

    return true;
}

bool    Program :: setUniformUInt ( const char * name, GLuint value )
{
    int loc = glGetUniformLocation ( program, name );

    if ( loc < 0 )
        return false;

    glUniform1ui ( loc, value );

    return true;
}

bool    Program :: setUniformUInt ( int loc, GLuint value )
{
    glUniform1ui ( loc, value );

    return true;
}

bool    Program :: setUniformMatrix  ( const char * name, const mat4& value )
{
    int loc = glGetUniformLocation ( program, name );

    if ( loc < 0 )
        return false;

    glUniformMatrix4fv ( loc, 1, GL_TRUE, value.data () );		// require matrix transpose

    return true;
}

bool    Program :: setUniformMatrix  ( const char * name, const mat3& value )
{
    int loc = glGetUniformLocation ( program, name );

    if ( loc < 0 )
        return false;

    glUniformMatrix3fv ( loc, 1, GL_TRUE, value.data () );		// require matrix transpose

    return true;
}

bool    Program :: setUniformMatrix  ( const char * name, float value [16] )
{
    int loc = glGetUniformLocation ( program, name );

    if ( loc < 0 )
        return false;

    glUniformMatrix4fv ( loc, 1, GL_FALSE, value );

    return true;
}

int     Program :: locForUniformName ( const char * name )
{
    return glGetUniformLocation ( program, name );
}

int		Program :: indexForUniformBlock ( const char * name ) const
{
	return glGetUniformBlockIndex ( program, name );
}

int	Program :: uniformBlockSize ( int blockIndex ) const
{
	int	size;
	
	glGetActiveUniformBlockiv ( program, blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &size );
	
	return size;
}

int	Program :: uniformBlockBinding ( int blockIndex ) const
{
	int	binding;
	
	glGetActiveUniformBlockiv ( program, blockIndex, GL_UNIFORM_BLOCK_BINDING, &binding );
	
	return binding;
}

int	Program :: uniformBlockActiveUniforms ( int blockIndex ) const
{
	int	num;
	
	glGetActiveUniformBlockiv ( program, blockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &num );
	
	return num;
}

bool	Program :: getUniformBlockVar ( int blockIndex, const char * varName, int& offs, int& size )
{
	GLuint	varIndex;
	
	glGetUniformIndices ( program, 1, &varName, &varIndex );
	
	if ( varIndex == GL_INVALID_INDEX )
		return false;
		
	glGetActiveUniformsiv ( program, 1, &varIndex, GL_UNIFORM_SIZE,   &size );
	glGetActiveUniformsiv ( program, 1, &varIndex, GL_UNIFORM_OFFSET, &offs );
	
	return true;
}

void	Program :: bindBufferToIndex ( int uniformBlockIndex, int bindingIndex )
{
	 glUniformBlockBinding ( program, (GLuint)uniformBlockIndex, (GLuint)bindingIndex );
}

vec4    Program :: getUniformVector ( const char * name )
{
    float   values [4];

    int loc = glGetUniformLocation ( program, name );

    if ( loc < 0 )
        return vec4 ( 0, 0, 0, 0 );

    glGetUniformfv ( program, loc, values );

    return vec4 ( values [0], values [1], values [2], values [3] );
}

bool    Program :: setTexture ( const char * name, int texUnit )
{
    int loc = glGetUniformLocation ( program, name );

    if ( loc == -1 )
        return false;

    glUniform1i ( loc, texUnit );

    return true;
}

bool    Program :: setTexture ( int loc, int texUnit )
{
    if ( loc < 0 )
        return false;

    glUniform1i ( loc, texUnit );

    return true;
}

bool	Program :: bindFragOut ( const char * name, int no )
{
	glBindFragDataLocation ( program, no, name );
	
	linkRequired = true;
	
	return true;
}	

bool	Program :: bindFragOutIndexed ( const char * name, int no, int index )
{
	glBindFragDataLocationIndexed ( program, no, index, name );
	
	linkRequired = true;
	
	return true;
}	

int     Program :: indexForAttrName ( const char * name )
{
    return glGetAttribLocation ( program, name );
}

bool	Program :: setAttrPtr ( const char * name, int numComponents, GLsizei stride, void * ptr, GLenum type, bool normalized )
{
	int	loc = indexForAttrName ( name );

	if ( loc < 0 )
		return false;
		
	glVertexAttribPointer ( loc, 					// index
							numComponents, 			// number of values per vertex
							type, 					// type (GL_FLOAT)
							normalized ? GL_TRUE : GL_FALSE,
							stride, 				// stride (offset to next vertex data)
							(const GLvoid*) ptr );
		
	glEnableVertexAttribArray ( loc );
	
	return true;
}

bool	Program :: setAttrPtr ( int loc, int numComponents, GLsizei stride, void * ptr, GLenum type, bool normalized )
{
	if ( loc < 0 )
		return false;
		
	glVertexAttribPointer ( loc, 					// index
							numComponents, 			// number of values per vertex
							type, 					// type (GL_FLOAT)
							normalized ? GL_TRUE : GL_FALSE,
							stride, 				// stride (offset to next vertex data)
							(const GLvoid*) ptr );
		
	glEnableVertexAttribArray ( loc );
	
	return true;
}

void	Program :: transformFeedbacksVars ( const char * names, GLenum mode )
{
	char	buf      [2048];				// hope this will be enough
	char  * varyings [128];
	size_t	count = 0;
	
	for ( char * ptr = strtok ( strcpy ( buf, names ), " ,;" ); ptr; ptr = strtok ( NULL, " ,;" ) )
		varyings [count++] = ptr;

	glTransformFeedbackVaryings ( program, count, (const GLchar **)varyings, mode );
	
	linkRequired = true;
}

int	Program :: getGeometryInputType () const
{
	int	type;
	
	glGetProgramiv ( program, GL_GEOMETRY_INPUT_TYPE, &type );
	
	return type;
}

int	Program :: getGeometryOutputType () const
{
	int	type;
	
	glGetProgramiv ( program, GL_GEOMETRY_OUTPUT_TYPE, &type );
	
	return type;
}

int	Program :: getGeometryVerticesOut () const
{
	int	num;
	
	glGetProgramiv ( program, GL_GEOMETRY_VERTICES_OUT, &num );
	
	return num;
}

void	Program :: setTessDefaultOuterLevel ( const float v [4] )
{
	if ( isTesselationShaderSupported () )
		glPatchParameterfv ( GL_PATCH_DEFAULT_OUTER_LEVEL, v );
}

void	Program :: setTessDefaultInnerLevel ( const float v [2] )
{
	if ( isTesselationShaderSupported () )
		glPatchParameterfv ( GL_PATCH_DEFAULT_INNER_LEVEL, v );
}

int	Program :: tessControlVerticesOut () const
{
	int	n = 0;

	glGetProgramiv ( program, GL_TESS_CONTROL_OUTPUT_VERTICES, &n );

	return n;
}

bool    Program :: isSupported ()
{
    return GLEW_ARB_shading_language_100 && GLEW_ARB_shader_objects && GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader;
}

bool	Program :: isGeometryShaderSupported ()
{
	return GLEW_EXT_geometry_shader4 != 0;
}

bool	Program :: isTesselationShaderSupported ()
{
	return GLEW_ARB_tessellation_shader != 0;
}

string  Program :: version ()
{
	glGetError ();		// clear old error status, if any

    const char * slVer = (const char *) glGetString ( GL_SHADING_LANGUAGE_VERSION );

    if ( glGetError() != GL_NO_ERROR )
        return "1.051";

    return string ( slVer );
}

int     Program :: maxVertexUniformComponents ()
{
    GLint maxVertexUniformComponents;

    glGetIntegerv ( GL_MAX_VERTEX_UNIFORM_COMPONENTS, &maxVertexUniformComponents );

    return maxVertexUniformComponents;
}

int     Program :: maxVertexAttribs ()
{
    GLint maxVertexAttribs;

    glGetIntegerv ( GL_MAX_VERTEX_ATTRIBS, &maxVertexAttribs );

    return maxVertexAttribs;
}

int     Program :: maxFragmentTextureUnits ()
{
    GLint maxFragmentTextureUnits;

    glGetIntegerv ( GL_MAX_TEXTURE_IMAGE_UNITS, &maxFragmentTextureUnits );

    return maxFragmentTextureUnits;
}

int     Program :: maxVertexTextureUnits ()
{
    GLint maxVertexTextureUnits;

    glGetIntegerv ( GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &maxVertexTextureUnits );

    return maxVertexTextureUnits;
}

int     Program :: maxCombinedTextureUnits ()
{
    GLint maxCombinedTextureUnits;

    glGetIntegerv ( GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxCombinedTextureUnits );

    return maxCombinedTextureUnits;
}

int     Program :: maxVaryingFloats ()
{
    GLint maxVaryingFloats;

    glGetIntegerv ( GL_MAX_VARYING_FLOATS, &maxVaryingFloats );

    return maxVaryingFloats;
}

int     Program :: maxFragmentUniformComponents ()
{
    GLint maxFragmentUniformComponents;

    glGetIntegerv ( GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &maxFragmentUniformComponents );

    return maxFragmentUniformComponents;
}

int	Program :: maxGeometryOutputVertices ()
{
	int	maxVerts;
	
    glGetIntegerv ( GL_MAX_GEOMETRY_OUTPUT_VERTICES, &maxVerts );

	return maxVerts;
}

int Program :: maxTessEvalOutputComponents ()
{
	int	m;
	
	glGetIntegerv ( GL_MAX_TESS_EVALUATION_OUTPUT_COMPONENTS, &m );
	
	return m;
}

int Program :: maxVertexOutputComponents ()
{
	int	m;
	
	glGetIntegerv ( GL_MAX_VERTEX_OUTPUT_COMPONENTS, &m );
	
	return m;
}

int	Program :: maxGeometryUniformComponents ()
{
	int	m;
	
	glGetIntegerv ( GL_MAX_GEOMETRY_UNIFORM_COMPONENTS, &m );
	
	return m;
}

int	Program :: maxGeometryInputComponents ()
{
	int	m;
	
	glGetIntegerv ( GL_MAX_GEOMETRY_INPUT_COMPONENTS, &m );
	
	return m;
}

int Program :: maxGeometryTotalOutputComponents ()
{
	int	m;
	
	glGetIntegerv ( GL_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS, &m );
	
	return m;
}

int	Program :: maxGeometryOutputComponents ()
{
	int	m;
	
	glGetIntegerv ( GL_MAX_GEOMETRY_OUTPUT_COMPONENTS, &m );
	
	return m;
}

int Program :: maxTessControlUniformComponents ()
{
	int	m;
	
	glGetIntegerv ( GL_MAX_TESS_CONTROL_UNIFORM_COMPONENTS, &m );
	
	return m;
}

int Program :: maxCombinedTessControlUnfiromComponents ()
{
	int	m;
	
	glGetIntegerv ( GL_MAX_COMBINED_TESS_CONTROL_UNIFORM_COMPONENTS, &m );
	
	return m;
}

int	Program :: maxTessControlInputComponents ()
{
	int	m;
	
	glGetIntegerv ( GL_MAX_TESS_CONTROL_INPUT_COMPONENTS, &m );
	
	return m;
}

int	Program :: maxTessControlOutputComponents ()
{
	int	m;
	
	glGetIntegerv ( GL_MAX_TESS_CONTROL_OUTPUT_COMPONENTS, &m );
	
	return m;
}

int	Program :: maxTessPatchComponents ()
{
	int	m;
	
	glGetIntegerv ( GL_MAX_TESS_PATCH_COMPONENTS, &m );
	
	return m;
}

int Program :: maxTessControlTotalOutputComponents ()
{
	int	m;
	
	glGetIntegerv ( GL_MAX_TESS_CONTROL_TOTAL_OUTPUT_COMPONENTS, &m );
	
	return m;
}

int	Program :: maxTessGenLevel ()
{
	int	m;
	
	glGetIntegerv ( GL_MAX_TESS_GEN_LEVEL, &m );
	
	return m;
}

int	Program :: maxTessEvalUniformComponents ()
{
	int	m;
	
	glGetIntegerv ( GL_MAX_TESS_EVALUATION_UNIFORM_COMPONENTS, &m );
	
	return m;
}

int Program :: maxCombinedTessEvalUniformComponents ()
{
	int	m;
	
	glGetIntegerv ( GL_MAX_COMBINED_TESS_EVALUATION_UNIFORM_COMPONENTS, &m );
	
	return m;
}

int Program :: maxTessEvalInputComponents ()
{
	int	m;
	
	glGetIntegerv ( GL_MAX_TESS_EVALUATION_INPUT_COMPONENTS, &m );
	
	return m;
}

int		Program :: maxTransformFeedbackSeparateAttribs ()
{
	int	m;
	
	glGetIntegerv ( GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS, &m );
	
	return m;
}

int		Program :: maxTransformFeedbackInterleavedComponents ()
{
	int	m;
	
	glGetIntegerv ( GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS, &m );
	
	return m;
}

int	Program :: maxUniformBufferBindings ()
{
	int	m;
	
	glGetIntegerv ( GL_MAX_UNIFORM_BUFFER_BINDINGS, &m );
	
	return m;
}
/*
int	Program :: maxCombinedUniformComponents ()
{
	int	m;
	
	glGetIntegerv ( GL_MAX_COMBINED_UNIFORM_COMPONENTS, &m );
	
	return m;
}
*/

bool	Program :: isSeparateShadersSupported ()
{
	return glewGetExtension ( "GL_ARB_separate_shader_objects" );
}
