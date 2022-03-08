//
// Basic class for 3d vectors.
// Supports all basic vector operations
//
// Author: Alexey Boreskov
//

#include	<stdlib.h>
#include	"vec4.h"

vec4	vec4 :: zero ( 0, 0, 0, 0 );
vec4	vec4 :: one  ( 1, 1, 1, 1 );

static	inline	float	rnd ()		// return float random number in [-1,1]
{
	return 2.0f * float ( rand () ) / float ( RAND_MAX ) - 1.0f;
}

int	vec4 :: getMainAxis () const
{
	int		axis = 0;
	float	val  = (float) fabs ( x );

	for ( register int i = 1; i < 4; i++ )
	{
		float	vNew = (float) fabs ( operator [] ( i ) );

		if ( vNew > val )
		{
			val  = vNew;
			axis = i;
		}
	}

	return axis;
}

vec4	vec4 :: getRandomVector ( float len )
{
	vec4	v;

	for ( ; ; )
	{
		v.x = rnd ();
		v.y = rnd ();
		v.z = rnd ();
		v.w = rnd ();

		if ( v.lengthSq () < EPS )
			continue;

		v *= len / v.length ();

		return v;
	}
}

vec4&	vec4 :: clamp ( float lower, float upper )
{
	if ( x < lower )
		x = lower;
	else
	if ( x > upper )
		x = upper;

	if ( y < lower )
		y = lower;
	else
	if ( y > upper )
		y = upper;
			
	if ( z < lower )
		z = lower;
	else
	if ( z > upper )
		z = upper;

	if ( w < lower )
		w = lower;
	else
	if ( w > upper )
		w = upper;

	return *this;
}
