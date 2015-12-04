#include <linux/tables.h>
#include <asm/x86_init_fn.h>
#include <asm/bootparam.h>

#include <linux/pci.h>

void early_init_pci(void) {
	sleep(1);
}

bool detect_pci(void) {
	return true;
}

X86_INIT_EARLY_ALL(pci, detect_pci, NULL, early_init_pci, NULL, NULL);
