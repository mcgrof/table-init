#include <stdio.h>
#include <unistd.h>
#include <errno.h>

#include "init.h"

static void early_init_memory(void) {
	sleep(1);
}

static bool detect_memory(void) {
	return true;
}

struct x86_init_fn memory_init_fn __init_fn(INIT_EARLY) = {
	.supp_hardware_subarch = X86_SUBARCH_ALL_SUBARCHS,
	.detect = detect_memory,
	.early_init = early_init_memory,
	.name = "Memory",
};
