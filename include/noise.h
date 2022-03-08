//
// Simple Perlin noise class
//
// Author: Alexey V. Boreskov <steps3d@gmail.com>, <steps3d@narod.ru>
//

#pragma once
#ifndef	__NOISE__
#define	__NOISE__

#include	"vec3.h"

class	Noise
{
private:
	enum
	{
		n = 256
	};

	int	      * p;
	vec3  * g;

	int	fold ( int i, int j, int k ) const
	{
		return p [(p [(p[i & (n-1)] + j) & (n-1)] + k) & (n-1)];
	}

	float	drop ( float t ) const
	{
		float	ta = (float) fabs ( t );

		if ( t <= 1 )
			return 1 + ta*ta*(2*ta - 3);

		return 0;
	}

	float	omega ( long i, long j, long k, const vec3& pt ) const
	{
		return dot(g [fold ( i, j, k )], pt) * drop ( pt.x ) * drop ( pt.y ) * drop ( pt.z );
	}

/*
	vec3	dOmega  ( long i, long j, long k, const vec3& pt ) const
	{
    	float		v ( dot(g [fold ( i, j, k)], pt ) );
		vec3	d ( drop ( pt.x ), drop ( pt.y ), drop ( pt.z ) );

		return v * vec3 ( ddrop ( pt.x ) * d.y * d.z,
							  ddrop ( pt.y ) * d.x * d.z,
							  ddrop ( pt.z ) * d.x * d.y );
	}
*/
	static	vec3	large;

public:
	Noise  ( int seed = -1 );
	~Noise ();

	float	noise ( const vec3& pt ) const
	{
		long	ip  = (long) floor ( pt.x );
		long	jp  = (long) floor ( pt.y );
		long	kp  = (long) floor ( pt.z );
		float	sum = 0;

		for ( register long i = ip; i <= ip + 1; i++ )
			for ( register long j = jp; j <= jp + 1; j++ )
				for ( register long k = kp; k <= kp + 1; k++ )
					sum += omega ( i, j, k, vec3 ( pt.x - i, pt.y - j, pt.z - k ) );

		return sum;
	}

	vec3	dNoise ( const vec3& pt ) const;

	vec3	noise3D ( const vec3& pt ) const
	{
		return vec3 ( noise ( pt - large ), noise ( pt ), noise ( pt + large ) );
	}

	float	turbulence       ( const vec3& pt, int octaves ) const;
	float	turbulence       ( const vec3& pt, float minFreq, float maxFreq ) const;
    float   signedTurbulence ( const vec3& pt, float minFreq, float maxFreq ) const;
	float	fBm              ( const vec3& pt, float h, float lacunarity, float octaves ) const;
};

#endif
