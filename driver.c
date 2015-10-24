#include <stdio.h>
#include <unistd.h>

#include "init.h"
#include "pci.h"

static int early_init_driver(void) {
	sleep(2);

	return 0;
}

static int detect_driver(void) {
	return 1;
}

struct init_fn driver_init_fn __init_fn(INIT_EARLY) = {
	.detect = detect_driver,
	.depend = detect_pci,
	.early_init = early_init_driver,
	.name = "ACME(TM) Driver",
};
