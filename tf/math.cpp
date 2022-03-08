/****************************************
 * By Will Perone
 * Original: 17-09-2003   
 * Revised:  05-12-2003
 *           12-12-2003
 *
 * Special thanks to Cbloom, Graphics Gems, Braeden Shosa
 *
 * Notes: 
 *    e^(i*pi) + 1 = 0
 *    e^(i*z) = cos(z) + i*sin(z)
 *    cos(z) = ( e^(i*z) + e^(-i*z) ) / 2
 *    sin(z) = ( e^(i*z) - e^(-i*z) ) / (2*i)
 *    sum(n=0 to k)(z^n) = ( 1-z^(k+1) ) / (1-z)
 *    sine rule: a/sin(A) = b/sin(B) = c/sin(C) 
 *    cos(a)*cos(a) + sin(a)*sin(a) = 1
 *    sin(-t) = -sin(t) 
 *    cos(-t) = cos(t) 
 *    tan(-t) = -tan(t)
 *    sin(pi-t) = sin(t) 
 *    cos(pi-t) = -cos(t) 
 *    tan(pi-t) = -tan(t) 
 *    sin(s+t) = sin(s)*cos(t) + cos(s)*sin(t) 
 *    cos(s+t) = cos(s)*cos(t) - sin(s)*sin(t)
 *    sin(s-t) = sin(s)*cos(t) - cos(s)*sin(t) 
 *    cos(s-t) = cos(s)*cos(t) + sin(s)*sin(t)
 *    sin(2*t) = 2*sin(t)*cos(t) 
 *    cos(2*t) = cos(2*t) - sin(2*t) = 2*cos(2*t) - 1 = 1 - 2*sin(2*t)
 *    sin(t/2) = +sqrt((1 - cos(t)) / 2) 
 *    cos(t/2) = +sqrt((1 + cos(t)) / 2)
 *    sin(s) + sin(t) = 2 * sin((s+t)/2) * cos((s-t)/2)
 *    sin(s) - sin(t) = 2 * sin((s-t)/2) * cos((s+t)/2)
 *    cos(s) + cos(t) = 2 * cos((s+t)/2) * cos((s-t)/2)
 *    cos(s) - cos(t) = -2 * sin((s+t)/2) * sin((s-t)/2)
 *    sin(s)*cos(t) = ( sin(s+t) + sin(s-t) ) / 2
 *    cos(s)*cos(t) = ( cos(s+t) + cos(s-t) ) / 2
 *    sin(s)*sin(t) = ( cos(s-t) - cos(s+t) ) / 2
 ****************************************/

#pragma once 

#include "Assert.h" 

// swap a and b
template<class Type> inline void Swap(Type &a,Type &b)
{  Type c(a); a = b; b = c;  }

// clamp x to be within lo & hi range
#define CLAMP(v,l,h)	((v) < (l) ? (l) : (v) > (h) ? (h) : (v)) 
// minimum of a or b
#define MIN(a,b)    ( (a) < (b) ? (a) : (b) ) 
// maximum of a or b
#define MAX(a,b)    ( (a) > (b) ? (a) : (b) ) 
// absolute value of a 
#define ABS(a)		(((a) < 0) ? -(a) : (a)) 
// round a to nearest int 
#define ROUND(a)	((a) > 0 ? (int)((a)+0.5) : -(int)(0.5-(a))) 
// take sign of a, either -1, 0, or 1 
#define ZSGN(a)		(((a)<0) ? -1 : (a) > 0 ? 1 : 0)	 
// take binary sign of a, either -1, or 1 if >= 0 
#define SGN(a)		(((a)<0) ? -1 : 1) 


// cast float to it's bitwise representation
#define FLOAT_AS_INT(f)			(reinterpret_cast<const unsigned long &>(f)) 
#define INT_AS_FLOAT(i)			(reinterpret_cast<const float &>(i)) 

#define DBL_EPSILON     2.2204460492503131e-016 // smallest such that 1.0+DBL_EPSILON != 1.0  
#define DBL_MAX         1.7976931348623158e+308 // max value  
#define DBL_MIN         2.2250738585072014e-308 // min positive value  
#define FLT_EPSILON     1.192092896e-07F        // smallest such that 1.0+FLT_EPSILON != 1.0  
#define FLT_MAX         3.402823466e+38F        // max value  
#define FLT_MIN         1.175494351e-38F        // min positive value  


