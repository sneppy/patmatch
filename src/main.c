#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "math.h"
#include "karp_rabin.h"

int main(int argc, char ** argv)
{
	srand(clock());

	const uint32 k = 16;

	//////////////////////////////////////////////////
	// Init data
	//////////////////////////////////////////////////

	if (argc < 2)
	{
		fprintf(stderr, "usage: patmatch pattern\n");
		return 1;
	}

	// TODO: Read input text
	const char text[] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Praesent elit nulla, dapibus in sapien at, molestie iaculis magna. Fusce quis magna mauris. Mauris felis nulla, sollicitudin in luctus mattis, varius vel quam. Nunc at dui at risus dictum malesuada. Cras sit amet iaculis purus. Sed imperdiet eros sit amet turpis euismod ultricies. Mauris a orci at orci venenatis gravida. Nullam porta cursus aliquam. Aenean venenatis et nisl in blandit. Vestibulum eleifend eros ligula, id ullamcorper nisi ornare metus.";

	// Get pattern
	const char * pattern = argv[1];
	const uint32 len = strlen(pattern);

	//////////////////////////////////////////////////
	// Init KP algorithm
	//////////////////////////////////////////////////


	// Init KP params
	KarpRabinParams params[k];
	for (uint32 i = 0; i < k; ++i)
	{
		// Generate a random prime
		uint32 p = getRandomPrime(1, len * (sizeof(text) - len));

		kpInitParams(params + i, p);
	}

	//////////////////////////////////////////////////
	// Run KP
	//////////////////////////////////////////////////

	// Generate initial fingerprints
	umat2 pfp[k], tfp[k];
	ubyte match = 1;
	for (uint32 i = 0; i < k; ++i)
	{
		kpGenerateFingerprint(pfp + i, pattern, len, params + i);
		kpGenerateFingerprint(tfp + i, text, len, params + i);

		match &= umat2_cmpeq(pfp + i, tfp + i) == 0xffff;
	}
	
	uint32 i = len; for (; !match && i < sizeof(text) - 1; ++i)
	{
		match = 1;
		for (uint32 j = 0; j < k; ++j)
		{
			kpIncrementFingerprint(tfp + j, text + i, len, params + j);
			match &= umat2_cmpeq(pfp + j, tfp + j) == 0xffff;
		}
	}

	if (match)
		printf("match found at %u!\n", i - len);
	else
		printf("no match found\n");

	return 0;
}