//
// Axis-aligned bounding box
//
// Author: Alexey V. Boreskov
//

#pragma once
#ifndef __BOUNDING_BOX__
#define __BOUNDING_BOX__

#include    "vec3.h"
#include	"mat3.h"
#include	"plane.h"

class   bbox
{
    vec3    minPoint;
    vec3    maxPoint;

public:
    bbox ()
    {
        reset ();
    }

    bbox ( const vec3& v1, const vec3& v2 );
	bbox ( const bbox& b1, const bbox& b2 ) : minPoint ( b1.minPoint ), maxPoint ( b1.maxPoint )
	{
		merge ( b2 );
	}

	vec3	getMinPoint () const
	{
		return minPoint;
	}

	vec3	getMaxPoint () const
	{
		return maxPoint;
	}

    vec3    getVertex ( int index ) const
    {
        return vec3 ( index & 1 ? maxPoint.x : minPoint.x,
                      index & 2 ? maxPoint.y : minPoint.y,
                      index & 4 ? maxPoint.z : minPoint.z );
    }

	vec3	getCenter () const
	{
		return (minPoint + maxPoint) * 0.5f;
	}

	vec3	getSize () const
	{
		return vec3 ( maxPoint.x - minPoint.x, maxPoint.y - minPoint.y, maxPoint.z - minPoint.z );
	}

    bbox&   addVertex   ( const vec3& v );
	bbox&	addVertices ( const vec3 * v, int numVertices );
    int 	classify    ( const plane& plane ) const;
	bool	isEmpty () const
	{
		return minPoint.x > maxPoint.x || minPoint.y > maxPoint.y || minPoint.z > maxPoint.z;
	}

    bool	contains ( const vec3& pos ) const
    {
        return pos.x >= minPoint.x && pos.x <= maxPoint.x &&
               pos.y >= minPoint.y && pos.y <= maxPoint.y &&
               pos.z >= minPoint.z && pos.z <= maxPoint.z;
    }

	bool	intersects ( const bbox& box ) const
	{
		if (( maxPoint.x < box.minPoint.x) || (minPoint.x > box.maxPoint.x) )
			return false;

		if (( maxPoint.y < box.minPoint.y) || (minPoint.y > box.maxPoint.y) )
			return false;

		if (( maxPoint.z < box.minPoint.z) || (minPoint.z > box.maxPoint.z) )
			return false;

		return true;
	}

    void    reset ()
    {
        minPoint.x = MAX_COORD;
        minPoint.y = MAX_COORD;
        minPoint.z = MAX_COORD;

        maxPoint.x = -MAX_COORD;
        maxPoint.y = -MAX_COORD;
        maxPoint.z = -MAX_COORD;
    }

    void merge ( const bbox& box )
    {
        if ( box.minPoint.x < minPoint.x )
           minPoint.x = box.minPoint.x;

        if ( box.minPoint.y < minPoint.y )
           minPoint.y = box.minPoint.y;

        if ( box.minPoint.z < minPoint.z )
           minPoint.z = box.minPoint.z;

        if ( box.maxPoint.x > maxPoint.x )
           maxPoint.x = box.maxPoint.x;

        if ( box.maxPoint.y > maxPoint.y )
           maxPoint.y = box.maxPoint.y;

        if ( box.maxPoint.z > maxPoint.z )
           maxPoint.z = box.maxPoint.z;
    }

	void	grow ( const vec3& delta )
	{
		minPoint -= delta;
		maxPoint += delta;
	}

	void	grow ( float delta )
	{
		minPoint.x -= delta;
		minPoint.y -= delta;
		minPoint.z -= delta;
		maxPoint.x -= delta;
		maxPoint.y -= delta;
		maxPoint.z -= delta;
	}
	
	float volume () const
	{
		vec3	size = maxPoint - minPoint;

		return size.x * size.y * size.z;
	}
/*
									// distance from point along given direction to this box
	float	getDistanceTo ( const vec3& from, const vec3& dir ) const
	{
		return (getVertex ( computeNearPointMask ( dir ) ) - from) & dir;
	}

									// estimate distance from point given near point mask and main axis of direction
	float	getDistanceToAlongAxis ( const vec3& from, int index, int axis ) const
	{
		return (float)fabs ( (getVertex ( index ) - from) [axis] );
	}

	float	distanceTo ( const plane& plane ) const
	{
		float	dist = plane.signedDistanceTo ( plane.makeNearPoint ( minPoint, maxPoint ) );

		if ( dist > 0 )						// box lies in positive halfspace, return positive distance
			return dist;

        dist = plane.signedDistanceTo ( plane.makeFarPoint ( minPoint, maxPoint ) );

        if ( dist < 0 )						// box lies in negative halfspace
            return dist;

        return 0;							// plane crosses this box
	}
*/
};

#endif
