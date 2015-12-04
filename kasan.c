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

X86_INIT_EARLY_PC(kasan, NULL, NULL, kasan_early_init, kasan_init, NULL);
