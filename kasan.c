#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include "init.h"

static bool __is_kasan_setup = false;

int setup_kasan_bare_metal(void) {
	__is_kasan_setup = true;
	return 0;
}

bool is_kasan_setup(void)
{
	return __is_kasan_setup;
}

static int init_kasan(void) {
	sleep(1);

	if (!is_kasan_setup())
		return -EOPNOTSUPP;

	return 0;
}

static int detect_kasan(void) {
	return 1;
}

struct init_fn kasan_init_fn __init_fn(INIT_EARLY) = {
	.detect = detect_kasan,
	.early_init = init_kasan,
	.name = "Kasan",
	.critical = true,
};
