//
// Quaternion class
//
// Author: Alexey V. Boreskov <steps3d@gmail.com>, <steps3d@narod.ru>
//

#include	"quat.h"
#include	"mat3.h"
#include	"mat4.h"

					// build a quternion for rotation around axis on angle
quat :: quat ( float angle, const vec3& axis )
{
	float	sine   = (float)sin ( angle * 0.5f );
	float	cosine = (float)cos ( angle * 0.5f );

	x = axis.x * sine;
	y = axis.y * sine;
	z = axis.z * sine;
	w = cosine;
}

quat :: quat ( const mat3& mat )
{
	float	s, q [4];
	int		i, j, k;
	int		nxt [3] = { 1, 2, 0 };
	float	trace   = mat [0][0] + mat [1][1] + mat [2][2];

	if ( trace > EPS )
	{
		s = (float) sqrt ( trace + 1.0f );
		w = s / 2.0f;
		s = 0.5f / s;
		x = ( mat [2][1] - mat [1][2] ) * s;
		y = ( mat [0][2] - mat [2][0] ) * s;
		z = ( mat [1][0] - mat [0][1] ) * s;
	}
	else
	{
		i = 0;

		if ( mat [1][1] > mat [0][0] )
			i = 1;

		if ( mat [2][2] > mat [i][i] )
			i = 2;

		j = nxt [i];
		k = nxt [j];

		s = (float) sqrt ( ( mat [i][i] - ( mat [j][j] + mat [k][k] )) + 1.0f );

		q [i] = s * 0.5f;

		if ( s != 0.0f )
			s = 0.5f / s;

		q [3] = ( mat [k][j] - mat [j][k] ) * s;
		q [j] = ( mat [j][i] + mat [i][j] ) * s;
		q [k] = ( mat [k][i] + mat [i][k] ) * s;

		x = q [0];
		y = q [1];
		z = q [2];
		w = q [3];
	}
}

quat :: quat ( float mat [3][3] )
{
	float	s, q [4];
	int		i, j, k;
	int		nxt [3] = { 1, 2, 0 };

	float	trace = mat [0][0] + mat [1][1] + mat [2][2];

	if ( trace > 0.0f )
	{
		s = (float) sqrt ( trace + 1.0f );
		w = s / 2.0f;
		s = 0.5f / s;
		x = ( mat [2][1] - mat [1][2] ) * s;
		y = ( mat [0][2] - mat [2][0] ) * s;
		z = ( mat [1][0] - mat [0][1] ) * s;
	}
	else
	{
		i = 0;

		if ( mat [1][1] > mat [0][0] )
			i = 1;

		if ( mat [2][2] > mat [i][i] )
			i = 2;

		j = nxt [i];
		k = nxt [j];

		s = (float) sqrt ( ( mat [i][i] - ( mat [j][j] + mat [k][k] )) + 1.0f );

		q [i] = s * 0.5f;

		if ( s != 0.0f )
			s = 0.5f / s;

		q [3] = ( mat [k][j] - mat [j][k] ) * s;
		q [j] = ( mat [j][i] + mat [i][j] ) * s;
		q [k] = ( mat [k][i] + mat [i][k] ) * s;

		x = q [0];
		y = q [1];
		z = q [2];
		w = q [3];
	}
}

vec3	quat :: rotate ( const vec3& v ) const
{
	quat	p     ( v );
	quat	qConj ( -x, -y, -z, w );

	p  = *this * p * qConj;

	return vec3 ( p.x, p.y, p.z );
}

quat&	quat :: normalize ()
{
	float	lenSq = x * x + y * y + z * z;

	if ( lenSq > 1.0f - EPS )
	{
		float	invLen = 1.0f / lenSq;

		x *= invLen;
		y *= invLen;
		z *= invLen;
		w = 0.0f;
	}
	else
		w = (float) sqrt ( 1.0f - lenSq );

	return *this;
}

quat&	quat :: initWithAngles ( float yaw, float pitch, float roll )
{
	yaw   *= 0.5f;
	pitch *= 0.5f;
	roll  *= 0.5f;

	float	cx = (float)cos ( yaw );
	float	cy = (float)cos ( pitch );
	float	cz = (float)cos ( roll );
	float	sx = (float)sin ( yaw );
	float	sy = (float)sin ( pitch );
	float	sz = (float)sin ( roll );

	float	cc = cx * cz;
	float	cs = cx * sz;
	float	sc = sx * cz;
	float	ss = sx * sz;

	x = cy * sc - sy * cs;
	y = cy * ss + sy * cc;
	z = cy * cs - sy * sc;
	w = cy * cc + sy * ss;

	return *this;
}

					// build a homogenous matrix from quaternion
					// in OpenGL style
