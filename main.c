#include <stdio.h>
#include <unistd.h>
#include "init.h"

static void init_y(void) {
	printf("y is about to start\n");
	sleep(1);
	printf("y started!\n");
}

struct init_fn y_init_fn __init_fn(INIT_EARLY) = {
	.initialise = init_y,
	//.name = "Y thing",
};

int main(void)
{
	int ret;

	printf("Initializing world\n");
	/*
	 * XXX: capture ret value on some levels of init which
	 * are critical.
	 */
	ret = init();

	return ret;
}
