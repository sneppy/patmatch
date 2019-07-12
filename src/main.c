#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "math.h"
#include "karp_rabin.h"

int32 krRun(const char * text, uint32 textLen, const char * pattern, uint32 patternLen)
{
	const uint32 k = KR_DEFAULT_FP_COUNT;

	KRParams krParams[k];
	for (uint32 i = 0; i < k; ++i)
	{
		// Generate fingerprint
		uint32 p = getRandomPrime(1, patternLen * textLen * textLen);
		krInitParams(krParams + i, p);
	}

	umat2 pfp[k], tfp[k];
	ubyte match = 1;

	for (uint32 i = 0; i < k; ++i)
	{
		krGenerateFingerprint(pfp + i, pattern, patternLen, krParams + i);
		krGenerateFingerprint(tfp + i, text, patternLen, krParams + i);

		match &= umat2_cmpeq(pfp + i, tfp + i);
	}

	uint32 i = patternLen; for (; match == 0 && i < textLen - 1; ++i)
	{
		match = 1;
		for (uint32 j = 0; j < k; ++j)
		{
			krIncrementFingerprint(tfp + j, text + i, patternLen, krParams + j);
			match &= umat2_cmpeq(pfp + j, tfp + j);
		}
	}

	return match ? i : -1;
}

int main(int argc, char ** argv)
{
	return 0;
}