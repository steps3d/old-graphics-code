//
// Classes and functions to load meshes from Wavefront .obj files
//

#pragma once

#include	<vector>
#include	<map>
#include	<string>
#include	"BasicMesh.h"

class Data;

class	Triple
{
public:
	int	iv, it, in;
	
	Triple ( int i1, int i2, int i3 ) : iv ( i1 ), it ( i2 ), in ( i3 ) {}
	
	bool operator < ( const Triple& t ) const
	{
		if ( iv < t.iv )
			return true;
			
		if ( iv > t.iv )
			return false;
			
		if ( it < t.it )
			return true;
			
		if ( it > t.it )
			return false;
			
		return in < t.in;
	}
	
	bool	operator == ( const Triple& t ) const
	{
		return iv == t.iv && it == t.it && in == t.in;
	}
};

class	ObjLoader
{
	vector<BasicVertex>	vertices;	// array of vertices
	vector<int>			indices;	// array of indices

	map<Triple, int>	vi;			// vertex index
	vector<vec3>		v;			// temp array for vertices
	vector<vec3>		n;			// temp array for normals
	vector<vec2>		tx;			// temp array for normals

public:
	ObjLoader () {}
	
	const vector<BasicVertex>&	getVertices () const
	{
		return vertices;
	}
	
	const vector<int>&	getIndices () const
	{
		return indices;
	}
	
	bool	loadObj ( Data * data, vector<BasicMesh *>& mesh );

private:
									// lookup for vertex with indices,
									// if not found then create new
	int 	indexOf   ( const Triple& t );
	
									// parse one 'f' line
	void	parseFace ( const string& str );
	
									// create sub-mesh from data
	BasicMesh * buildMesh ();
};

