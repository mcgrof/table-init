#include <stdio.h>
#include <stdbool.h>

#include <linux/tables.h>
#include <asm/x86_init_fn.h>
#include <asm/x86.h>

extern struct x86_init_fn __tbl_x86_start_init_fns[], __tbl_x86_end_init_fns[];

void startup_xen(void)
{
	x86_init_fn_sort(__tbl_x86_start_init_fns, __tbl_x86_end_init_fns);
	x86_init_fn_check(__tbl_x86_start_init_fns, __tbl_x86_end_init_fns);

	pr_info("Initializing Xen guest\n");

	x86_init_fn_early_init();

	x86_64_start_reservations();
}
