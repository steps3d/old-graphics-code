//
// Wrapper for OpenGL pipeline object
//
// Author: Alexey V. Boreskov, <steps3d@gmail.com>, <steps3d@narod.ru>
//

#pragma once

#include	"Program.h"

class	Pipeline
{
	GLuint	id;

	Pipeline ( const Pipeline& );
	Pipeline& operator = ( const Pipeline& );
public:

	Pipeline  ();
	~Pipeline ();

	GLuint	getId () const
	{
		return id;
	}
	
	bool	isOk () const
	{
		return id != 0;
	}

	string getLog () const;

	bool	create  ();
	void	destroy ();
	bool	bind    ();
	bool	unbind  ();

	bool	setStage         ( GLbitfield mask, Program * );
	bool	setAllStages     ( Program * );
	bool	setActiveProgram ( Program * );
	bool	validate ();

	static int active ();		// id of active pipeline object
};

