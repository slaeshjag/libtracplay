#include <stdlib.h>
#include <stdio.h>

#include "tracplay.h"

int main(int argc, char **argv) {
	if (modLoad("player/aspartame.mod") == NULL)
		fprintf(stderr, "Unable to open\n");

	return 0;
}
