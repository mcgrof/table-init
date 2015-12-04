#include <stdio.h>
#include <stdbool.h>
#include "x86_init_fn.h"
#include "x86.h"

void start_kernel(void)
{
	pr_info("Calling start_kernel()...\n");

	setup_arch();
	late_init();
}
