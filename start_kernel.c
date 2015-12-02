#include <stdio.h>
#include <stdbool.h>
#include "init.h"
#include "x86.h"

void start_kernel(void)
{
	printf("Calling start_kernel()...\n");

	setup_arch();
	late_init();
}
