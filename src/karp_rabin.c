#include "karp_rabin.h"

/**
 * Recursive utility to compute fingerprints
 * in a divide-and-conquer fashion
 * 
 * @param [in] k number of bits
 */
void kpComputeFingerprints(KarpRabinParams * params, umat2 * kr, umat2 * kw, umat2 * kir, umat2 * kiw, const uint32 i)
{
	if (i < 16)
	{
		// Build up fingerprints
		for (uint32 x = 0; x < i; ++x)
			for (uint32 y = 0; y < i; ++y)
			{
				const uint32 idx = x * i + y;

				kw[idx].vector = kr[y].vector;
				umat2_dot(kw + idx, kr + x);
				umat2_mod(kw + idx, &params->z);

				kiw[idx].vector = kir[x].vector;
				umat2_dot(kiw + idx, kir + y);
				umat2_mod(kiw + idx, &params->z);
			}
		
		// Recursive call, invert read and write buffers
		kpComputeFingerprints(params, kw, kr, kiw, kir, i * i);
	}
	else
	{
		// Set fingerprints
		for (uint32 x = 0; x < i; ++x)
			for (uint32 y = 0; y < i; ++y)
			{
				const uint32 idx = x * i + y;

				// Compute fingerprint
				params->fp[idx].vector = kr[y].vector;
				umat2_dot(params->fp + idx, kr + x);
				umat2_mod(params->fp + idx, &params->z);

				params->fpi[idx].vector = kir[x].vector;
				umat2_dot(params->fpi + idx, kir + y);
				umat2_mod(params->fpi + idx, &params->z);
			}
	}
}

void kpInitParams(KarpRabinParams * params, uint32 p)
{
	// Set prime
	params->p = p;

	// Set modulo matrix
	params->z.vector = _mm_set1_epi32(p);

	// Partial fingerprints
	umat2 k[2][16] = {
		{
			{
				1, 0,
				1, 1
			},
			{
				1, 1,
				0, 1
			}
		}
	};

	// Partial inverse fingerprints
	umat2 ki[2][16] = {
		{
			{
				1, 0,
				p - 1, 1
			},
			{
				1, p - 1,
				0, 1
			}
		}
	};

	// Compute fingerprints
	kpComputeFingerprints(params, k[0], k[1], ki[0], ki[1], 2);
}

void kpGenerateFingerprint(umat2 * fp, const char * stream, uint32 len, const KarpRabinParams * params)
{
	// Reset fp with identity
	fp->vector = eye.vector;

	// Multiply each character fingerprint
	for (uint32 i = 0; i < len; ++i)
	{
		umat2_dot(fp, params->fp + stream[i]);
		umat2_mod(fp, &params->z);
	}
}

void kpIncrementFingerprint(umat2 * fp, const char * stream, uint32 len, const KarpRabinParams * params)
{
	umat2_dot_bab_z(params->fpi + *(stream - len), fp, params->fp + *stream, &params->z);
}