//
// Several clases to hold simple meshes.
// For every vertex position, texture coordinate and TBN basis is stored
//

#include "BasicMesh.h"
#include "mat4.h"
#include "mat3.h"
#include "vec4.h"

BasicMesh :: BasicMesh ( BasicVertex * vertices, const int * indices, int nv, int nt )
{
	numVertices  = nv;
	numTriangles = nt;
	name         = "";
	
	if ( vertices [0].n.lengthSq () < 0.001 )
		computeNormals  ( vertices, indices, nv, nt );
		
	vao.create ();
	vao.bind   ();
	vertBuf.create    ();
	vertBuf.bind    ( GL_ARRAY_BUFFER );
	vertBuf.setData ( numVertices * sizeof ( BasicVertex ), vertices, GL_STATIC_DRAW );
	
							// register all components for locations
							// 0 -> position
	glVertexAttribPointer ( 0, 	
							3, 			// number of values per vertex
							GL_FLOAT,
							GL_FALSE,	// normalized
							sizeof(BasicVertex), 			// stride (offset to next vertex data)
							(const GLvoid*) NULL );
		
							// 1 -> texture coordinates
	glVertexAttribPointer ( 1, 	
							2, 			// number of values per vertex
							GL_FLOAT,
							GL_FALSE,	// normalized
							sizeof(BasicVertex), 			// stride (offset to next vertex data)
							(const GLvoid*) sizeof ( vec3 ) );
		
							// 2 -> normal
	glVertexAttribPointer ( 2, 	
							3, 			// number of values per vertex
							GL_FLOAT,
							GL_FALSE,	// normalized
							sizeof(BasicVertex), 			// stride (offset to next vertex data)
							(const GLvoid*)(sizeof (vec3) + sizeof ( vec2 )) );
		
							// 3 -> tangent
	glVertexAttribPointer ( 3, 	
							3, 			// number of values per vertex
							GL_FLOAT,
							GL_FALSE,	// normalized
							sizeof(BasicVertex), 			// stride (offset to next vertex data)
							(const GLvoid*) (2*sizeof(vec3)+sizeof (vec2)) );
		
							// 4 -> binormal
	glVertexAttribPointer ( 4, 	
							3, 			// number of values per vertex
							GL_FLOAT,
							GL_FALSE,	// normalized
							sizeof(BasicVertex), 	// stride (offset to next vertex data)
							(const GLvoid*) (3*sizeof(vec3)+sizeof (vec2)) );
		
	glEnableVertexAttribArray ( 0 );
	glEnableVertexAttribArray ( 1 );
	glEnableVertexAttribArray ( 2 );
	glEnableVertexAttribArray ( 3 );
	glEnableVertexAttribArray ( 4 );
	
	indBuf.create  ();
	indBuf.bind    ( GL_ELEMENT_ARRAY_BUFFER );
	indBuf.setData ( 3 * numTriangles * sizeof ( int ), indices, GL_STATIC_DRAW );
	
	vao.unbind ();
}
	
void	BasicMesh :: render ()
{
	vao.bind ();
	
	glDrawElements ( GL_TRIANGLES, 3*numTriangles, GL_UNSIGNED_INT, NULL );

	vao.unbind ();
}

void	BasicMesh :: renderInstanced ( int primCount )
{
	vao.bind ();
	
	glDrawElementsInstanced ( GL_TRIANGLES, 3*numTriangles, GL_UNSIGNED_INT, NULL, primCount );

	vao.unbind ();
}

void	computeNormals  ( BasicVertex * vertices, const int * indices, int nv, int nt )
{
	for ( int face = 0; face < nt; face++ )
	{
		int	 index = 3*face;
		vec3 v0 = vertices [indices[index+0]].pos;
		vec3 v1 = vertices [indices[index+1]].pos;
		vec3 v2 = vertices [indices[index+2]].pos;
		vec3 n  = cross ( v1 - v0, v2 - v0 );
		
		vertices [indices[index+0]].n += n;
		vertices [indices[index+1]].n += n;
		vertices [indices[index+2]].n += n;
	}

	for ( int i = 0; i < nv; i++ )
		vertices [i].n = normalize ( vertices [i].n );
}

