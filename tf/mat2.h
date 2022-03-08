//
// Basic class for 2d matrices.
//
// Author: Alexey V. Boreskov
//

#pragma once
#ifndef	__MAT2__
#define	__MAT2__

#include	"common.h"
#include	"vec2.h"

class mat3;
class mat4;

class	mat2
{
	float m [4];
	
public:
	mat2() {}
	explicit mat2 ( const float * values );
	mat2 ( float v );
	mat2 ( const vec2& v );
	mat2 ( float m0, float m1, float m2, float m3 );
	mat2 ( const mat2& a);

	const float * data () const
	{
		return m;
	}
	
	float * operator [] ( int i )				// returns pointer to i-th row
	{ 
		return m + i*2; 
	}
	
	const float * operator [] ( int i ) const 	// returns pointer to i-th row
	{ 
		return m + i*2; 
	}
	
	vec2	rowAt ( int index ) const
	{
		return vec2 ( m [index * 2], m [index * 2 + 1] );
	}
	
	vec2	colAt ( int index ) const
	{
		return vec2 ( m [index], m [index + 2] );
	}
	
	mat2& operator =  ( const mat2& a );
	mat2& operator += ( const mat2& a );
	mat2& operator -= ( const mat2& a );
	mat2& operator *= ( const mat2& a );
	mat2& operator *= ( float f );
	mat2& operator /= ( float f );

	bool	invert    ();
	
	mat2 	transpose ()
	{
		return mat2 ( m [0], m [2], m [1], m [3] );
	}

	float	det       () const
	{
		return m [0] * m [3] - m [1] * m [2];
	}

	bool	isSymmetric () const
	{
		return fabs ( m [1] - m [2] ) < EPS;
	}
	
	friend mat2 operator + ( const mat2&, const mat2& );
	friend mat2 operator - ( const mat2&, const mat2& );
	friend mat2 operator * ( const mat2&, const mat2& );
	friend mat2 operator * ( const mat2&, float );
	friend mat2 operator * ( float, const mat2& );
	friend mat2 operator / ( const mat2&, float );
	friend vec2 operator * ( const mat2&, const vec2& );

	friend class mat3;
	friend class mat4;
};

#endif
