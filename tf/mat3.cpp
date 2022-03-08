//
// Basic class for 3d matrices
// Supports all basic matrix operations
//
// Author: Alexey V. Boreskov
//

#include "mat3.h"
#include "eig3x3/eig3.h"

mat3 :: mat3 ( const float * values )
{
	m [0] = values [0]; 
	m [1] = values [1]; 
	m [2] = values [2];
	m [3] = values [3]; 
	m [4] = values [4]; 
	m [5] = values [5];
	m [6] = values [6]; 
	m [7] = values [7]; 
	m [8] = values [8];
}

mat3 :: mat3 ( float v )
{
	m [0] = v; 
	m [1] = 0; 
	m [2] = 0;
	m [3] = 0; 
	m [4] = v; 
	m [5] = 0;
	m [6] = 0; 
	m [7] = 0; 
	m [8] = v;
}

mat3 :: mat3 ( const vec3& v )
{
	m [0] = v.x; 
	m [1] = 0; 
	m [2] = 0;
	m [3] = 0; 
	m [4] = v.y; 
	m [5] = 0;
	m [6] = 0; 
	m [7] = 0; 
	m [8] = v.z;
}

mat3 :: mat3 ( const mat3& a )
{
	m [0] = a.m [0];
	m [1] = a.m [1];
	m [2] = a.m [2];
	m [3] = a.m [3];
	m [4] = a.m [4];
	m [5] = a.m [5];
	m [6] = a.m [6];
	m [7] = a.m [7];
	m [8] = a.m [8];
}

mat3 :: mat3 ( float m0, float m1, float m2,
	           float m3, float m4, float m5,
	           float m6, float m7, float m8 )
{
	m [0] = m0;
	m [1] = m1;
	m [2] = m2;
	m [3] = m3;
	m [4] = m4;
	m [5] = m5;
	m [6] = m6;
	m [7] = m7;
	m [8] = m8;
}

mat3& mat3 :: operator = ( const mat3& a )
{
	m [0] = a.m [0];
	m [1] = a.m [1];
	m [2] = a.m [2];
	m [3] = a.m [3];
	m [4] = a.m [4];
	m [5] = a.m [5];
	m [6] = a.m [6];
	m [7] = a.m [7];
	m [8] = a.m [8];

	return *this;
}

mat3& mat3 :: operator += ( const mat3 &a )
{
	m [0] += a.m [0]; 
	m [1] += a.m [1]; 
	m [2] += a.m [2];
	m [3] += a.m [3]; 
	m [4] += a.m [4]; 
	m [5] += a.m [5];
	m [6] += a.m [6]; 
	m [7] += a.m [7]; 
	m [8] += a.m [8];
	
	return *this;
}

mat3& mat3 :: operator -= ( const mat3 &a )
{
	m [0] -= a.m [0]; 
	m [1] -= a.m [1]; 
	m [2] -= a.m [2];
	m [3] -= a.m [3]; 
	m [4] -= a.m [4]; 
	m [5] -= a.m [5];
	m [6] -= a.m [6]; 
	m [7] -= a.m [7]; 
	m [8] -= a.m [8];
	
	return *this;
}

mat3& mat3 :: operator *= ( const mat3 &a )
{
	float	t [9];
	
	t [0] = m [0] * a.m [0] + m [1] * a.m [3] + m [2] * a.m [6];
	t [1] = m [0] * a.m [1] + m [1] * a.m [4] + m [2] * a.m [7];
	t [2] = m [0] * a.m [2] + m [1] * a.m [5] + m [2] * a.m [8];
	t [3] = m [3] * a.m [0] + m [4] * a.m [3] + m [5] * a.m [6];
	t [4] = m [3] * a.m [1] + m [4] * a.m [4] + m [5] * a.m [7];
	t [5] = m [3] * a.m [2] + m [4] * a.m [5] + m [5] * a.m [8];
	t [6] = m [6] * a.m [0] + m [7] * a.m [3] + m [8] * a.m [6];
	t [7] = m [6] * a.m [1] + m [7] * a.m [4] + m [8] * a.m [7];
	t [8] = m [6] * a.m [2] + m [7] * a.m [5] + m [8] * a.m [8];
		
	m [0] = t [0];
	m [1] = t [1];
	m [2] = t [2];
	m [3] = t [3];
	m [4] = t [4];
	m [5] = t [5];
	m [6] = t [6];
	m [7] = t [7];
	m [8] = t [8];
	
	return *this;
}

mat3& mat3 :: operator *= ( float f )
{
	m [0] *= f; 
	m [1] *= f; 
	m [2] *= f;
	m [3] *= f; 
	m [4] *= f; 
	m [5] *= f;
	m [6] *= f; 
	m [7] *= f; 
	m [8] *= f;
	
	return *this;
}

