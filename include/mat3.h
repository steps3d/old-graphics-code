//
// Basic class for 3d matrices.
//
// Author: Alexey Boreskov
//

#pragma once
#ifndef	__MAT3__
#define	__MAT3__

#include	"common.h"
#include	"vec3.h"

class	mat2;
class	mat4;

class	mat3
{
	float m [9];
	
public:
	mat3 () {}
	explicit mat3 ( const float * values );
	mat3 ( const mat3 &m );
	mat3 ( float v );
	mat3 ( const vec3& v );
	mat3 ( float m0, float m1, float m2,
	       float m3, float m4, float m5,
	       float m6, float m7, float m8 );
		   
	const float * data () const
	{
		return m;
	}
	
	float * operator [] ( int i )				// returns pointer to i-th row
	{ 
		return m + i*3; 
	}
	
	const float * operator [] ( int i ) const 	// returns pointer to i-th row
	{ 
		return m + i*3; 
	}
	
	vec3	rowAt ( int index ) const
	{
		return vec3 ( m [index * 3], m [index * 3 + 1], m [index * 3 + 2] );
	}
	
	vec3	colAt ( int index ) const
	{
		return vec3 ( m [index], m [index + 3], m [index + 6] );
	}
	
	mat3& operator =  ( const mat3& a );
	mat3& operator += ( const mat3& a );
	mat3& operator -= ( const mat3& a );
	mat3& operator *= ( const mat3& a );
	mat3& operator *= ( float f );
	mat3& operator /= ( float f );

	bool	invert       ();
	mat3& 	transpose    ();
	float	det          () const;	
	bool	isSymmetric  () const;
	void	mat3GetEigen ( float l [3], vec3 e [3] ) const;
	
	static	mat3	identity    ();
	static	mat3	scale       ( const vec3& );
	static	mat3	rotateX     ( float );
	static	mat3	rotateY     ( float );
	static	mat3	rotateZ     ( float );
	static	mat3	rotate      ( const vec3&, float );
	static	mat3	rotateEuler ( float yaw, float pitch, float roll );
	static	mat3	mirrorX     ();
	static	mat3	mirrorY     ();
	static	mat3	mirrorZ     ();
	
	friend mat3 operator + ( const mat3&, const mat3& );
	friend mat3 operator - ( const mat3&, const mat3& );
	friend mat3 operator * ( const mat3&, const mat3& );
	friend mat3 operator * ( const mat3&, float );
	friend mat3 operator * ( float, const mat3& );
	friend mat3 operator / ( const mat3&, float );
	friend vec3 operator * ( const mat3&, const vec3& );

	friend class mat2;
	friend class mat4;
};

					// build Euler angles from rotation matrix
vec3 eulerFromMatrix ( const mat3& m );

#endif