namespace Math
{
	// pi
	const double PI=       3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679;
	// pi^2
	const double PI_SQ=    PI*PI;
	// degrees to radians conversion
	const double DEG2RAD=  PI/180;
	// radians to degrees conversion
	const double RAD2DEG=  180/PI;
	// natural logarithmic base
    const double E=        2.7182818284590452353602874713526624977572470936999595749669676277240766303535475945713821785251664274;
	// euler constant
	const double GAMMA=    0.57721566490153286060651209008240243104215933593992359880576723488486772677766467093694706329174674;
	// golden ratio = (1+sqrt(5))/2
	const double PHI=      1.6180339887498948482045868343656381177203091798057628621354486227052604628189024497072072041893911374;
	// 1/log(2)
	const double inv_log2= 1.44269504088896340735992468100189213742664595415298593413544940693110921918118507988552662289350;
	// ln(10)
	const double LN10=     2.3025850929940456840179914546843642076011014886287729760333279009675726096773524802359972050895982983;
	// ln(pi)
	const double LNPI=     1.1447298858494001741434273513530587116472948129153115715136230714721377698848260797836232702754897077;	
	// lg(e)
	const double LOG2E=    1.44269504088896340736;
	// log(e)
	const double LOG10E=   0.434294481903251827651;
	// ln(2)
	const double LN2=      0.693147180559945309417;	
	// sqrt(2)
	const double SQRT2=    1.4142135623730950488016887242097;
	// sqrt(3)
	const double SQRT3=    1.7320508075688772935274463415059;


	//! evaluates a degree (arraysize-1) polynomial in the order x^n*array[0] + ... + array[arraysize-1]
	template <class type> 
	inline type HornerEval(const float x, const type *array, unsigned int arraysize)
	{			
	    type p = *array;
	    while (--arraysize) p= *++array + p*x;	
		return p;
	}


	//! linear interpolation of 2 points
	template <class type>
	inline interp_lin(const type &a, const type &b, float t)
	{	return a*(1-t) + b*t; 	}

	//! cosine interpolation of 2 points
	template <class type>
	inline interp_cos(const type &a, const type &b, float t)
	{   
		t = (1 - cos(t*PI))/2;
	    return  a*(1-t) + b*t;
	}

	//! returns the cubic interpolation of 4 points
	template <class type>
	inline interp_cubic(const type &v0, const type &v1, const type &v2, const type &v3, float t)
	{
		type P = (v3 - v2) - (v0 - v1);
		type Q = (v0 - v1) - P;	
		return ((P*t + Q)*t + (v2-v0))*t + v1; //using expanded out horner's rule
	}	


	//! retuns the angle to side a given an arbitrary triangle of sides of length a,b,c
	inline float cosine_law_sss(float a, float b, float c)
	{   return acosf((b*b + c*c - a*a)/(2*b*c));  }

	//! returns the length of the side across from angle in an arbitrary triangle
	inline float cosine_law_sas(float len1, float angle, float len2)
	{   return sqrtf(len1*len1 + len2*len2 - 2*len1*len2*cosf(angle));  }


	//! returns if a float is valid
	inline bool valid(const float f)
	{
		// assert on the freed memory values :
		// these *are* valid floats, but the chance of
		// getting one on purpose is very slim
		Assert(FLOAT_AS_INT(f) != 0xCDCDCDCD, "Probably incorrect float: 0xCDCDCDCD");
		Assert(FLOAT_AS_INT(f) != 0xDDDDDDDD, "Probably incorrect float: 0xDDDDDDDD");
		
		// this works because NAN always returns false on compares 
		return (f >= -FLT_MAX && f <= FLT_MAX);
	}

	//! performs a fast round on a float
	inline int round(const float f)
	{
		// Add a magical cookie to the float to transform its bits into its rounded integer representation
		// See http://www.d6.com/users/checker/pdfs/gdmfp.pdf		
		return (int)( (double)(f + 6755399441055744.L) );
	}

	// tests for floating point fuzzy equalities
	inline bool equal(const float f1,const float f2, const float epsilon= FLT_EPSILON)
	{	return f1 >= f2 - epsilon && f1 <= f2 + epsilon;	}

