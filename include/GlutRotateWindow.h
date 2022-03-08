#ifndef	__GLUT_ROTATE_WINDOW__
#define	__GLUT_ROTATE_WINDOW__

#include "GlutWindow.h"
#include "glUtilities.h"
#include "mat4.h"

class GlutRotateWindow : public GlutWindow
{
protected:
	int	mouseOldX;
	int    	mouseOldY;
	vec3	rot;

public:
	GlutRotateWindow ( int xo, int yo, int w, int h, const char * caption ) : GlutWindow ( xo, yo, w, h, caption ) 
	{
		mouseOldX = 0;
		mouseOldY = 0;
		rot       = vec3 ( 0.0f );
	}

	mat4 getRotation () const
	{
		return mat4 :: rotateZ ( toRadians ( -rot.z ) ) * 
		    	mat4 :: rotateY ( toRadians ( rot.y ) ) *
			mat4 :: rotateX ( toRadians ( rot.x ) );
	}

    virtual void	mouseMotion ( int x, int y )
	{
		rot.x -= ((mouseOldY - y) * 180.0f) / 200.0f;
		rot.z -= ((mouseOldX - x) * 180.0f) / 200.0f;
		rot.y  = 0;

		if ( rot.z > 360 )
			rot.z -= 360;

		if ( rot.z < -360 )
			rot.z += 360;

		if ( rot.y > 360 )
			rot.y -= 360;

		if ( rot.y < -360 )
			rot.y += 360;

		mouseOldX = x;
		mouseOldY = y;

		glutPostRedisplay ();
	}
	
	virtual void	mouseClick ( int button, int state, int modifiers, int x, int y )
	{
		if ( state == GLUT_DOWN )
		{
			mouseOldX = x;
			mouseOldY = y;
		}
	}
};

#endif