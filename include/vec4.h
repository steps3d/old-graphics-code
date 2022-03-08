//
// Basic class for 3d vectors.
// Supports all basic vector operations
//
// Author: Alexey Boreskov
//

#pragma once
#ifndef	__VEC4__
#define	__VEC4__

#include	"common.h"
#include	"vec3.h"
#include	"vec2.h"

class	vec4
{
public:
	float	x, y, z, w;

	vec4 () {}
	vec4 ( float v ) : x ( v ), y ( v ), z ( v ), w ( v ) {}
	vec4 ( float px, float py, float pz = 0, float pw = 1 ) : x ( px ), y ( py ), z ( pz ), w ( pw ) {}
	vec4 ( const vec4& v ) : x ( v.x ), y ( v.y ), z ( v.z ), w ( v.w ) {}
	vec4 ( const vec3& v, float a ) : x ( v.x ), y (v.y ), z (v.z ), w ( a ) {}
	explicit vec4 ( const float * v ) : x ( v [0] ), y ( v [1] ), z ( v [2] ), w ( v [3] ) {}

	bool	operator == ( const vec4& v ) const
	{
		return x == v.x && y == v.y && z == v.z && w == v.w;
	}
	
	bool	operator != ( const vec4& v ) const
	{
		return x != v.x || y != v.y || z != v.z || w != v.w;
	}
	
	bool	equalWithTolerance ( const vec4& v, float eps = EPS )
	{
		return fabs ( x - v.x ) < eps && fabs ( y - v.y ) < eps && fabs ( z - v.z ) < eps && fabs ( w - v.w ) < eps;
	}
	
	vec3	xyz () const
	{
		return vec3 ( x, y, z );
	}
	
	vec2	xy () const
	{
		return vec2 ( x, y );
	}
	
	vec4& operator = ( const vec4& v )
	{
		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;

		return *this;
	}
	
	vec4 operator - () const
	{
		return vec4 ( -x, -y, -z, -w );
	}

	vec4& operator += ( const vec4& v )
	{
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;

		return *this;
	}

	vec4& operator -= ( const vec4& v )
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		w -= v.w;

		return *this;
	}

	vec4& operator *= ( const vec4& v )
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
		w *= v.w;

		return *this;
	}

	vec4& operator *= ( float f )
	{
		x *= f;
		y *= f;
		z *= f;
		w *= f;

		return *this;
	}

	vec4& operator /= ( const vec4& v )
	{
		x /= v.x;
		y /= v.y;
		z /= v.z;
		w /= v.w;

		return *this;
	}

	vec4& operator /= ( float f )
	{
		x /= f;
		y /= f;
		z /= f;
		w /= f;

		return *this;
	}

	float& operator [] ( int index )
	{
		return * ( index + &x );
	}

	float operator [] ( int index ) const
	{
		return * ( index + &x );
	}

	operator float * ()
	{
		return &x;
	}

	operator const float * () const
	{
		return &x;
	}

	float	length () const
	{
		return (float) sqrt ( x * x + y * y + z * z + w * w);
	}

    float   lengthSq () const
    {
        return x * x + y * y + z * z + w * w;
    }

	vec4&	normalize ()
	{
		return (*this) /= length ();
	}

    float   maxLength () const
    {
        return max ( max ( (float)fabs (x), (float)fabs (y) ), max ( (float)fabs (z), (float) fabs(w) ) );
    }

    float   distanceToSq ( const vec4& p ) const
    {
        return sqr ( x - p.x ) + sqr ( y - p.y ) + sqr ( z - p.z ) + sqr ( w - p.w );
    }

    float   distanceTo ( const vec4& p ) const
    {
        return (float)sqrt ( sqr ( x - p.x ) + sqr ( y - p.y ) + sqr ( z - p.z ) + sqr ( w - p.w ) );
    }

	float	distanceToAlongAxis ( const vec4& p, int axis ) const
	{
		return (float)fabs ( operator [] ( axis ) - p [axis] );
	}

	int	getMainAxis () const;
	vec4&	clamp   ( float lower, float upper );

	static	vec4	getRandomVector ( float len = 1 );

	static	inline	vec4 vmin ( const vec4& v1, const vec4& v2 )
	{
		return vec4 ( v1.x < v2.x ? v1.x : v2.x, v1.y < v2.y ? v1.y : v2.y, 
		              v1.z < v2.z ? v1.z : v2.z, v1.w < v2.w ? v1.w : v2.w );
	}

	static	inline	vec4 vmax ( const vec4& v1, const vec4& v2 )
	{
		return vec4 ( v1.x > v2.x ? v1.x : v2.x, v1.y > v2.y ? v1.y : v2.y, 
					  v1.z > v2.z ? v1.z : v2.z, v1.w > v2.w ? v1.w : v2.w );
	}

    static	vec4	zero;
    static	vec4	one;
};

inline vec4 operator + ( const vec4& u, const vec4& v )
{
	return vec4 ( u.x + v.x, u.y + v.y, u.z + v.z, u.w + v.w );
}

inline vec4 operator - ( const vec4& u, const vec4& v )
{
	return vec4 ( u.x - v.x, u.y - v.y, u.z - v.z, u.w - v.w );
}

inline vec4 operator * ( const vec4& u, const vec4& v )
{
	return vec4 ( u.x*v.x, u.y*v.y, u.z * v.z, u.w * v.w );
}

inline vec4 operator * ( const vec4& v, float a )
{
	return vec4 ( v.x*a, v.y*a, v.z*a, v.w * a );
}

inline vec4 operator * ( float a, const vec4& v )
{
	return vec4 ( v.x*a, v.y*a, v.z*a, v.w * a );
}

inline vec4 operator / ( const vec4& u, const vec4& v )
{
	return vec4 ( u.x/v.x, u.y/v.y, u.z/v.z, u.w / v.w );
}

inline vec4 operator / ( const vec4& v, float a )
{
	return vec4 ( v.x / a, v.y / a, v.z / a, v.w / a );
}

inline vec4 operator / ( float a, const vec4& v )
{
	return vec4 ( a / v.x, a / v.y, a / v.z, a / v.w );
}

inline float dot ( const vec4& a, const vec4& b )
{
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

inline	vec4 lerp ( const vec4& a, const vec4& b, float t )
{
	return a + t * (b - a);
}

inline vec4 normalize ( const vec4& a )
{
	return a / a.length ();
}

#endif
