#include <stdio.h>
#include <unistd.h>
#include "init.h"

static void init_bar(void) {
	sleep(2);
}

static int detect_bar(void) {
	return 1;
}

struct init_fn bar_init_fn __init_fn (INIT_NORMAL) = {
	.detect = detect_bar,
	.initialise = init_bar,
	.name = "Bar thing",
};
