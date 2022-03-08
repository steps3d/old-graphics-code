//
// Class to encapsulate GLSL program and shader objects and working with them
//
// Author: Alexey V. Boreskov <steps3d@gmail.com>, <steps3d@narod.ru>
//

#pragma once
#ifndef __GLSL_PROGRAM__
#define __GLSL_PROGRAM__

#include    <GL/glew.h>
#include    <string>

using namespace std;

class   vec2;
class   vec3;
class   vec4;
class	mat2;
class	mat3;
class   mat4;
class   Data;

class   Program
{
protected:
    GLuint 	program;                            // program object handle
    GLuint 	vertexShader;
    GLuint 	fragmentShader;
	GLuint	geometryShader;
	GLuint	tessControlShader;
	GLuint	tessEvalShader;
    bool    ok;                                 // whether program is loaded and ready to be used
	bool	linkRequired;
    string	glError;
    string  log;
	bool	separate;

								// disable copying
	Program ( const Program& );
	Program& operator = ( const Program& );
	
public:
    Program  ();
    ~Program ();

	void destroy ();
												// load all shaders separated by -- shader-type
	bool	loadProgram           ( const string& fileName );
	bool	loadProgram           ( Data * data );
	bool	loadProgramFromString ( const string& source );
	bool	loadSeparate          ( GLenum type, Data * data );
	bool	loadSeparate          ( GLenum type, const string& fileName );
	
												// load and compile a single shader, no linking
	bool	loadShaderOfType ( Data * data, GLenum type );
	bool	loadShaderOfType ( const char * fileName, GLenum type );
	
                                                // load shaders and link
    bool    loadShaders ( const char * vertexFileName, const char * fragmentFileName );
    bool    loadShaders ( Data * vertexShaderData, Data * fragmentShaderData );

	GLuint	getProgram () const
	{
		return program;
	}

    string  getLog () const                         // get current log
    {
        return log;
    }

	bool	getLinkStatus () const;
	
    bool    isOk () const                               // whether shader is ok
    {
        return ok;
    }

	bool	isLinkRequired () const
	{
		return linkRequired;
	}
	
    string  getGlError () const
    {
        return glError;
    }

	bool	isSeparate () const
	{
		return separate;
	}
	
    void    bind   ();
    void    unbind ();
    bool    relink ();
														// program binary support
	Data  * getBinary  ( GLenum * binaryFormat ) const;	
	bool	saveBinary ( const char * fileName, GLenum * binaryFormat ) const;
	bool	loadBinary ( Data * data, GLenum binaryFormat );
	bool	loadBinary ( const char * fileName, GLenum binaryFormat );
	
                                                        // uniform variables handling methods
    int     locForUniformName ( const char * name );
    bool    setUniformVector  ( const char * name, const vec4& value  );
    bool    setUniformVector  ( int loc,            const vec4& value );
    bool    setUniformVector  ( const char * name, const vec3& value  );
    bool    setUniformVector  ( int loc,            const vec3& value );
    bool    setUniformVector  ( const char * name, const vec2& value  );
    bool    setUniformVector  ( int loc,            const vec2& value );
    bool    setUniformFloat   ( const char * name, float value        );
    bool    setUniformFloat   ( int loc,            float value       );
    bool    setUniformMatrix  ( const char * name, float value [16]   );
    bool    setUniformMatrix  ( const char * name, const mat4& value  );
    bool    setUniformMatrix  ( const char * name, const mat3& value  );
    bool    setUniformInt     ( const char * name, int value          );
    bool    setUniformInt     ( int loc,           int value          );
    bool    setUniformUInt    ( const char * name, GLuint value       );
    bool    setUniformUInt    ( int loc,           GLuint value       );
    vec4    getUniformVector  ( const char * name );
    vec4    getUniformVector  ( int loc           );
	
														// UBO support
	int		indexForUniformBlock       ( const char * name ) const;
	int		uniformBlockSize           ( int blockIndex ) const;
	int		uniformBlockBinding        ( int blockIndex ) const;
	int		uniformBlockActiveUniforms ( int blockIndex ) const;
	bool	getUniformBlockVar         ( int blockIndex, const char * varName, int& offs, int& size );
	void	bindBufferToIndex          ( int uniformBlockIndex, int bindingIndex );

    int     indexForAttrName   ( const char * name );
	bool	setAttrPtr         ( const char * name, int numComponents, GLsizei stride, void * ptr, 
	                             GLenum type = GL_FLOAT, bool normalized = false );
	bool	setAttrPtr         ( int location, int numComponents, GLsizei stride, void * ptr, 
	                             GLenum type = GL_FLOAT, bool normalized = false );
	
														// bind texture units to uniforms
    bool	setTexture         ( const char * name, int texUnit );
    bool	setTexture         ( int loc,           int texUnit );

													// bind frgament outputs
	bool	bindFragOut        ( const char * name, int no );
	bool	bindFragOutIndexed ( const char * name, int no, int index );
	
													// names - comma or space separated list of varyings
	void	transformFeedbacksVars ( const char * names, GLenum mode );
	
													// geometry-shader specific
	int	getGeometryInputType   () const;
	int	getGeometryOutputType  () const;
	int	getGeometryVerticesOut () const;
	int	tessControlVerticesOut () const;
													// tesselation-shader specific
	void	setPatchSize ( int count )				// set number of vertices per patch
	{
		glPatchParameteri ( GL_PATCH_VERTICES, count );
	}
	
	static	void	setTessDefaultOuterLevel ( const float v [4] );
	static	void	setTessDefaultInnerLevel ( const float v [2] );
	
                                                    
    static  bool    isSupported                  ();    // check whether there is a support for GLSL
    static  string  version                      ();	// GLSL version
	static	bool	isGeometryShaderSupported    ();
	static	bool	isTesselationShaderSupported ();
	static	bool	isSeparateShadersSupported   ();
					
														// transform feedback limitations
	static	int		maxTransformFeedbackSeparateAttribs       ();
	static	int		maxTransformFeedbackInterleavedComponents ();
	
	
                                                        // some limitations on program
	static	int	maxUniformBufferBindings     ();
    static  int maxVertexUniformComponents   ();
    static  int maxVertexAttribs             ();
    static  int maxFragmentTextureUnits      ();
    static  int maxVertexTextureUnits        ();
	static	int maxVertexOutputComponents    ();
    static  int maxCombinedTextureUnits      ();
    static  int maxVaryingFloats             ();
    static  int maxFragmentUniformComponents ();
	
	static	int	maxGeometryOutputVertices    ();
	static	int	maxGeometryUniformComponents ();
	static	int	maxGeometryInputComponents   ();
	static	int	maxGeometryOutputComponents  ();
	static	int maxGeometryTotalOutputComponents ();
	
	static	int maxTessControlUniformComponents ();
	static	int maxCombinedTessControlUnfiromComponents ();
	static	int	maxTessControlInputComponents ();
	static	int	maxTessControlOutputComponents ();
	static	int	maxTessPatchComponents ();
	static	int maxTessControlTotalOutputComponents ();
	static	int	maxTessGenLevel ();
	
	static	int maxTessEvalInputComponents  ();
	static	int maxTessEvalOutputComponents  ();
	static	int	maxTessEvalUniformComponents ();
	static	int maxCombinedTessEvalUniformComponents ();

protected:
    bool    loadShader     ( GLuint shader, Data * data );
    bool    checkGlError   ();
    void    loadProgramLog ( GLuint object );
    void    loadShaderLog  ( GLuint object );
};

#endif
