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
	const char text[] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Donec pellentesque posuere urna semper ornare. Phasellus in arcu mattis, rutrum risus in, scelerisque dui. Aliquam feugiat ut est non maximus. Morbi consequat accumsan mi a tincidunt. Curabitur mollis imperdiet augue, et bibendum turpis egestas at. Duis erat augue, condimentum at aliquam porttitor, faucibus laoreet lacus. Vivamus sit amet faucibus nunc. Sed nec erat ac elit ornare mollis. Etiam vel egestas nibh, in faucibus ipsum. Vestibulum faucibus sit amet lectus sed finibus. Nulla aliquet placerat elit et commodo. Aliquam erat volutpat. Aenean sagittis erat euismod felis condimentum molestie. Donec tincidunt est ipsum, at laoreet orci volutpat id. Nulla vel velit non dolor elementum vestibulum. Vestibulum sit amet nulla felis."
""
"Morbi dapibus scelerisque nulla eu varius. In rhoncus, est vel mollis finibus, metus est placerat sem, vel aliquet lectus metus eget lectus. Aliquam erat ligula, condimentum sit amet gravida sed, accumsan semper purus. Aliquam nec ipsum eget nulla commodo pretium nec sit amet est. Curabitur rutrum sollicitudin est vel blandit. Nunc tempus orci sed sollicitudin consequat. Cras lacinia finibus sollicitudin."
""
"Sed fringilla vel neque et blandit. Curabitur vehicula purus non dolor consectetur porta. Nunc semper ante mi, non porta orci pellentesque sed. Ut pellentesque cursus elit, quis condimentum dolor aliquam et. Vivamus mollis risus pharetra, faucibus lacus at, pharetra mi. Donec consectetur venenatis facilisis. Cras eget pulvinar dui, quis tempor est. Proin dignissim libero odio, ut pulvinar felis accumsan quis. Duis ac diam pulvinar, luctus nisi at, hendrerit sem. In sed pretium enim, eu cursus libero."
""
"Phasellus varius sit amet massa tincidunt congue. Nam porttitor, mi non faucibus scelerisque, diam ipsum congue diam, eu rhoncus risus sem bibendum lacus. Etiam dignissim metus risus, at accumsan sem elementum at. Pellentesque est lacus, accumsan vel dictum nec, fringilla nec eros. Ut vestibulum finibus pellentesque. Orci varius natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Morbi ultricies purus blandit, dapibus libero et, sodales urna."
""
"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nullam lacinia nunc vitae lacus ornare, eget scelerisque purus viverra. Nam laoreet lorem eget justo aliquet tincidunt a et enim. Pellentesque lacinia nulla ut orci cursus tempus. Aenean tincidunt risus ut mi feugiat, sed molestie ligula facilisis. In elit odio, volutpat ut sollicitudin vitae, gravida sit amet tortor. Nunc mollis faucibus sodales. Nam ut fermentum mauris. Curabitur id ipsum elementum, malesuada tellus sed, imperdiet quam. Duis luctus volutpat ornare. Fusce placerat metus eu semper imperdiet. Nullam accumsan felis risus, et ultrices ipsum interdum sed. Cras sit amet velit sapien. Mauris varius, lectus at molestie luctus, purus sem tincidunt metus, nec lacinia nulla turpis non neque.";

	// Get pattern
	const char * pattern = argv[1];
	const uint32 len = strlen(pattern);

	//////////////////////////////////////////////////
	// Init KP algorithm
	//////////////////////////////////////////////////

	// Init KP params
	uint32 patternLen = len;
	uint32 textLen = sizeof(text);
	KarpRabinParams params[k];
	for (uint32 i = 0; i < k; ++i)
	{
		// Generate a random prime
		uint32 p = getRandomPrime(1, patternLen * textLen * textLen);

		krInitParams(params + i, p);
	}

	//////////////////////////////////////////////////
	// Run KP
	//////////////////////////////////////////////////

	// Generate initial fingerprints
	umat2 pfp[k], tfp[k];
	ubyte match = 1;
	for (uint32 i = 0; i < k; ++i)
	{
		krGenerateFingerprint(pfp + i, pattern, len, params + i);
		krGenerateFingerprint(tfp + i, text, len, params + i);

		match &= umat2_cmpeq(pfp + i, tfp + i);
	}
	
	uint32 i = patternLen; for (; match == 0 && i < textLen - 1; ++i)
	{
		match = 1;
		for (uint32 j = 0; j < k; ++j)
		{
			krIncrementFingerprint(tfp + j, text + i, patternLen, params + j);
			match &= umat2_cmpeq(pfp + j, tfp + j);
		}
	}

	if (match)
		printf("match found at %u!\n", i - len);
	else
		printf("no match found\n");

	return 0;
}