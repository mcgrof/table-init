#include <stdio.h>
#include <unistd.h>
#include "init.h"

static int init_kasan(void) {
	sleep(1);

	return 0;
}

static int detect_kasan(void) {
	return 1;
}

struct init_fn kasan_init_fn __init_fn(INIT_EARLY) = {
	.detect = detect_kasan,
	.early_init = init_kasan,
	.name = "Kasan",
};
