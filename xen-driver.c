#include <stdio.h>
#include <unistd.h>

#include "init.h"
#include "pci.h"
#include "xen.h"

static int early_xen_init_driver(void) {
	sleep(2);

	return 0;
}

static int detect_xen_driver(void) {
	/* XXX: replace with a hook annotation, this is fragile as
	 * it requires the developer to know and it means some folks
	 * may not be setting these checks elsewhere.
	 */
	if (!booting_xen())
		return 0;

	return 1;
}

struct init_fn driver_xen_init_fn __init_fn(INIT_NORMAL) = {
	.detect = detect_xen_driver,
	.depend = detect_pci,
	.early_init = early_xen_init_driver,
	.name = "Xen Driver",
};
