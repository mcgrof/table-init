#include <stdio.h>
#include <unistd.h>

#include "init.h"
#include "pci.h"
#include "kernel.h"

static void early_init_driver(void) {
	sleep(2);
}

static bool detect_driver(void) {
	return true;
}

X86_INIT_EARLY_PC(acme, detect_driver, detect_pci,
		  early_init_driver, NULL, NULL);