void	computeTangents ( BasicVertex& v0, const BasicVertex& v1, const BasicVertex& v2 )
{
	vec3	e0 ( v1.pos.x - v0.pos.x, v1.tex.x - v0.tex.x, v1.tex.y - v0.tex.y );
	vec3	e1 ( v2.pos.x - v0.pos.x, v2.tex.x - v0.tex.x, v2.tex.y - v0.tex.y );
	vec3	cp = cross ( e0, e1 );

	if ( fabs ( cp.x ) > EPS )
	{
		v0.t.x = -cp.y / cp.x;
		v0.b.x = -cp.z / cp.x;
	}
	else
	{
		v0.t.x = 0;
		v0.b.x = 0;
	}

	e0.x = v1.pos.y - v0.pos.y;
	e1.x = v2.pos.y - v0.pos.y;
	cp   = cross ( e0, e1 );

	if ( fabs ( cp.x ) > EPS )
	{
		v0.t.y = -cp.y / cp.x;
		v0.b.y = -cp.z / cp.x;
	}
	else
	{
		v0.t.y = 0;
		v0.b.y = 0;
	}

	e0.x = v1.pos.z - v0.pos.z;
	e1.x = v2.pos.z - v0.pos.z;
	cp   = cross ( e0, e1 );

	if ( fabs ( cp.x ) > EPS )
	{
		v0.t.z = -cp.y / cp.x;
		v0.b.z = -cp.z / cp.x;
	}
	else
	{
		v0.t.z = 0;
		v0.b.z = 0;
	}

	if ( dot ( cross ( v0.t, v0.b ), v0.n ) < 0 )
		v0.t = -v0.t;
}

BasicMesh * createSphere ( const vec3& org, float r, int n1, int n2 )
{
	int	          numVertices = n1*n2;
	int	          numTris     = n1*n2*2;
	BasicVertex * vertices    = new BasicVertex [numVertices];
	int         * faces       = new int [6*numVertices];
	float	      d1 = 1.0f / (float) n1;
	float	      d2 = 1.0f / (float) n2;
	float	      deltaPhi = d1 *  M_PI;
	float	      deltaPsi = d2 * 2.0 * M_PI;
	int		      index    = 0;
	
	for ( int i = 0; i < n1; i++ )
	{
		float phi    = i * deltaPhi;
		float sinPhi = sin ( phi );
		float cosPhi = cos ( phi );

		for ( int j = 0; j < n2; j++ )
		{
			float psi = j * deltaPsi;
			
			vertices [index].n   = vec3 ( sinPhi * cos ( psi ), sinPhi * sin ( psi ), cosPhi );
			vertices [index].pos = org + r * vertices [index].n;
			vertices [index].tex = vec2 ( i * d1, j * d2 );
			vertices [index].t   = vec3 ( sin ( psi ), cos ( psi ), 0 );
			vertices [index].b   = cross ( vertices [index].n, vertices [index].t );
			index++;
		}
	}
	
	index = 0;

	for ( int i = 0; i < n1; i++ )
		for ( int j = 0; j < n2; j++ )
		{
			int	i1 = (i + 1) % n1;
			int	j1 = (j + 1) % n2;

			faces [index++] = i*n2 + j;
			faces [index++] = i1*n2 + j;
			faces [index++] = i1*n2 + j1;

			faces [index++] = i*n2 + j;
			faces [index++] = i1*n2 + j1;
			faces [index++] = i*n2 + j1;
		}
		
	BasicMesh * mesh = new BasicMesh ( vertices, faces, numVertices, numTris );
	
	delete vertices;
	delete faces;
	
	return mesh;		
}

BasicMesh * createQuad  ( const vec3& org, const vec3& dir1, const vec3& dir2 )
{
	BasicVertex vertices [4];
	int			indices [6] = { 0, 1, 2, 0, 2, 3 };
	vec3		n = normalize( cross ( dir1, dir2 ) );
	vec3		t = normalize( dir1 );
	vec3	 	b = normalize( dir2 );
	
	vertices [0].pos = org;
	vertices [0].tex = vec2 ( 0, 0 );
	vertices [0].n   = n;
	vertices [0].t   = t;
	vertices [0].b   = b;

	vertices [1].pos = org + dir2;
	vertices [1].tex = vec2 ( 0, 1 );
	vertices [1].n   = n;
	vertices [1].t   = t;
	vertices [1].b   = b;
	
	vertices [2].pos = org + dir1 + dir2;
	vertices [2].tex = vec2 ( 1, 1 );
	vertices [2].n   = n;
	vertices [2].t   = t;
	vertices [2].b   = b;
	
	vertices [3].pos = org + dir1;
	vertices [3].tex = vec2 ( 1, 0 );
	vertices [3].n   = n;
	vertices [3].t   = t;
	vertices [3].b   = b;
	
	return new BasicMesh ( vertices, indices, 4, 6 );
}

