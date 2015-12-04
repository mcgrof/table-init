#include <stdio.h>
#include <unistd.h>

#include <linux/kernel.h>
#include <asm/x86_init_fn.h>

static void early_init_beta(void) {
}

X86_INIT_EARLY_PC(beta, NULL, NULL, early_init_beta, NULL, NULL);
