//
// Class to hold simple meshes.
// For every vertex position, texture coordinate and TBN basis is stored
//

#pragma once
#ifndef	__BASIC_MESH__
#define	__BASIC_MESH__

#include <string>

#include "vec2.h"
#include "vec3.h"
#include "VertexBuffer.h"
#include "VertexArray.h"

class	mat4;

using namespace std;

struct  BasicVertex
{
	vec3	pos;
	vec2	tex;
	vec3	n;
	vec3	t, b;
};

class BasicMesh
{
	int	         numVertices;
	int	         numTriangles;
	VertexArray	 vao;     // array with all bindings
	VertexBuffer vertBuf; // vertex data
	VertexBuffer indBuf;  // index buffer
	string		 name;
	
public:
	BasicMesh ( BasicVertex * vertices, const int * indices, int nv, int nt );
	
	void	render ();
	void	renderInstanced ( int primCount );
	
	const string& getName () const
	{
		return name;
	}
	
	int getNumVertices () const
	{
		return numVertices;
	}
	
	int getNumTriangles () const
	{
		return numTriangles;
	}
	
	void setName ( const string& theName )
	{
		name = theName;
	}
};

void	computeTangents ( BasicVertex& v0, const BasicVertex& v1, const BasicVertex& v2 );
void	computeNormals  ( BasicVertex * vertices, const int * indices, int nv, int nt );

BasicMesh * createSphere( const vec3& org, float radius, int n1, int n2 );
BasicMesh * createQuad  ( const vec3& org, const vec3& dir1, const vec3& dir2 );
BasicMesh * createBox   ( const vec3& pos, const vec3& size, const mat4 * mat = NULL, bool invertNormal = false );
BasicMesh * createTorus ( float r1, float r2, int n1, int n2 );
BasicMesh * createKnot  ( float r1, float r2, int n1, int n2 );

		// create parametric surface given domain and function
BasicMesh * createParamatric ( float sMin, float sMax, float tMin, float tMax, int n1, int n2, vec3 (*f)( const vec2& uv ) );

inline vec3 trefoilKnot ( const vec2& uv )
{
	const float a = 0.5f;
	const float b = 0.3f;
	const float c = 0.5f;
	const float d = 0.15f;
	
	float u = (2*M_PI - uv.x) * 2;
	float v = uv.y;
	float r = a + b * cos(1.5f * u);
	float x = r * cos(u);
	float y = r * sin(u);
	float z = c * sin(1.5f * u);
	
	vec3 dv;
	dv.x = -1.5f * b * sin(1.5f * u) * cos(u) - (a + b * cos(1.5f * u)) * sin(u);
	dv.y = -1.5f * b * sin(1.5f * u) * sin(u) + (a + b * cos(1.5f * u)) * cos(u);
	dv.z =  1.5f * c * cos(1.5f * u);
	
	vec3 t  = normalize ( dv );
	vec3 t2 = normalize ( vec3 ( t.y, -t.x, 0 ) );
	vec3 n  = cross ( t, t2 );
	vec3 range;
	
	range.x = x + d * (t2.x * cos(v) + n.x * sin(v));
	range.y = y + d * (t2.y * cos(v) + n.y * sin(v));
	range.z = z + d * n.z * sin(v);
	
	return range;
}

inline vec3 kleinBottle ( const vec2& uv )
{
	float v  = 1 - uv.x;
	float u  = uv.y;
	float x0 = 3 * cos(u) * (1 + sin(u)) + (2 * (1 - cos(u) / 2)) * cos(u) * cos(v);
	float y0 = 8 * sin(u) + (2 * (1 - cos(u) / 2)) * sin(u) * cos(v);
	float x1 = 3 * cos(u) * (1 + sin(u)) + 	(2 * (1 - cos(u) / 2)) * cos(v + M_PI);
	float y1 = 8 * sin(u);
	
	return vec3 ( u < M_PI ? x0 : x1, u < M_PI ? -y0 : -y1, -2*(1 - cos(u) / 2) * sin(v) );
}

inline	vec3 mobiusStrip ( const vec2& uv )
{
	float major = 1.25;
	float a     = 0.125f;
	float b     = 0.5f;
	float phi = uv.x / 2.0f;
	
				// General equation for an ellipse where phi is the angle
				// between the major axis and the X axis.
	float x = a * cos(uv.y) * cos(phi) - b * sin(uv.y) * sin(phi);
	float y = a * cos(uv.y) * sin(phi) + b * sin(uv.y) * cos(phi);

				// Sweep the ellipse along a circle, like a torus.
	return vec3 ( (major + x) * cos(uv.x), (major + x) * sin(uv.x), y );
}

#endif
