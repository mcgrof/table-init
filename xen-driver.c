#include <stdio.h>
#include <unistd.h>

#include "init.h"
#include "pci.h"
#include "xen.h"

static int early_xen_init_driver(void) {
	sleep(2);

	return 0;
}

struct init_fn driver_xen_init_fn __init_fn(INIT_NORMAL) = {
	.supp_hardware_subarch = BIT(X86_SUBARCH_XEN),
	.depend = detect_pci,
	.early_init = early_xen_init_driver,
	.name = "Xen Driver",
};
