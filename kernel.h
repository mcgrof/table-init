#include <stdio.h>

#define BUG() do {										\
	fprintf(stderr, "----------------------------------------------------------\n");	\
	fprintf (stderr, "BUG on %s at %s: %i\n", __func__, __FILE__, __LINE__);		\
}												\
while (0)
