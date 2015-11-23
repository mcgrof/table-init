#include <stdio.h>
#include <unistd.h>
#include <errno.h>

#include "tables.h"
#include "init.h"
#include "pci.h"
#include "bootparam.h"

static int early_init_pci(void) {
	sleep(1);

	return -EINVAL;
}

int detect_pci(void) {
	return 1;
}

struct init_fn pci_init_fn __init_fn(INIT_NORMAL) = {
	.detect = detect_pci,
	.early_init = early_init_pci,
	.name = "PCI buses",
	.critical = true,
	.supp_hardware_subarch =
			X86_SUBARCH_PC |
			X86_SUBARCH_XEN,
};
