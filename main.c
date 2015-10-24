#include <stdio.h>
#include <unistd.h>
#include "tables.h"
#include "init.h"

extern struct init_fn __tbl[], __tbl_end[];

int main(void)
{
	int ret;

	printf("Initializing world\n");

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
