#pragma once

#include "core_types.h"

typedef struct MontgomeryParamsStruct
{
	// Extended radix (must be 2^32)
	uint64 r;

	// Modulo
	uint64 n;

	// Radix multiplicative inverse
	uint64 rinv;

	// Modulo multiplicative inverse
	uint64 ninv;
} MontgomeryParams;

/**
 * Initialize montgomery parameters, compute inverses
 * 
 * @param [out] params initialized parameters
 */
void montgomeryInitParams(MontgomeryParams * params, uint64 r, uint64 n);

/**
 * Convert from and to montgomery domain
 * @{
 */
static inline uint64 montgomeryFromU32(uint32 n, const MontgomeryParams * params)
{
	uint64 m = n;
	m *= params->r;
	m %= params->n;

	return m;
}

static inline uint32 montgomeryToU32(uint64 m, const MontgomeryParams * params)
{
	uint64 n = m;
	n *= params->rinv;
	n %= params->n;

	return (uint32)n;
}
/// @}

/**
 * Montgomery addition
 */
static inline uint64 montgomeryAdd(uint64 a, uint64 b, const MontgomeryParams * params)
{
	uint64 c = a + b;
	return c < params->n ? c : c - params->n;
}

/**
 * Montgomery multiplication
 * 
 * @param [in] a,b operands
 * @param [in] params montgomery parameters
 */
uint64 montgomeryMul(uint64 a, uint64 b, const MontgomeryParams * params);

/**
 * Montgometry exponentiation
 * 
 * @param [in] b base
 * @param [in] e exponent
 * @param [in] params montgomery parameters
 */
uint64 montgomeryExp(uint64 b, uint32 e, const MontgomeryParams * params);

/**
 * Square and multiply, non-recursive
 * 
 * @param [in] base base integer
 * @param [in] exp exponent integer
 * @param [in] z modulo
 */
uint32 squareAndMultiply(uint32 base, uint32 exp, uint32 z);