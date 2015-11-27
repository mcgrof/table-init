#include <stdio.h>
#include <unistd.h>
#include <errno.h>

#include "init.h"

int early_init(void)
{
	int ret;
	struct init_fn *init_fn;

	unsigned int num_inits = table_num_entries(INIT_FNS);

	printf("Number of init entries: %d\n", num_inits);

	for_each_table_entry(init_fn, INIT_FNS) {
		if (!init_fn->detect)
			init_fn->flags |= INIT_DETECTED;
		else {
			ret = init_fn->detect();
			if (ret > 0)
				init_fn->flags |= INIT_DETECTED;
		}

		if (init_fn->flags & INIT_DETECTED) {
			init_fn->flags |= INIT_DETECTED;
			printf("Initializing %s ...\n", init_fn->name);
			ret = init_fn->early_init();
			if (ret) {
				if (init_fn->critical) {
					printf("Failed to initialize %s on early init\n", init_fn->name);
					return ret;
				}
				printf("Failed to initialize %s on early init, but its not critical\n", init_fn->name);

			} else
				printf("Completed initializing %s !\n", init_fn->name);
			if (init_fn->flags & INIT_FINISH_IF_DETECTED)
				break;
		}
	}

	return 0;
}

int late_init(void)
{
	int ret;
	struct init_fn *init_fn;

	for_each_table_entry(init_fn, INIT_FNS) {
		if ((init_fn->flags & INIT_DETECTED) && init_fn->late_init) {
			printf("Running late init for %s ...\n", init_fn->name);
			ret = init_fn->late_init();
			if (ret) {
				if (init_fn->critical) {
					printf("Failed to initialize %s on late init\n", init_fn->name);
					return ret;
				}
				printf("Failed to initialize %s on late init, but its not critical\n", init_fn->name);

			} else
				printf("Completed late initializing of %s !\n", init_fn->name);
		}
	}

	return 0;
}

void setup_arch_init(void)
{
	struct init_fn *init_fn;

	for_each_table_entry(init_fn, INIT_FNS) {
		if ((init_fn->flags & INIT_DETECTED) && init_fn->setup_arch) {
			printf("Running setup_arch for %s ...\n", init_fn->name);
			init_fn->setup_arch();
		}
	}
}