BasicMesh * createBox ( const vec3& pos, const vec3& size, const mat4 * mat, bool invertNormal )
{
    float   	x2 = pos.x + size.x;
    float   	y2 = pos.y + size.y;
    float   	z2 = pos.z + size.z;
    float   	ns = invertNormal ? -1 : 1;
	int			numVertices = 4*6;			// 4 vertices per each face
	int			numTris     = 6*2;			// 2 tris per face
	int			c = 0;
	BasicVertex	vertices [4*6];
	int			indices  [6*4*6];
		
                                    // front face
	vertices [0].pos = vec3 ( pos.x, pos.y, z2 );
	vertices [0].tex = vec2 ( 0, 0 );
	vertices [0].n   = vec3 ( 0, 0, ns );
	vertices [0].t   = vec3 ( 1, 0, 0 );
	vertices [0].b   = vec3 ( 0, 1, 0 );

	vertices [1].pos = vec3 ( x2, pos.y, z2 );
	vertices [1].tex = vec2 ( size.x, 0 );
	vertices [1].n   = vec3 ( 0, 0, ns );
	vertices [1].t   = vec3 ( 1, 0, 0 );
	vertices [1].b   = vec3 ( 0, 1, 0 );

	vertices [2].pos = vec3 ( x2, y2, z2 );
	vertices [2].tex = vec2 ( size.x, size.y );
	vertices [2].n   = vec3 ( 0, 0, ns );
	vertices [2].t   = vec3 ( 1, 0, 0 );
	vertices [2].b   = vec3 ( 0, 1, 0 );

	vertices [3].pos = vec3 ( pos.x, y2, z2 );
	vertices [3].tex = vec2 ( 0, size.y );
	vertices [3].n   = vec3 ( 0, 0, ns );
	vertices [3].t   = vec3 ( 1, 0, 0 );
	vertices [3].b   = vec3 ( 0, 1, 0 );

								// back face
	vertices [4].pos = vec3 ( x2, pos.y, pos.z );
	vertices [4].tex = vec2 ( size.x, 0 );
	vertices [4].n   = vec3 ( 0, 0, -ns );
	vertices [4].t   = vec3 ( -1, 0, 0 );
	vertices [4].b   = vec3 ( 0, -1, 0 );

	vertices [5].pos = vec3 ( pos.x, pos.y, pos.z );
	vertices [5].tex = vec2 ( 0, 0 );
	vertices [5].n   = vec3 ( 0, 0, -ns );
	vertices [5].t   = vec3 ( -1, 0, 0 );
	vertices [5].b   = vec3 ( 0, -1, 0 );

	vertices [6].pos = vec3 ( pos.x, y2, pos.z );
	vertices [6].tex = vec2 ( 0, size.y );
	vertices [6].n   = vec3 ( 0, 0, -ns );
	vertices [6].t   = vec3 ( -1, 0, 0 );
	vertices [6].b   = vec3 ( 0, -1, 0 );

	vertices [7].pos = vec3 ( x2, y2, pos.z );
	vertices [7].tex = vec2 ( size.x, size.y );
	vertices [7].n = vec3 ( 0, 0, -ns );
	vertices [7].t = vec3 ( -1, 0, 0 );
	vertices [7].b = vec3 ( 0, -1, 0 );

								// left face
	vertices [8].pos = vec3 ( pos.x, pos.y, pos.z );
	vertices [8].tex = vec2 ( 0, 0 );
	vertices [8].n   = vec3 ( -ns, 0, 0 );
	vertices [8].t   = vec3 ( 0, 0, -1 );
	vertices [8].b =   vec3 ( 0, -1, 0 );

	vertices [9].pos = vec3 ( pos.x, pos.y, z2 );
	vertices [9].tex = vec2 ( 0, size.z );
	vertices [9].n = vec3 ( -ns, 0, 0 );
	vertices [9].t = vec3 ( 0, 0, -1 );
	vertices [9].b = vec3 ( 0, -1, 0 );

	vertices [10].pos = vec3 ( pos.x, y2, z2 );
	vertices [10].tex = vec2 ( size.y, size.z );
	vertices [10].n = vec3 ( -ns, 0, 0 );
	vertices [10].t = vec3 ( 0, 0, -1 );
	vertices [10].b = vec3 ( 0, -1, 0 );

	vertices [11].pos = vec3 ( pos.x, y2, pos.z );
	vertices [11].tex = vec2 ( size.y, 0 );
	vertices [11].n = vec3 ( -ns, 0, 0 );
	vertices [11].t = vec3 ( 0, 0, -1 );
	vertices [11].b = vec3 ( 0, -1, 0 );

								// right face
	vertices [12].pos = vec3 ( x2, pos.y, z2 );
	vertices [12].tex = vec2 ( 0, size.z );
	vertices [12].n = vec3 ( ns, 0, 0 );
	vertices [12].t = vec3 ( 0, 0, 1 );
	vertices [12].b = vec3 ( 0, 1, 0 );

	vertices [13].pos = vec3 ( x2, pos.y, pos.z );
	vertices [13].tex = vec2 ( 0, 0 );
	vertices [13].n = vec3 ( ns, 0, 0 );
	vertices [13].t = vec3 ( 0, 0, 1 );
	vertices [13].b = vec3 ( 0, 1, 0 );

	vertices [14].pos = vec3 ( x2, y2, pos.z );
	vertices [14].tex = vec2 ( size.y, 0 );
	vertices [14].n = vec3 ( ns, 0, 0 );
	vertices [14].t = vec3 ( 0, 0, 1 );
	vertices [14].b = vec3 ( 0, 1, 0 );

	vertices [15].pos = vec3 ( x2, y2, z2 );
	vertices [15].tex = vec2 ( size.y, size.z );
	vertices [15].n = vec3 ( ns, 0, 0 );
	vertices [15].t = vec3 ( 0, 0, 1 );
	vertices [15].b = vec3 ( 0, 1, 0 );

								// top face
	vertices [16].pos = vec3 ( pos.x, y2, z2 );
	vertices [16].tex = vec2 ( 0, size.z );
	vertices [16].n = vec3 ( 0, ns, 0 );
	vertices [16].t = vec3 ( 1, 0, 0 );
	vertices [16].b = vec3 ( 0, 0, 1 );

	vertices [17].pos = vec3 ( x2, y2, z2 );
	vertices [17].tex = vec2 ( size.x, size.z );
	vertices [17].n = vec3 ( 0, ns, 0 );
	vertices [17].t = vec3 ( 1, 0, 0 );
	vertices [17].b = vec3 ( 0, 0, 1 );

	vertices [18].pos = vec3 ( x2, y2, pos.z );
	vertices [18].tex = vec2 ( size.x, 0 );
	vertices [18].n = vec3 ( 0, ns, 0 );
	vertices [18].t = vec3 ( 1, 0, 0 );
	vertices [18].b = vec3 ( 0, 0, 1 );

	vertices [19].pos = vec3 ( pos.x, y2, pos.z );
	vertices [19].tex = vec2 ( 0, 0 );
	vertices [19].n = vec3 ( 0, ns, 0 );
	vertices [19].t = vec3 ( 1, 0, 0 );
	vertices [19].b = vec3 ( 0, 0, 1 );

								// bottom face
	vertices [20].pos = vec3 ( x2, pos.y, z2 );
	vertices [20].tex = vec2 ( size.x, size.z );
	vertices [20].n = vec3 ( 0, -ns, 0 );
	vertices [20].t = vec3 ( -1, 0, 0 );
	vertices [20].b = vec3 ( 0, 0, -1 );

	vertices [21].pos = vec3 ( pos.x, pos.y, z2 );
	vertices [21].tex = vec2 ( 0, size.z );
	vertices [21].n = vec3 ( 0, -ns, 0 );
	vertices [21].t = vec3 ( -1, 0, 0 );
	vertices [21].b = vec3 ( 0, 0, -1 );

	vertices [22].pos = vec3 ( pos.x, pos.y, pos.z );
	vertices [22].tex = vec2 ( 0, 0 );
	vertices [22].n = vec3 ( 0, -ns, 0 );
	vertices [22].t = vec3 ( -1, 0, 0 );
	vertices [22].b = vec3 ( 0, 0, -1 );

	vertices [23].pos = vec3 ( x2, pos.y, pos.z );
	vertices [23].tex = vec2 ( size.x, 0 );
	vertices [23].n = vec3 ( 0, -ns, 0 );
	vertices [23].t = vec3 ( -1, 0, 0 );
	vertices [23].b = vec3 ( 0, 0, -1 );

	for ( int face = 0; face < 6; face++ )
	{
		indices [c++] = face * 4;
		indices [c++] = face * 4 + 1;
		indices [c++] = face * 4 + 2;
		indices [c++] = face * 4 + 2;
		indices [c++] = face * 4 + 3;
		indices [c++] = face * 4;
	}

	if ( mat != NULL )
	{
		mat3 nm = normalMatrix ( *mat );
		
		for ( int i = 0; i < numVertices; i++ )
		{
			vec4 v = (*mat) * vec4 ( vertices [i].pos, 1 );
			
			vertices [i].pos = vec3 ( v.x, v.y, v.z );
			vertices [i].n   = nm * vertices [i].n;
			vertices [i].t   = nm * vertices [i].t;
			vertices [i].b   = nm * vertices [i].b;
		}
	}
	
	return new BasicMesh ( vertices, indices, numVertices, numTris );
}

