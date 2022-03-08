//
// Basic class for 4d matrices.
//
// Author: Alexey Boreskov
//

#pragma once
#ifndef	__MAT4__
#define	__MAT4__

#include	"common.h"
#include	"vec3.h"
#include	"vec4.h"
#include	"mat3.h"

class	mat2;

class	mat4
{
	float m [16];
public:

	mat4 () {}
	mat4 ( const mat4& a );
	mat4 ( float d );		// create diagonal matrix d * I
	mat4 ( const vec3& a );
	mat4 ( const vec4& a );
	mat4 ( const mat3& a );
	explicit mat4 ( const float * values );
	mat4 ( float a0,  float a1,  float a2,  float a3,		// first row
	       float a4,  float a5,  float a6,  float a7,		// second row
		   float a8,  float a9,  float a10, float a11,		// third row
		   float a12, float a13, float a14, float a15 );	// fourth row

    mat4& initFromGl  ( const float * a );					// init from matrix from OpenGL column-major matrix
    void  toGl        ( float * a );						// convert to OpenGL column-major format

	const float * data () const
	{
		return m;
	}
	
	float * operator [] ( int i )				// returns pointer to i-th row
	{ 
		return m + i*4; 
	}
	
	const float * operator [] ( int i ) const 	// returns pointer to i-th row
	{ 
		return m + i*4; 
	}

	vec4	rowAt ( int index ) const
	{
		return vec4 ( m [index * 4], m [index * 4 + 1], m [index * 4 + 2], m [index*4 + 3] );
	}
	
	vec4	colAt ( int index ) const
	{
		return vec4 ( m [index], m [index + 4], m [index + 8], m [index + 12] );
	}
	
	mat4& operator =  ( const mat4& a );
	mat4& operator += ( const mat4& a );
	mat4& operator -= ( const mat4& a );
	mat4& operator *= ( const mat4& a );
	mat4& operator *= ( float f );
	mat4& operator /= ( float f );

	bool	invert      ();
	mat4& 	transpose   ();
	float	det         () const;
	bool	isSymmetric () const;

	mat4	inverse () const
	{
		mat4 m ( *this );

		m.invert ();

		return m;
	}

	vec3 transformPoint ( const vec3& b ) const
	{
		return vec3 ( m [0]*b.x + m [1]*b.y + m [2]*b.z + m [3],
			          m [4]*b.x + m [5]*b.y + m [6]*b.z + m [7],
		              m [8]*b.x + m [9]*b.y + m [10]*b.z + m [11] );
	}

	vec3 transformDirection ( const vec3& b ) const
	{
		return vec3 ( m [0]*b.x + m [1]*b.y + m [2]*b.z,
			          m [4]*b.x + m [5]*b.y + m [6]*b.z,
		              m [8]*b.x + m [9]*b.y + m [10]*b.z );
	}

	mat3	upper3x3 () const
	{
		return mat3 ( m [0], m [1], m [2], 
		              m [4], m [5], m [6],
					  m [8], m [9], m [10] );
	}
	
	static	mat4	identity ();
	
	static	mat4	scale     ( const vec3& s );
	static	mat4	translate ( const vec3& a );

											// rotations
	static	mat4	rotateX  ( float );		// rotate counter-clockwiose, angle in radians
	static	mat4	rotateY  ( float );		// rotate counter-clockwiose, angle in radians
	static	mat4	rotateZ  ( float );		// rotate counter-clockwiose, angle in radians
	static	mat4	rotate   ( const vec3& v, float angle );
	static	mat4	rotateEuler ( float yaw, float pitch, float roll );

												// reflections
	static	mat4	mirrorX  ();
	static	mat4	mirrorY  ();
	static	mat4	mirrorZ  ();

	friend mat4 operator + ( const mat4&, const mat4& );
	friend mat4 operator - ( const mat4&, const mat4& );
	friend mat4 operator * ( const mat4&, const mat4& );
	friend mat4 operator * ( const mat4&, float );
	friend mat4 operator * ( float, const mat4& );
	friend vec3 operator * ( const mat4&, const vec3& );
	friend vec4 operator * ( const mat4&, const vec4& );
	
	friend class mat2;
	friend class mat3;
};

mat4 ortho        ( float left, float right, float bottom, float top, float zNear, float zFar );
mat4 frustum      ( float left, float right, float bottom, float top, float zNear, float zFar);
mat4 perspective  ( float fov, float aspect, float zNear, float zFar );
mat4 lookAt       ( const vec3& eye, const vec3& center, const vec3& up );
vec4 unproject    ( const vec3& win, const mat4& mv, const mat4& proj, int viewport [4] );

					// build normal matrix from model-view
mat3 normalMatrix ( const mat4& mv );

					// build Euler angles from rotation matrix
vec3 eulerFromMatrix ( const mat4& m );

#endif