mat3& mat3 :: operator /= ( float f )
{
	m [0] /= f; 
	m [1] /= f; 
	m [2] /= f;
	m [3] /= f; 
	m [4] /= f; 
	m [5] /= f;
	m [6] /= f; 
	m [7] /= f; 
	m [8] /= f;
	
	return *this;
}

float mat3 :: det () const
{
	return m [0] * (m [4] * m [8] - m [5] * m [7])
	     - m [1] * (m [3] * m [8] - m [5] * m [6])
	     + m [2] * (m [3] * m [7] - m [4] * m [6]);
}

bool	mat3 :: invert ()
{
	float d = det ();
	mat3  a;
	
	if ( fabs ( d ) < EPS )
		return false;
		
	d = 1.0f / d;
	
					// compute a determinant
	a.m [0] = (m [4] * m [8] - m [5] * m [7]) * d;
	a.m [1] = (m [2] * m [7] - m [1] * m [8]) * d;
	a.m [2] = (m [1] * m [5] - m [2] * m [4]) * d;
	a.m [3] = (m [5] * m [6] - m [3] * m [8]) * d;
	a.m [4] = (m [0] * m [8] - m [2] * m [6]) * d;
	a.m [5] = (m [2] * m [3] - m [0] * m [5]) * d;
	a.m [6] = (m [3] * m [7] - m [4] * m [6]) * d;
	a.m [7] = (m [1] * m [6] - m [0] * m [7]) * d;
	a.m [8] = (m [0] * m [4] - m [1] * m [3]) * d;
	
	return true;
}

mat3&	mat3 :: transpose ()
{
	mat3 a ( m [0], m [3], m [6], m [1], m [4], m [7], m [2], m [5], m [8] );

	return *this = a;
}

bool	mat3 :: isSymmetric () const
{
	return fabs ( m [1] - m [3] ) < EPS && fabs ( m [2] - m [6] ) < EPS && fabs ( m [5] - m [7] ) < EPS;
}

mat3 operator + ( const mat3 &a, const mat3& b )
{
	return mat3 ( a.m [0] + b.m [0], 
				  a.m [1] + b.m [1], 
				  a.m [2] + b.m [2],
				  a.m [3] + b.m [3], 
				  a.m [4] + b.m [4], 
				  a.m [5] + b.m [5],
				  a.m [6] + b.m [6], 
				  a.m [7] + b.m [7], 
				  a.m [8] + b.m [8] );
}

mat3 operator - ( const mat3 &a, const mat3& b )
{
	return mat3 ( a.m [0] - b.m [0], 
				  a.m [1] - b.m [1], 
				  a.m [2] - b.m [2],
				  a.m [3] - b.m [3], 
				  a.m [4] - b.m [4], 
				  a.m [5] - b.m [5],
				  a.m [6] - b.m [6], 
				  a.m [7] - b.m [7], 
				  a.m [8] - b.m [8] );
}

mat3 operator * ( const mat3 &a, const mat3& b )
{
	return mat3 ( a.m [0] * b.m [0] + a.m [1] * b.m [3] + a.m [2] * b.m [6],
				  a.m [0] * b.m [1] + a.m [1] * b.m [4] + a.m [2] * b.m [7],
				  a.m [0] * b.m [2] + a.m [1] * b.m [5] + a.m [2] * b.m [8],
				  a.m [3] * b.m [0] + a.m [4] * b.m [3] + a.m [5] * b.m [6],
				  a.m [3] * b.m [1] + a.m [4] * b.m [4] + a.m [5] * b.m [7],
				  a.m [3] * b.m [2] + a.m [4] * b.m [5] + a.m [5] * b.m [8],
				  a.m [6] * b.m [0] + a.m [7] * b.m [3] + a.m [8] * b.m [6],
				  a.m [6] * b.m [1] + a.m [7] * b.m [4] + a.m [8] * b.m [7],
				  a.m [6] * b.m [2] + a.m [7] * b.m [5] + a.m [8] * b.m [8] );
}

mat3 operator * ( const mat3& a, float f )
{
	return mat3 ( a.m [0] * f, a.m [1] * f, a.m [2] * f,
				  a.m [3] * f, a.m [4] * f, a.m [5] * f,
				  a.m [6] * f, a.m [7] * f, a.m [8] * f);
}

mat3 operator * ( float f, const mat3& a )
{
	return mat3 ( a.m [0] * f, a.m [1] * f, a.m [2] * f,
				  a.m [3] * f, a.m [4] * f, a.m [5] * f,
				  a.m [6] * f, a.m [7] * f, a.m [8] * f);
}

