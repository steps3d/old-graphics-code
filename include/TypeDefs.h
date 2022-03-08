//
// Basic typedefs for data types
//

#pragma once
#ifndef	__TYPEDEFS__
#define	__TYPEDEFS__

#if defined(_MSC_VER)
	typedef __int8           int8_t;
	typedef unsigned __int8  uint8_t;
	typedef __int16          int16_t;
	typedef unsigned __int16 uint16_t;
	typedef __int32          int32_t;
	typedef unsigned __int32 uint32_t;
#else
	#include <stdint.h>
#endif

typedef	unsigned char		uint8;
typedef	signed	 char		int8;
typedef	unsigned short		uint16;
typedef	signed	 short		int16;
typedef	unsigned int		uint32;
typedef	signed	 int		int32;

#ifdef	_WIN32
typedef	unsigned __int64	uint64;
typedef	signed	 __int64	int64;
#else
typedef	unsigned long long	uint64;
typedef signed   long long	int64;
#endif

typedef	unsigned char	byte;
typedef	uint16			word;
typedef	uint32			dword;

#endif
