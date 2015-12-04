#include <stdio.h>
#include <unistd.h>

#include "init.h"
#include "kernel.h"

static void early_init_beta(void) {
}

X86_INIT_EARLY_PC(beta, NULL, NULL, early_init_beta, NULL, NULL);
