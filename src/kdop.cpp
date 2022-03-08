//
// Wrapper for kDop bounding volume
//
// Author: Alexey V. Boreskov, <steps3d@narod.ru>, <steps3s@gmail.com>
//

#include	<assert.h>
#include	"kdop.h"
#include	"plane.h"

kdopBase :: kdopBase ( const vec3 * normals, int theCount )
{
	count = theCount;
	
	for ( int i = 0; i < count; i++ )
		n [i] = normals [i];
}

kdop :: kdop ( kdopBase * theBase )
{
	base = theBase;
	
	for ( int i = 0; i < base -> getCount (); i++ )
	{
		dMin [i] =  MAX_COORD;
		dMax [i] = -MAX_COORD;
	}
}

kdop :: kdop ( const kdop& k )
{
	base = k.base;
	
	for ( int i = 0; i < base -> getCount (); i++ )
	{
		dMin [i] = k.dMin [i];
		dMax [i] = k.dMax [i];
	}
}

void kdop :: addVertices ( const vec3 * v, int n )
{
	for ( int i = 0; i < n; i++ )
		for ( int j = 0; j < base -> getCount (); j++ )
		{
			float	d = dot ( v [i], base -> getNormal ( j ) );
			
			if ( d < dMin [j] )
				dMin [j] = d;
				
			if ( d > dMax [j] )
				dMax [j] = d;
		}
}

void kdop :: merge ( const kdop& k )
{
	assert ( base == k.base );
	
	for ( int i = 0; i < base -> getCount (); i++ )
	{
		if ( k.dMin [i] < dMin [i] )
			dMin [i] = k.dMin [i];
			
		if ( k.dMax [i] > dMax [i] )
			dMax [i] = k.dMax [i];			
	}
}

bool kdop :: intersects ( const kdop& k ) const
{
	assert ( base == k.base );
	
	for ( int i = 0; i < base -> getCount (); i++ )
	{
		if ( k.dMax [i] < dMin [i] )
			return false;
			
		if ( dMax [i] > k.dMin [i] )
			return false;
	}
	
	return true;
}

int  kdop :: classify ( const plane& ) const
{
	return 0; 		// XXX - what to do here
}
