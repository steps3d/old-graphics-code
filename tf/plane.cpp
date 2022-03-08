//
// Class for simple plane (p,n) + dist = 0
//
// Author: Alex V. Boreskoff
//

#include    "plane.h"

int	computeNearPointMask ( const vec3& n )
{
    if ( n.x > 0.0f )
        if ( n.y > 0.0f )
            if ( n.z > 0.0f )
                return 0;
            else
                return 4;
        else
            if ( n.z > 0.0f )
                return 2;
            else
                return 6;
    else
        if ( n.y > 0.0f )
            if ( n.z > 0.0f )
                return 1;
            else
                return 5;
        else
            if ( n.z > 0.0f )
                return 3;
            else
                return 7;
}

void    plane :: computeNearPointMaskAndMainAxis ()
{
	nearPointMask = computeNearPointMask ( n );
    mainAxis      = n.getMainAxis ();
}
