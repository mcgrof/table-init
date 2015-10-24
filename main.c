#include <stdio.h>
#include <unistd.h>
#include "init.h"

static int early_init_y(void) {
	sleep(1);

	return 0;
}

static int detect_y(void) {
	return 1;
}

struct init_fn y_init_fn __init_fn(INIT_EARLY) = {
	.detect = detect_y,
	.early_init = early_init_y,
	.name = "Y thing",
};

int main(void)
{
	int ret;

	printf("Initializing world\n");
	ret = early_init();

	if (ret) {
		printf("Early init failed\n");
		return ret;
	}

	ret = late_init();

	return ret;
}
