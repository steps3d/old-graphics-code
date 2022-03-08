//
// ARB_separate_shader_objects example 
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
#include	"Pipeline.h"
#include	"glUtilities.h"
#include	"mat4.h"
#include	"mat3.h"
#include	"vec2.h"
#include	"VertexArray.h"
#include	"VertexBuffer.h"

int     mouseOldX = 0;
int     mouseOldY = 0;
float	angle     = 0;
vec3	rot   ( 0.0f );
vec3	eye   ( 7, 7, 7 );
vec3	light ( 7, 7, 7 );

Pipeline		p1, p2;				// create two pipelines
Program			vp, fp1, fp2;		// create one common vertex program and two fragment programs
VertexArray		vao;
VertexBuffer	vertexBuf, normalBuf, texBuf;
VertexBuffer	indexBuf;
bool			first = true;

int	eyeLoc, lightLoc, projLoc, mvLoc, nmLoc;

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

    mat4	mv = mat4 :: rotateZ ( toRadians ( rot.z ) ) * mat4 :: rotateX ( toRadians ( rot.x ) ) * mat4 :: rotateY ( toRadians ( rot.y ) );
	mat3	nm = normalMatrix ( mv );
	
	glProgramUniformMatrix4fv ( vp.getProgram (), mvLoc, 1, GL_TRUE, mv.data () );
	glProgramUniformMatrix3fv ( vp.getProgram (), nmLoc, 1, GL_TRUE, nm.data () );

	if ( first )
		p1.bind ();
	else
		p2.bind ();
		
	vao.bind ();

	glDrawElements ( GL_TRIANGLES, 3*mesh.numTris, GL_UNSIGNED_INT, NULL );

	vao.unbind ();
	
	if ( first )
		p1.unbind ();
	else
		p2.unbind ();
		

	glutSwapBuffers ();
}

void reshape ( int w, int h )
{
	glViewport ( 0, 0, (GLsizei)w, (GLsizei)h );
   
	mat4 proj = perspective ( 60.0f, (float)w / (float)h, 0.5f, 20.0f ) * lookAt ( eye, vec3 :: zero, vec3 ( 0, 0, 1 ) );

	glProgramUniform3fv       ( vp.getProgram (), eyeLoc,   1, eye );
	glProgramUniform3fv       ( vp.getProgram (), lightLoc, 1, light );
	glProgramUniformMatrix4fv ( vp.getProgram (), projLoc,  1, GL_TRUE, proj.data () );
}

void key ( unsigned char key, int x, int y )
{
	if ( key == 27 || key == 'q' || key == 'Q' )	//	quit requested
    	exit ( 0 );
		
	if ( key == ' ' )
		first = !first;

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

void	animate ()
{
	angle  = 0.004f * glutGet ( GLUT_ELAPSED_TIME );

	light.x = 8*cos ( angle );
	light.y = 8*sin ( 1.4 * angle );
	light.z = 8 + 0.5 * sin ( angle / 3 );
	
	p1.bind ();
	glProgramUniform3fv ( vp.getProgram (), eyeLoc,   1, eye );
	glProgramUniform3fv ( vp.getProgram (), lightLoc, 1, light );
	p1.unbind ();

checkGlError ( "Set eye/light" );
	
	glutPostRedisplay ();
}

int main ( int argc, char * argv [] )
{
								// initialize glut
	glutInit            ( &argc, argv );
	glutInitDisplayMode ( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	glutInitWindowSize  ( 500, 500 );

								// prepare context for 3.3
	glutInitContextVersion ( 4, 1 );
	glutInitContextFlags   ( GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG );
	glutInitContextProfile ( GLUT_CORE_PROFILE );

								// create window
	glutCreateWindow ( "Program pipeline test" );

	GLint MajorVersion = 0;
	GLint MinorVersion = 0;
	glGetIntegerv(GL_MAJOR_VERSION, &MajorVersion);
	glGetIntegerv(GL_MINOR_VERSION, &MinorVersion);

printf ( "%d.%d\n", MajorVersion, MinorVersion );	

	glewExperimental = GL_TRUE;

	glewInit ();

	if ( !GLEW_ARB_separate_shader_objects )
	{
		printf ( "ARB_separate_shader_objects not supported.\n" );
		
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

	p1.create   ();
	p1.bind     ();

	if ( !vp.loadSeparate  ( GL_VERTEX_SHADER, "sep-vp.glsl" ) )
		printf ( "Error loadSeparate vp\n" );
	
	p1.setStage ( GL_VERTEX_SHADER_BIT, &vp );

	if ( !fp1.loadSeparate ( GL_FRAGMENT_SHADER, "sep-fp1.glsl" ) )
		printf ( "Error loadSeparate fp\n" );
	

	p1.setStage ( GL_FRAGMENT_SHADER_BIT, &fp1 );
	
	if ( !p1.validate () )
		printf ( "Error validating: %s\n", p1.getLog ().c_str () );

	p1.unbind ();	
		
	p2.create ();
	p2.bind   ();

	p2.setStage ( GL_VERTEX_SHADER_BIT, &vp );

	if ( !fp2.loadSeparate ( GL_FRAGMENT_SHADER, "sep-fp2.glsl" ) )
		printf ( "Error loadSeparate fp\n" );
	
	p2.setStage ( GL_FRAGMENT_SHADER_BIT, &fp2 );
	
	if ( !p2.validate () )
		printf ( "Error validating: %s\n", p2.getLog ().c_str () );

	p2.unbind ();

	eyeLoc   = glGetUniformLocation ( vp.getProgram (), "eye" );
	lightLoc = glGetUniformLocation ( vp.getProgram (), "light" );
	projLoc  = glGetUniformLocation ( vp.getProgram (), "proj" );
	mvLoc    = glGetUniformLocation ( vp.getProgram (), "mv" );
	nmLoc    = glGetUniformLocation ( vp.getProgram (), "nm" );

	makeTorus ( mesh, 1, 4, 30, 30 );
			
	vao.create ();
	vao.bind   ();

	vertexBuf.create  ();
	vertexBuf.bind    ( GL_ARRAY_BUFFER );
	vertexBuf.setData ( mesh.numVertices * sizeof ( vec3 ), mesh.vertices, GL_STATIC_DRAW );

	vertexBuf.setAttrPtr ( 0, 3, sizeof ( vec3 ), (void *) 0 , GL_FLOAT );

	normalBuf.create ();
	normalBuf.bind ( GL_ARRAY_BUFFER );
	normalBuf.setData ( mesh.numVertices * sizeof ( vec3 ), mesh.normals, GL_STATIC_DRAW );

	vertexBuf.setAttrPtr ( 1, 3, sizeof ( vec3 ), (void *) 0 , GL_FLOAT );

	indexBuf.create  ();
	indexBuf.bind    ( GL_ELEMENT_ARRAY_BUFFER );
	indexBuf.setData ( 3 * mesh.numTris * sizeof ( unsigned ), mesh.faces, GL_STATIC_DRAW );
	
	vao.unbind ();

    glutMainLoop ();

	return 0;
}
