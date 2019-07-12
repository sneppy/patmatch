#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "math.h"
#include "boyer_moore.h"
#include "karp_rabin.h"
#include "galil_seiferas.h"

int32 bmRun(const char * text, uint32 textLen, const char * pattern, uint32 patternLen)
{
	bmMatch((const ubyte*)text, textLen, (const ubyte*)pattern, patternLen);
}

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

	return match ? i - patternLen : -1;
}

int32 gsRun(const char * text, uint32 textLen, const char * pattern, uint32 patternLen)
{
	GSParams gsParams;
	gsInitParams(&gsParams, text, textLen, pattern, patternLen);

	return gsMatch(&gsParams);
}

int main(int argc, char ** argv)
{
	const char text[] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Cras finibus, enim a tincidunt porttitor, ante tellus rhoncus felis, ac mollis libero magna eget orci. Etiam posuere sapien accumsan leo tempor, quis euismod metus tincidunt. Orci varius natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Curabitur id feugiat diam, vitae lacinia ante. Proin condimentum pellentesque felis. Proin ultricies eu erat ornare eleifend. Donec nec cursus mi. Sed molestie nisl quis massa bibendum posuere.";
	const char pattern[] = "tincidunt porttitor";

	printf("Boyer-Moore: %d\n", bmRun(text, sizeof(text) - 1, pattern, sizeof(pattern) - 1));
	printf("Karp-Rabin: %d\n", krRun(text, sizeof(text) - 1, pattern, sizeof(pattern) - 1));
	printf("Galil-Seiferas: %d\n", gsRun(text, sizeof(text) - 1, pattern, sizeof(pattern) - 1));

	return 0;
}
