//
// Basic class for 2d vectors.
// Supports all basic vector operations
//
// Author: Alexey V. Boreskov
//

#include	<stdlib.h>
#include	"vec2.h"

vec2	vec2 :: zero ( 0, 0 );
vec2	vec2 :: one  ( 1, 1 );

static	inline	float	rnd ()		// return float random number in [-1,1]
{
	return 2.0f * float ( rand () ) / float ( RAND_MAX ) - 1.0f;
}

vec2	vec2 :: getRandomVector ( float len )
{
	vec2	v;

	for ( ; ; )
	{
		v.x = rnd ();
		v.y = rnd ();

		if ( v.lengthSq () < EPS )
			continue;

		v *= len / v.length ();

		return v;
	}
}

vec2&	vec2 :: clamp ( float lower, float upper )
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

	return *this;
}
