#include <stdbool.h>
#include <linux/types.h>

#include "kernel.h"
#include "tables.h"
#include "bootparam.h"

/**
 * struct init_fn - architecture and kernel init
 *
 * Linux initialization is split up into 4 parts, an early init,
 * an architecture specific init call, and a late init call. This
 * init structure enables kernel features annotate where in the init
 * process a respective init call for a feature must be called. When
 * needed it also enables building dependency relationships between
 * components.
 *
 * Since you may wish to enable at build time features which may not be
 * available for different architecture run time environments, but its
 * still desirable to enable different architecture run time environments
 * in a single binary, usin the init structures also enables kernel features
 * to annotate known supported run time environments.
 *
 * @detect: if you do not have a detect call its assume you don't need
 *	a special check for detection and the init core is able to safely
 *	call the featutres's early_init(), setup_arch() and late_init().
 *
 * @supp_hardware_subarch: the supported X86_SUBARCH_* archs. The only
 *	issue with using this is X86_SUBARCH_PC is 0 and as such it
 *	cannot really be annotated as supported. The boot loader would
 *	also need to set this. Evaluation if difficulties of using
 *	this is currently being done. We also currently don't have a
 *	value for KVM. A value might be desirable however given that
 *	the only other thing available to us during early boot is
 *	the boot params. For instance the alternative X86_FEATURE_HYPERVISOR
 *	feature is actually only made available to the booted kernel
 *	after early_cpu_init(), way after setup_arch. We want a tool
 *	which enables us to distinguish requirements between protected
 *	mode and even after setup_arch(), which is not Xen specific.
 *
 *	Further evaluation of this use is currently being done.
 */
struct init_fn {                                                                
	int (*detect)(void);
	int (*depend)(void);
	int (*early_init)(void); /* No memory allocate available. */
	void (*setup_arch)(void);
	int (*late_init)(void); /* Yes, can allocate memory. */
	bool critical;
	const char *name;
#define INIT_FINISH_IF_DETECTED (1<<0)
#define INIT_DETECTED           (1<<1)
	int flags;
	__u32 supp_hardware_subarch;
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
int late_init(void);
void setup_arch_init(void);

void sort_table(struct init_fn *start,
		      struct init_fn *finish);
void check_table_entries(struct init_fn *start,
			 struct init_fn *finish);