BasicMesh * createTorus ( float r1, float r2, int rings, int sides )
{
	float 	ringDelta = 2.0 * M_PI / rings;
	float 	sideDelta = 2.0 * M_PI / sides;
	float	invRings  = 1.0 / (float) rings;
	float	invSides  = 1.0 / (float) sides;
	int 	i, j;
	int		index = 0;

	int	numVertices = (sides+1)*(rings+1);
	int	numTris     = sides * rings * 2;
	
	BasicVertex * vertices = new BasicVertex [(sides+1)*(rings+1)];
	int         * faces    = new int [sides*rings*6];

	for ( i = 0; i <= rings; i++ )
	{
    	float theta    = i * ringDelta;
	    float cosTheta = cos ( theta );
    	float sinTheta = sin ( theta );

		for ( j = 0; j <= sides; j++ )
		{
			float phi    = j * sideDelta;
			float cosPhi = cos ( phi );
			float sinPhi = sin ( phi );
			float dist   = r2 + r1 * cosPhi;

      	// (x,y,z) below is the parametric equation for a torus
        // when theta and phi both vary from 0 to pi.

      				// x = cos(theta) * (R + r * cos(phi))
      				// y = -sin(theta) * (R + r * cos(phi))
      				// z = r * sin(phi)

      		vertices [index].pos = vec3 ( cosTheta * dist, -sinTheta * dist, r1 * sinPhi );
			vertices [index].tex = vec2 ( j * invSides, i * invRings );
			vertices [index].n   = vec3 ( cosTheta * cosPhi, -sinTheta * cosPhi, sinPhi );
			
      // normalize the partial derivative of (x,y,z) with respect to theta.
      // T = normalize([dx/dtheta,dy/dtheta,dz/dtheta])

      		float	dxdtheta = -sinTheta * dist;
			float	dydtheta = -cosTheta * dist;
			float	d        = 1 / sqrt ( dxdtheta*dxdtheta + dydtheta*dydtheta );

      		vertices [index].t = vec3 ( dxdtheta * d, dydtheta * d, 0 );
			vertices [index].b = cross ( vertices [index].n, vertices [index].t );
      		index++;
		}
	}
											// Create faces
	index = 0;

	for ( i = 0; i < rings; i++ )
		for ( j = 0; j < sides; j++ )
		{
			int	i1 = i + 1;
			int	j1 = j + 1;

			faces [index++] = i*(sides+1) + j;
			faces [index++] = i1*(sides+1) + j;
			faces [index++] = i1*(sides+1) + j1;

			faces [index++] = i*(sides+1) + j;
			faces [index++] = i1*(sides+1) + j1;
			faces [index++] = i*(sides+1) + j1;
		}
	
	BasicMesh * mesh = new BasicMesh ( vertices, faces, numVertices, numTris );
	
	delete vertices;
	delete faces;
	
	return mesh;
}

