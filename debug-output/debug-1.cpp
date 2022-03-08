//
// Example of using ARB_debug_output
//
// Author: Alexey V. Boreskov <steps3d@gmail.com>, <steps3d@narod.ru>
//

#include	<GL/glew.h>

#ifdef	_WIN32
	#include	<GL/wglew.h>
#else
	#include	<GL/glxew.h>
#endif

#include	<freeglut.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

#include	"Program.h"
#include	"VertexArray.h"
#include	"VertexBuffer.h"
#include	"glUtilities.h"
#include	"mat4.h"

#define	NUM_VERTICES	3
#define	VERTEX_SIZE		(6*sizeof(float))

#ifndef	_WIN32
	#define	CALLBACK
#endif

Program			program;
VertexArray		vao;
VertexBuffer	buf;

static const float vertices [] = 
{
	/* 1 Coords: */ -1.0f, -1.0f, -2.0f, /* Color: */ 1.0f, 0.0f, 0.0f,
	/* 2 Coords: */  0.0f,  1.0f, -2.0f, /* Color: */ 0.0f, 1.0f, 0.0f,
	/* 3 Coords: */  1.0f, -1.0f, -2.0f, /* Color: */ 0.0f, 0.0f, 1.0f
};

void init ()
{
	glClearColor ( 0.5, 0.5, 0.5, 1.0 );
	glEnable     ( GL_DEPTH_TEST );
	glDepthFunc  ( GL_LEQUAL );
}

void display ()
{
	glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	program.bind ();
	vao.bind ();
	
	glDrawArrays ( GL_TRIANGLES, 0, NUM_VERTICES );
	
	vao.unbind ();
	program.unbind ();

	glutSwapBuffers ();
}

void reshape ( int w, int h )
{
   glViewport ( 0, 0, (GLsizei)w, (GLsizei)h );
   
	mat4 proj = perspective ( 60.0f, (float)w / (float)h, 0.5f, 5.0f );
	
	program.bind ();
	program.setUniformMatrix ( "proj", proj );
	program.unbind ();  
}

void key ( unsigned char key, int x, int y )
{
	if ( key == 27 || key == 'q' || key == 'Q' )	//	quit requested
    	exit ( 0 );
}

const char * stringForSource ( GLenum source )
{
	switch ( source )
	{
		case GL_DEBUG_SOURCE_API_ARB:
			return "API";
			
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB:
			return "Window system";
			
        case GL_DEBUG_SOURCE_SHADER_COMPILER_ARB:
			return "Shader compiler";
			
        case GL_DEBUG_SOURCE_THIRD_PARTY_ARB:
			return "Third party";
			
        case GL_DEBUG_SOURCE_APPLICATION_ARB:
			return "Application";
			
        case GL_DEBUG_SOURCE_OTHER_ARB:
			return "Other";
			
		default:
			return "Source unknown";
	}
}

const char * stringForType ( GLenum type )
{
	switch ( type )
	{
		case GL_DEBUG_TYPE_ERROR_ARB:
			return "Error";
			
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB:
			return "Deprecated behaviour";
			
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB:
			return "Undefined behaviour";
			
        case GL_DEBUG_TYPE_PORTABILITY_ARB:
			return "Portability";
			
        case GL_DEBUG_TYPE_PERFORMANCE_ARB:
			return "Performance";
			
        case GL_DEBUG_TYPE_OTHER_ARB:
			return "Type other";
			
		default:
			return "Type unknown";
	}
}

const char * stringForSeverity ( GLenum severity )
{
	switch ( severity )
	{
		case GL_DEBUG_SEVERITY_HIGH_ARB:
			return "High";
			
        case GL_DEBUG_SEVERITY_MEDIUM_ARB:
			return "Medium";
			
        case GL_DEBUG_SEVERITY_LOW_ARB:
			return "Low";
			
		default:
			return "Severity unknown";
	}
}

void	CALLBACK errorCallback ( GLenum source, GLenum type, GLuint id, 
                                 GLenum severity, GLsizei length, const GLchar * message, GLvoid * userParam )
{
	printf ( "-----------------------------------------------------------------------------------\n" );
	printf ( "Error:\n\tSource: %s\n\tType: %s\n\tId: %d\n\tSeverity: %s\n\tMessage: %s\n",
			stringForSource ( source ), stringForType ( type ), id, stringForSeverity ( severity ), message );
}

int main ( int argc, char * argv [] )
{
								// initialize glut
	glutInit            ( &argc, argv );
	glutInitDisplayMode ( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	glutInitWindowSize  ( 500, 500 );

								// prepare context for 3.3
	glutInitContextVersion ( 3, 3 );
	glutInitContextFlags   ( GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG );
	glutInitContextProfile ( GLUT_CORE_PROFILE );

								// create window
	glutCreateWindow ( "OpenGL ARB_debug_error" );

	glewExperimental = GL_TRUE;

	glewInit ();

	if ( !GLEW_VERSION_3_3 )
	{
		printf ( "OpenGL 3.3 not supported.\n" );
		
		return 1;
	}

	if ( !GLEW_ARB_debug_output )
	{
		printf ( "ARB_debug_output not supported\n" );
		
		return 2;
	}
	
	glDebugMessageCallbackARB ( errorCallback, NULL );
	
	glPolygonMode ( GL_FRONT, GL_BACK );	// Error: not supported in core profile
	glBindTexture ( 0, 0 );					// Error: illegal enum
	glEnable      ( -1 );					// Error: invalid enum
	
											// insert custom message
	glDebugMessageInsertARB ( GL_DEBUG_SOURCE_APPLICATION_ARB, GL_DEBUG_TYPE_ERROR_ARB, 1, 
	                          GL_DEBUG_SEVERITY_HIGH_ARB, -1, "Custom message" );
	
											// register handlers
	glutDisplayFunc  ( display );
	glutReshapeFunc  ( reshape );
	glutKeyboardFunc ( key     );

	if ( !GL_ARB_vertex_array_object )
		printf ( "No VAO support\n" );

	printf ( "GLSL Version: %s\n", Program::version().c_str () );
	
	if ( !program.loadProgram ( "1.glsl" ) )
	{
		printf ( "Error building program: %s\n", program.getLog ().c_str () );
		
		return 2;
	}
	
	program.bind ();

	vao.create  ();
	vao.bind    ();
	buf.create  ();
	buf.bind    ( GL_ARRAY_BUFFER );
	buf.setData ( NUM_VERTICES * VERTEX_SIZE, vertices, GL_STATIC_DRAW );

	program.setAttrPtr ( "position", 3, VERTEX_SIZE, (void *) 0 );
	program.setAttrPtr ( "color",    3, VERTEX_SIZE, (void *)(3 * sizeof ( float ) ) );

	buf.unbind     ();
	vao.unbind     ();
	program.unbind ();

    glutMainLoop ();

	return 0;
}
