//
// Transform feedback in OpenGL 3.3 example 
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
#include	"glUtilities.h"
#include	"mat4.h"
#include	"mat3.h"
#include	"vec2.h"
#include	"VertexArray.h"
#include	"VertexBuffer.h"
#include	"randUtils.h"

#define	NUM_PARTICLES	5000

int     mouseOldX = 0;
int     mouseOldY = 0;
float	angle     = 0;
vec3	rot   ( 0.0f );
vec3	eye   ( 3, 3, 3 );
vec3	light ( 7, 7, 7 );

int		ind = 0;
int		loc1 = -1, loc2 = -1;
vec3	p [NUM_PARTICLES];
vec3	v [NUM_PARTICLES];

Program			program;
VertexArray		vao [2];
VertexBuffer	vertexBuf [2], velBuf [2];

void	initParticles ()
{
	for ( int i = 0; i < NUM_PARTICLES; i++ )
	{
		p [i] = vec3 ( 0, 0, 0 );
		v [i] = vec3 ( randUniform ( -0.1, 0.1 ), randUniform ( -0.1, 0.1 ), randUniform ( -0.1, 0.1 ) );
	}
}

void init ()
{
	glClearColor ( 0.5, 0.5, 0.5, 1.0 );
	glEnable     ( GL_DEPTH_TEST );
	glDepthFunc  ( GL_LEQUAL );
}

void display ()
{
	static float lastTime = 0;
	
	float tm  = 0.001f * glutGet ( GLUT_ELAPSED_TIME );

	glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    mat4	mv = mat4 :: rotateZ ( toRadians ( rot.z ) ) * mat4 :: rotateX ( toRadians ( rot.x ) ) * mat4 :: rotateY ( toRadians ( rot.y ) );

	program.bind ();
	program.setUniformMatrix ( "mv",  mv );
	program.setUniformFloat  ( "dt",  tm - lastTime );
	
	vertexBuf [ind^1].bindBase ( GL_TRANSFORM_FEEDBACK_BUFFER, 0 );
	velBuf    [ind^1].bindBase ( GL_TRANSFORM_FEEDBACK_BUFFER, 1 );

	glBeginTransformFeedback ( GL_POINTS );

	vao [ind].bind ();

	glDrawArrays ( GL_POINTS, 0, NUM_PARTICLES );
	
	vao [ind].unbind ();
	
	glEndTransformFeedback ();

	program.unbind ();

	lastTime = tm;
	
	glutSwapBuffers ();
}

void reshape ( int w, int h )
{
	glViewport ( 0, 0, (GLsizei)w, (GLsizei)h );
   
	mat4 proj = perspective ( 60.0f, (float)w / (float)h, 0.5f, 20.0f ) * lookAt ( eye, vec3 :: zero, vec3 ( 0, 1, 0 ) );

	program.bind ();
	program.setUniformMatrix ( "proj",    proj );
	program.setUniformVector ( "boxSize", vec3 ( 2 ) );
	program.unbind ();  
}

void key ( unsigned char key, int x, int y )
{
	if ( key == 27 || key == 'q' || key == 'Q' )	//	quit requested
    	exit ( 0 );
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

void	animate ()
{
	ind ^= 1;
	
	glutPostRedisplay ();
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
	glutCreateWindow ( "Transform feedback" );

	glewExperimental = GL_TRUE;

	glewInit ();

	if ( !GLEW_VERSION_3_3 )
	{
		printf ( "OpenGL 3.3 not supported.\n" );
		
		return 1;
	}
	
	getGlErrorString ();		// glew gives GL_INVALID_ENUM error, just remove it
	
								// register handlers
	glutDisplayFunc  ( display );
	glutReshapeFunc  ( reshape );
	glutKeyboardFunc ( key     );
    glutMouseFunc    ( mouse   );
    glutMotionFunc   ( motion  );
	glutIdleFunc     ( animate );

	init ();
	
	if ( !GL_ARB_vertex_array_object )
		printf ( "No VAO support\n" );

	if ( !program.loadProgram ( "tf3.glsl" ) )
	{
		printf ( "Error loading shader: %s\n", program.getLog ().c_str () );
		
		return 1;
	}
	else
		printf ( "Shader loaded:\n%s\n", program.getLog ().c_str () );

	initParticles ();

	program.bind ();
	program.transformFeedbacksVars ( "newPos;newVel", GL_SEPARATE_ATTRIBS );
	program.relink ();
	program.unbind ();

	for ( int i = 0; i < 2; i++ )
	{
		vao [i].create ();
		vao [i].bind   ();

		vertexBuf [i].create     ();
		vertexBuf [i].bind       ( GL_ARRAY_BUFFER );
		vertexBuf [i].setData    ( NUM_PARTICLES * sizeof ( vec3 ), p, GL_STATIC_DRAW );
		vertexBuf [i].setAttrPtr ( 0, 3, sizeof ( vec3 ), (void *) 0 );

		velBuf [i].create     ();
		velBuf [i].bind       ( GL_ARRAY_BUFFER );
		velBuf [i].setData    ( NUM_PARTICLES * sizeof ( vec3 ), v, GL_STATIC_DRAW );
		velBuf [i].setAttrPtr ( 1, 3, sizeof ( vec3 ), (void *) 0 );
	
		vao [i].unbind     ();
	}
	
    glutMainLoop ();

	return 0;
}
