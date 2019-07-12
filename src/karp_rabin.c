#include "karp_rabin.h"

/**
 * Recursive utility to compute fingerprints
 * in a divide-and-conquer fashion
 * 
 * @param [in] k number of bits
 */
void krComputeFingerprints(const KRParams * params, umat2 * kr, umat2 * kw, umat2 * kir, umat2 * kiw, const uint32 i)
{
	if (i < 16)
	{
		// Build up fingerprints
		for (uint32 x = 0; x < i; ++x)
			for (uint32 y = 0; y < i; ++y)
			{
				const uint32 idx = x * i + y;

				memcpy(kw + idx, kr + y, sizeof(*kr));
				umat2_dot(kw + idx, kr + x, &params->mp);

				memcpy(kiw + idx, kir + x, sizeof(*kir));
				umat2_dot(kiw + idx, kir + y, &params->mp);
			}
		
		// Recursive call, invert read and write buffers
		krComputeFingerprints(params, kw, kr, kiw, kir, i * i);
	}
	else
	{
		// Set fingerprints
		for (uint32 x = 0; x < i; ++x)
			for (uint32 y = 0; y < i; ++y)
			{
				const uint32 idx = x * i + y;

				// Compute fingerprint
				memcpy(params->fp + idx, kr + y, sizeof(*kr));
				umat2_dot(params->fp + idx, kr + x, &params->mp);

				memcpy(params->fpi + idx, kir + x, sizeof(*kir));
				umat2_dot(params->fpi + idx, kir + y, &params->mp);
			}
	}
}

void krInitParams(KRParams * params, uint32 p)
{
	// Set prime
	params->p = p;
	
	// Set modulo in montgomery parameters
	montgomeryInitParams(&params->mp, 1u << 31, p);

	const uint64 one = montgomeryFromU32(1u, &params->mp);
	const uint64 zero = montgomeryFromU32(0u, &params->mp);
	const uint64 mone = montgomeryFromU32(p - 1u, &params->mp);

	// Partial fingerprints
	umat2 k[2][16] = {
		{
			{
				one, zero,
				one, one
			},
			{
				one, one,
				zero, one
			}
		}
	};

	// Partial inverse fingerprints
	umat2 ki[2][16] = {
		{
			{
				one, zero,
				mone, one
			},
			{
				one, mone,
				zero, one
			}
		}
	};

	// Compute fingerprints
	krComputeFingerprints(params, k[0], k[1], ki[0], ki[1], 2);
}

void krGenerateFingerprint(umat2 * fp, const char * stream, uint32 len, const KRParams * params)
{
	// Multiply each character fingerprint
	memcpy(fp, params->fp + stream[0], sizeof(*params->fp));
	for (uint32 i = 1; i < len; ++i)
		umat2_dot(fp, params->fp + stream[i], &params->mp);
}

void krIncrementFingerprint(umat2 * fp, const char * stream, uint32 len, const KRParams * params)
{
	umat2_dot_bab(params->fpi + *(stream - len), fp, params->fp + *stream, &params->mp);
}