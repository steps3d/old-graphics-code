//
// Loader for Wavefront .obj file
//

#include <stdlib.h>
#include "Data.h"
#include "vec2.h"
#include "vec3.h"
#include "vec4.h"
#include "stringUtils.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "BasicMesh.h"
#include "obj-loader.h"

using namespace std;

BasicMesh * ObjLoader :: buildMesh ()
{
	if ( vertices.size () < 1 || indices.size () < 3 )
		return NULL;
		
	BasicMesh * mesh = new BasicMesh ( &vertices [0], &indices [0], vertices.size (), indices.size () / 3 );
	
//	mesh -> calcTBN ();
	
	vertices.clear ();
	indices.clear  ();
	vi.clear       ();
	
	return mesh;
}

int	ObjLoader :: indexOf ( const Triple& t )
{
	map<Triple,int>::iterator it = vi.find ( t );
	
	if ( it != vi.end () )			// such a vertex already exists
		return it -> second;
		
	BasicVertex	vv;
	
	vv.pos = v [t.iv];
	vv.tex = t.it > 0 ? tx [t.it-1] : vec2::zero;
	vv.n   = t.in > 0 ? n  [t.in-1] : vec3::zero;
	
	vertices.push_back ( vv );		// add vertex
	
									// add and return it's index
	return vi [t] = vertices.size () - 1;
}

void ObjLoader :: parseFace ( const string& str )
{
	Tokenizer	tk ( str );
	int			index [1024];	// hope it will be enough for a single face :)
	int			k = 0;
	
	while ( !tk.atEnd () )			// face can be more than 3 vertices
	{
		string	s = tk.next ();		// can be int, int/int, int//int or int/int/int
									// vertex, vertex/tex, vertex//normal, vertex/tex/normal
									// indices start with 1
		index [k  ] = 0;
		index [k+1] = -1;
		index [k+2] = -1;
		
		for ( size_t i = 0; i < s.length (); i++ )
			if ( isdigit ( s [i] ) )
			{
				if ( index [k] == -1 )
					index [k] = 0;
					
				index [k] = index [k] * 10 + s [i] - '0';
			}
			else
			if ( s [i] == '/' )
				k++;
		
		k = (k/3)*3 + 3;			// these indices are done
	}
									// adjust indices since they start at 1
	for ( int i = 0; i < k; i++ )
		if ( index [i] != -1 )
			index [i]--;
									// now build data as a triangles
	int		numVertices = k / 3;
	Triple	v0 ( index [0], index [1], index [2] );
	
	for ( int i = 1; i < numVertices - 1; i++ )
	{
		Triple	v1 ( index [3*i  ], index [3*i+1], index [3*i+2] );
		Triple	v2 ( index [3*i+3], index [3*i+4], index [3*i+5] );
		
		indices.push_back ( indexOf ( v0 ) );
		indices.push_back ( indexOf ( v1 ) );
		indices.push_back ( indexOf ( v2 ) );
	}
}

vec2	readVec2 ( const string& str )
{
	vec2	v ( 0, 0 );
	size_t	p = str.find ( ' ' );
	
	if ( p == string::npos )		// some error
		return v;
		
	v.x = atof ( str.substr ( 0, p ).c_str () );
	
	while ( p < str.length () && isspace ( str [p] ) )
		p++;
		
	v.y = atof ( str.substr ( p ).c_str () );
	
	return v;	
}

vec3	readVec3 ( const string& str )
{
	vec3	v ( 0, 0, 0 );
	size_t	p = str.find ( ' ' );
	
	if ( p == string::npos )		// some error
		return v;
		
	v.x = atof ( str.substr ( 0, p ).c_str () );
	
	while ( p < str.length () && isspace ( str [p] ) )
		p++;
		
	v.y = atof ( str.substr ( p ).c_str () );
	
	while ( p < str.length () && !isspace ( str [p] ) )
		p++;
		
	while ( p < str.length () && isspace ( str [p] ) )
		p++;
		
	v.z = atof ( str.substr ( p ).c_str () );
	
	return v;	
}

bool	ObjLoader :: loadObj ( Data * data, vector<BasicMesh *>& mesh )
{
	string			str, cmd, args;
	vec3			v3;
	vec2			v2;
	string			name;
	
	while ( data -> getString ( str, '\n' ) )
	{
		if ( str.empty () || str [0] == '#' )			// skip empty lines and comments
			continue;

		stringParse ( str, cmd, args );

		if ( cmd == "v" )			// v v.x v.y v.z
		{
			v3 = readVec3 ( args );
			v.push_back ( v3 );
		}
		else
		if ( cmd == "vn" )			// vn n.x n.y n.z
		{
			v3 = readVec3 ( args );
			n.push_back ( v3 );
		}
		else
		if ( cmd == "vt" )			// vt t.x t.y
		{
			v2 = readVec2 ( args );
			tx.push_back ( v2 );
		}
		else
		if ( cmd == "f" )			// can be f n1 n2 ...
		{								// or     f n1/n2 n3/n4
										// or     f n1//n2 n3//n4 ..
										// or     f n1/n2/n3 n4/n5/n6
										// there may be more than three vertices in a face
			parseFace ( args );
		
		}
		else
		if ( cmd == "g" )			// start of new sub-mesh, finish previous
		{
			BasicMesh * m = buildMesh ();
			
			if ( m != NULL )
			{
				m -> setName ( name );
				mesh.push_back ( m );
			}

			name = args;
		}
		else
		if ( cmd == "s" )
		{
		
		}
	}

						// finish last part
	BasicMesh * m = buildMesh ();
	
	if ( m != NULL )
	{
		m -> setName ( name );
		mesh.push_back ( m );
	}

	return true;
}

/*
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
*/