	inline bool lessthan(const float f1,const float f2, const float epsilon= FLT_EPSILON)
	{   return f1 + epsilon < f2;  }

	inline bool lessthanorequalto(const float f1,const float f2, const float epsilon= FLT_EPSILON)
	{   return f1 + epsilon <= f2;  }

	inline bool greaterthan(const float f1,const float f2, const float epsilon= FLT_EPSILON)
	{   return f1 - epsilon > f2;  }

	inline bool greaterthanorequalto(const float f1,const float f2, const float epsilon= FLT_EPSILON)
	{   return f1 - epsilon >= f2;  }
};




//////////////////////////////////////////////////////////////////////


/****************************************
 * Quaternion class
 * By Will Perone
 * Original: 12-09-2003  
 * Revised:  27-09-2003
 *           22-11-2003
 *           10-12-2003
 *           15-01-2004
 *           16-04-2004
 * 
 * Notes:  
 * if |q|=1 then q is a unit quaternion
 * if q=(0,v) then q is a pure quaternion 
 * if |q|=1 then q conjugate = q inverse
 * if |q|=1 then q= [cos(angle), u*sin(angle)] where u is a unit vector 
 * q and -q represent the same rotation 
 * q*q.conjugate = (q.length_squared, 0) 
 * ln(cos(theta),sin(theta)*v)= ln(e^(theta*v))= (0, theta*v)
 ****************************************/

#pragma once 

#include "matrix4.h" 
#include "assert.h" 


struct quaternion
{
	union {
		struct {
			float    s; //!< the real component
			vector3f v; //!< the imaginary components
		};
		struct { float elem[4]; }; //! the raw elements of the quaternion
	};


	//! constructors
	quaternion() {}
	quaternion(float real, float x, float y, float z): s(real), v(x,y,z) {}
	quaternion(float real, const vector3f &i): s(real), v(i) {}

	//! from 3 euler angles
	quaternion(float theta_z, float theta_y, float theta_x)
	{
		float cos_z_2 = cosf(0.5*theta_z);
		float cos_y_2 = cosf(0.5*theta_y);
		float cos_x_2 = cosf(0.5*theta_x);

		float sin_z_2 = sinf(0.5*theta_z);
		float sin_y_2 = sinf(0.5*theta_y);
		float sin_x_2 = sinf(0.5*theta_x);

		// and now compute quaternion
		s   = cos_z_2*cos_y_2*cos_x_2 + sin_z_2*sin_y_2*sin_x_2;
		v.x = cos_z_2*cos_y_2*sin_x_2 - sin_z_2*sin_y_2*cos_x_2;
		v.y = cos_z_2*sin_y_2*cos_x_2 + sin_z_2*cos_y_2*sin_x_2;
		v.z = sin_z_2*cos_y_2*cos_x_2 - cos_z_2*sin_y_2*sin_x_2;

	}
	
	//! from 3 euler angles 
	quaternion(const vector3f &angles)
	{	
		float cos_z_2 = cosf(0.5*angles.z);
		float cos_y_2 = cosf(0.5*angles.y);
		float cos_x_2 = cosf(0.5*angles.x);

		float sin_z_2 = sinf(0.5*angles.z);
		float sin_y_2 = sinf(0.5*angles.y);
		float sin_x_2 = sinf(0.5*angles.x);

		// and now compute quaternion
		s   = cos_z_2*cos_y_2*cos_x_2 + sin_z_2*sin_y_2*sin_x_2;
		v.x = cos_z_2*cos_y_2*sin_x_2 - sin_z_2*sin_y_2*cos_x_2;
		v.y = cos_z_2*sin_y_2*cos_x_2 + sin_z_2*cos_y_2*sin_x_2;
		v.z = sin_z_2*cos_y_2*cos_x_2 - cos_z_2*sin_y_2*sin_x_2;		
	} 
		
	//! basic operations
	quaternion &operator =(const quaternion &q)		
	{ s= q.s; v= q.v; return *this; }

	const quaternion operator +(const quaternion &q) const	
	{ return quaternion(s+q.s, v+q.v); }

	const quaternion operator -(const quaternion &q) const	
	{ return quaternion(s-q.s, v-q.v); }

