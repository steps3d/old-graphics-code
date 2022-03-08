//
// Quaternion class
//
// Author: Alexey V. Boreskov <steps3d@gmail.com>, <steps3d@narod.ru>
//

#pragma once
#ifndef	__QUATERNION__
#define	__QUATERNION__

#include	"vec3.h"

class	mat3;
class	mat4;

class	quat
{
public:							// make all members public
	float	x, y, z, w;

	quat () {}
	quat ( float theX, float theY = 0, float theZ = 0, float theW = 0 ) : x ( theX ), y ( theY ), z ( theZ ) , w ( theW ) {}
	quat ( const quat& q ) : x ( q.x ), y ( q.y ), z ( q.z ), w ( q.w ) {}
	quat ( const vec3& v ) : x ( v.x ), y ( v.y ), z ( v.z ), w ( 0 ) {}
	
	quat ( float angle, const vec3& axis );
	quat ( const mat3& mat );
	quat ( float mat [3][3]    );

	float	lengthSq () const
	{
		return x * x + y * y + z * z + w * w;
	}
	
	float	length () const
	{
		return sqrtf ( x * x + y * y + z * z + w * w );
	}
	
	quat operator + () const
	{
		return *this;
	}

	quat operator - () const
	{
		return quat ( -x, -y, -z, w );
	}

	quat& conj ()
	{
		x = -x;
		y = -y;
		z = -z;

		return *this;
	}

	bool	operator == ( const quat& q ) const
	{
		return x == q.x && y == q.y && z == q.z && w == q.w;
	}
	
	bool	operator != ( const quat& q ) const
	{
		return x != q.x || y != q.y || z != q.z || w != q.w;
	}
	
	quat& operator += ( const quat& q )
	{
		x += q.x;
		y += q.y;
		z += q.z;
		w += q.w;

		return *this;
	}

	quat& operator -= ( const quat& q )
	{
		x -= q.x;
		y -= q.y;
		z -= q.z;
		w -= q.w;

		return *this;
	}

	quat& operator *= ( const quat& q )
	{
		*this = quat ( y * q.z - z * q.y + w * q.x + x * q.w,
							 z * q.x - x * q.z + w * q.y + y * q.w,
							 x * q.y - y * q.x + w * q.z + z * q.w,
							 w * q.w - x * q.x - y * q.y - z * q.z );
		return *this;
	}

	quat	inverse () const
	{
		float	l2 = lengthSq ();
		
		return quat ( -x / l2, -y / l2, -z /l2, w / l2 );
	}
	
	quat&	invert ()
	{
		float	ls = 1.0f / lengthSq ();
		
		x *= -ls;
		y *= -ls;
		z *= -ls;
		w *=  ls;
		
		return *this;
	}
	
	vec3	rotate         ( const vec3& v ) const;
	quat&	normalize      ();
	quat&	initWithAngles ( float yaw, float pitch, float roll );
	void	getMatrix      ( mat3& m ) const;
	void	getMatrix      ( mat4& m ) const;	
	quat	exp            () const;
	quat	log            () const;
};

inline	quat operator + ( const quat& q1, const quat& q2 )
{
	return quat ( q1.x + q2.x, q1.y + q2.y, q1.z + q2.z, q1.w + q2.w );
}

inline	quat operator - ( const quat& q1, const quat& q2 )
{
	return quat ( q1.x - q2.x, q1.y - q2.y, q1.z - q2.z, q1.w - q2.w );
}

inline	quat operator * ( const quat& q1, float f )
{
	return quat ( q1.x * f, q1.y * f, q1.z * f, q1.w * f );
}

inline	quat operator / ( const quat& q1, float f )
{
	return quat ( q1.x / f, q1.y / f, q1.z / f, q1.w / f );
}

inline	quat operator * ( const quat& q1, const quat& q2 )
{
	return quat ( q1.y * q2.z - q1.z * q2.y + q1.w * q2.x + q1.x * q2.w,
		          q1.z * q2.x - q1.x * q2.z + q1.w * q2.y + q1.y * q2.w,
				  q1.x * q2.y - q1.y * q2.x + q1.w * q2.z + q1.z * q2.w,
				  q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z );
}

inline float dot ( const quat& q1, const quat& q2 )
{
	return q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;
}

inline quat normalize ( const quat& q )
{
	quat	qt ( q );
	
	return qt.normalize ();
}

quat	slerp ( const quat& q1, const quat& q2, float t );
quat	squad ( const quat& q1, const quat& q2, const quat& a, const quat&b, float t );

#endif
