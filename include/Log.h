//
// Basic log class
//
// Author: Alexey V. Boreskov, <steps3d@gmail.com>, <steps3d@narod.ru>
//

#pragma once
#include	<string>
#include	<stdlib.h>
#include	<stdio.h>
#include	"vec2.h"
#include	"vec3.h"
#include	"vec4.h"

#ifndef	_CVTBUFSIZE
	#define	_CVTBUFSIZE	80
#endif

using namespace std;

class	Log
{
	string	str;
	string	logName;

public:
	Log  ( const char * logFileName );
	~Log ();

	Log& append ( const char * s )
	{
		str += s;

		return *this;
	}

	Log& append ( const string& s )
	{
		str += s;

		return *this;
	}

	Log& flush ();

	class	endl
	{
	};

};

inline	Log& operator << ( Log& log, Log::endl )
{
	return log.flush ();
}

inline	Log& operator << ( Log& log, const char * str )
{
	return log.append ( str );
}

inline	Log& operator << ( Log& log, const string& str )
{
	return log.append ( str );
}

inline	Log& operator << ( Log& log, int n )
{
	char	buf [32];

#ifdef	_WIN32
	itoa ( n, buf, 10 );
#else
	sprintf ( buf, "%d", n );
#endif

	return log.append ( buf );
}


inline Log& operator << ( Log& log, float f )
{
	char	buf [_CVTBUFSIZE];

	return log.append ( gcvt ( f, 12, buf ) );
}

inline Log& operator << ( Log& log, double f )
{
	char	buf [_CVTBUFSIZE];

	return log.append ( gcvt ( f, 12, buf ) );
}

inline Log& operator << ( Log& log, const vec2& v )
{
	return log << "( " << v.x << ", " << v.y << ") ";
}

inline Log& operator << ( Log& log, const vec3& v )
{
	return log << "( " << v.x << ", " << v.y << ", " << v.z << ") ";
}

inline Log& operator << ( Log& log, const vec4& v )
{
	return log << "( " << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ") ";
}
