//
// Lighting example with UBO
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
#include	"mat3.h"
#include	"vec2.h"
#include	"VertexArray.h"
#include	"VertexBuffer.h"
#include	"TypeDefs.h"

int     mouseOldX = 0;
int     mouseOldY = 0;
float	angle     = 0;
vec3	rot   ( 0.0f );
vec3	eye   ( 7, 7, 7 );
vec3	light ( 7, 7, 7 );

vec4	diffColor ( 0.5, 0.0, 0.0, 1.0 );
vec4	specColor ( 0.7, 0.9, 0.0, 1.0 );
float	specPower = 70.0;

Program			program;
VertexArray		vao;
VertexBuffer	vertexBuf, normalBuf, texBuf;
VertexBuffer	indexBuf;
VertexBuffer	ubo;						// uniform buffer object

struct Mesh
{
	int	   numVertices;
	int	   numTris;
	vec3 * vertices;
	vec3 * normals;
	vec2 * tex;
	unsigned * faces;		// triangle indices
};

Mesh	mesh;
	
void	setupUbo ( VertexBuffer& buf, int bindingPoint, const vec4 dColor, const vec4 sColor, float specPwr )
{
	static const char * names [3] =
	{
		"diffColor",
		"specColor",
		"specPower"	
	};
	
	static	GLuint	index  [3];			// index for every variable
	static	int		offset [3];
	static	GLuint	blockIndex;
	static	int		blockSize;
	static	bool	inited = false;
	
	if ( !inited )
	{
		inited     = true;
		blockIndex = program.indexForUniformBlock ( "Lighting" );
		blockSize  = program.uniformBlockSize ( blockIndex );
		glGetUniformIndices   ( program.getProgram (), 3, names, index );
		glGetActiveUniformsiv ( program.getProgram (), 3, index, GL_UNIFORM_OFFSET, offset );
		
										// init with zero's
		byte  * buffer = new byte [blockSize];
		
		memset ( buffer, 0, blockSize );
		
		buf.create   ();
		buf.bindBase ( GL_UNIFORM_BUFFER, bindingPoint );
		buf.setData  ( blockSize, buffer, GL_STREAM_DRAW );
		
		delete buffer;
	}
	
	buf.bindBase ( GL_UNIFORM_BUFFER, bindingPoint );
	program.bindBufferToIndex ( blockIndex, bindingPoint );

	byte  * ptr = (byte *) buf.map ( GL_WRITE_ONLY );
	
	memcpy ( ptr + offset [0], &dColor.x, 16 );
	memcpy ( ptr + offset [1], &sColor.x, 16 );
	memcpy ( ptr + offset [2], &specPwr,  4  );
	
	buf.unmap ();
}

