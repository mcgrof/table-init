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

X86_INIT_EARLY_ALL(pci, detect_pci, NULL, early_init_pci, NULL, NULL);
