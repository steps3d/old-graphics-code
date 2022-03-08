//
// Wrapper for kDop bounding volume
//
// Author: Alexey V. Boreskov, <steps3d@narod.ru>, <steps3s@gmail.com>
//

#pragma once
#include "vec3.h"

class	plane;

class kdopBase
{
public:
	enum 
	{
		maxCount = 20
	};

private:
	vec3	n [maxCount];
	int	count;

public:
	kdopBase ( const vec3 *, int );

	int getCount () const
	{
		return count;
	}

	const vec3& getNormal ( int i ) const
	{
		return n [i];
	}
};

class kdop
{
	kdopBase * base;
	float      dMin [kdopBase::maxCount];
	float      dMax [kdopBase::maxCount];

public:
	kdop ( kdopBase * theBase );
	kdop ( const kdop& );

	void addVertices ( const vec3 * v, int n );
	void merge       ( const kdop& );

	bool intersects ( const kdop& ) const;
	int  classify   ( const plane& ) const;
};

