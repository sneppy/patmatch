#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "mat2.h"

umat2 fp[256] = {};
umat2 fp_inv[256] = {};

ubyte isPrime(uint32 p)
{
	if (p < 4) return 1;
	if (p & 0x1 == 0) return 0;
	
	uint32 s = p;
	while (s > 1)
	{
		do s >>= 1; while (s & 0x1);
		if (s > 0 && p / s * s == p) return 0;
	}

	return 1;
}

uint32 getRandomPrime(uint32 min, uint32 max)
{
	const uint32 len = max - min;
	uint32 p;

	do p = rand() % len + min; while (!isPrime(p));
	
	return p;
};

int main(int argc, char ** argv)
{
	srand(clock());

	uint32 p = getRandomPrime(128, 256);

	const umat2
				k[] = {{1, 0,
						1, 1},

					   {1, 1,
						0, 1}},
				y[] = {{1    , 0,
					    p - 1, 1},
					   {1, p - 1,
					    0, 1}},
				eye = {1, 0,
					   0, 1},
				mod = {p, p,
					   p, p};
	
	printf("using prime: %u\n", p);

	//////////////////////////////////////////////////
	// Build up lookup tables
	//////////////////////////////////////////////////
	
	for (uint32 i = 0, w = 0, w_inv = 0; i < 1u << 8; ++i)
	{
		fp[i].vector = eye.vector;
		fp_inv[i].vector = eye.vector;

		w = i, w_inv = i;
		for (uint32 j = 0; j < 8; ++j)
		{
			umat2_dot(fp + i, k + (w & 0x1));

			umat2_dot(fp_inv + i, y + (w_inv > 0x7f));
			umat2_mod(fp_inv + i, &mod);

			w >>= 1, w_inv = (w_inv << 1) & 0xff;
		}
	}

#if 0
	for (uint32 i = 0; i < 1u << 8; ++i)
	{
		printf("fp    [%u] = {%u, %u, %u, %u};\n", i, fp[i].a, fp[i].b, fp[i].c, fp[i].d);
		printf("fp_inv[%u] = {%u, %u, %u, %u};\n", i, fp_inv[i].a, fp_inv[i].b, fp_inv[i].c, fp_inv[i].d);
	}
#endif

	//////////////////////////////////////////////////
	// Init data
	//////////////////////////////////////////////////

	const char text[] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Ut malesuada porta quam eu interdum. Curabitur vitae consectetur erat, ac placerat metus. Pellentesque sollicitudin, sapien non dapibus tempus, purus sapien pretium neque, ac faucibus erat dolor a enim. Praesent malesuada porttitor risus, eget dignissim dui pulvinar sed. Vestibulum ultrices feugiat dolor, et scelerisque erat viverra eget. Quisque rhoncus leo eget metus placerat pretium. Proin pharetra justo venenatis massa lobortis, a auctor urna venenatis.";
	
	typedef struct String
	{
		/// String length
		size_t len;

		/// Buffer size
		size_t size;

		/// String buffer
		char * buffer;
	} string;

	// Read pattern
	string pattern = {0, 8, malloc(8)};
	char c; while ((c = getc(stdin)) != '\n' && c != '\r')
	{
		if (++pattern.len > pattern.size)
			pattern.buffer = realloc(pattern.buffer, pattern.size *= 2);
		
		pattern.buffer[pattern.len - 1] = c;
	}

	if (pattern.len + 1 > pattern.size)
		pattern.buffer = realloc(pattern.buffer, pattern.size *= 2);
	
	pattern.buffer[pattern.len] = '\0';

	//////////////////////////////////////////////////
	// Run algorithm
	//////////////////////////////////////////////////

	// Create fingerprints
	umat2 text_fp = {.vector = eye.vector}, pattern_fp = {.vector = eye.vector};
	for (uint32 i = 0; i < pattern.len; ++i)
	{
		umat2_dot(&text_fp, fp + text[i]);
		umat2_mod(&text_fp, &mod);

		umat2_dot(&pattern_fp, fp + pattern.buffer[i]);
		umat2_mod(&pattern_fp, &mod);
	}

	ubyte match = umat2_cmpeq(&text_fp, &pattern_fp) == 0xffff;
	for (uint32 i = pattern.len; match == 0 && i < sizeof(text) - 1; ++i)
	{
		// Next character
		umat2_dot_bac_z(fp_inv + text[i - pattern.len], &text_fp, fp + text[i], &mod);
		match = umat2_cmpeq(&text_fp, &pattern_fp) == 0xffff;
	}

	printf("%s\n", match ? "Match found!" : "Match not found");

	return 0;
}