#include <stdio.h>
#include <unistd.h>

#include "x86_init_fn.h"
#include "pci.h"
#include "kernel.h"

static void early_init_driver(void) {
	sleep(2);
}

static bool detect_driver(void) {
	return true;
}

X86_INIT_NORMAL_PC(acme, detect_driver, detect_pci,
		   early_init_driver, NULL, NULL);
