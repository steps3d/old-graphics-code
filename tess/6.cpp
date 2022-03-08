//
// Example of using freeglut and glew to get OpenGL 3.3/4.1
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
#include	"checks.h"
#include	"mat4.h"

#define	NUM_VERTICES	3
#define	VERTEX_SIZE		(3*sizeof(float))

int     mouseOldX = 0;
int     mouseOldY = 0;
vec3	rot ( 0.0f );
float	inner = 1.3;
float	outer = 1.3;

GLuint 	vbo, vao;
Program	program;

static const float vertices [] = 
{
   -2.0f, -2.0f, 0.0f, 
	0.0f,  2.0f, 0.0f, 
	2.0f, -2.0f, 0.0f
};

void init ()
{
	glClearColor ( 0.5, 0.5, 0.5, 1.0 );
	glEnable     ( GL_DEPTH_TEST );
	glDepthFunc  ( GL_LEQUAL );

	glHint ( GL_POLYGON_SMOOTH_HINT,         GL_NICEST );
	glHint ( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
}

void updateCaption ()
{
	char str [128];

	sprintf ( str, "Inner Level = %4.1f, Outer Level = %4.1f (odd spacing)", inner, outer );

	glutSetWindowTitle ( str );
}

void display ()
{
	glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    mat4	mv = mat4 :: rotateZ ( toRadians ( rot.z ) ) * mat4 :: rotateY ( toRadians ( rot.y ) ) * mat4 :: rotateX ( toRadians ( rot.x ) );
	
	program.bind ();
	program.setUniformMatrix ( "mv", mv );
	program.setUniformFloat ( "inner", inner );
	program.setUniformFloat ( "outer", outer );

	glBindVertexArray ( vao );
	glPolygonMode ( GL_FRONT_AND_BACK, GL_LINE );
	glPatchParameteri ( GL_PATCH_VERTICES, 3);
	glDrawArrays ( GL_PATCHES, 0, NUM_VERTICES );
	glBindVertexArray ( 0 );

	program.unbind ();

	glutSwapBuffers ();
}

void reshape ( int w, int h )
{
	vec3	eye ( 0, 0, 5 );
	
	glViewport ( 0, 0, (GLsizei)w, (GLsizei)h );
   
	mat4 proj = perspective ( 60.0f, (float)w / (float)h, 0.5f, 15.0f ) * lookAt ( eye, vec3 :: zero, vec3 ( 0, 1, 0 ) );

	program.bind ();
	program.setUniformMatrix ( "proj", proj );
	program.unbind ();  
}

void key ( unsigned char key, int x, int y )
{
	if ( key == 27 || key == 'q' || key == 'Q' )	//	quit requested
    	exit ( 0 );
		
	if ( key == '+' )
		inner += 0.3;
	else
	if ( key == '-' && inner > 1 )
		inner -= 0.3;
		
	if ( key == '*' )
		outer += 0.3;
	else
	if ( key == '/' && outer > 1)
		outer -= 0.3;
		
	if ( key == '+' || key == '-' || key == '/' || key == '*' )
		updateCaption ();

	glutPostRedisplay ();
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
	glutInitWindowSize  ( 400, 400 );

								// prepare context for 4.1
	glutInitContextVersion ( 4, 1 );
	glutInitContextFlags   ( GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG );
	glutInitContextProfile ( GLUT_CORE_PROFILE );

								// create window
	glutCreateWindow ( "Tesselation demo (odd spacing)" );

	glewExperimental = GL_TRUE;

	glewInit ();

	if ( !GLEW_VERSION_4_1 )
	{
		printf ( "OpenGL 4.1 not supported.\n" );
		
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

	if ( !program.loadShaderOfType ( "3.vsh", GL_VERTEX_SHADER ) )
	{
		printf ( "Error loading vertex shader: %s\n", program.getLog ().c_str () );
		
		return 1;
	}
	
	if ( !program.loadShaderOfType ( "3.fsh", GL_FRAGMENT_SHADER ) )
	{
		printf ( "Error loading fragment shader: %s\n", program.getLog ().c_str () );
		
		return 1;
	}
	
	if ( !program.loadShaderOfType ( "5.tcs", GL_TESS_CONTROL_SHADER ) )
	{
		printf ( "Error loading tess control shader: %s\n", program.getLog ().c_str () );
		
		return 1;
	}
	
	if ( !program.loadShaderOfType ( "3.tes", GL_TESS_EVALUATION_SHADER ) )
	{
		printf ( "Error loading tess eval shader: %s\n", program.getLog ().c_str () );
		
		return 1;
	}
	
	if ( !program.loadShaderOfType ( "3.gsh", GL_GEOMETRY_SHADER ) )
	{
		printf ( "Error loading tess eval shader: %s\n", program.getLog ().c_str () );
		
		return 1;
	}
	
	if ( !program.relink () )
	{
		printf ( "Error linking: %s\n", program.getLog ().c_str () );
		
		return 1;
	}


	program.bind ();

	glGenVertexArrays ( 1, &vao );
	glBindVertexArray ( vao );

	glGenBuffers ( 1, &vbo );
	glBindBuffer ( GL_ARRAY_BUFFER, vbo );
	glBufferData ( GL_ARRAY_BUFFER, NUM_VERTICES * VERTEX_SIZE, vertices, GL_STATIC_DRAW );	

	program.setAttrPtr ( "position", 3, VERTEX_SIZE, (void *) 0 );
	program.setAttrPtr ( "color",    3, VERTEX_SIZE, (void *)(3 * sizeof ( float ) ) );

	program.unbind ();

	updateCaption ();
    glutMainLoop ();

	return 0;
}
