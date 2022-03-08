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
#include	<map>

#include	"Program.h"
#include	"checks.h"
#include	"mat4.h"
#include	"mat3.h"
#include	"vec2.h"
#include	"VertexArray.h"
#include	"VertexBuffer.h"

using namespace std;

int     mouseOldX = 0;
int     mouseOldY = 0;
vec3	rot ( 0.0f );
vec3	eye ( 7, 7, 7 );
float	finSize = 0.3;

Program			program;
VertexArray		vao;
VertexBuffer	vertexBuf, normalBuf, texBuf;
VertexBuffer	indexBuf;

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
	
class	Edge
{
	int	from, to;
public:
	Edge ( int f, int t )
	{
		from = f;
		to   = t;
	}
	
	bool	operator == ( const Edge& e ) const
	{
		return from == e.from && to == e.to;
	}
	
	bool	operator < ( const Edge& e ) const
	{
		if ( from < e.from )
			return true;
			
		if ( from > e.from )
			return false;
			
		return to < e.to;
	}
};

unsigned * makeAdjacencyIndex ( int numTris, unsigned * src )
{
	int	       n   = 3 * numTris;
	unsigned * res = new unsigned [2*n];	// it will double number of elements
	map<Edge, unsigned>	adj;				// for every edge keep vertex of triangle
	
											// build adjacence map
	for ( int i = 0; i < numTris; i++ )
	{
		int	j = 3 * i;
		
		adj [Edge ( src [j],   src [j+1])] = src [j+2];
		adj [Edge ( src [j+1], src [j+2])] = src [j];
		adj [Edge ( src [j+2], src [j]  )] = src [j+1];
	}
	
										// now fill new structure
	for ( int i = 0; i < numTris; i++ )
	{
		int	j = 6 * i;
		int	k = 3 * i;
		
		res [j+0] = src [k+0];			// source triangle
		res [j+2] = src [k+1];
		res [j+4] = src [k+2];
										// now adjacent vertices
		res [j+1] = adj [Edge ( src [k+1], src [k  ] )];
		res [j+3] = adj [Edge ( src [k+2], src [k+1] )];
		res [j+5] = adj [Edge ( src [k],   src [k+2] )];
	}
	
	return res;
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
		
											// change mesh type to GL_TRIANGLES_ADJACENCY
	unsigned * newIndex = makeAdjacencyIndex ( mesh.numTris, mesh.faces );
	
	delete mesh.faces;
	
	mesh.faces = newIndex;

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
	mat4	nm ( mv );
	
	nm.invert ();
	nm.transpose ();
	
	program.bind ();
	program.setUniformMatrix ( "mv",  mv );
	program.setUniformMatrix ( "nm",  nm );
	vao.bind ();
	
	glDrawElements ( GL_TRIANGLES_ADJACENCY, 6*mesh.numTris, GL_UNSIGNED_INT, NULL );

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
	program.setUniformFloat  ( "finSize", finSize );
	program.unbind ();  
}

void key ( unsigned char key, int x, int y )
{
	if ( key == 27 || key == 'q' || key == 'Q' )	//	quit requested
    	exit ( 0 );
		
	if ( key == '+' )
		finSize += 0.1;
	else
	if ( key == '-' )
		finSize -= 0.1;
		
	program.bind ();
	program.setUniformFloat (  "finSize", finSize );
	program.unbind ();  
	
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
	glutInitWindowSize  ( 500, 500 );

								// prepare context for 3.3
	glutInitContextVersion ( 3, 3 );
	glutInitContextFlags   ( GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG );
	glutInitContextProfile ( GLUT_CORE_PROFILE );

								// create window
	glutCreateWindow ( "Geometry shader example - fins extrusion" );

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

	init ();
	
	if ( !GL_ARB_vertex_array_object )
		printf ( "No VAO support\n" );

	if ( !program.loadProgram ( "geom-4.glsl" ) )
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
	indexBuf.setData ( 6 * mesh.numTris * sizeof ( unsigned ), mesh.faces, GL_STATIC_DRAW );
	
	vao.unbind     ();
	program.unbind ();

    glutMainLoop ();

	return 0;
}
