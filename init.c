#include <stdio.h>
#include <unistd.h>
#include "init.h"

static void init_x(void) {
	sleep(1);
}

struct init_fn x_init_fn __init_fn(INIT_EARLY) = {
	.initialise = init_x,
	.name = "X thing",
};

int init(void)
{
	struct init_fn *init_fn;                                                

	unsigned int num_inits = table_num_entries(INIT_FNS);

	printf("Number of init entries: %d\n", num_inits);

	for_each_table_entry (init_fn, INIT_FNS) {
		printf("Initializing %s ...\n", init_fn->name);
		init_fn->initialise();
		printf("Completed initializing %s !\n", init_fn->name);
	}

	return 0;
}
