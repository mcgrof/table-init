#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include "tables.h"
#include "init.h"
#include "start_kernel.h"
#include "kasan.h"

extern struct init_fn __tbl[], __tbl_end[];

int x86_64_start_reservations(void)
{
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
	if (is_kasan_setup())
		setup_arch_kasan();
}
