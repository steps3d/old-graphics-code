//
// Camera class for OpenGL
//
// Author: Alexey V. Boreskov <steps3d@narod.ru>, <steps3d@gmail.com>
//

#pragma once
#ifndef __CAMERA__
#define __CAMERA__

#include	"vec3.h"
#include	"mat3.h"
#include	"mat4.h"
#include	"plane.h"

class	quat;

class   Camera
{
	mat4		proj;				// projection matrix for the camera (perspective projection)
	mat4		mv;					// model-view matrix (rotation and translation)
    vec3		pos;				// camera position
	vec3		rotation;			// yaw, pitch and roll angles in radians
	mat3		rot;				// rotation matrix corresponding to rotation andles
    bool		rightHanded;		// whether camera is righthanded
    bool		infinite;			// whether it uses zFar equal infinity
    float		fov;				// field of view angle (in degrees)
	float		zNear;				// near clipping z-value
	float		zFar;				// far clipping z-value
	int			width;				// view width in pixels
	int			height;				// view height in pixels
	float		aspect;				// aspect ratio of camera

public:
    Camera ( const vec3& p, float yaw, float pitch, float roll,
             float aFov = 60, float nearZ = 0.1, float farZ = 100, bool rHanded = true );

    Camera ( const vec3& p, const quat& orintation,
             float aFov = 60, float nearZ = 0.1, float farZ = 100, bool rHanded = true );

    Camera ( const Camera& camera );
	Camera ();

    const vec3&   getPos () const
    {
        return pos;
    }

    vec3	getViewDir () const
    {
		mat3	r ( rot );
		
        return r.transpose () * vec3 ( 0, 0, -1 );		// ? mb -1
    }

    vec3	getSideDir () const
    {
		mat3	r ( rot );
		
        return r.transpose () * vec3 ( 1, 0, 0 );		// ? mb -1
    }

    vec3	getUpDir () const
    {
		mat3	r ( rot );
		
        return r.transpose () * vec3 ( 0, 1, 0 );		// ? mb -1
    }

	bool	isRightHanded () const
	{
		return rightHanded;
	}

	float	getZNear () const
	{
		return zNear;
	}

	float	getZFar () const
	{
		return zFar;
	}

    float   getFov () const
    {
        return fov;
    }

	int	getWidth () const
	{
		return width;
	}

	int	getHeight () const
	{
		return height;
	}

	float	getAspect () const
	{
		return aspect;
	}

	const mat4& getProjection () const
	{
		return proj;
	}
	
	const mat4& getModelview () const
	{
		return mv;
	}
	
	quat	getOrientation () const;

/*
											// map vector from world space to camera space
	vec3	mapFromWorld ( const vec3& p ) const
	{
		vec3	tmp ( p - pos );

		return vec3 ( dot ( tmp, sideDir ), dot ( tmp, upDir ), - dot ( tmp, viewDir ) );
	}

											// map vector from camera space to world space
	vec3	mapToWorld ( const vec3& p ) const
	{
		return pos + p.x * sideDir + p.y * upDir + p.z * viewDir;
	}

											// map vector to screen space
	vec3	mapToScreen ( const vec3& p ) const
	{
		vec3	scr ( transf * ( p - pos ) );

		return scr /= scr.z;				// do perspective transform
	}
*/
											// move camera (absolute & relative)
    void    moveTo ( const vec3& newPos )
    {
        pos = newPos;

		computeMatrix ();
    }

    void    moveBy ( const vec3& delta )
    {
        pos += delta;

		computeMatrix ();					// XXX-since clipping planes must be rebuild
    }
											// set orientation either via Euler angles or
											// via quaternion
    void    setEulerAngles  ( float theYaw, float thePitch, float theRoll );
    void	setOrientation  ( const quat& orientation );

    										// set viewport parameters
	void	setViewSize     ( int theWidth, int theHeight, float theFov );
    void    setFov          ( float newFovAngle );
    void	setRightHanded  ( bool flag );

    										// transform camera
    void    mirror          ( const plane& );
//	void	transform       ( const Transform3D& );

											// return poly (quad) for intersection of plane paraller to zNear plane with given z
	void	getPlanePolyForZ ( float z, vec3 * poly ) const;
	
private:
    void    computeMatrix ();				// compute vectors, transform matrix and build
    										// viewing frustrum
};

#endif