static inline	vec3	knot1D ( float t )
{
	float	r   = 1.8 + 0.8 * cos ( 3*t );
	float	phi = 0.2 * M_PI * sin ( 3*t );

	return r * vec3 ( cos ( phi ) * sin ( 2*t ), cos ( phi ) * cos ( 2*t ), sin ( phi ) );
}

static inline	vec3	knot ( float u, float v, vec3& n, vec3& t, vec3& b )
{
	t = normalize ( knot1D (u + 0.01) - knot1D (u - 0.01) );
	b = normalize ( cross ( t, vec3 ( 0, 0, 1 ) ) );
	n = cross ( t, b );

	n = sin (v) * b + cos (v) * n;
	b = cross ( n, t );
	
	return knot1D ( u ) + 0.6 * n;
}

BasicMesh * createKnot  ( float r1, float r2, int rings, int sides )
{
	float 	ringDelta = 2.0 * M_PI / rings;
	float 	sideDelta = 2.0 * M_PI / sides;
	float	invRings  = 1.0 / (float) rings;
	float	invSides  = 1.0 / (float) sides;
	int		index     = 0;
	int	numVertices        = (sides+1)*(rings+1);
	int	numTris            = sides * rings * 2;
	BasicVertex * vertices = new BasicVertex [numVertices];
	int         * faces    = new int [3*sides*rings*2];
	int 	i, j;

	for ( i = 0; i <= rings; i++ )
	{
		float	phi = i * ringDelta;

		for ( j = 0; j <= sides; j++ )
		{
			float psi = j * sideDelta;

			vertices [index].pos = knot ( phi, psi, vertices [index].n, vertices [index].t, vertices [index].b );
			vertices [index].tex = vec2 ( j * invSides, i * invRings );
      		index++;
		}
	}
											// Create faces
	index = 0;

	for ( i = 0; i < rings; i++ )
		for ( j = 0; j < sides; j++ )
		{
			int	i1 = i + 1;
			int	j1 = j + 1;

			faces [index++] = i  * (sides+1) + j;
			faces [index++] = i1 * (sides+1) + j;
			faces [index++] = i1 * (sides+1) + j1;
			faces [index++] = i  * (sides+1) + j;
			faces [index++] = i1 * (sides+1) + j1;
			faces [index++] = i  * (sides+1) + j1;
		}
		
	BasicMesh * mesh = new BasicMesh ( vertices, faces, numVertices, numTris );
	
	delete vertices;
	delete faces;
	
	return mesh;
}

