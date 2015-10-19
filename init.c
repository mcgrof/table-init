#include <stdio.h>
#include <unistd.h>
#include "init.h"

static void init_x(void) {
	printf("x is about to start\n");
	sleep(1);
	printf("x started!\n");
}

struct init_fn x_init_fn __init_fn(INIT_EARLY) = {
	.initialise = init_x,
	//.name = "X thing",
};

int init(void)
{
	struct init_fn *init_fn;                                                

	unsigned int num_inits = table_num_entries(INIT_FNS);

	printf("Number of init entries: %d\n", num_inits);

	for_each_table_entry (init_fn, INIT_FNS) {
		//printf("Initializing %s ...", init_fn->name);
		init_fn->initialise();
	}

	return 0;
}
