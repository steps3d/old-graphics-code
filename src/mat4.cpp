
#include	<memory.h>
#include	"mat4.h"

mat4 :: mat4 ( float d )
{
	m [ 0] = d;
	m [ 1] = 0;
	m [ 2] = 0;
	m [ 3] = 0;
	m [ 4] = 0;
	m [ 5] = d;
	m [ 6] = 0;
	m [ 7] = 0;
	m [ 8] = 0;
	m [ 9] = 0;
	m [10] = d;
	m [11] = 0;
	m [12] = 0;
	m [13] = 0;
	m [14] = 0;
	m [15] = d;
}

mat4 :: mat4 ( const vec3& a )
{
	m [ 0] = a.x;
	m [ 1] = 0;
	m [ 2] = 0;
	m [ 3] = 0;
	m [ 4] = 0;
	m [ 5] = a.y;
	m [ 6] = 0;
	m [ 7] = 0;
	m [ 8] = 0;
	m [ 9] = 0;
	m [10] = a.z;
	m [11] = 0;
	m [12] = 0;
	m [13] = 0;
	m [14] = 0;
	m [15] = 1;
}

mat4 :: mat4 ( const vec4& a )
{
	m [ 0] = a.x;
	m [ 1] = 0;
	m [ 2] = 0;
	m [ 3] = 0;
	m [ 4] = 0;
	m [ 5] = a.y;
	m [ 6] = 0;
	m [ 7] = 0;
	m [ 8] = 0;
	m [ 9] = 0;
	m [10] = a.z;
	m [11] = 0;
	m [12] = 0;
	m [13] = 0;
	m [14] = 0;
	m [15] = a.w;
}

mat4 :: mat4 ( const mat3& a )
{
	m [ 0] = a.m [0];
	m [ 1] = a.m [1];
	m [ 2] = a.m [2];
	m [ 3] = 0;
	m [ 4] = a.m [3];
	m [ 5] = a.m [4];
	m [ 6] = a.m [5];
	m [ 7] = 0;
	m [ 8] = a.m [6];
	m [ 9] = a.m [7];
	m [10] = a.m [8];
	m [11] = 0;
	m [12] = 0;
	m [13] = 0;
	m [14] = 0;
	m [15] = 1;
}

mat4 :: mat4 ( const mat4& a )
{
	memcpy ( m, a.m, sizeof ( m ) );
}

mat4 :: mat4 ( const float * values )
{
	memcpy ( m, values, sizeof ( m ) );
}

mat4 :: mat4 ( float a0,  float a1,  float a2,  float a3,		// first row
			   float a4,  float a5,  float a6,  float a7,		// second row
		       float a8,  float a9,  float a10, float a11,		// third row
		       float a12, float a13, float a14, float a15 )		// fourth row
{
	m [ 0] = a0;
	m [ 1] = a1;
	m [ 2] = a2;
	m [ 3] = a3;
	m [ 4] = a4;
	m [ 5] = a5;
	m [ 6] = a6;
	m [ 7] = a7;
	m [ 8] = a8;
	m [ 9] = a9;
	m [10] = a10;
	m [11] = a11;
	m [12] = a12;
	m [13] = a13;
	m [14] = a14;
	m [15] = a15;
}

mat4& mat4 :: operator = ( const mat4& a )
{
	memcpy ( m, a.m, sizeof ( m ) );
	
	return *this;
}

mat4& mat4 :: initFromGl ( const float * a )
{
    m [0]  = a [0];
    m [1]  = a [4];
    m [2]  = a [8];
    m [3]  = a [12];
    m [4]  = a [1];
    m [5]  = a [5];
    m [6]  = a [9];
    m [7]  = a [13];
    m [8]  = a [2];
    m [9]  = a [6];
    m [10] = a [10];
    m [11] = a [14];
    m [12] = a [3];
    m [13] = a [7];
    m [14] = a [11];
    m [15] = a [15];

    return *this;
}

