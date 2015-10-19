#include <stdio.h>
#include <unistd.h>
#include "init.h"

static void init_bar(void) {
	printf("bar is going to init\n");
	sleep(2);
}

struct init_fn bar_init_fn __init_fn (INIT_NORMAL) = {
	.initialise = init_bar,
	//.name = "Bar thing",
};
