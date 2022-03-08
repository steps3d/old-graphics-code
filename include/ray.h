//
// Simple ray class
//

#pragma once
#ifndef	__RAY__
#define	__RAY__

#include	"vec3.h"

class	ray
{
	vec3	org;
	vec3	dir;		// should be normalized
public:
	ray ( const vec3& theOrigin, const vec3& theDir, bool normalizeDir = true );

	const vec3&	getOrigin () const
	{
		return org;
	}

	const	vec3&	getDir () const
	{
		return dir;
	}

	vec3	pointAt ( float t ) const
	{
		return org + t * dir;
	}
};

#edif
