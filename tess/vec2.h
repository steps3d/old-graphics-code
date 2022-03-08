//
// Basic class for 2d vectors.
// Supports all basic vector operations
//
// Author: Alexey Boreskov
//

#ifndef	__VEC2__
#define	__VEC2__

#include	"common.h"

class	vec2
{
public:
	float	x, y;

	vec2 () {}
	vec2 ( float v ) : x ( v ), y ( v ) {}
	vec2 ( float px, float py ) : x ( px  ), y ( py  ) {}
	vec2 ( const vec2& v  ) : x ( v.x ), y ( v.y ) {}
	explicit vec2 ( const float * v ) : x ( v [0] ), y ( v [0] ) {}

	vec2& operator = ( const vec2& v )
	{
		x = v.x;
		y = v.y;

		return *this;
	}

	vec2 operator - () const
	{
		return vec2 ( -x, -y );
	}

	vec2& operator += ( const vec2& v )
	{
		x += v.x;
		y += v.y;

		return *this;
	}

	vec2& operator -= ( const vec2& v )
	{
		x -= v.x;
		y -= v.y;

		return *this;
	}

	vec2& operator *= ( const vec2& v )
	{
		x *= v.x;
		y *= v.y;

		return *this;
	}

	vec2& operator *= ( float f )
	{
		x *= f;
		y *= f;

		return *this;
	}

	vec2& operator /= ( const vec2& v )
	{
		x /= v.x;
		y /= v.y;

		return *this;
	}

	vec2& operator /= ( float f )
	{
		x /= f;
		y /= f;

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

	int	operator == ( const vec2& v ) const
	{
		return x == v.x && y == v.y;
	}

	int	operator != ( const vec2& v ) const
	{
		return x != v.x || y != v.y;
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
		return (float) sqrt ( x * x + y * y );
	}

    float   lengthSq () const
    {
	    return x * x + y * y;
    }

	vec2&	normalize ()
	{
		return (*this) /= length ();
	}

    float   maxLength () const
    {
        return max ( (float) fabs (x), (float) fabs (y) );
    }

    float   distanceToSq ( const vec2& p ) const
    {
        return sqr ( x - p.x ) + sqr ( y - p.y );
    }

    float   distanceTo ( const vec2& p ) const
    {
        return (float) sqrt ( sqr ( x - p.x ) + sqr ( y - p.y ) );
    }

    vec2 ort () const
    {
        return vec2 ( -y, x );
    }

	vec2&	clamp ( float lower, float upper );

	static	vec2	getRandomVector ( float len = 1 );

	static	inline	vec2 vmin ( const vec2& v1, const vec2& v2 )
	{
		return vec2 ( v1.x < v2.x ? v1.x : v2.x, v1.y < v2.y ? v1.y : v2.y );
	}

	static	inline	vec2 vmax ( const vec2& v1, const vec2& v2 )
	{
		return vec2 ( v1.x > v2.x ? v1.x : v2.x, v1.y > v2.y ? v1.y : v2.y );
	}

	friend vec2 operator + ( const vec2&, const vec2& );
	friend vec2 operator - ( const vec2&, const vec2& );
	friend vec2 operator * ( const vec2&, const vec2& );
	friend vec2 operator * ( float,           const vec2& );
	friend vec2 operator * ( const vec2&, float );
	friend vec2 operator / ( const vec2&, float );
	friend vec2 operator / ( const vec2&, const vec2& );

    static	vec2	zero;
    static	vec2	one;
};

inline vec2 operator + ( const vec2& u, const vec2& v )
{
	return vec2 ( u.x + v.x, u.y + v.y );
}

inline vec2 operator - ( const vec2& u, const vec2& v )
{
	return vec2 ( u.x - v.x, u.y - v.y );
}

inline vec2 operator * ( const vec2& u, const vec2& v )
{
	return vec2 ( u.x*v.x, u.y*v.y );
}

inline vec2 operator * ( const vec2& v, float a )
{
	return vec2 ( v.x*a, v.y*a );
}

inline vec2 operator * ( float a, const vec2& v )
{
	return vec2 ( v.x*a, v.y*a );
}

inline vec2 operator / ( const vec2& u, const vec2& v )
{
	return vec2 ( u.x/v.x, u.y/v.y );
}

inline vec2 operator / ( const vec2& v, float a )
{
	return vec2 ( v.x/a, v.y/a );
}

inline vec2 operator / ( float a, const vec2& v )
{
	return vec2 ( a / v.x, a / v.y );
}

inline	vec2 lerp ( const vec2& a, const vec2& b, float t )
{
	return a + t * (b - a);
}

inline float dot ( const vec2& a, const vec2& b )
{
	return a.x * b.x + a.y * b.y;
}

inline vec2 normalize ( const vec2& a )
{
	return a / a.length ();
}

inline	bool	areCollinear ( const vec2& a, const vec2& b )
{
	return dot ( a.ort (), b ) < EPS;
}

#endif