	const quaternion operator *(const quaternion &q) const	
	{	return quaternion(s*q.s - v*q.v,
				  v.y*q.v.z - v.z*q.v.y + s*q.v.x + v.x*q.s,
				  v.z*q.v.x - v.x*q.v.z + s*q.v.y + v.y*q.s,
				  v.x*q.v.y - v.y*q.v.x + s*q.v.z + v.z*q.s);
	}

	const quaternion operator /(const quaternion &q) const	
	{
		quaternion p(q); 
		p.invert(); 
		return *this * p;
	}

	const quaternion operator *(float scale) const
	{ return quaternion(s*scale,v*scale); }

	const quaternion operator /(float scale) const
	{ return quaternion(s/scale,v/scale); }

	const quaternion operator -() const
	{ return quaternion(-s, -v); }
	
	const quaternion &operator +=(const quaternion &q)		
	{ v+=q.v; s+=q.s; return *this; }

	const quaternion &operator -=(const quaternion &q)		
	{ v-=q.v; s-=q.s; return *this; }

	const quaternion &operator *=(const quaternion &q)		
	{			
		s= s*q.s - v*q.v;
		
		float x= v.x, y= v.y, z= v.z;
		v.x= y*q.v.z - z*q.v.y + s*q.v.x + x*q.s;
		v.y= z*q.v.x - x*q.v.z + s*q.v.y + y*q.s;
		v.z= x*q.v.y - y*q.v.x + s*q.v.z + z*q.s;
		return *this;
	}
	
	const quaternion &operator *= (float scale)			
	{ v*=scale; s*=scale; return *this; }

	const quaternion &operator /= (float scale)			
	{ v/=scale; s/=scale; return *this; }
	

	//! gets the length of this quaternion
	float length() const
	{ return (float)sqrt(s*s + v*v); }

	//! gets the squared length of this quaternion
	float length_squared() const
	{ return (float)(s*s + v*v); }

	//! normalizes this quaternion
	void normalize()
	{ *this/=length(); }

	//! returns the normalized version of this quaternion
	quaternion normalized() const
	{ return  *this/length(); }

	//! computes the conjugate of this quaternion
	void conjugate()
	{ v=-v; }

	//! inverts this quaternion
	void invert()
	{ conjugate(); *this/=length_squared(); }
	
	//! returns the logarithm of a quaternion = v*a where q = [cos(a),v*sin(a)]
	quaternion log() const
	{
		float a = (float)acos(s);
		float sina = (float)sin(a);
		quaternion ret;

		ret.s = 0;
		if (sina > 0)
		{
			ret.v.x = a*v.x/sina;
			ret.v.y = a*v.y/sina;
			ret.v.z = a*v.z/sina;
		} else {
			ret.v.x= ret.v.y= ret.v.z= 0;
		}
		return ret;
	}

	//! returns e^quaternion = exp(v*a) = [cos(a),vsin(a)]
	quaternion exp() const
	{
		float a = (float)v.length();
		float sina = (float)sin(a);
		float cosa = (float)cos(a);
		quaternion ret;

		ret.s = cosa;
		if (a > 0)
		{
			ret.v.x = sina * v.x / a;
			ret.v.y = sina * v.y / a;
			ret.v.z = sina * v.z / a;
		} else {
			ret.v.x = ret.v.y = ret.v.z = 0;
		}
		return ret;
	}

	//! casting to a 4x4 isomorphic matrix for right multiplication with vector
	operator matrix4() const
	{			
		return matrix4(s,  -v.x, -v.y,-v.z,
				v.x,  s,  -v.z, v.y,
				v.y, v.z,    s,-v.x,
				v.z,-v.y,  v.x,   s);
	}
	
	//! casting to 3x3 rotation matrix
	operator matrix3() const
	{
		Assert(length() > 0.9999 && length() < 1.0001, "quaternion is not normalized");		
		return matrix3(1-2*(v.y*v.y+v.z*v.z), 2*(v.x*v.y-s*v.z),   2*(v.x*v.z+s*v.y),   
				2*(v.x*v.y+s*v.z),   1-2*(v.x*v.x+v.z*v.z), 2*(v.y*v.z-s*v.x),   
				2*(v.x*v.z-s*v.y),   2*(v.y*v.z+s*v.x),   1-2*(v.x*v.x+v.y*v.y));
	}

	//! computes the dot product of 2 quaternions
	static inline float dot(const quaternion &q1, const quaternion &q2) 
	{ return q1.v*q2.v + q1.s*q2.s; }

