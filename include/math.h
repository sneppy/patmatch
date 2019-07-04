#pragma once

#include "core_types.h"
#include "mat2.h"

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
static inline uint64 montgomeryFromU32(uint32 n, MontgomeryParams * params)
{
	uint64 m = n;
	m *= params->r;
	m %= params->n;

	return m;
}

static inline uint32 montgomeryToU32(uint64 m, MontgomeryParams * params)
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
static inline uint64 montgomeryAdd(uint64 a, uint64 b, MontgomeryParams * params)
{
	uint64 c = a + b;
	return c > params->n ? c - params->n : c;
}

/**
 * Montgomery multiplication
 * 
 * @param [in] a,b operands
 * @param [in] params montgomery parameters
 */
uint64 montgomeryMul(uint64 a, uint64 b, MontgomeryParams * params);

/**
 * Montgometry exponentiation
 * 
 * @param [in] b base
 * @param [in] e exponent
 * @param [in] params montgomery parameters
 */
uint64 montgomeryExp(uint64 b, uint32 e, MontgomeryParams * params);

/**
 * Square and multiply, non-recursive
 * 
 * @param [in] base base integer
 * @param [in] exp exponent integer
 * @param [in] z modulo
 */
uint32 squareAndMultiply(uint32 base, uint32 exp, uint32 z);

/**
 * Returns true if number is prime
 * 
 * @param [in] number
 * @return true if is prime
 */
ubyte isPrime(uint32 n);

/**
 * Generate a random prime in the
 * range [a,b)
 * 
 * @param [in] a,b acceptable range
 */
uint32 getRandomPrime(uint32 a, uint32 b);