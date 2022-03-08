//
// Simple random generators
//


#include	<math.h>
#include	<stdlib.h>
#include	"randUtils.h"

float	randUniform ()
{
	return rand () / (float) RAND_MAX;
}

float	randUniform ( float a, float b )
{
	return a + randUniform () * (b - a);
}

float	randGauss ( float m, float sigma )
{
  // from http://www.taygeta.com/random/gaussian.html
  // Algorithm by Dr. Everett (Skip) Carter, Jr.

	float x1, x2, w;
 
	do 
	{
		x1 = randUniform ( -1, 1 );
		x2 = randUniform ( -1, 1 );
		w = x1 * x1 + x2 * x2;
         } while ( w >= 1.0 );

         w = sqrt( (-2.0 * log ( w ) ) / w );

         return x1 * w * sigma + m;

}

float	randPoisson ( float lambda )
{
	float l = exp (-lambda );
	float p = 1.0;
	int k = 0;

	do 
	{
		k++;
		p *= randUniform ();
  	} while ( p > l );

	return k - 1;
}

vec3	randOnSphere ()
{
	float	t = randUniform ( 0, 2*M_PI );		// случайное число, равномерно распределенное в [0,2*pi]
	float	z = randUniform ( -1, 1 );  		// случайное число, равномерно распределенное в [0,1]
	float	r = sqrt ( 1 - z*z );

	return vec3 ( r * cos ( t ), r * sin ( t ), z );

}
vec3	randInTraingle ( const vec3& a, const vec3& b, const vec3& c )
{
	float	u = randUniform ( 0, 1 );
	float	v = randUniform ( 0, 1 );
	float	w;

	if ( u + v > 1 )
	{
		u = 1 - u;
    		v = 1 - v;
	}

	w = 1 - u - v;

	return u*a + v*b + w*c;
}