void mat4 :: toGl ( float * a )
{
    a [0]  = m [0];
    a [1]  = m [4];
    a [2]  = m [8];
    a [3]  = m [12];
    a [4]  = m [1];
    a [5]  = m [5];
    a [6]  = m [9];
    a [7]  = m [13];
    a [8]  = m [2];
    a [9]  = m [6];
    a [10] = m [10];
    a [11] = m [14];
    a [12] = m [3];
    a [13] = m [7];
    a [14] = m [11];
    a [15] = m [15];
}

mat4& mat4 :: operator += ( const mat4& a )
{
	m [ 0] += a.m [ 0]; 
	m [ 1] += a.m [ 1]; 
	m [ 2] += a.m [ 2]; 
	m [ 3] += a.m [ 3];
	m [ 4] += a.m [ 4]; 
	m [ 5] += a.m [ 5]; 
	m [ 6] += a.m [ 6]; 
	m [ 7] += a.m [ 7];
	m [ 8] += a.m [ 8]; 
	m [ 9] += a.m [ 9]; 
	m [10] += a.m [10]; 
	m [11] += a.m [11];
	m [12] += a.m [12]; 
	m [13] += a.m [13]; 
	m [14] += a.m [14]; 
	m [15] += a.m [15];
	
	return *this;
}

mat4& mat4 :: operator -= ( const mat4& a )
{
	m[ 0] -= a.m [ 0]; 
	m[ 1] -= a.m [ 1]; 
	m[ 2] -= a.m [ 2]; 
	m[ 3] -= a.m [ 3];
	m[ 4] -= a.m [ 4]; 
	m[ 5] -= a.m [ 5]; 
	m[ 6] -= a.m [ 6]; 
	m[ 7] -= a.m [ 7];
	m[ 8] -= a.m [ 8]; 
	m[ 9] -= a.m [ 9]; 
	m[10] -= a.m [10]; 
	m[11] -= a.m [11];
	m[12] -= a.m [12]; 
	m[13] -= a.m [13]; 
	m[14] -= a.m [14]; 
	m[15] -= a.m [15];
	
	return *this;
}

mat4& mat4 :: operator *= ( const mat4& a )
{
	float	t [16];			// temp result
	
	t [ 0] = m [ 0] * a.m [ 0] + m [ 1] * a.m [ 4] + m [ 2] * a.m [ 8] + m [ 3] * a.m [12];
	t [ 1] = m [ 0] * a.m [ 1] + m [ 1] * a.m [ 5] + m [ 2] * a.m [ 9] + m [ 3] * a.m [13],
	t [ 2] = m [ 0] * a.m [ 2] + m [ 1] * a.m [ 6] + m [ 2] * a.m [10] + m [ 3] * a.m [14],
	t [ 3] = m [ 0] * a.m [ 3] + m [ 1] * a.m [ 7] + m [ 2] * a.m [11] + m [ 3] * a.m [15],
	t [ 4] = m [ 4] * a.m [ 0] + m [ 5] * a.m [ 4] + m [ 6] * a.m [ 8] + m [ 7] * a.m [12],
	t [ 5] = m [ 4] * a.m [ 1] + m [ 5] * a.m [ 5] + m [ 6] * a.m [ 9] + m [ 7] * a.m [13],
	t [ 6] = m [ 4] * a.m [ 2] + m [ 5] * a.m [ 6] + m [ 6] * a.m [10] + m [ 7] * a.m [14],
	t [ 7] = m [ 4] * a.m [ 3] + m [ 5] * a.m [ 7] + m [ 6] * a.m [11] + m [ 7] * a.m [15],
	t [ 8] = m [ 8] * a.m [ 0] + m [ 9] * a.m [ 4] + m [10] * a.m [ 8] + m [11] * a.m [12],
	t [ 9] = m [ 8] * a.m [ 1] + m [ 9] * a.m [ 5] + m [10] * a.m [ 9] + m [11] * a.m [13],
	t [10] = m [ 8] * a.m [ 2] + m [ 9] * a.m [ 6] + m [10] * a.m [10] + m [11] * a.m [14],
	t [11] = m [ 8] * a.m [ 3] + m [ 9] * a.m [ 7] + m [10] * a.m [11] + m [11] * a.m [15],
	t [12] = m [12] * a.m [ 0] + m [13] * a.m [ 4] + m [14] * a.m [ 8] + m [15] * a.m [12],
	t [13] = m [12] * a.m [ 1] + m [13] * a.m [ 5] + m [14] * a.m [ 9] + m [15] * a.m [13],
	t [14] = m [12] * a.m [ 2] + m [13] * a.m [ 6] + m [14] * a.m [10] + m [15] * a.m [14],
	t [15] = m [12] * a.m [ 3] + m [13] * a.m [ 7] + m [14] * a.m [11] + m [15] * a.m [15];
	
	memcpy ( m, t, sizeof ( m ) );
	
	return *this;
}

