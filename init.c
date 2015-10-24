#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include "init.h"

static int early_init_x(void) {
	sleep(1);

	return 0;
}

static int detect_x(void) {
	return 1;
}

struct init_fn x_init_fn __init_fn(INIT_EARLY) = {
	.detect = detect_x,
	.early_init = early_init_x,
	.name = "X thing",
	.critical = true,
};

int early_init(void)
{
	int ret;
	struct init_fn *init_fn;                                                

	unsigned int num_inits = table_num_entries(INIT_FNS);

	printf("Number of init entries: %d\n", num_inits);

	for_each_table_entry (init_fn, INIT_FNS) {
		if (init_fn->detect && init_fn->detect() > 0) {
			printf("Initializing %s ...\n", init_fn->name);
			ret = init_fn->early_init();
			if (ret) {
				if (init_fn->critical)
					return ret;
				printf("Failed to initialize %s on early init, but its not critical\n", init_fn->name);

			} else
				printf("Completed initializing %s !\n", init_fn->name);
		}
	}

	return 0;
}
