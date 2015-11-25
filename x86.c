#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include "tables.h"
#include "init.h"
#include "start_kernel.h"
#include "kasan.h"
#include "setup.h"
#include "bootparam.h"

extern struct init_fn __tbl[], __tbl_end[];

int x86_64_start_reservations(void)
{
	switch (boot_params.hdr.hardware_subarch) {
	case X86_SUBARCH_PC:
		printf("Booting bare metal\n");
		break;
	case X86_SUBARCH_LGUEST:
		printf("Booting lguest not supported\n");
		return -EOPNOTSUPP;
	case X86_SUBARCH_XEN:
		printf("Booting a Xen guest\n");
		break;
	case X86_SUBARCH_INTEL_MID:
		printf("Booting Intel MID not supported\n");
		return -EOPNOTSUPP;
	case X86_SUBARCH_CE4100:
		printf("Booting Intel CE4100 not supported\n");
		return -EOPNOTSUPP;
	default:
		printf("Booting sunsupported x86 hardware subarch\n");
		return -EOPNOTSUPP;
	}

	return start_kernel();
}

static int x86_64_start_kernel(void)
{
	int ret;

	sort_table(__tbl, __tbl_end);
	check_table_entries(__tbl, __tbl_end);

	ret = early_init();
	if (ret) {
		printf("Early init failed\n");
		return ret;
	}

	ret = kasan_early_init();
	if (ret)
		return ret;

	return x86_64_start_reservations();
}

int startup_64(void)
{
	printf("Initializing x86 bare metal world\n");
	return x86_64_start_kernel();
}

void setup_arch(void)
{
	setup_arch_init();
	kasan_init();
}
