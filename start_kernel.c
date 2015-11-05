#include <stdio.h>
#include <stdbool.h>
#include "init.h"

int start_kernel(void)
{
	return late_init();
}

