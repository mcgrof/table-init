#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include "init.h"

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

void setup_arch_kasan(void) {
	if (!is_kasan_setup())
		return;
	printf("Calling setup_arch work for Kasan...\n");
}
