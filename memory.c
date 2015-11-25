#include <stdio.h>
#include <unistd.h>
#include <errno.h>

#include "init.h"

static int early_init_memory(void) {
	sleep(1);

	return 0;
}

static int detect_memory(void) {
	return 1;
}

struct init_fn memory_init_fn __init_fn(INIT_EARLY) = {
	.supp_hardware_subarch = X86_SUBARCH_ALL_SUBARCHS,
	.detect = detect_memory,
	.early_init = early_init_memory,
	.name = "Memory",
	.critical = true,
};