mat4& mat4 :: operator *= ( float f )
{
	m [ 0] *= f; 
	m [ 1] *= f; 
	m [ 2] *= f; 
	m [ 3] *= f;
	m [ 4] *= f; 
	m [ 5] *= f; 
	m [ 6] *= f; 
	m [ 7] *= f;
	m [ 8] *= f;
	m [ 9] *= f; 
	m [10] *= f; 
	m [11] *= f;
	m [12] *= f; 
	m [13] *= f; 
	m [14] *= f; 
	m [15] *= f;
	
	return *this;
}

mat4& mat4 :: operator /= ( float f )
{
	m [ 0] /= f; 
	m [ 1] /= f; 
	m [ 2] /= f; 
	m [ 3] /= f;
	m [ 4] /= f; 
	m [ 5] /= f; 
	m [ 6] /= f; 
	m [ 7] /= f;	
	m [ 8] /= f; 
	m [ 9] /= f; 
	m [10] /= f; 
	m [11] /= f;
	m [12] /= f; 
	m [13] /= f; 
	m [14] /= f; 
	m [15] /= f;
	
	return *this;
}

bool	mat4 :: invert ()
{
#define SWAP_ROWS(a, b) {  float * _tmp = a; (a)=(b); (b)=_tmp; }
#define MAT(m,r,c)     m [r*4+c]

	float	wtmp [4][8];
	float	m0, m1, m2, m3, s;
	float	* r0, * r1, * r2, * r3;

	r0 = wtmp [0];
	r1 = wtmp [1];
	r2 = wtmp [2];
	r3 = wtmp [3];

	r0 [0] = MAT(m,0,0);
	r0 [1] = MAT(m,0,1);
	r0 [2] = MAT(m,0,2);
	r0 [3] = MAT(m,0,3);
	r0 [4] = 1;
	r0 [5] =
	r0 [6] =
	r0 [7] = 0;

	r1 [0] = MAT(m,1,0);
	r1 [1] = MAT(m,1,1);
	r1 [2] = MAT(m,1,2);
	r1 [3] = MAT(m,1,3);
	r1 [5] = 1;
	r1 [4] =
	r1 [6] =
	r1 [7] = 0,

	r2 [0] = MAT(m,2,0);
	r2 [1] = MAT(m,2,1);
	r2 [2] = MAT(m,2,2);
	r2 [3] = MAT(m,2,3);
	r2 [6] = 1;
	r2 [4] =
	r2 [5] =
	r2 [7] = 0;

	r3 [0] = MAT(m,3,0);
	r3 [1] = MAT(m,3,1);
	r3 [2] = MAT(m,3,2);
	r3 [3] = MAT(m,3,3);
	r3 [7] = 1;
	r3 [4] =
	r3 [5] =
	r3 [6] = 0;
											// choose pivot - or die
	if ( fabs (r3 [0] )> fabs ( r2 [0] ) )
		SWAP_ROWS ( r3, r2 );

	if ( fabs ( r2 [0] ) > fabs ( r1 [0] ) )
		SWAP_ROWS ( r2, r1 );

	if ( fabs ( r1 [0] ) > fabs ( r0 [0 ] ) )
		SWAP_ROWS ( r1, r0 );

	if ( r0 [0] == 0 )
		return false;
											// eliminate first variable
	m1 = r1[0]/r0[0];
	m2 = r2[0]/r0[0];
	m3 = r3[0]/r0[0];

	s = r0[1];
	r1[1] -= m1 * s;
	r2[1] -= m2 * s;
	r3[1] -= m3 * s;

	s = r0[2];
	r1[2] -= m1 * s;
	r2[2] -= m2 * s;
	r3[2] -= m3 * s;

	s = r0[3];
	r1[3] -= m1 * s;
	r2[3] -= m2 * s;
	r3[3] -= m3 * s;

	s = r0[4];

	if ( s != 0 )
	{
		r1[4] -= m1 * s; r2[4] -= m2 * s; r3[4] -= m3 * s;
	}

	s = r0[5];

	if ( s != 0.0 )
	{
		r1[5] -= m1 * s; r2[5] -= m2 * s; r3[5] -= m3 * s;
	}

	s = r0[6];

	if ( s != 0 )
	{
		r1[6] -= m1 * s; r2[6] -= m2 * s; r3[6] -= m3 * s;
	}

	s = r0[7];

	if ( s != 0 )
	{
		r1[7] -= m1 * s; r2[7] -= m2 * s; r3[7] -= m3 * s;
	}

											// choose pivot - or die
	if ( fabs (r3 [1] ) > fabs ( r2 [1] ) )
		SWAP_ROWS ( r3, r2 );

	if ( fabs ( r2 [1] ) > fabs ( r1 [1] ) )
		SWAP_ROWS ( r2, r1 );

	if ( r1 [1] == 0 )
		return false;


											// eliminate second variable
	m2     = r2[1]/r1[1]; m3     = r3[1]/r1[1];
	r2[2] -= m2 * r1[2];  r3[2] -= m3 * r1[2];
	r2[3] -= m2 * r1[3];  r3[3] -= m3 * r1[3];

	s = r1[4];

	if ( 0 != s )
	{
		r2[4] -= m2 * s; r3[4] -= m3 * s;
	}

	s = r1[5];

	if ( 0 != s )
	{
		r2[5] -= m2 * s; r3[5] -= m3 * s;
	}

	s = r1[6];

	if ( 0 != s )
	{
		r2[6] -= m2 * s; r3[6] -= m3 * s;
	}

	s = r1[7];

	if ( 0 != s )
	{
		r2[7] -= m2 * s; r3[7] -= m3 * s;
	}

											// choose pivot - or die
	if ( fabs ( r3 [2] ) > fabs ( r2 [2] ) )
		SWAP_ROWS ( r3, r2 );

	if ( r2 [2] == 0)
	    return false;

											// eliminate third variable
	m3     = r3[2]/r2[2];
	r3[3] -= m3 * r2[3], r3[4] -= m3 * r2[4],
	r3[5] -= m3 * r2[5], r3[6] -= m3 * r2[6],
	r3[7] -= m3 * r2[7];

											// last check
	if ( r3 [3] == 0 )
		return false;


											// now back substitute row 3
	s      = 1/r3[3];
	r3[4] *= s; r3[5] *= s; r3[6] *= s; r3[7] *= s;

											// now back substitute row 2
	m2    = r2[3];
	s     = 1/r2[2];
	r2[4] = s * (r2[4] - r3[4] * m2), r2[5] = s * (r2[5] - r3[5] * m2),
	r2[6] = s * (r2[6] - r3[6] * m2), r2[7] = s * (r2[7] - r3[7] * m2);

	m1     = r1[3];
	r1[4] -= r3[4] * m1, r1[5] -= r3[5] * m1,
	r1[6] -= r3[6] * m1, r1[7] -= r3[7] * m1;

	m0     = r0[3];
	r0[4] -= r3[4] * m0, r0[5] -= r3[5] * m0,
	r0[6] -= r3[6] * m0, r0[7] -= r3[7] * m0;

											// now back substitute row 1
	m1    = r1[2];
	s     = 1/r1[1];
	r1[4] = s * (r1[4] - r2[4] * m1), r1[5] = s * (r1[5] - r2[5] * m1),
	r1[6] = s * (r1[6] - r2[6] * m1), r1[7] = s * (r1[7] - r2[7] * m1);

	m0     = r0[2];
	r0[4] -= r2[4] * m0, r0[5] -= r2[5] * m0,
	r0[6] -= r2[6] * m0, r0[7] -= r2[7] * m0;

											// now back substitute row 0
	m0    = r0[1];
	s     = 1/r0[0];
	r0[4] = s * (r0[4] - r1[4] * m0), r0[5] = s * (r0[5] - r1[5] * m0),
	r0[6] = s * (r0[6] - r1[6] * m0), r0[7] = s * (r0[7] - r1[7] * m0);

	MAT(m,0,0) = r0[4]; MAT(m,0,1) = r0[5],
	MAT(m,0,2) = r0[6]; MAT(m,0,3) = r0[7],
	MAT(m,1,0) = r1[4]; MAT(m,1,1) = r1[5],
	MAT(m,1,2) = r1[6]; MAT(m,1,3) = r1[7],
	MAT(m,2,0) = r2[4]; MAT(m,2,1) = r2[5],
	MAT(m,2,2) = r2[6]; MAT(m,2,3) = r2[7],
	MAT(m,3,0) = r3[4]; MAT(m,3,1) = r3[5],
	MAT(m,3,2) = r3[6]; MAT(m,3,3) = r3[7];

#undef MAT
#undef SWAP_ROWS


/*
 * Another variant
 *
 

*/
	return true;
}

