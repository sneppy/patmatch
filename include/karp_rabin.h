#pragma once

#include "core_types.h"
#include "math.h"

/**
 * Karp-Rabin parameters struct
 */
typedef struct KRParamsStruct
{
	/// Prime number bound to this instance
	uint32 p;
	
	/// Montgomery params
	MontgomeryParams mp;

	/// Fingerprints for char type
	umat2 fp[256];

	/// Inverse fingerprints
	umat2 fpi[256];
} KRParams;

/// Default number of fingerprint
#define KR_DEFAULT_FP_COUNT 16

/**
 * Initialize parameters with prime number
 * 
 * @param [out] params initialized parameters
 * @parma [in] p prime number
 */
void krInitParams(KRParams * params, uint32 p);

/**
 * Generate new fingerprint from stream
 * 
 * @param [out] fp out fingerprint
 * @param [in] stream character stream
 * @param [in] len pattern length
 * @param [in] params KP parameters
 */
void krGenerateFingerprint(umat2 * fp, const char * stream, uint32 len, const KRParams * params);

/**
 * Increment fingerprint
 * 
 * @param [out] fp curret fingerprint
 * @param [in] stream character stream
 * @param [in] len pattern length
 * @param [in] params KP parameters
 */
void krIncrementFingerprint(umat2 * fp, const char * stream, uint32 len, const KRParams * params);