//
// Basic class for 3d vectors.
// Supports all basic vector operations
//
// Author: Alexey Boreskov
//

#pragma once
#ifndef	__VEC3__
#define	__VEC3__

#include	"common.h"
#include	"vec2.h"

class	vec3
{
public:
	float	x, y, z;

	vec3 () {}
	vec3 ( float v ) : x ( v ), y ( v ), z ( v ) {}
	vec3 ( float px, float py, float pz ) : x ( px ), y ( py ), z ( pz ) {}
	vec3 ( const vec3& v ) : x ( v.x ), y ( v.y ), z ( v.z ) {}
	explicit vec3 ( const float * v ) : x ( v [0] ), y ( v [1] ), z ( v [2] ) {}

	bool	operator == ( const vec3& v ) const
	{
		return x == v.x && y == v.y && z == v.z;
	}
	
	bool	operator != ( const vec3& v ) const
	{
		return x != v.x || y != v.y || z != v.z;
	}
	
	bool	equalWithTolerance ( const vec3& v, float eps = EPS )
	{
		return fabs ( x - v.x ) < eps && fabs ( y - v.y ) < eps && fabs ( z - v.z ) < eps;
	}
	
	vec2	xy () const
	{
		return vec2 ( x, y );
	}
	
	vec3& operator = ( const vec3& v )
	{
		x = v.x;
		y = v.y;
		z = v.z;

		return *this;
	}
	
	vec3 operator - () const
	{
		return vec3 ( -x, -y, -z );
	}

	vec3& operator += ( const vec3& v )
	{
		x += v.x;
		y += v.y;
		z += v.z;

		return *this;
	}

	vec3& operator -= ( const vec3& v )
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;

		return *this;
	}

	vec3& operator *= ( const vec3& v )
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;

		return *this;
	}

	vec3& operator *= ( float f )
	{
		x *= f;
		y *= f;
		z *= f;

		return *this;
	}

	vec3& operator /= ( const vec3& v )
	{
		x /= v.x;
		y /= v.y;
		z /= v.z;

		return *this;
	}

	vec3& operator /= ( float f )
	{
		x /= f;
		y /= f;
		z /= f;

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
		return (float) sqrt ( x * x + y * y + z * z );
	}

    float   lengthSq () const
    {
        return x * x + y * y + z * z;
    }

	vec3&	normalize ()
	{
		return (*this) /= length ();
	}

    float   maxLength () const
    {
        return max3 ( (float)fabs (x), (float)fabs (y), (float)fabs (z) );
    }

    float   distanceToSq ( const vec3& p ) const
    {
        return sqr ( x - p.x ) + sqr ( y - p.y ) + sqr ( z - p.z );
    }

    float   distanceTo ( const vec3& p ) const
    {
        return (float)sqrt ( sqr ( x - p.x ) + sqr ( y - p.y ) + sqr ( z - p.z ) );
    }

	float	distanceToAlongAxis ( const vec3& p, int axis ) const
	{
		return (float)fabs ( operator [] ( axis ) - p [axis] );
	}

	int		getMainAxis () const;
	vec3&	clamp       ( float lower, float upper );

	static	vec3	getRandomVector ( float len = 1 );

	static	inline	vec3 vmin ( const vec3& v1, const vec3& v2 )
	{
		return vec3 ( v1.x < v2.x ? v1.x : v2.x, v1.y < v2.y ? v1.y : v2.y, v1.z < v2.z ? v1.z : v2.z );
	}

	static	inline	vec3 vmax ( const vec3& v1, const vec3& v2 )
	{
		return vec3 ( v1.x > v2.x ? v1.x : v2.x, v1.y > v2.y ? v1.y : v2.y, v1.z > v2.z ? v1.z : v2.z );
	}

	static inline vec3 sum ( const vec3 * v, int n )
	{
		vec3	s ( 0.0f );
		
		for ( int i = 0; i < n; i++ )
			s += v [i];
			
		return s;
	}
	
	static inline vec3 ave ( const vec3 * v, int n )
	{
		vec3	s ( 0.0f );
		
		for ( int i = 0; i < n; i++ )
			s += v [i];
			
		return vec3 ( s.x / n, s.y / n, s.z / n );
	}
	
    static	vec3	zero;
    static	vec3	one;

private:
    float   max3 ( float a, float b, float c ) const
    {
        return a > b ? (a > c ? a : (b > c ? b : c)) :
                       (b > c ? b : (a > c ? a : c));
    }

    float   min3 ( float a, float b, float c ) const
    {
        return a < b ? (a < c ? a : (b < c ? b : c)) :
                       (b < c ? b : (a < c ? a : c));
    }

    float   sqr ( float x ) const
    {
        return x*x;
    }
};

inline vec3 operator + ( const vec3& u, const vec3& v )
{
	return vec3 ( u.x + v.x, u.y + v.y, u.z + v.z );
}

inline vec3 operator - ( const vec3& u, const vec3& v )
{
	return vec3 ( u.x - v.x, u.y - v.y, u.z - v.z );
}

inline vec3 operator * ( const vec3& u, const vec3& v )
{
	return vec3 ( u.x * v.x, u.y * v.y, u.z * v.z );
}

inline vec3 operator * ( const vec3& v, float a )
{
	return vec3 ( v.x * a, v.y * a, v.z * a );
}

inline vec3 operator * ( float a, const vec3& v )
{
	return vec3 ( v.x * a, v.y * a, v.z * a );
}

inline vec3 operator / ( const vec3& u, const vec3& v )
{
	return vec3 ( u.x/v.x, u.y/v.y, u.z/v.z );
}

inline vec3 operator / ( const vec3& v, float a )
{
	return vec3 ( v.x/a, v.y/a, v.z/a );
}

inline vec3 operator / ( float a, const vec3& v )
{
	return vec3 ( a / v.x, a / v.y, a / v.z );
}

inline float dot ( const vec3& a, const vec3& b )
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline vec3 cross ( const vec3& u, const vec3& v )
{
	return vec3 (u.y*v.z - u.z*v.y, u.z*v.x - u.x*v.z, u.x*v.y - u.y*v.x);
}

inline	vec3 lerp ( const vec3& a, const vec3& b, float t )
{
	return a + t * (b - a);
}

inline	float	mixedProduct ( const vec3& a, const vec3& b, const vec3& c )
{
	return dot ( a, cross ( b, c ) );
}

inline	bool	areCollinear ( const vec3& a, const vec3& b, const vec3& c )
{
	return cross ( (b - a), (c - a) ).lengthSq () < EPS * EPS;
}

inline	bool	areComplanar ( const vec3& a, const vec3& b, const vec3& c, const vec3& d )
{
	return fabs ( mixedProduct ( b - a, c - a, d - a ) ) < EPS * EPS * EPS;
}

inline vec3 reflect ( const vec3& v, const vec3& n )	// for unit vectors only
{
	return v - (2.0f * dot ( v, n )) * n;
}

inline vec3 normalize ( const vec3& a )
{
	return a / a.length ();
}

#endif
