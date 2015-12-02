#include <stdio.h>
#include <unistd.h>
#include <errno.h>

#include "tables.h"
#include "init.h"
#include "pci.h"
#include "bootparam.h"

void early_init_pci(void) {
	sleep(1);
}

bool detect_pci(void) {
	return true;
}

struct x86_init_fn pci_init_fn __init_fn(INIT_NORMAL) = {
	.supp_hardware_subarch = X86_SUBARCH_ALL_SUBARCHS,
	.detect = detect_pci,
	.early_init = early_init_pci,
	.name = "PCI buses",
};
