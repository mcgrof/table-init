#include <stdio.h>
#include <unistd.h>
#include <errno.h>

#include <linux/kernel.h>

#include <asm/x86_init_fn.h>
#include <asm/bootparam.h>

void kasan_early_init(void) {
	pr_info("Early init for Kasan...\n");
}

void kasan_init(void)
{
	pr_info("Calling setup_arch work for Kasan...\n");
}

X86_INIT_EARLY_PC(kasan, NULL, NULL, kasan_early_init, kasan_init, NULL);
