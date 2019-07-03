#include <stdlib.h>
#include <stdio.h>

#include "galil_seiferas.h"

int main(int argc, char ** argv)
{
	const char pattern[] = "ipsum";
	const char text[] = "Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum.";

	GSParams params;
	gsInitParams(&params, pattern, text);
	int32 i = gsMatch(&params);

	printf("%d\n", i);

	return 0;
}