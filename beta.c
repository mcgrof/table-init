#include <stdio.h>
#include <unistd.h>

#include "init.h"
#include "kernel.h"

static void early_init_beta(void) {
}

struct x86_init_fn beta_init_fn __init_fn(INIT_EARLY) = {
	.supp_hardware_subarch = BIT(X86_SUBARCH_PC),
	.early_init = early_init_beta,
	.name = "beta",
};