mat3 operator / ( const mat3& a, float f )
{
	return mat3(a.m [0] / f, a.m [1] / f, a.m [2] / f,
				a.m [3] / f, a.m [4] / f, a.m [5] / f,
				a.m [6] / f, a.m [7] / f, a.m [8] / f);
}

vec3 operator * ( const mat3& a, const vec3& b )
{
	return vec3 ( a.m [0]*b.x + a.m [1]*b.y + a.m [2]*b.z,
			      a.m [3]*b.x + a.m [4]*b.y + a.m [5]*b.z,
		          a.m [6]*b.x + a.m [7]*b.y + a.m [8]*b.z );
}

void	mat3 :: mat3GetEigen ( float l [3], vec3 e [3] ) const
{
	double	      a [3][3];
	double        d [3];
	double        v [3][3];
	double      * aPtr = (double *)a;
	const float * mPtr = m;

	for ( int i = 0; i < 9; i++ )
		*aPtr++ = (double) *mPtr++;

	eigen_decomposition ( a, v, d );

	l [0] = (float) d [0];
	l [1] = (float) d [1];
	l [2] = (float) d [2];

	e [0].x = (float) v [0][0];
	e [0].y = (float) v [0][1];
	e [0].z = (float) v [0][2];

	e [1].x = (float) v [1][0];
	e [1].y = (float) v [1][1];
	e [1].z = (float) v [1][2];

	e [2].x = (float) v [2][0];
	e [2].y = (float) v [2][1];
	e [2].z = (float) v [2][2];
}

mat3	mat3 :: identity ()
{
	return mat3 ( 1 );
}

mat3 mat3 :: scale ( const vec3& v )
{
	return mat3 ( v );
}

mat3 mat3 :: rotateX ( float angle )
{
	mat3 a ( 1 );
	float	 cosine = (float)cos ( angle );
	float	 sine   = (float)sin ( angle );

	a.m [4] = cosine;
	a.m [5] = sine;
	a.m [7] = -sine;
	a.m [8] = cosine;

	return a;
}

mat3 mat3 :: rotateY ( float angle )
{
	mat3 a ( 1 );
	float	 cosine = (float)cos ( angle );
	float	 sine   = (float)sin ( angle );

	a.m [0] = cosine;
	a.m [2] = -sine;
	a.m [6] = sine;
	a.m [8] = cosine;

	return a;
}

mat3 mat3 :: rotateZ ( float angle )
{
	mat3 a ( 1 );
	float	 cosine = (float)cos ( angle );
	float	 sine   = (float)sin ( angle );

	a.m [0] = cosine;
	a.m [1] = sine;
	a.m [3] = -sine;
	a.m [4] = cosine;

	return a;
}

mat3 mat3 :: rotate ( const vec3& v, float angle )
{
	mat3 a;
	float	 cosine = (float)cos ( angle );
	float	 sine   = (float)sin ( angle );

	a.m [0] = v.x * v.x + (1-v.x*v.x) * cosine;
	a.m [1] = v.x * v.y * (1-cosine) + v.z * sine;
	a.m [2] = v.x * v.z * (1-cosine) - v.y * sine;
	a.m [3] = v.x * v.y * (1-cosine) - v.z * sine;
	a.m [4] = v.y * v.y + (1-v.y*v.y) * cosine;
	a.m [5] = v.y * v.z * (1-cosine) + v.x * sine;
	a.m [6] = v.x * v.z * (1-cosine) + v.y * sine;
	a.m [7] = v.y * v.z * (1-cosine) - v.x * sine;
	a.m [8] = v.z * v.z + (1-v.z*v.z) * cosine;

	return a;
}

mat3 mat3 :: rotateEuler ( float yaw, float pitch, float roll )
{
    return rotateX ( pitch ) * rotateY ( yaw ) * rotateZ ( roll );
}

mat3 mat3 :: mirrorX ()
{
	return mat3 ( vec3 ( -1, 1, 1 ) );
}

mat3 mat3 :: mirrorY ()
{
	return mat3 ( vec3 ( 1, -1, 1 ) );
}

mat3 mat3 :: mirrorZ ()
{
	return mat3 ( vec3 ( 1, 1, -1 ) );
}

vec3 eulerFromMatrix ( const mat3& m )
{
	const float * data = m.data ();
	vec3          angle;
	float		  c;
	
	angle.x = -asinf ( data [2] );
	c       =  cosf  ( angle.x  );

	if ( fabsf ( c ) > EPS )
	{
		angle.y = atan2f ( data [5] / c, data [8] / c );		// m12, m22
		angle.z = atan2f ( data [1] / c, data [0] / c );
	} 
	else
	{
		angle.y = 0.0f;
		angle.z = atan2f ( data [3], data [4] );
	}

	return angle;
}
