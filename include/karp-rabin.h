#pragma once

#include "core_types.h"
#include "math.h"

/**
 * Karp-Rabin parameters struct
 */
typedef struct KarpRabinParamsStruct
{
	/// Prime number bound to this instance
	uint32 p;

	/// Modulo matrix
	umat2 z;

	/// Fingerprints for char type
	umat2 fp[256];

	/// Inverse fingerprints
	umat2 fpi[256];
} KarpRabinParams;

/**
 * Initialize parameters with prime number
 * 
 * @param [out] params initialized parameters
 * @parma [in] p prime number
 */
void karpRabinInitParams(KarpRabinParams * params, uint32 p);

/**
 * Generate new fingerprint from stream
 * 
 * @param [out] fp out fingerprint
 * @param [in] stream character stream
 * @param [in] len pattern length
 * @param [in] params KP parameters
 */
void karpRabinGenerateFingerprint(umat2 * fp, const char * stream, uint32 len, const KarpRabinParams * params);

/**
 * Increment fingerprint
 * 
 * @param [out] fp curret fingerprint
 * @param [in] stream character stream
 * @param [in] len pattern length
 * @param [in] params KP parameters
 */
void karpRabinIncrementFingerprint(umat2 * fp, const char * stream, uint32 len, const KarpRabinParams * params);