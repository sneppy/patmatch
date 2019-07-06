#include "math.h"

const umat2 eye = {1, 0, 0, 1};

void montgomeryInitParams(MontgomeryParams * params, uint64 r, uint64 n)
{
	params->r = r << 1;
	params->n = n;

	// Euclidean algorithm
	// Assumes r power of two
	uint64 a = r, b = n;
	uint64 u = 1, v = 0;

	while (a > 0)
	{
		a >>= 1;

		if ((u & 0x1) == 0)
		{
			u >>= 1;
			v >>= 1;
		}
		else
		{
			u = ((u ^ b) >> 1) + (u & b);
			v = (v >> 1) + r;
		}
	}

	params->rinv = u;
	params->ninv = v;
}

uint64 montgomeryMul(uint64 a, uint64 b, const MontgomeryParams * params)
{
	// Assume radix is 2^32
	if (a == 0 || b == 0) return 0;

	uint64 c = 0;
	for (uint8 i = 0; i < 32; ++i, a >>= 1)
	{
		c += (a & 0x1) * b;
		ubyte q = params->ninv & c & 0x1;
		c = (c + q * params->n) >> 1;
	}

	return c < params->n ? c : c - params->n;
}

uint64 montgomeryExp(uint64 b, uint32 e, const MontgomeryParams * params)
{
	uint64 c = montgomeryFromU32(1u, params);

	while (e > 0)
	{
		if (e & 0x1) c = montgomeryMul(c, b, params);

		b = montgomeryMul(b, b, params);
		e >>= 1;
	}

	return c;
}

uint32 squareAndMultiply(uint32 base, uint32 exp, uint32 z)
{
	MontgomeryParams mp;
	montgomeryInitParams(&mp, 1u << 31, z);

	uint64 b = montgomeryFromU32(base, &mp);
	uint64 c = montgomeryExp(b, exp, &mp);

	return montgomeryToU32(c, &mp);
}

ubyte isPrime(uint32 n)
{
	if ((n & 0x1) == 0) return 0;

	// Miller-Rabin test, 1 - (1/4)^k accuracy
	const uint32 k = 32;

	// Decompose n
	uint32 p = n - 1;
	uint32 s = 0;

	if ((p & 0xffff) == 0) s += 16, p >>= 16;
	if ((p & 0xff) == 0) s += 8, p >>= 8;
	if ((p & 0xf) == 0) s += 4, p >>= 4;
	if ((p & 0x3) == 0) s += 2, p >>= 2;
	if ((p & 0x1) == 0) s += 1, p >>= 1;

	// p - 1 = d * (2 ^ s)
	uint32 d = p;

	// Generate a random sequence of numbers
	MontgomeryParams mp;
	montgomeryInitParams(&mp, 1u << 31, n);
	uint64 r = montgomeryFromU32(2u, &mp);
	uint64 a = r;
	uint64 u = montgomeryFromU32(1u, &mp), v = montgomeryFromU32(n - 1, &mp);

	for (uint32 i = 0; i < k; ++i)
	{
		// Base to test
		//a = montgomeryAdd(a, r, &mp);
		a = rand() % (n - 2) + 2;
		
		// Test
		uint64 t = montgomeryExp(a, d, &mp);
		if (t != u && t != v)
		{
			uint32 j; for (j = 1; j < s; ++j)
			{
				t = montgomeryMul(t, t, &mp);
				if (t == v) break;
			}

			if (j == s) return 0;
		}
	}

	return 1;
}

uint32 getRandomPrime(uint32 a, uint32 b)
{
	const uint32 range = b - a;

	for (;;)
	{
		uint32 p = (rand() % range) + a;
		if (isPrime(p)) return p;
	}
}