void quat :: getMatrix ( mat4& m ) const
{
	float * matrix = (float *) m.data ();
	
		// 1st row
	matrix [ 0] = 1.0f - 2.0f * ( y * y + z * z );  
	matrix [ 1] = 2.0f * ( x * y - w * z );  
	matrix [ 2] = 2.0f * ( x * z + w * y );  
	matrix [ 3] = 0.0f;  

		// 2nd row
	matrix [ 4] = 2.0f * ( x * y + w * z );  
	matrix [ 5] = 1.0f - 2.0f * ( x * x + z * z );  
	matrix [ 6] = 2.0f * ( y * z - w * x );  
	matrix [ 7] = 0.0f;  

		// 3rd row
	matrix [ 8] = 2.0f * ( x * z - w * y );  
	matrix [ 9] = 2.0f * ( y * z + w * x );  
	matrix [10] = 1.0f - 2.0f * ( x * x + y * y );  
	matrix [11] = 0.0f;  

		// 4th row
	matrix [12] = 0;  
	matrix [13] = 0;  
	matrix [14] = 0;  
	matrix [15] = 1;
}

void	quat :: getMatrix ( mat3& m ) const
{
		// 1st row
	m [0][0] = 1.0f - 2.0f * ( y * y + z * z );
	m [0][1] = 2.0f * ( x * y - w * z );
	m [0][2] = 2.0f * ( x * z + w * y );

		// 2nd row
	m [1][0] = 2.0f * ( x * y + w * z );
	m [1][1] = 1.0f - 2.0f * ( x * x + z * z );
	m [1][2] = 2.0f * ( y * z - w * x );

		// 3rd row
	m [2][0] = 2.0f * ( x * z - w * y );
	m [2][1] = 2.0f * ( y * z + w * x );
	m [2][2] = 1.0f - 2.0f * ( x * x + y * y );
}

quat	quat :: exp () const
{
	float t = sqrtf ( x * x + y * y + z * z );		// v.length ();
	
	if ( t < EPS )
		return quat ( 0, 0, 0, expf ( w ) );
		
	float	s = sinf ( t ) / t;
	float	c = cosf ( t );
	
	return expf ( w ) * quat ( s * x, s * y, s* z, c );
}

quat	quat :: log () const
{
	float l    = length ();
	
	if ( l < EPS )
		return quat ( 0, 0, 0, 0 );			// some error
		
	float a = (float)acos ( w / l );
	float s = sinf ( a );
	
	return quat ( x * s, y * s, z * s, logf ( l ) );
}

quat	slerp ( const quat& q1, const quat& q2, float t )
{
	float	d = dot ( q1, q2 );
	float	scale1, scale2;

					// check whether q1 and q2 are opposite
	if ( 1 + d > EPS )
	{
		if ( 1 - d > EPS )
		{
			float	omega   = (float) acos ( d );
			float	invSine = 1.0f / (float) sin ( omega );
			
			scale1  = invSine * (float) sin ( ( 1.0f - t ) * omega );
			scale2  = invSine * (float) sin ( t * omega );
		}
		else
		{
			scale1 = 1.0f - t;
			scale2 = t;
		}
		
		return quat ( scale1 * q1.x + scale2 * q2.x,
					  scale1 * q1.y + scale2 * q2.y,
					  scale1 * q1.z + scale2 * q2.z,
					  scale1 * q1.w + scale2 * q2.w );
	}

				// quaternions are nearly opposite, create a perpendicual quaternion and s;erp it
	scale1 = (float) sin ( ( 1.0f - t ) * M_PI * 0.5f );
	scale2 = (float) sin ( t * M_PI * 0.5f );

	return quat ( scale1 * q1.x + scale2 * ( -q2.y ),
				  scale1 * q1.y + scale2 * q2.x,
				  scale1 * q1.z + scale2 * ( -q2.w ),
				  scale1 * q1.w + scale2 * q2.w );
}

quat	squad ( const quat& q1, const quat& q2, const quat& a, const quat&b, float t )
{
	quat	p1 = slerp ( q1, q2, t );
	quat	p2 = slerp ( a,  b,  t );

	return slerp ( p1, p2, 2*t*(1 - t) );
}
