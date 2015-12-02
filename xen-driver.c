#include <stdio.h>
#include <unistd.h>

#include "init.h"
#include "pci.h"
#include "xen.h"

static void early_xen_init_driver(void) {
	sleep(2);
}

struct x86_init_fn driver_xen_init_fn __init_fn(INIT_NORMAL) = {
	.supp_hardware_subarch = BIT(X86_SUBARCH_XEN),
	.depend = detect_pci,
	.early_init = early_xen_init_driver,
	.name = "Xen Driver",
};
