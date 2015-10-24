#include <stdio.h>
#include <unistd.h>
#include "init.h"

static void init_foo(void) {
	sleep(1);
}

static int detect_foo(void) {
	return 1;
}

struct init_fn foo_init_fn __init_fn(INIT_EARLY) = {
	.detect = detect_foo,
	.initialise = init_foo,
	.name = "Foo thing",
};
