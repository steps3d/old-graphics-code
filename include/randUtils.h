//
// Simple random generators
//

#pragma once
#ifndef	__RAND_UTILS__
#define	__RAND_UTILS__

#include "vec3.h"

float	randUniform  ();
float	randUniform  ( float a, float b );
float	randGauss    ( float m, float sigma );
float	randPoisson  ( float lambda );
vec3	randOnSphere ();
vec3	randInTraingle ( const vec3& a, const vec3& b, const vec3& c );
#endif
