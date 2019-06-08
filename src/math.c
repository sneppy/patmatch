#include "math.h"

const umat2 eye = {1, 0, 0, 1};

ubyte isPrime(uint32 n)
{
	if (n < 4)			return 1;
	if ((n & 0x1) == 0)	return 0;

	uint32 d = n >> 1; d = d & 0x1 ? d : d - 1;
	do
	{
		if ((n / d) * d == n) return 0;
		d -= 2;
	} while (d > 2);

	return 1;
}

ubyte getRandomPrime(uint32 a, uint32 b)
{
	const uint32 range = b - a;

	for (;;)
	{
		uint32 p = (rand() % range) + a;
		if (isPrime(p)) return p;
	}
}