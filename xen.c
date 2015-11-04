#include <stdio.h>
#include <stdbool.h>

static bool __booting_xen = false;

bool booting_xen(void)
{
	return __booting_xen;
}

int startup_xen(void)
{
	__booting_xen = true;

	printf("Initializing Xen guest\n");

	return 0;
}
