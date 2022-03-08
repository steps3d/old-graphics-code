//
// Frustum class for OpenGL
//
// Author: Alexey V. Boreskov <steps3d@narod.ru>, <steps3d@gmail.com>
//

#include	"Frustum.h"
#include	"bbox.h"
#include	"mat4.h"

Frustum :: Frustum ()
{
	get ();
}

void	Frustum :: set ( const mat4& matrix )
{
	mat4	m ( matrix );
	float * clip = m.transpose ().data ();									
	vec3	n [6];							// now extract clip planes params:
	float	d [6];
											// right plane
	n [0].x = clip [3]  - clip [0];
	n [0].y = clip [7]  - clip [4];
	n [0].z = clip [11] - clip [8];
	d [0]   = clip [15] - clip [12];
											// left plane
	n [1].x = clip [3]  + clip [0];
	n [1].y = clip [7]  + clip [4];
	n [1].z = clip [11] + clip [8];
	d [1]   = clip [15] + clip [12];
											// top plane
	n [2].x = clip [3]  - clip [1];
	n [2].y = clip [7]  - clip [5];
	n [2].z = clip [11] - clip [9];
	d [2]   = clip [15] - clip [13];
											// bottom plane
	n [3].x = clip [3]  + clip [1];
	n [3].y = clip [7]  + clip [5];
	n [3].z = clip [11] + clip [9];
	d [3]   = clip [15] + clip [13];
											// far plane
	n [4].x = clip [3]  - clip [2];
	n [4].y = clip [7]  - clip [6];
	n [4].z = clip [11] - clip [10];
	d [4]   = clip [15] - clip [14];
											// near plane
	n [5].x = clip [3]  + clip [2];
	n [5].y = clip [7]  + clip [6];
	n [5].z = clip [11] + clip [10];
	d [5]   = clip [15] + clip [14];
											// normalize
	for ( int i = 0; i < 6; i++ )
	{
		float	len = n [i].length ();

		if ( len > EPS )
		{
			n [i] /= len;
			d [i] /= len;
		}

		planes [i] = plane ( n [i], d [i] );
	}	
}

bool	Frustum :: pointInFrustum ( const vec3& v ) const
{
	for ( register int i = 0; i < 6; i++ )
		if ( planes [i].classify ( v ) == IN_BACK )
			return false;

	return true;
}

bool	Frustum :: boxInFrustum ( const bbox& box ) const
{
	for ( register int i = 0; i < 6; i++ )
		if ( box.classify ( planes [i] ) == IN_BACK )
			return false;
			
	return true;
}
