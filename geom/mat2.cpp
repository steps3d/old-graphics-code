//
// Basic class for 2d matrices.
//
// Author: Alexey V. Boreskov
//

#include	"mat2.h"

mat2 :: mat2 ( const float * values )
{
	m [0] = values [0]; 
	m [1] = values [1];
	m [2] = values [2]; 
	m [3] = values [3];
}

mat2 :: mat2 ( float v )
{
	m [0] = v; 
	m [1] = 0;
	m [2] = 0; 
	m [3] = v;
}

mat2 :: mat2 ( const vec2& v )
{
	m [0] = v.x; 
	m [1] = 0;
	m [2] = 0; 
	m [3] = v.y;
}

mat2 :: mat2 ( float m0, float m1, float m2, float m3 )
{
	m [0] = m0;
	m [1] = m1;
	m [2] = m2;
	m [3] = m3;
}

mat2 :: mat2 ( const mat2 &a )
{
	m [0] = a.m [0]; 
	m [1] = a.m [1];
	m [2] = a.m [2]; 
	m [3] = a.m [3];
}

mat2& mat2 :: operator = ( const mat2& a )
{
	m [0] = a.m [0];
	m [1] = a.m [1];
	m [2] = a.m [2];
	m [3] = a.m [3];

	return *this;
}

mat2& mat2 :: operator += ( const mat2& a )
{
	m [0] += a.m [0]; 
	m [1] += a.m [1];
	m [2] += a.m [2]; 
	m [3] += a.m [3];
	
	return *this;
}

mat2& mat2 :: operator -= ( const mat2& a )
{
	m [0] -= a.m [0]; 
	m [1] -= a.m [1];
	m [2] -= a.m [2]; 
	m [3] -= a.m [3];
	
	return *this;
}

mat2& mat2 :: operator *= ( const mat2& a )
{
	mat2	t;
	
	t.m [0] = m [0] * a.m [0] + m [1] * a.m [2];
	t.m [1] = m [0] * a.m [1] + m [1] * a.m [3];
	t.m [2] = m [2] * a.m [0] + m [3] * a.m [2];
	t.m [3] = m [2] * a.m [1] + m [3] * a.m [3];
	
	return *this = t;
}

mat2& mat2 :: operator *= ( float f )
{
	m [0] *= f; 
	m [1] *= f;
	m [2] *= f; 
	m [3] *= f;
	
	return *this;
}

mat2& mat2 :: operator /= ( float f )
{
	m [0] /= f; 
	m [1] /= f;
	m [2] /= f; 
	m [3] /= f;
	
	return *this;
}

mat2 operator + ( const mat2&a, const mat2& b )
{
	return mat2 ( a.m [0] + b.m [0], a.m [1] + b.m [1], a.m [2] + b.m [2], a.m [3] + b.m [3] );
}

mat2 operator - (const mat2& a, const mat2& b )
{
	return mat2 ( a.m [0] - b.m [0], a.m [1] - b.m [1], a.m [2] - b.m [2], a.m [3] - b.m [3] );
}

mat2 operator * ( const mat2& a, const mat2& b )
{
	return mat2 ( a.m [0] * b.m [0] + a.m [1] * b.m [2],
				  a.m [0] * b.m [1] + a.m [1] * b.m [3],
				  a.m [2] * b.m [0] + a.m [3] * b.m [2],
				  a.m [2] * b.m [1] + a.m [3] * b.m [3] );
}

mat2 operator * ( const mat2& a, float f )
{
	return mat2 ( a.m [0] * f, a.m [1] * f, a.m [2] * f, a.m [3] * f );
}

mat2 operator * ( float f, const mat2& a )
{
	return mat2 ( a.m [0] * f, a.m [1] * f, a.m [2] * f, a.m [3] * f );
}

vec2 operator * ( const mat2& a, const vec2& b )
{
	return vec2 ( a.m [0] * b.x + a.m [1] * b.y, a.m [2] * b.x + a.m [3] * b.y );
}

mat2 operator / ( const mat2& a, float f )
{
	return mat2 ( a.m [0] / f, a.m [1] / f, a.m [2] / f, a.m [3] / f );
}

bool	mat2:: invert ()
{
	float d = det();
	
	if ( fabs ( d ) < EPS )
		return false;
		
	d = 1.0 / d;
	
	*this = mat2 ( m [3] * d, -m [2] * d, -m [1] * d,  m [0] * d );
			   
	return true;
}
