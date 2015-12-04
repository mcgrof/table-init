#include <stdio.h>
#include <unistd.h>

#include <linux/kernel.h>
#include <asm/x86_init_fn.h>

static void early_init_alpha(void) {
}

X86_INIT_EARLY_PC(alpha, NULL, NULL, early_init_alpha, NULL, NULL);
