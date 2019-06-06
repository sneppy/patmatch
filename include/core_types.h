#pragma once

//////////////////////////////////////////////////
// Basic types
//////////////////////////////////////////////////

typedef unsigned char		uint8;
typedef unsigned short		uint16;
typedef unsigned int		uint32;
typedef unsigned long long	uint64;

typedef signed char		byte;
typedef unsigned char	ubyte;

typedef float		float32;
typedef double		float64;
typedef long double	float128;

//////////////////////////////////////////////////
// Compiler definitions
//////////////////////////////////////////////////

#ifndef BUILD_DEBUG
	#define BUILD_DEBUG 0
#endif
#ifndef BUILD_DEVELOPMENT
	#define BUILD_DEVELOPMENT 0
#endif
#ifndef BUILD_RELEASE
	#define BUILD_RELEASE 0
#endif

#if BUILD_DEBUG
	#define FORCE_INLINE inline
#else
	#define FORCE_INLINE __attribute__((always_inline))
#endif