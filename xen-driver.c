#include <stdio.h>
#include <unistd.h>

#include "init.h"
#include "pci.h"
#include "xen.h"

static void early_xen_init_driver(void) {
	sleep(2);
}

X86_INIT_NORMAL_XEN(xen_driver, NULL, detect_pci,
		    early_xen_init_driver, NULL, NULL);
