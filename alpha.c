#include <stdio.h>
#include <unistd.h>

#include "x86_init_fn.h"
#include "kernel.h"

static void early_init_alpha(void) {
}

X86_INIT_EARLY_PC(alpha, NULL, NULL, early_init_alpha, NULL, NULL);
