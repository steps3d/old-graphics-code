//
// Simple class to render screen quad
// Params are passed to vertex shader as a vec4 args ( width, height, sMax, tmax )
//

#pragma once
#ifndef	__SCREEN_QUAD__
#define	__SCREEN_QUAD__

#include	"vec4.h"
#include	"VertexArray.h"
#include	"VertexBuffer.h"

class	ScreenQuad
{
	int				width, height;
	float			sMax, tMax;
	vec4			args;				// to pass data to program "uniform vec4 args;"
	VertexArray		vao;
	VertexBuffer	buf;
	
public:
	ScreenQuad ( int theWidth, int theHeight, float theSMax, float theTMax );
	
	const vec4&	getArgs () const
	{
		return args;
	}
	
	void	render ();
	
protected:
	void	createBuffers ();
};

#endif
