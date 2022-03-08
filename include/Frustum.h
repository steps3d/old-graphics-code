//
// Frustum class for OpenGL
//
// Author: Alexey V. Boreskov <steps3d@narod.ru>, <steps3d@gmail.com>
//

#pragma once
#ifndef    __FRUSTUM__
#define    __FRUSTUM__

#include	"vec3.h"
#include	"plane.h"

class	bbox;
class	mat4;

class      Frustum
{
	plane	planes [6];
	
public:
	Frustum ();
	
	void	set ( const mat4& matrix );			// product of ModelView and Projection matrices
	
								// checks
	bool	pointInFrustum ( const vec3& v   ) const;
	bool	boxInFrustum   ( const bbox& box ) const;
};

#endif

