#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include "tables.h"
#include "x86_init_fn.h"
#include "start_kernel.h"
#include "kasan.h"
#include "setup.h"
#include "bootparam.h"

extern struct x86_init_fn __tbl_x86_start_init_fns[], __tbl_x86_end_init_fns[];

void x86_64_start_reservations(void)
{
	switch (boot_params.hdr.hardware_subarch) {
	case X86_SUBARCH_PC:
		pr_info("Booting bare metal\n");
		break;
	case X86_SUBARCH_LGUEST:
		pr_info("Booting lguest not supported\n");
		BUG();
	case X86_SUBARCH_XEN:
		pr_info("Booting a Xen guest\n");
		break;
	case X86_SUBARCH_INTEL_MID:
		pr_info("Booting Intel MID not supported\n");
		BUG();
	case X86_SUBARCH_CE4100:
		pr_info("Booting Intel CE4100 not supported\n");
		BUG();
	default:
		pr_info("Booting sunsupported x86 hardware subarch\n");
		BUG();
	}

	start_kernel();
}

static void x86_64_start_kernel(void)
{
	sort_table(__tbl_x86_start_init_fns, __tbl_x86_end_init_fns);
	check_table_entries(__tbl_x86_start_init_fns, __tbl_x86_end_init_fns);

	early_init();

	x86_64_start_reservations();
}

void startup_64(void)
{
	pr_info("Initializing x86 bare metal world\n");
	x86_64_start_kernel();
}

void setup_arch(void)
{
	setup_arch_init();
}
