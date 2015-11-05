#include "x86.h"
#include "xen.h"

int main(int arg, char *argc[])
{
	int ret;

	if (arg > 1)
		ret = startup_xen();
	else
		ret = startup_64();

	return ret;
}
