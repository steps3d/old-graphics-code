//
// Class for simple plane (p,n) + dist = 0
//
// Author: Alexey V. Boreskov
//

#pragma once
#ifndef __PLANE__
#define __PLANE__

#include    "vec3.h"

enum
{
	IN_FRONT = 1,
	IN_BACK  = 2,
	IN_BOTH  = 3,
	IN_PLANE = 4
};

class   plane
{
public:
    vec3 	n;          		// normal vector
    float   dist;       		// signed distance along n
                                // build plane from normal and signed distance
    int      nearPointMask;     // -1 if not initialized
    int      mainAxis;          // index of main axis

    plane () : n ( 0, 0, 1 ), dist ( 0 )
    {
        computeNearPointMaskAndMainAxis ();
    }

    plane ( const vec3& normal, float d ) : n ( normal ), dist ( d ), nearPointMask ( -1 )
    {
	float	len = n.length ();

        n /= len;
	d /= len;

        computeNearPointMaskAndMainAxis ();
    }
                                // build plane from plane equation
    plane ( float nx, float ny, float nz, float d ) : n (nx, ny, nz), dist ( d ), nearPointMask ( -1 )
    {
	float	len = n.length ();

        n /= len;
	d /= len;

        computeNearPointMaskAndMainAxis ();
    }
                                // build plane from normal and point on plane
    plane ( const vec3& normal, const vec3& point ) : n ( normal ), nearPointMask ( -1 )
    {
        n.normalize ();

        dist = - dot( point, n );

        computeNearPointMaskAndMainAxis ();
    }
                                // build plane from 3 points
    plane ( const vec3& p1, const vec3& p2, const vec3& p3 ) : nearPointMask ( -1 )
    {
        n = cross ( p2 - p1, p3 - p1 );

        n.normalize ();

        dist = - dot ( p1, n );

        computeNearPointMaskAndMainAxis ();
    }

    plane ( const plane& plane ) : n ( plane.n ), dist ( plane.dist )
    {
        nearPointMask = plane.nearPointMask;
        mainAxis      = plane.mainAxis;
    }

    float    signedDistanceTo ( const vec3& v ) const
    {
        return dot ( v, n ) + dist;
    }

	float	distanceTo ( const vec3& v ) const
	{
		return (float)fabs ( signedDistanceTo ( v ) );
	}
	                            // get point on plane
    vec3    point () const
    {
        return (-dist) * n;
    }
                            // classify point
    int	classify ( const vec3& p ) const
    {
        float   v = signedDistanceTo ( p );

        if ( v > EPS )
            return IN_FRONT;
        else
        if ( v < -EPS )
            return IN_BACK;

        return IN_PLANE;
    }

    void    flip ()
    {
        n    = -n;
        dist = -dist;

        computeNearPointMaskAndMainAxis ();
    }

    float   closestPoint ( const vec3& p, vec3& res ) const
    {
        float   distanceToPlane = - dist - dot ( p, n );

        res = p + distanceToPlane * n;

        return distanceToPlane;
    }

    bool	intersectByRay ( const vec3& org, const vec3& dir, float& t ) const
    {
        float   numer = - (dist + dot ( org, n ));
        float   denom = dot ( dir, n );

        if ( fabs ( denom ) < EPS )
           return false;

		t = numer / denom;

        return true;
    }

                            // mirror position (point), depends on plane posit.
    void    reflectPos ( vec3& v ) const
    {
        v -= (2.0f * (dot ( v, n ) + dist)) * n;
    }

                            // mirror direction, depends only on plane normal
    void    reflectDir ( vec3& v ) const
    {
        v -= (2.0f * dot ( v, n )) * n;
    }

    vec3    makeNearPoint ( const vec3& minPoint, const vec3& maxPoint ) const
    {
        return vec3 ( nearPointMask & 1 ? maxPoint.x : minPoint.x,
                      nearPointMask & 2 ? maxPoint.y : minPoint.y,
                      nearPointMask & 4 ? maxPoint.z : minPoint.z );
    }

    vec3    makeFarPoint ( const vec3& minPoint, const vec3& maxPoint ) const
    {
        return vec3 ( nearPointMask & 1 ? minPoint.x : maxPoint.x,
                      nearPointMask & 2 ? minPoint.y : maxPoint.y,
                      nearPointMask & 4 ? minPoint.z : maxPoint.z );
    }

    int getMainAxis () const
    {
        return mainAxis;
    }

protected:
    void    computeNearPointMaskAndMainAxis ();
};

#endif