void makeTorus ( Mesh& mesh, float r1, float r2, int rings, int sides )
{
	float 	ringDelta = 2.0 * M_PI / rings;
	float 	sideDelta = 2.0 * M_PI / sides;
	float	invRings  = 1.0 / (float) rings;
	float	invSides  = 1.0 / (float) sides;
	int 	i, j;
	int	index = 0;

	mesh.numVertices = sides*rings;
	mesh.numTris     = sides * rings * 2;
	mesh.vertices    = new vec3 [sides*rings];
	mesh.normals     = new vec3 [sides*rings];
	mesh.tex         = new vec2 [sides*rings];
	mesh.faces       = new unsigned [3*sides*rings*2];

	for ( i = 0; i < rings; i++ )
	{
    	float theta    = i * ringDelta;
	    float cosTheta = cos ( theta );
    	float sinTheta = sin ( theta );

		for ( j = 0; j < sides; j++ )
		{
			float phi    = j * sideDelta;
			float cosPhi = cos ( phi );
			float sinPhi = sin ( phi );
			float dist = r2 + r1 * cosPhi;

      	// (x,y,z) below is the parametric equation for a torus
        // when theta and phi both vary from 0 to pi.

      				// x = cos(theta) * (R + r * cos(phi))
      				// y = -sin(theta) * (R + r * cos(phi))
      				// z = r * sin(phi)

      		mesh.vertices [index].x = cosTheta * dist;
      		mesh.vertices [index].y = -sinTheta * dist;
      		mesh.vertices [index].z = r1 * sinPhi;

      // now setup texture coordinates

			mesh.tex [index].x = j * invSides;
      		mesh.tex [index].y = i * invRings;

		// compute unit normal

			mesh.normals [index].x =  cosTheta * cosPhi;
			mesh.normals [index].y = -sinTheta * cosPhi;
			mesh.normals [index].z = sinPhi;

      		index++;
		}
	}
											// Create faces
	index = 0;

	for ( i = 0; i < rings; i++ )
		for ( j = 0; j < sides; j++ )
		{
			int	i1 = (i + 1) % rings;
			int	j1 = (j + 1) % sides;

			mesh.faces [index++] = i*sides + j;
			mesh.faces [index++] = i1*sides + j;
			mesh.faces [index++] = i1*sides + j1;

			mesh.faces [index++] = i*sides + j;
			mesh.faces [index++] = i1*sides + j1;
			mesh.faces [index++] = i*sides + j1;
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
	glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    mat4	mv = mat4 :: rotateZ ( toRadians ( rot.z ) ) * mat4 :: rotateY ( toRadians ( rot.y ) ) * mat4 :: rotateX ( toRadians ( rot.x ) );
	mat3	nm = normalMatrix ( mv );
	
	program.bind ();
	program.setUniformMatrix ( "mv",  mv );
	program.setUniformMatrix ( "nm",  nm );
	vao.bind ();
	
	glDrawElements ( GL_TRIANGLES, 3*mesh.numTris, GL_UNSIGNED_INT, NULL );

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
	program.setUniformVector ( "eye",  eye );
	program.setUniformVector ( "light",  light );
	program.unbind ();  
}

void key ( unsigned char key, int x, int y )
{
	if ( key == 27 || key == 'q' || key == 'Q' )	//	quit requested
    	exit ( 0 );
		
	if ( key == '1' )
	{
		program.bind ();
		vao.bind ();
		setupUbo ( ubo, 0, diffColor, specColor, specPower );
		vao.unbind ();
		program.unbind ();
	}
	else
	if ( key == '2' )
	{
		program.bind ();
		vao.bind ();
		setupUbo ( ubo, 0, vec4 ( 0, 0.8, 0, 1 ), vec4 ( 0, 1, 0, 1 ), 50 );
		vao.unbind ();
		program.unbind ();
	}
		
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

void	animate ()
{
	angle  = 0.004f * glutGet ( GLUT_ELAPSED_TIME );

	light.x = 8*cos ( angle );
	light.y = 8*sin ( 1.4 * angle );
	light.z = 8 + 0.5 * sin ( angle / 3 );

	program.bind ();
	program.setUniformVector ( "eye",   eye   );
	program.setUniformVector ( "light", light );
	program.unbind ();

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
	glutCreateWindow ( "UBO example - use 1 and 2 keys to change data" );

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

	if ( !program.loadProgram ( "light-ub.glsl" ) )
	{
		printf ( "Error loading shader: %s\n", program.getLog ().c_str () );
		
		return 1;
	}
	else
		printf ( "Shader loaded:\n%s\n", program.getLog ().c_str () );
	
	program.bind ();

	makeTorus ( mesh, 1, 4, 30, 30 );
	
	vao.create ();
	vao.bind   ();

	vertexBuf.create  ();
	vertexBuf.bind    ( GL_ARRAY_BUFFER );
	vertexBuf.setData ( mesh.numVertices * sizeof ( vec3 ), mesh.vertices, GL_STATIC_DRAW );

	program.setAttrPtr ( "pos", 3, sizeof ( vec3 ), (void *) 0 );

	normalBuf.create ();
	normalBuf.bind ( GL_ARRAY_BUFFER );
	normalBuf.setData ( mesh.numVertices * sizeof ( vec3 ), mesh.normals, GL_STATIC_DRAW );

	program.setAttrPtr ( "normal", 3, sizeof ( vec3 ), (void *) 0 );

	indexBuf.create  ();
	indexBuf.bind    ( GL_ELEMENT_ARRAY_BUFFER );
	indexBuf.setData ( 3 * mesh.numTris * sizeof ( unsigned ), mesh.faces, GL_STATIC_DRAW );
	
	setupUbo ( ubo, 0, diffColor, specColor, specPower );
	
	vao.unbind     ();
	program.unbind ();

    glutMainLoop ();

	return 0;
}
