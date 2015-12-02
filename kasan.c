#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include "init.h"
#include "kernel.h"
#include "bootparam.h"

void kasan_early_init(void) {
	printf("Early init for Kasan...\n");
}

void kasan_init(void)
{
	printf("Calling setup_arch work for Kasan...\n");
}

struct x86_init_fn kasan_init_fn __init_fn(INIT_EARLY) = {
	.supp_hardware_subarch = BIT(X86_SUBARCH_PC),
	.early_init = kasan_early_init,
	.setup_arch = kasan_init,
	.name = "Kasan",
};
