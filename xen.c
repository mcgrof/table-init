#include <stdio.h>
#include <stdbool.h>
#include "init.h"
#include "tables.h"
#include "x86.h"

extern struct init_fn __tbl[], __tbl_end[];

static bool __booting_xen = false;

bool booting_xen(void)
{
	return __booting_xen;
}

int startup_xen(void)
{
	int ret;

	__booting_xen = true;

	sort_table(__tbl, __tbl_end);
	check_table_entries(__tbl, __tbl_end);

	ret = early_init();
	if (ret) {
		printf("Early init failed\n");
		return ret;
	}

	printf("Initializing Xen guest\n");

	return x86_64_start_reservations();
}