	//! linear quaternion interpolation
	static quaternion lerp(const quaternion &q1, const quaternion &q2, float t) 
	{ return (q1*(1-t) + q2*t).normalized(); }

	//! spherical linear interpolation
	static quaternion slerp(const quaternion &q1, const quaternion &q2, float t) 
	{
		quaternion q3;
		float dot = quaternion::dot(q1, q2);

		/*	dot = cos(theta)
			if (dot < 0), q1 and q2 are more than 90 degrees apart,
			so we can invert one to reduce spinning	*/
		if (dot < 0)
		{
			dot = -dot;
			q3 = -q2;
		} else q3 = q2;
		
		if (dot < 0.95f)
		{
			float angle = acosf(dot);
			return (q1*sinf(angle*(1-t)) + q3*sinf(angle*t))/sinf(angle);
		} else // if the angle is small, use linear interpolation								
			return lerp(q1,q3,t);		
	}

	//! This version of slerp, used by squad, does not check for theta > 90.
	static quaternion slerpNoInvert(const quaternion &q1, const quaternion &q2, float t) 
	{
		float dot = quaternion::dot(q1, q2);

		if (dot > -0.95f && dot < 0.95f)
		{
			float angle = acosf(dot);			
			return (q1*sinf(angle*(1-t)) + q2*sinf(angle*t))/sinf(angle);
		} else  // if the angle is small, use linear interpolation								
			return lerp(q1,q2,t);			
	}

	//! spherical cubic interpolation
	static quaternion squad(const quaternion &q1,const quaternion &q2,const quaternion &a,const quaternion &b,float t)
	{
		quaternion c= slerpNoInvert(q1,q2,t),
			       d= slerpNoInvert(a,b,t);		
		return slerpNoInvert(c,d,2*t*(1-t));
	}

	//! Shoemake-Bezier interpolation using De Castlejau algorithm
	static quaternion bezier(const quaternion &q1,const quaternion &q2,const quaternion &a,const quaternion &b,float t)
	{
		// level 1
		quaternion q11= slerpNoInvert(q1,a,t),
				q12= slerpNoInvert(a,b,t),
				q13= slerpNoInvert(b,q2,t);		
		// level 2 and 3
		return slerpNoInvert(slerpNoInvert(q11,q12,t), slerpNoInvert(q12,q13,t), t);
	}

	//! Given 3 quaternions, qn-1,qn and qn+1, calculate a control point to be used in spline interpolation
	static quaternion spline(const quaternion &qnm1,const quaternion &qn,const quaternion &qnp1)
	{
		quaternion qni(qn.s, -qn.v);	
		return qn * (( (qni*qnm1).log()+(qni*qnp1).log() )/-4).exp();
	}

	//! converts from a normalized axis - angle pair rotation to a quaternion
	static inline quaternion from_axis_angle(const vector3f &axis, float angle)
	{ return quaternion(cosf(angle/2), axis*sinf(angle/2)); }

	//! returns the axis and angle of this unit quaternion
	void to_axis_angle(vector3f &axis, float &angle) const
	{
		angle = acosf(s);

		// pre-compute to save time
		float sinf_theta_inv = 1.0/sinf(angle);

		// now the vector
		axis.x = v.x*sinf_theta_inv;
		axis.y = v.y*sinf_theta_inv;
		axis.z = v.z*sinf_theta_inv;

		// multiply by 2
		angle*=2;
	}

	//! rotates v by this quaternion (quaternion must be unit)
	vector3f rotate(const vector3f &v)
	{   
		quaternion V(0, v);
		quaternion conjugate(*this);
		conjugate.conjugate();
		return (*this * V * conjugate).v;
	}

	//! returns the euler angles from a rotation quaternion
	vector3f euler_angles() const
	{
		float sqw = s*s;    
		float sqx = v.x*v.x;    
		float sqy = v.y*v.y;    
		float sqz = v.z*v.z;    

		vector3f euler;
		euler.x = atan2f(2.f * (v.x*v.y + v.z*s), sqx - sqy - sqz + sqw);    		
		euler.y = asinf(-2.f * (v.x*v.z - v.y*s));
		euler.z = atan2f(2.f * (v.y*v.z + v.x*s), -sqx - sqy + sqz + sqw);    
		return euler;
	}
};

