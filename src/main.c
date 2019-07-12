#include <stdlib.h>
#include <stdio.h>

#include "galil_seiferas.h"

int32 gsRun(const char * text, uint32 textLen, const char * pattern, uint32 patternLen)
{
	GSParams gsParams;
	gsInitParams(&gsParams, text, textLen, pattern, patternLen);

	return gsMatch(&gsParams);
}

int main(int argc, char ** argv)
{
	return 0;
}