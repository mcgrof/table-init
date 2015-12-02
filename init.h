#include <stdbool.h>
#include <linux/types.h>

#include "kernel.h"
#include "tables.h"
#include "bootparam.h"

/**
 * struct x86_init_fn - x86 generic kernel init call
 *
 * Linux x86 features vary in complexity, features may require work
 * done at different levels of the full x86 init sequence. Today there
 * are also two different possible entry points for Linux on x86, one for
 * bare metal, KVM and Xen HVM, and another for Xen PV guests / dom0.
 * Assuming bootloader has set up 64-bit mode, roughly the x86 init sequence
 * follows this path:
 *
 * Bare metal, KVM, Xen HVM                      Xen PV / dom0
 *       startup_64()                             startup_xen()
 *              \                                     /
 *      x86_64_start_kernel()                 xen_start_kernel()
 *                           \               /
 *                      x86_64_start_reservations()
 *                                   |
 *                              start_kernel()
 *                              [   ...        ]
 *                              [ setup_arch() ]
 *                              [   ...        ]
 *                                  init
 *
 * x86_64_start_kernel() and xen_start_kernel() are the respective first C code
 * entry starting points. The different entry points exist to enable Xen to
 * skip a lot of hardware setup already done and managed on behalf of the
 * hypervisor. The different levels of init calls exist to account for these
 * slight differences but also share a common entry x86 specific path,
 * x86_64_start_reservations().
 *
 * A generic x86 feature can have different initialization calls, one on
 * each different init sequence, but must also address both entry points in
 * order to work properly across the board on all supported x86
 * subarchitectures. x86 features can also have dependencies on other setup
 * code or features. Using struct x86_init_fn x86 feature developers must
 * annotate supported subarchitectures, dependencies and also declare
 * a two-digit decimal number to impose an ordering relative to other
 * features when required.
 *
 * @supp_hardware_subarch: Must be set, it represents the bitmask of supported
 *	subarchitectures.  We require each struct x86_init_fn to have this set
 *	to require developer considerations for each supported x86
 *	subarchitecture and to build strong annotations of different possible
 *	run time states particularly in consideration for the two main
 *	different entry points for x86 Linux.
 *
 *	The subarchitecture is read by the kernel at early boot from the
 *	struct boot_params hardware_subarch. Support for the subarchitecture
 *	exists as of x86 boot protocol 2.07. The bootloader would have set up
 *	the respective hardware_subarch on the boot sector as per
 *	Documentation/x86/boot.txt.
 *
 *	What x86 entry point is used is determined at run time by the
 *	bootloader. Linux pv_ops was designed to help enable to build one Linux
 *	binary to support bare metal and different hypervisors.  pv_ops setup
 *	code however is limited in that all pv_ops setup code is run late in
 *	the x86 init sequence, during setup_arch(). In fact cpu_has_hypervisor
 *	only works after early_cpu_init() during setup_arch(). If an x86
 *	feature requires an earlier determination of what hypervisor was used,
 *	or if it needs to annotate only support for certain hypervisors, the
 *	x86 hardware_subarch should be set by the bootloader and
 *	@supp_hardware_subarch set by the x86 feature. Using hardware_subarch
 *	enables x86 features to fill the semantic gap between the Linux x86
 *	entry point used and what pv_ops has to offer through a hypervisor
 *	agnostic mechanism.
 *
 *	Each supported subarchitecture is set using the respective
 *	X86_SUBARCH_* as a bit in the bitmask. For instance if a feature
 *	is supported on PC and Xen subarchitectures only you would set this
 *	bitmask to:
 *
 *		BIT(X86_SUBARCH_PC) |
 *		BIT(X86_SUBARCH_XEN);
 * @detect: if set returns true if the feature has been detected to be
 *	required, it returns false if the feature has been detected to
 *	not be required.
 * @depend: if set this set of init routines must be called prior to the
 * 	init routine who's respective detect routine we have set this
 * 	depends callback to. This is only used for sorting purposes.
 *	If you do not have a depend callback set its assumed the order level
 *	(__init_fn(level)) set by the init routine suffices to set the order
 *	for when the feature's respective callbacks are called with respect to
 *	other calls. Sorting of init calls between on the same order level is
 *	determined by linker order, determined by order listed on the Makefile.
 *
 *	XXX: we can't enforce validity of the order level as the order level is
 *	an annotation and used as part of the table name. One possibility to
 *	enforce checking is to extend struct x86_init_fn with a numeric order
 *	level, and use macro declarations for declaring order level both for
 *	the table name and x86_init_fn explicit struct numeric order. If we
 *	want to avoid extending the struct we could also use Coccinelle,
 *	however we can't use C annotations on Coccinelle, but Coccinelle does
 *	understand macro declarers such as DECLARE_MUTEX(), one could use
 *	SmPL rules with expected uses of declarers.
 * @early_init: if set would be run during before x86_64_start_reservations().
 *	Memory is not yet available.
 * @setup_arch: if set would be run during setup_arch().
 * @late_init: if set would be run right before init is spawned. You can count
 * 	on memory being set up.
 * @flags: private internal flags
 */
struct x86_init_fn {
	__u32 supp_hardware_subarch;
	bool (*detect)(void);
	bool (*depend)(void);
	void (*early_init)(void);
	void (*setup_arch)(void);
	void (*late_init)(void);
	const char *name;
	__u32 flags;
};

/**
 * enum x86_init_fn_flags: private flags for init sequences
 *
 * INIT_FINISH_IF_DETECTED: tells the core that once this init sequence
 *	has completed it can break out of the loop for init sequences on
 *	its own level.
 * INIT_DETECTED: the x86 core has determined that this
 * 	init sequence has been detected and it all of its callbacks
 * 	must be run during initialization.
 */
enum x86_init_fn_flags {
	INIT_FINISH_IF_DETECTED = BIT(0),
	INIT_DETECTED = BIT(1),
};

/** Initialisation function table */
#define INIT_FNS __table(struct x86_init_fn, "init_fns")

/** Declare an initialisation functon */
#define __init_fn( init_order ) __table_entry (INIT_FNS, init_order)

#define INIT_EARLY	01	/**< Early initialisation */
#define INIT_SERIAL	02	/**< Serial driver initialisation */
#define INIT_CONSOLE	03	/**< Console initialisation */
#define INIT_NORMAL	04	/**< Normal initialisation */

void early_init(void);
void late_init(void);
void setup_arch_init(void);

void sort_table(struct x86_init_fn *start,
		      struct x86_init_fn *finish);
void check_table_entries(struct x86_init_fn *start,
			 struct x86_init_fn *finish);