float	mat4 :: det () const
{
	return 
		(m [0] * m [5] - m [1] * m [4]) * (m [10] * m [15] - m [11] * m [14]) -
		(m [0] * m [6] - m [2] * m [4]) * (m [9]  * m [15] - m [11] * m [13]) +
		(m [0] * m [7] - m [3] * m [4]) * (m [9]  * m [14] - m [10] * m [13]) +
		(m [1] * m [6] - m [2] * m [5]) * (m [8]  * m [15] - m [11] * m [12]) -
		(m [1] * m [7] - m [3] * m [5]) * (m [8]  * m [14] - m [10] * m [12]) +
		(m [2] * m [7] - m [3] * m [6]) * (m [8]  * m [13] - m [ 9] * m [12]);
}

mat4& mat4 :: transpose ( )
{
	float	t;
	
#define	SWAP(x, y)	t=x; x=y; y=t;

	SWAP( m[1], m[4]   )
	SWAP( m[2], m[8]   )
	SWAP( m[3], m[12]  )
	SWAP( m[6], m[9]   )
	SWAP( m[7], m[13]  )
	SWAP( m[11], m[14] )
	
#undef SWAP
	
	return *this;
}

mat4 operator + ( const mat4& a, const mat4& b )
{
	return mat4 ( a.m [ 0] + b.m [ 0], a.m [ 1] + b.m [ 1], a.m [ 2] + b.m [ 2], a.m [ 3] + b.m [ 3],
				  a.m [ 4] + b.m [ 4], a.m [ 5] + b.m [ 5], a.m [ 6] + b.m [ 6], a.m [ 7] + b.m [ 7],
				  a.m [ 8] + b.m [ 8], a.m [ 9] + b.m [ 9], a.m [10] + b.m [10], a.m [11] + b.m [11],
				  a.m [12] + b.m [12], a.m [13] + b.m [13], a.m [14] + b.m [14], a.m [15] + b.m [15] );
}

