#include <stdio.h>
#include <unistd.h>
#include "init.h"

static void init_foo(void) {
	sleep(1);
}

struct init_fn foo_init_fn __init_fn(INIT_EARLY) = {
	.initialise = init_foo,
	.name = "Foo thing",
};
