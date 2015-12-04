#include <stdio.h>
#include <unistd.h>
#include <errno.h>

#include "init.h"

static void early_init_memory(void) {
	sleep(1);
}

static bool detect_memory(void) {
	return true;
}

X86_INIT_EARLY_ALL(memory, detect_memory, NULL,
		   early_init_memory, NULL, NULL);
