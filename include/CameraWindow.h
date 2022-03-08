//
// Window controlled by the Camera
//

#include	"GlutWindow.h"
#include	"Camera.h"

class CameraWindow : public GlutWindow
{
protected:
	Camera	camera;
	float	yaw, pitch, roll;
	float	mouseSensitivity;

public:
	CameraWindow ( const vec3& eye, int x, int y, int w, int h, const char * t ) : GlutWindow ( x, y, w, h, t ), camera ( eye, 0, 0, 0 )
	{
		camera.setRightHanded ( false );

		yaw              = 0;
		pitch            = 0;
		roll             = 0;
		mouseSensitivity = 0.2;
	}

	void reshape ( int w, int h )
	{
		GlutWindow::reshape ( w, h );
		
		camera.setViewSize ( w, h, 60 );
	}

    void	keyTyped ( unsigned char key, int modifiers, int x, int y )
	{
		if ( key == 27 || key == 'q' || key == 'Q' )	//	quit requested
			exit ( 0 );
			
		if ( key == 'w' || key == 'W' )
			camera.moveBy ( camera.getViewDir () * mouseSensitivity );
		else
		if ( key == 'x' || key == 'X' )
			camera.moveBy ( -camera.getViewDir () * mouseSensitivity );
		else
		if ( key == 'a' || key == 'A' )
			camera.moveBy ( -camera.getSideDir () * mouseSensitivity );
		else
		if ( key == 'd' || key == 'D' )
			camera.moveBy ( camera.getSideDir () * mouseSensitivity );
	}

     void	specialKey ( int key, int modifier, int x, int y )
	{
		if ( key == GLUT_KEY_UP )
			pitch += M_PI / 90;
		else
		if ( key == GLUT_KEY_DOWN )
			pitch -= M_PI / 90;
		else
		if ( key == GLUT_KEY_RIGHT )
			yaw -= M_PI / 90;
		else
		if ( key == GLUT_KEY_LEFT )
			yaw += M_PI / 90;

		camera.setEulerAngles ( yaw, pitch, roll );

		glutPostRedisplay ();
	}

	void	mousePassiveMotion ( int x, int y )
	{
		static	int	lastX = -1;
		static	int	lastY = -1;

		if ( lastX == -1 )				// not initialized
		{
			lastX = x;
			lastY = y;
		}

		yaw   -= (x - lastX) * 0.01;
		pitch -= (y - lastY) * 0.01;
		lastX  = x;
		lastY  = y;

		camera.setEulerAngles ( yaw, pitch, roll );

		glutPostRedisplay ();
	}

    void mouseWheel ( int wheel, int dir, int x, int y )
	{
		vec3 eye = camera.getPos ();
		
		eye += 0.5 * dir * vec3 ( eye ).normalize ();

		camera.moveTo ( eye );
		
		reshape ( glutGet ( GLUT_WINDOW_WIDTH ), glutGet ( GLUT_WINDOW_HEIGHT ) );
	
		glutPostRedisplay ();
	}
};
