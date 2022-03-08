//
// PN-triangle tessellation for OpenGL 4.1
//
// Author: Alex V. Boreskoff <steps3d@gmail.com>, <steps3d@narod.ru>
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
#include	"glUtilities.h"
#include	"mat4.h"
#include	"VertexArray.h"
#include	"VertexBuffer.h"
#include	"Log.h"

#define	PATCH_SIZE		3							// 3 control points 
#define	VERTEX_SIZE		(6*sizeof(float))
#define	NUM_VERTICES	(PATCH_SIZE)

vec3	rot ( 0.0f );
vec3	eye ( 3, 3, 4 );
int     mouseOldX = 0;
int     mouseOldY = 0;
int		inner = 2;
int		outer = 2;

VertexArray		vao;
VertexBuffer	buf;
Program			program;

static const float vertices [] = 
{
	//  Coords                 Normal
	3.0,  0.0,  0.0,	   -3.0, -1.0, -1.0,
	0.0,  3.0,  0.0,	   -1.0, -3.0, -1.0,
	0.0,  0.0,  3.0,	    1.0,  0.0,  7.0
};

void init ()
{
	glClearColor ( 0.5, 0.5, 0.5, 1.0 );
	glEnable     ( GL_DEPTH_TEST );
	glDepthFunc  ( GL_LEQUAL );
}

void updateCaption ()
{
	char str [128];

	sprintf ( str, "PN-triangle - Inner Level: %2d, Outer Level: %2d", inner, outer );

	glutSetWindowTitle ( str );
}

void display ()
{
	glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    mat4	mv = mat4 :: rotateZ ( toRadians ( rot.z ) ) * mat4 :: rotateY ( toRadians ( rot.y ) ) * mat4 :: rotateX ( toRadians ( rot.x ) );
	mat3	nm = normalMatrix ( mv );
	
	program.bind ();
	program.setUniformMatrix ( "mv",  mv );
	program.setUniformMatrix ( "nm",  nm );
	program.setUniformInt    ( "inner", inner );
	program.setUniformInt    ( "outer", outer );
	program.setPatchSize     ( PATCH_SIZE );
	
	vao.bind ();
	
	glPolygonMode ( GL_FRONT_AND_BACK, GL_LINE );
	glDrawArrays  ( GL_PATCHES, 0, NUM_VERTICES );
	
	vao.unbind ();
	program.unbind ();

	glutSwapBuffers ();
}

void reshape ( int w, int h )
{
	glViewport ( 0, 0, (GLsizei)w, (GLsizei)h );
   
	mat4 proj = perspective ( 60.0f, (float)w / (float)h, 0.5f, 20.0f ) * lookAt ( eye, vec3 :: zero, vec3 ( 0, 0, 1 ) );

	program.bind ();
	program.setUniformMatrix ( "proj", proj );
	program.unbind ();  
}


void key ( unsigned char key, int x, int y )
{
	if ( key == 27 || key == 'q' || key == 'Q' )	//	quit requested
    	exit ( 0 );
		
	if ( key == '+' )
		inner++;
	else
	if ( key == '-' && inner > 1 )
		inner--;
		
	if ( key == '*' )
		outer++;
	else
	if ( key == '/' && outer > 1)
		outer--;
		
	if ( key == '+' || key == '-' || key == '/' || key == '*' )
		updateCaption ();

	glutPostRedisplay ();
}

void motion ( int x, int y )
{
    rot.x += ((mouseOldY - y) * 180.0f) / 200.0f;
    rot.z += ((mouseOldX - x) * 180.0f) / 200.0f;
    rot.y  = 0;

    if ( rot.z > 360 )
        rot.z -= 360;

    if ( rot.z < -360 )
        rot.z += 360;

    if ( rot.y > 360 )
        rot.y -= 360;

    if ( rot.y < -360 )
        rot.y += 360;

    mouseOldX = x;
    mouseOldY = y;

    glutPostRedisplay ();
}

void mouse ( int button, int state, int x, int y )
{
    if ( state == GLUT_DOWN )
    {
        mouseOldX = x;
        mouseOldY = y;
    }
}

int main ( int argc, char * argv [] )
{
								// initialize glut
	glutInit            ( &argc, argv );
	glutInitDisplayMode ( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	glutInitWindowSize  ( 500, 500 );

								// prepare context for 4.1
	glutInitContextVersion ( 4, 1 );
	glutInitContextFlags   ( GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG );
	glutInitContextProfile ( GLUT_CORE_PROFILE );

								// create window
	glutCreateWindow ( "PN-triangle tesselation demo" );

	glewExperimental = GL_TRUE;

	glewInit ();

	if ( !GLEW_VERSION_4_1 )
	{
		printf ( "OpenGL 4.1 not supported.\n" );
		
		return 1;
	}
	
	getGlErrorString ();		// glew gives GL_INVALID_ENUM error, just remove it

								// register handlers
	glutDisplayFunc  ( display );
	glutReshapeFunc  ( reshape );
	glutKeyboardFunc ( key     );
    glutMouseFunc    ( mouse   );
    glutMotionFunc   ( motion  );

	init ();
	
	if ( !program.loadProgram ( "pn.glsl" ) )
	{
		printf ( "Error loading shader: %s\n", program.getLog ().c_str () );
		
		return 1;
	}
	else
		printf ( "Shader loaded:\n%s\n", program.getLog ().c_str () );

	program.bind ();

	vao.create ();
	vao.bind   ();

	buf.create  ();
	buf.bind    ( GL_ARRAY_BUFFER );
	buf.setData ( NUM_VERTICES * VERTEX_SIZE, vertices, GL_STATIC_DRAW );

	program.setAttrPtr ( "position", 3, VERTEX_SIZE, (void *) 0 );
	program.setAttrPtr ( "normal",   3, VERTEX_SIZE, (void *)(3 * sizeof ( float ) ) );

	buf.unbind     ();
	vao.unbind     ();
	program.unbind ();

	updateCaption ();
    glutMainLoop ();

	return 0;
}
