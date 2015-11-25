#include <stdio.h>
#include <unistd.h>
#include <errno.h>

#include "tables.h"
#include "init.h"
#include "pci.h"
#include "bootparam.h"

static int early_init_pci(void) {
	sleep(1);

	return 0;
}

int detect_pci(void) {
	return 1;
}

struct init_fn pci_init_fn __init_fn(INIT_NORMAL) = {
	.supp_hardware_subarch = X86_SUBARCH_ALL_SUBARCHS,
	.detect = detect_pci,
	.early_init = early_init_pci,
	.name = "PCI buses",
	.critical = true,
};