mat4 operator - ( const mat4& a, const mat4& b )
{
	return mat4 ( a.m [ 0] - b.m [ 0], a.m [ 1] - b.m [ 1], a.m [ 2] - b.m [ 2], a.m [ 3] - b.m [ 3],
				  a.m [ 4] - b.m [ 4], a.m [ 5] - b.m [ 5], a.m [ 6] - b.m [ 6], a.m [ 7] - b.m [ 7],
				  a.m [ 8] - b.m [ 8], a.m [ 9] - b.m [ 9], a.m [10] - b.m [10], a.m [11] - b.m [11],
				  a.m [12] - b.m [12], a.m [13] - b.m [13], a.m [14] - b.m [14], a.m [15] - b.m [15] );
}

mat4 operator * ( const mat4& a, const mat4& b )
{
	return mat4 (
		a.m [ 0] * b.m [ 0] + a.m [ 1] * b.m [ 4] + a.m [ 2] * b.m [ 8] + a.m [ 3] * b.m [12],
		a.m [ 0] * b.m [ 1] + a.m [ 1] * b.m [ 5] + a.m [ 2] * b.m [ 9] + a.m [ 3] * b.m [13],
		a.m [ 0] * b.m [ 2] + a.m [ 1] * b.m [ 6] + a.m [ 2] * b.m [10] + a.m [ 3] * b.m [14],
		a.m [ 0] * b.m [ 3] + a.m [ 1] * b.m [ 7] + a.m [ 2] * b.m [11] + a.m [ 3] * b.m [15],
		a.m [ 4] * b.m [ 0] + a.m [ 5] * b.m [ 4] + a.m [ 6] * b.m [ 8] + a.m [ 7] * b.m [12],
		a.m [ 4] * b.m [ 1] + a.m [ 5] * b.m [ 5] + a.m [ 6] * b.m [ 9] + a.m [ 7] * b.m [13],
		a.m [ 4] * b.m [ 2] + a.m [ 5] * b.m [ 6] + a.m [ 6] * b.m [10] + a.m [ 7] * b.m [14],
		a.m [ 4] * b.m [ 3] + a.m [ 5] * b.m [ 7] + a.m [ 6] * b.m [11] + a.m [ 7] * b.m [15],
		a.m [ 8] * b.m [ 0] + a.m [ 9] * b.m [ 4] + a.m [10] * b.m [ 8] + a.m [11] * b.m [12],
		a.m [ 8] * b.m [ 1] + a.m [ 9] * b.m [ 5] + a.m [10] * b.m [ 9] + a.m [11] * b.m [13],
		a.m [ 8] * b.m [ 2] + a.m [ 9] * b.m [ 6] + a.m [10] * b.m [10] + a.m [11] * b.m [14],
		a.m [ 8] * b.m [ 3] + a.m [ 9] * b.m [ 7] + a.m [10] * b.m [11] + a.m [11] * b.m [15],
		a.m [12] * b.m [ 0] + a.m [13] * b.m [ 4] + a.m [14] * b.m [ 8] + a.m [15] * b.m [12],
		a.m [12] * b.m [ 1] + a.m [13] * b.m [ 5] + a.m [14] * b.m [ 9] + a.m [15] * b.m [13],
		a.m [12] * b.m [ 2] + a.m [13] * b.m [ 6] + a.m [14] * b.m [10] + a.m [15] * b.m [14],
		a.m [12] * b.m [ 3] + a.m [13] * b.m [ 7] + a.m [14] * b.m [11] + a.m [15] * b.m [15] );
}

