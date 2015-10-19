#include <stdio.h>
#include <unistd.h>
#include "init.h"

static void init_foo(void) {
	printf("foo is about to start\n");
	sleep(1);
	printf("foo started!\n");
}

struct init_fn foo_init_fn __init_fn(INIT_EARLY) = {
	.initialise = init_foo,
	//.name = "Foo thing",
};
