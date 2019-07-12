#include "galil_seiferas.h"

#include <string.h>

/**
 * Galil-Seiferas decompose automaton states
 * @{
 */
void gsP1(GSParams*);
void gsP2(GSParams*);

void gsP1(GSParams * params)
{
	const char * pat = params->pattern + params->s;
	while (pat[params->q1] == pat[params->q1 + params->p1]) ++params->q1;

	if (params->p1 + params->q1 == params->k * params->p1)
	{
		params->p2 = params->q1;
		params->q2 = 0;

		gsP2(params);
	}
	else if (params->s + params->p1 + params->q1 < params->m)
	{
		params->p1 += params->q1 / params->k + 1;
		params->q1 = 0;		

		gsP1(params);
	}
}

void gsP2(GSParams * params)
{
	const char * pat = params->pattern + params->s;
	while (pat[params->q2] == pat[params->q2 + params->p2] && params->p2 + params->q2 < params->k * params->p2) ++params->q2;

	if (params->p2 + params->q2 == params->k * params->p2)
	{
		do
		{
			while (pat[params->q1] == pat[params->p1 + params->q1])
				++params->q1;
			
			for (; params->p1 + params->q1 >= params->k * params->p1; params->s += params->p1, params->q1 -= params->p1);

			params->p1 += params->q1 / params->k + 1;
			params->q1 = 0;			
		} while (params->p1 < params->p2);

		gsP1(params);
	}
	else if (params->s + params->p2 + params->q2 < params->m)
	{
		if (params->q2 == params->p1 + params->q1)
		{
			params->p2 += params->p1;
			params->q2 -= params->p1;
		}
		else
		{
			params->p2 += params->q2 / params->k + 1;
			params->q2 = 0;
		}

		gsP2(params);
	}
}
/// @}

/**
 * Decompose pattern for Galil-Seiferas algorithm
 * 
 * @param [in] params GS parameters
 */
void gsDecomposePattern(GSParams * params)
{
	gsP1(params);
}

void gsInitParams(GSParams * params, const char * inText, uint32 textLen, const char * inPattern, uint32 patternLen)
{
	// Get pattern and text lengths
	params->text = inText;
	params->pattern = inPattern;

	params->n = strlen(inText);
	params->m = strlen(inPattern);

	// Default GS constant k
	params->k = 2;

	// Initial values
	params->p1 = 1;
	params->p2 = params->q1 = params->q2 = params->s = 0;

	// Decompose pattern
	gsDecomposePattern(params);
}

int32 gsMatch(GSParams * params)
{
	uint32 p = 0, q = 0;
	const char * pat = params->pattern + params->s;
	const char * txt = params->text + params->s;

	while (p <= params->n - params->m)
	{
		while (params->s + p + q < params->n && pat[q] == txt[p + q]) ++q;

		if (q == params->m - params->s && memcmp(pat, txt + p, params->s + 1) == 0)
			return p;
			
		if (q == params->p1 + params->q1)
		{
			p += params->p1;
			q -= params->p1;
		}
		else
		{
			p += q / params->k + 1;
			q = 0;
		}
	}

	return -1;
}