BasicMesh * createParamatric ( float sMin, float sMax, float tMin, float tMax, int n1, int n2, vec3 (*f)( const vec2& u ) )
{
	float	ds             = (sMax - sMin) / n1;
	float	dt             = (tMax - tMin) / n2;
	int	numVertices        = (n1+1)*(n2+1);
	int	numTris            = n1 * n2 * 2;
	BasicVertex * vertices = new BasicVertex [numVertices];
	int         * faces    = new int [3*2*n1*n2];
	int			  index    = 0;
	
	for ( int i = 0; i <= n1; i++ )
		for ( int j = 0; j <= n2; j++ )
		{
			vec2	uv ( sMin + i*ds, tMin + j*dt );
			vec3	p  = f ( uv );
			
			vertices [index].pos = p;
			vertices [index].tex = uv;
						// XXX
			index++;
		}
	
						// now create indices
	index = 0;
	
	for ( int i = 0; i < n1; i++ )
		for ( int j = 0; j < n2; j++ )
		{
			int	i1 = i + 1;
			int	j1 = j + 1;

			faces [index++] = i  * (n1+1) + j;
			faces [index++] = i1 * (n1+1) + j;
			faces [index++] = i1 * (n1+1) + j1;
			faces [index++] = i  * (n1+1) + j;
			faces [index++] = i1 * (n1+1) + j1;
			faces [index++] = i  * (n1+1) + j1;
		}
		
	return NULL;
}
