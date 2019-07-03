#pragma once

#include "core_types.h"

/**
 * Galil-Seiferas parameters struct (per-string)
 */
typedef struct GalilSeiferasParamsStruct
{
	/// Pattern string
	const char * pattern;

	/// Pattern string length
	uint32 n;

	/// Text string
	const char * text;

	/// Text string length
	uint32 m;

	/// Galil-Seiraf parameter (default: 4)
	uint32 k;

	/// Decomposition index [0, s - 1] + [s, n]
	uint32 s;
	
	/// Shortest prefix length
	uint32 p1;

	/// Shortest prefix period
	uint32 q1;

	/// Second shortest prefix length
	uint32 p2;

	/// Second shortest prefix period
	uint32 q2;
} GSParams;

/**
 * Init Galil-Seiferas parameters
 * 
 * @param [out] params GS parameters
 * @param [in] inPattern pattern string
 * @param [in] inText text string
 */
void gsInitParams(GSParams * params, const char * inPattern, const char * inText);

/**
 * Run matching algorithm
 * 
 * @param [in] params GS parameters
 */
int32 gsMatch(GSParams * params);