mat4 operator * ( const mat4& a, float b )
{
	return mat4 ( a.m [ 0] * b, a.m [ 1] * b, a.m [ 2] * b, a.m [ 3] * b,
	              a.m [ 4] * b, a.m [ 5] * b, a.m [ 6] * b, a.m [ 7] * b,
				  a.m [ 8] * b, a.m [ 9] * b, a.m [10] * b, a.m [11] * b,
				  a.m [12] * b, a.m [13] * b, a.m [14] * b, a.m [15] * b );
}

mat4 operator * ( float b, const mat4& a )
{
	return mat4 ( a.m [ 0] * b, a.m [ 1] * b, a.m [ 2] * b, a.m [ 3] * b,
	              a.m [ 4] * b, a.m [ 5] * b, a.m [ 6] * b, a.m [ 7] * b,
				  a.m [ 8] * b, a.m [ 9] * b, a.m [10] * b, a.m [11] * b,
				  a.m [12] * b, a.m [13] * b, a.m [14] * b, a.m [15] * b );
}

vec3 operator * ( const mat4& a, const vec3& b)
{
	return vec3 ( a.m[ 0]*b.x + a.m[ 1]*b.y + a.m[ 2]*b.z + a.m[ 3],
			      a.m[ 4]*b.x + a.m[ 5]*b.y + a.m[ 6]*b.z + a.m[ 7],
		          a.m[ 8]*b.x + a.m[ 9]*b.y + a.m[10]*b.z + a.m[11] );
}

