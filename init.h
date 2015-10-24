#include <stdbool.h>
#include "tables.h"

/**
 * An initialisation function
 *
 * Initialisation functions are called exactly once, as part of the
 * call to init().
 */
struct init_fn {                                                                
	int (* detect)(void);
	int (* early_init)(void);
	bool critical;
	const char *name;
};

/** Initialisation function table */
#define INIT_FNS __table (struct init_fn, "init_fns")

/** Declare an initialisation functon */
#define __init_fn( init_order ) __table_entry (INIT_FNS, init_order)

#define INIT_EARLY	01	/**< Early initialisation */
#define INIT_SERIAL	02	/**< Serial driver initialisation */
#define INIT_CONSOLE	03	/**< Console initialisation */
#define INIT_NORMAL	04	/**< Normal initialisation */

int early_init(void);
