#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include "init.h"
#include "kernel.h"

static bool __is_kasan_setup = false;

int kasan_early_init(void) {
	printf("Early init for Kasan...\n");
	__is_kasan_setup = true;
	return 0;
}

bool is_kasan_setup(void)
{
	return __is_kasan_setup;
}

int kasan_init(void)
{
	if (!__is_kasan_setup) {
		printf("Kasan was not set up...\n");
		BUG();
		return -EINVAL;
	}

	printf("Calling setup_arch work for Kasan...\n");

	return 0;
}