vec4 operator * ( const mat4& a, const vec4& b )
{
	return vec4 ( a.m[ 0]*b.x + a.m[ 1]*b.y + a.m[ 2]*b.z + a.m[ 3]*b.w,
			      a.m[ 4]*b.x + a.m[ 5]*b.y + a.m[ 6]*b.z + a.m[ 7]*b.w,
		          a.m[ 8]*b.x + a.m[ 9]*b.y + a.m[10]*b.z + a.m[11]*b.w,
                  a.m[12]*b.x + a.m[13]*b.y + a.m[14]*b.z + a.m[15]*b.w );
}

mat4	mat4 :: identity ()
{
	return mat4 ( 1 );
}

mat4 mat4 :: translate ( const vec3& a )
{
	return mat4 ( 1, 0, 0, a.x,
	              0, 1, 0, a.y,
	              0, 0, 1, a.z,
	              0, 0, 0, 1 );
}

mat4 mat4 :: scale ( const vec3& s )
{
	return mat4 ( s );
}

mat4 mat4 :: rotateX ( float angle )
{
	mat4 	a ( 1 );
	float	cosine = (float)cos ( angle );
	float	sine   = (float)sin ( angle );

	a.m [ 5] = cosine;
	a.m [ 6] = sine;
	a.m [ 9] = -sine;
	a.m [10] = cosine;

	return a;
}

mat4 mat4 :: rotateY ( float angle )
{
	float		cosine = (float)cos ( angle );
	float		sine   = (float)sin ( angle );

	return mat4 ( cosine, 0, -sine,   0,
	              0,      1, 0,      0,
				  sine,  0, cosine, 0,
				  0,      0, 0,      1 );
}

mat4 mat4 :: rotateZ ( float angle )
{
	float		cosine = (float)cos ( angle );
	float		sine   = (float)sin ( angle );

	return mat4 ( cosine, sine,  0, 0,
	              -sine,   cosine, 0, 0, 
				  0,      0,      1, 0,
				  0,      0,      0, 1 );
}

mat4 mat4 :: rotate ( const vec3& v, float angle )
{
	mat4	a;
	float		cosine = (float)cos ( angle );
	float		sine   = (float)sin ( angle );

	a.m [ 0] = v.x *v.x + (1-v.x*v.x) * cosine;
	a.m [ 1] = v.x *v.y * (1-cosine) + v.z * sine;
	a.m [ 2] = v.x *v.z * (1-cosine) - v.y * sine;
	a.m [ 3] = 0;
	a.m [ 4] = v.x *v.y * (1-cosine) - v.z * sine;
	a.m [ 5] = v.y *v.y + (1-v.y*v.y) * cosine;
	a.m [ 6] = v.y *v.z * (1-cosine) + v.x * sine;
	a.m [ 7] = 0;
	a.m [ 8] = v.x *v.z * (1-cosine) + v.y * sine;
	a.m [ 9] = v.y *v.z * (1-cosine) - v.x * sine;
	a.m [10] = v.z *v.z + (1-v.z*v.z) * cosine;
	a.m [11] = 0;
	a.m [12] =
	a.m [13] =
	a.m [14] = 0;
	a.m [15] = 1;

	return a;
}

mat4 mat4 :: rotateEuler ( float yaw, float pitch, float roll )
{
    return rotateX ( pitch ) * rotateY ( yaw ) * rotateZ ( roll );
/*
	mat4 a;
	float    cy = cos ( yaw );
	float    sy = sin ( yaw );
    float    cp = cos ( pitch );
    float    sp = sin ( pitch );
    float    cr = cos ( roll );
    float    sr = sin ( roll );

	a.x [0][0] = cy*cr + sy*sp*sr;
	a.x [1][0] = -cy*sr + sy*sp*cr;
	a.x [2][0] = sy*cp;
	a.x [0][1] = sr * cp;
	a.x [1][1] = cr*cp;
	a.x [2][1] = -sp;
	a.x [0][2] = -sy*cr - cy*sp*sr;
	a.x [1][2] = sr*sy + cy*sp*cr;
	a.x [2][2] = cy*cp;

	return a;
*/
}

