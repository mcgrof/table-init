#include <stdio.h>

#define BIT(nr)		(1UL << (nr))

#define BUG() do {										\
	fprintf(stderr, "----------------------------------------------------------\n");	\
	fprintf (stderr, "BUG on %s at %s: %i\n", __func__, __FILE__, __LINE__);		\
}												\
while (0)

#define WARN_ON(__test) do {									\
	if (__test) {										\
		fprintf(stderr, "----------------------------------------------------------\n");\
		fprintf (stderr, "WARN_ON on %s at %s: %i\n", __func__, __FILE__, __LINE__);	\
	}											\
}												\
while (0)
