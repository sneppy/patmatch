#pragma once

#include "core_types.h"

static inline int64 max(int64 a, int64 b)
{
	return a > b ? a : b;
}

/**
 * Pre-process pattern for Boyer-Moore algorithm
 * 
 * @param [in] pat pattern string
 * @param [in] patlen pattern string length
 * @param [in] delta1,delta2 BM parameters
 */
void bmPreprocess(ubyte *pat, int64 patlen, int64 *delta1, int64 *delta2);

/**
 * Run Boyer-Moore matching algorithm on text
 * 
 * @param [in] text text string
 * @param [in] textlen text string length
 * @param [in] pat pattern string
 * @param [in] patlen pattern string length
 */
int64 bmMatch(ubyte *text, int64 textlen, ubyte *pat, int64 patlen);