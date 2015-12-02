#include <string.h>

#include "x86.h"
#include "xen.h"
#include "bootparam.h"

struct boot_params boot_params __attribute__((aligned(16)));

int main(int arg, char *argc[])
{
	memset(&boot_params, 0, sizeof(struct boot_params));

	if (arg <= 1)
		startup_64();
	else {
		boot_params.hdr.hardware_subarch = X86_SUBARCH_XEN;
		startup_xen();
	}

	return 0;
}
