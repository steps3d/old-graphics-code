#include	<GL/glew.h>

#ifdef	_WIN32
	#include	<GL/wglew.h>
#else
	#include	<GL/glxew.h>
#endif

#include	<stdio.h>
#include	"GlutWindow.h"
#include	"Program.h"
#include	"mat4.h"
#include	"vec2.h"
#include	"Texture.h"
#include	"BasicMesh.h"
#include	"glUtilities.h"
#include	"VertexBuffer.h"

#define	NUM_VERTICES	3
#define	VERTEX_SIZE		(5*sizeof(float))

int     mouseOldX = 0;
int     mouseOldY = 0;
float	angle     = 0;
vec3	rot   ( 0.0f );
vec3	eye   ( 7, 7, 7 );
vec3	light ( 7, 7, 7 );
float	scale = 9.0;

class	MyWindow : public GlutWindow
{
	Program		 program;
	BasicMesh  * mesh;
	VertexBuffer counterBuf;

public:
	MyWindow () : GlutWindow ( 200, 100, 400, 400, "ARB_shader_atomic_counters" )
	{
		glClearColor ( 0.5, 0.5, 0.5, 1.0 );
		glEnable     ( GL_DEPTH_TEST );
		glDepthFunc  ( GL_LEQUAL );
	
		const char * err = getGlErrorString ();
		
		if ( !program.loadProgram ( "atomic-counters.glsl" ) )
		{
			printf ( "Error building program: %s\n", program.getLog ().c_str () );
			
			exit ( 2 );
		}
		else
			printf ( "Shader loaded:\n%s\n", program.getLog ().c_str () );
	
		mesh = createTorus ( 2, 4, 30, 30 );
		
		counterBuf.create ();
		counterBuf.bindBase ( GL_ATOMIC_COUNTER_BUFFER, 0 );	// unbind ???
	}

	virtual	void	redisplay ()
	{
		static	GLuint	buf [] = { 0, 0, 0, 0 };
		static	GLuint	counters [4];
		
		counterBuf.setData ( sizeof ( buf ), buf, GL_DYNAMIC_DRAW );
		
		glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		mat4	mv = mat4 :: rotateZ ( toRadians ( rot.z ) ) * mat4 :: rotateY ( toRadians ( rot.y ) ) * mat4 :: rotateX ( toRadians ( rot.x ) );
		mat3	nm = normalMatrix ( mv );
		
		program.bind ();
		program.setUniformMatrix ( "mv",  mv );
		program.setUniformMatrix ( "nm",  nm );

		mesh -> render ();

		program.unbind ();
		
		glFinish ();
		
		counterBuf.getSubData ( 0, sizeof ( buf ), counters );
		
		printf ( "%4d %4d %d %d\n", counters [0], counters [1], counters [2], counters [3] );
	}

	virtual	void	reshape ( int w, int h )
	{
		GlutWindow::reshape ( w, h );
		
		glViewport ( 0, 0, (GLsizei)w, (GLsizei)h );
	   
		mat4 proj = perspective ( 60.0f, (float)w / (float)h, 0.5f, 20.0f ) * lookAt ( eye, vec3 :: zero, vec3 ( 0, 0, 1 ) );

		program.bind ();
		program.setUniformMatrix ( "proj",  proj );
		program.setUniformVector ( "eye",   eye );
		program.setUniformVector ( "light", light );
		program.unbind ();  
	}
	
    virtual void	mouseMotion ( int x, int y ) 
	{
		rot.y += ((mouseOldY - y) * 180.0f) / 200.0f;
		rot.z += ((mouseOldX - x) * 180.0f) / 200.0f;
		rot.x  = 0;

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
	}
	
	virtual void	mouseClick ( int button, int state, int modifiers, int x, int y ) 
	{
		if ( state == GLUT_DOWN )
		{
			mouseOldX = x;
			mouseOldY = y;
		}
	}

    virtual void	keyTyped ( unsigned char key, int modifiers, int x, int y )
	{
		if ( key == 27 || key == 'q' || key == 'Q' )	//	quit requested
			exit ( 0 );
	}
	
	virtual	void	idle () 
	{
		angle  = 4 * getTime ();

		light.x = 8*cos ( angle );
		light.y = 8*sin ( 1.4 * angle );
		light.z = 8 + 0.5 * sin ( angle / 3 );

		program.bind ();
		program.setUniformVector ( "eye",   eye   );
		program.setUniformVector ( "light", light );
		program.unbind ();

		GlutWindow::idle ();		// for glutPostRedisplay ();
	}
};

int main ( int argc, char * argv [] )
{
								// initialize glut
	GlutWindow::init( argc, argv, 4, 2 );
	
	MyWindow	win;
	
	GlutWindow::run ();
								
	return 0;
}
