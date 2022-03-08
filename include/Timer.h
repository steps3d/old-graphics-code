#pragma once

#ifdef	_WIN32
#include <windows.h>

class Timer
{
private:
	double		invFreq;
	LONGLONG	start;
	 
public:
	Timer ()
	{
		LARGE_INTEGER     i1, i2;
		
		QueryPerformanceFrequency ( &i1 );
		QueryPerformanceCounter   ( &i2 );
		
		invFreq  = 1.0 / i1.QuadPart;
		start    = i.QuadPart;
	}

	inline double get () const			// get time elapsed since c-tor/reset
	{
		LARGE_INTEGER     i;
		
		QueryPerformanceCounter ( &i );

		return (i.QuadPart - start) * invFreq;
	}
	
	inline double reset ()				// reset start time, return elapsed since last reset
	{
		LARGE_INTEGER     i;
		
		QueryPerformanceCounter ( &i );
		
		double delta = (i.QuadPart - start) * invFreq;
		
		start = i.QuadPart;

		return delta;
	}
};

#else

#include <ctime>

class	Timer
{
    timespec start;

public:
	Timer ()
	{
		reset ();
	}
	
    inline double reset ()
	{
		timespec t;
		
		clock_gettime ( CLOCK_REALTIME, &t );
		
		double delta = (double)(t.tv_sec - start.tv_sec) + 
		       (t.tv_nsec - start.tv_nsec) / 1000000000.0;
	
		start = t;
		
		return delta;
    }

	inline double get () const
	{
		timespec t;
		
		clock_gettime ( CLOCK_REALTIME, &t );
		
		return (double)(t.tv_sec - start.tv_sec) + 
		       (t.tv_nsec - start.tv_nsec) / 1000000000.0;
	}
};

#endif
