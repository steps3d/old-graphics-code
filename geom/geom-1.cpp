//
// Geometry shader example 
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
#include	"checks.h"
#include	"mat4.h"
#include	"VertexArray.h"
#include	"VertexBuffer.h"

#define	NUM_VERTICES	2
#define	VERTEX_SIZE		(4*sizeof(float))

int     mouseOldX = 0;
int     mouseOldY = 0;
vec3	rot ( 0.0f );
vec3	eye ( 7, 7, 7 );

Program	program;
VertexArray	vao;
VertexBuffer	buf;

static float vertices [NUM_VERTICES * 4] =
{
	0, -1, 0, 1,
	1, 0, 0, 2,
};

void init ()
{
	glClearColor ( 0.5, 0.5, 0.5, 1.0 );
	glEnable     ( GL_DEPTH_TEST );
	glDepthFunc  ( GL_LEQUAL );

	glHint ( GL_POLYGON_SMOOTH_HINT,         GL_NICEST );
	glHint ( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
}

void display ()
{
	glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    mat4	mv = mat4 :: rotateZ ( toRadians ( rot.z ) ) * mat4 :: rotateY ( toRadians ( rot.y ) ) * mat4 :: rotateX ( toRadians ( rot.x ) );
    vec3	viewDir = vec3 ( 0, 0, 1 );
    vec3	sideDir = vec3 ( 0, 1, 0 );
    vec3	upDir   = vec3 ( 1, 0, 0 );
    mat4	nm ( mv );
	
    nm.invert ();
    nm.transpose ();
	
	program.bind ();
	program.setUniformMatrix ( "mv",    mv );
	program.setUniformVector ( "up",    upDir );
	program.setUniformVector ( "right", sideDir );

	vao.bind ();
	glDrawArrays ( GL_POINTS, 0, NUM_VERTICES );
	vao.unbind ();

	program.unbind ();

	glutSwapBuffers ();
}

void reshape ( int w, int h )
{
	glViewport ( 0, 0, (GLsizei)w, (GLsizei)h );
   
	mat4 proj = perspective ( 60.0f, (float)w / (float)h, 0.5f, 15.0f ) * lookAt ( eye, vec3 :: zero, vec3 ( 0, 0, 1 ) );

	program.bind ();
	program.setUniformMatrix ( "proj", proj );
	program.unbind ();  
}

void key ( unsigned char key, int x, int y )
{
	if ( key == 27 || key == 'q' || key == 'Q' )	//	quit requested
    	exit ( 0 );
}

void motion ( int x, int y )
{
    rot.x -= ((mouseOldY - y) * 180.0f) / 200.0f;
    rot.z -= ((mouseOldX - x) * 180.0f) / 200.0f;
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

								// prepare context for 3.3
	glutInitContextVersion ( 3, 3 );
	glutInitContextFlags   ( GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG );
	glutInitContextProfile ( GLUT_CORE_PROFILE );

								// create window
	glutCreateWindow ( "Geometry shader example - particles" );

	glewExperimental = GL_TRUE;

	glewInit ();

	if ( !GLEW_VERSION_3_3 )
	{
		printf ( "OpenGL 3.3 not supported.\n" );
		
		return 1;
	}

								// register handlers
	glutDisplayFunc  ( display );
	glutReshapeFunc  ( reshape );
	glutKeyboardFunc ( key     );
    glutMouseFunc    ( mouse   );
    glutMotionFunc   ( motion  );

	if ( !GL_ARB_vertex_array_object )
		printf ( "No VAO support\n" );

	printf ( "GLSL Version: %s\n", Program::version().c_str () );
	
	getGlErrorString ();

	if ( !program.loadProgram ( "geom-1.glsl" ) )
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

	program.setAttrPtr ( "posSize", 4, VERTEX_SIZE, (void *) 0 );

	buf.unbind  ();
	vao.unbind  ();
	program.unbind ();

    glutMainLoop ();

	return 0;
}
