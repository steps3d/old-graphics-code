//
// Simple Perlin noise class
//
// Author: Alexey V. Boreskov
//

#include	<stdlib.h>
#include	"noise.h"

vec3	Noise :: large ( -123.67f, 456.789f, 791.2345f );

static      float rotMatrix [3][3] =
{
    { 0.98860f, -0.07651f, -0.12967f },
    { 0.07958f,  0.99665f,  0.01871f },
    { 0.12780f, -0.02881f,  0.99138f }
};


inline float rnd ()
{
    return (float) rand () / (float) RAND_MAX;
}

Noise :: Noise  ( int seed )
{
	p = new int  [n];
	g = new vec3 [n];

	srand ( seed );

	for ( int i = 0; i < n; i++ )
	{
		vec3 v;

		do
		{
			v.x = 2 *(rnd () - 0.5f);
			v.y = 2 *(rnd () - 0.5f);
			v.z = 2 *(rnd () - 0.5f);
		} while ( v.lengthSq () > 1 );

        v.normalize ();

		g [i] = v;
	}

	for ( int i = 0; i < n; i++ )
		p [i] = i;

	for ( int i = 0; i < n; i++ )
	{
		int	j   = rand () % n;
		int	tmp = p [i];

		p [i] = p [j];
		p [j] = tmp;
	}
}

Noise :: ~Noise ()
{
	delete p;
	delete g;
}

float	Noise :: turbulence ( const vec3& pt, int octaves ) const
{
	float	val  = 0;
	float	sum  = 0;
	float	freq = 1;

	for ( int i = 0; i < octaves; i++ )
	{
		val  += fabs ( noise ( pt * freq ) ) / freq;
        sum  += 1.0 / (freq*freq);
		freq *= 2.01127f;
	}

	return val / (float)sqrt ( sum );

}

float	Noise :: turbulence ( const vec3& pt, float minFreq, float maxFreq ) const
{
	float	val = 0;
    float   sum = 0;

	for ( float freq = minFreq; freq <= maxFreq; freq *= 2.0235f )
    {
		val += fabs ( noise ( pt * freq ) ) / freq;
        sum += 1.0 / (freq*freq);
    }

	return val / (float)sqrt ( sum );
}

float	Noise :: signedTurbulence ( const vec3& pt, float minFreq, float maxFreq ) const
{
	float	val = 0;
    float   sum = 0;

	for ( float freq = minFreq; freq <= maxFreq; freq *= 1.97452f )
    {
		val += noise ( pt * freq ) / freq;
        sum += 1.0 / (freq*freq);
    }

	return val / (float)sqrt ( sum );
}

float	Noise :: fBm ( const vec3& pt, float h, float lacunarity, float octaves ) const
{
	float value = 0;
	vec3  p ( pt );
	int   i = 0;

	for ( ; i < octaves; i++ )
	{
		value += noise ( p ) * pow ( lacunarity, -i * h );
		p     *= lacunarity;
	}

	float	remainder = octaves - (int) octaves;

	if ( remainder )
		value += remainder * noise ( p ) * pow ( lacunarity, -i * h );

	return value;
}
