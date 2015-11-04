#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include "tables.h"
#include "init.h"
#include "xen.h"
#include "kasan.h"

extern struct init_fn __tbl[], __tbl_end[];

int bare_metal_start(void)
{
	int ret;

	printf("Initializing bare metal world\n");
	ret = setup_kasan_bare_metal();

	return ret;
}

int main(int arg, char *argc[])
{
	int ret;

	if (arg > 1)
		startup_xen();
	else
		bare_metal_start();

	sort_table(__tbl, __tbl_end);
	check_table_entries(__tbl, __tbl_end);

	ret = early_init();

	if (ret) {
		printf("Early init failed\n");
		return ret;
	}

	ret = late_init();

	return ret;
}
