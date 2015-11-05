#include <stdio.h>
#include <stdbool.h>
#include "init.h"
#include "x86.h"

int start_kernel(void)
{
	setup_arch();

	return late_init();
}