mat4 mat4 :: mirrorX ()
{
	return mat4 ( vec3 ( -1, 1, 1 ) );
}

mat4 mat4 :: mirrorY ()
{
	return mat4 ( vec3 ( 1, -1, 1 ) );
}

mat4 mat4 :: mirrorZ ()
{
	return mat4 ( vec3 ( 1, 1, -1 ) );
}

///////////////////// OpenGL matrices //////////////////////////

mat4 ortho ( float left, float right, float bottom, float top, float zNear, float zFar )
{
	float	m00 = 2.0f / (right - left);
	float	m11 = 2.0f / (top - bottom);
	float	m22 = 2.0f / (zNear - zFar);
	float	tx  = (left + right) / (left - right);
	float	ty  = (bottom + top) / (bottom - top);
	float	tz  = (zNear + zFar) / (zNear - zFar);
	
	return mat4 ( m00, 0,   0,   tx,
	              0,   m11, 0,   ty,
				  0,   0,   m22, tz,
				  0,   0,   0,   1 );
}

mat4 perspective ( float fov, float aspect, float zNear, float zFar )
{
	float f   = 1.0 / tanf ( 0.5f * toRadians ( fov ) );
	float m22 = (zNear + zFar) / (zNear - zFar);
	float m23 = (2.0f * zFar * zNear) / (zNear - zFar);

	return mat4 ( f / aspect, 0, 0,   0,
	              0,          f, 0,   0,
	              0,          0, m22, m23,
	              0,          0, -1,  0 );
}

mat4 frustum ( float left, float right, float bottom, float top, float zNear, float zFar)
{
	float	m00 = 2*zNear / (right - left);
	float	m11 = 2*zNear / (right - left);
	float	a   = (right + left) / (right - left);
	float	b   = (top + bottom) / (top - bottom);
	float	c   = (zNear + zFar) / (zNear - zFar);
	float	d   = 2*zNear*zFar / ( zNear - zFar);
	
	return mat4 ( m00, 0,   a, 0,
	              0,   m11, b, 0,
				  0,   0,   c, d,
				  0,   0,  -1, 0 );
}

mat4 lookAt ( const vec3& eye, const vec3& center, const vec3& up )
{
	vec3 f = normalize ( eye - center );
	vec3 s = cross     ( normalize ( up ), f );
	vec3 u = cross     ( f, s );
	
	mat4	m ( s.x,  s.y,  s.z, -dot(s, eye),
	            u.x,  u.y,  u.z, -dot(u, eye),
	            f.x,  f.y,  f.z, -dot(f, eye),
	            0,    0,    0,   1 );
				
	return m;
}

mat3 normalMatrix ( const mat4& mv )
{
	mat4	m = mv;
	
	m.invert ();
	
	mat3	nm = m.upper3x3 ();
	
	nm.transpose ();
	
	return nm;
}

vec3 eulerFromMatrix ( const mat4& m )
{
	const float * data = m.data ();
	vec3          angle;
	float		  c;

	angle.x = -asinf ( data [2] );
	c       =  cosf  ( angle.x );

	if ( fabsf ( c ) > EPS )
	{
		angle.y = atan2f ( data [6] / c, data [10] / c );
		angle.z = atan2f ( data [1] / c, data [0]  / c );
	} 
	else
	{
		angle.y = 0.0f;
		angle.z = atan2f ( data [4], data [5] );
	}

	return angle;
}

vec4	unproject ( const vec3& win, const mat4& mv, const mat4& proj, int viewport [4] )
{
	mat4	m = proj * mv;
	vec4	v ( 2*(win.x - viewport [0]) / viewport[2] - 1, 2*(win.y - viewport [1])/viewport [3] - 1, 2*win.z - 1, 1 );
	
	m.invert ();
	
	return m * v;
}