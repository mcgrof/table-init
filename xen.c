#include <stdio.h>
#include <stdbool.h>

#include <linux/tables.h>
#include <asm/x86_init_fn.h>
#include <asm/x86.h>

extern struct x86_init_fn __tbl_x86_start_init_fns[], __tbl_x86_end_init_fns[];

void startup_xen(void)
{
	sort_table(__tbl_x86_start_init_fns, __tbl_x86_end_init_fns);
	check_table_entries(__tbl_x86_start_init_fns, __tbl_x86_end_init_fns);

	printf("Initializing Xen guest\n");

	early_init();

	x86_64_start_reservations();
}
