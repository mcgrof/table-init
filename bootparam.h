#ifndef __BOOTPARAM_H
#define __BOOTPARAM_H

#include <linux/types.h>

struct setup_header {
	__u32 hardware_subarch;
} __attribute__((packed));

struct boot_params {
	struct setup_header hdr;
} __attribute__((packed));

enum {
	X86_SUBARCH_PC = 0,
	X86_SUBARCH_LGUEST,
	X86_SUBARCH_XEN,
	X86_SUBARCH_INTEL_MID,
	X86_SUBARCH_CE4100,
	X86_NR_SUBARCHS,
};

#define X86_SUBARCH_ALL_SUBARCHS 	\
	(				\
	BIT(X86_SUBARCH_PC) |		\
	BIT(X86_SUBARCH_LGUEST) |	\
	BIT(X86_SUBARCH_XEN) |		\
	BIT(X86_SUBARCH_INTEL_MID) |	\
	BIT(X86_SUBARCH_CE4100)		\
	)

#endif /* __BOOTPARAM_H */
