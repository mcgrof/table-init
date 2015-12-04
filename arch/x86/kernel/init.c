#include <linux/kernel.h>
#include <asm/x86_init_fn.h>
#include <asm/boot/boot.h>

static bool x86_init_fn_supports_subarch(struct x86_init_fn *fn)
{
	if (!fn->supp_hardware_subarch) {
		pr_info("Init sequence fails to declares supported subarchs: %s\n", fn->name);
		WARN_ON(1);
	}
	if (BIT(boot_params.hdr.hardware_subarch) & fn->supp_hardware_subarch)
		return true;
	return false;
}

void x86_init_fn_early_init(void)
{
	int ret;
	struct x86_init_fn *init_fn;

	unsigned int num_inits = table_num_entries(X86_INIT_FNS);

	pr_info("Number of init entries: %d\n", num_inits);

	for_each_table_entry(init_fn, X86_INIT_FNS) {
		if (!x86_init_fn_supports_subarch(init_fn))
			continue;
		if (!init_fn->detect)
			init_fn->flags |= INIT_DETECTED;
		else {
			ret = init_fn->detect();
			if (ret > 0)
				init_fn->flags |= INIT_DETECTED;
		}

		if (init_fn->flags & INIT_DETECTED) {
			init_fn->flags |= INIT_DETECTED;
			pr_info("Initializing %s ...\n", init_fn->name);
			init_fn->early_init();
			pr_info("Completed initializing %s !\n", init_fn->name);
			if (init_fn->flags & INIT_FINISH_IF_DETECTED)
				break;
		}
	}
}

void x86_init_fn_late_init(void)
{
	struct x86_init_fn *init_fn;

	for_each_table_entry(init_fn, X86_INIT_FNS) {
		if ((init_fn->flags & INIT_DETECTED) && init_fn->late_init) {
			pr_info("Running late init for %s ...\n", init_fn->name);
			init_fn->late_init();
			pr_info("Completed late initializing of %s !\n", init_fn->name);
		}
	}
}

void x86_init_fn_setup_arch(void)
{
	struct x86_init_fn *init_fn;

	for_each_table_entry(init_fn, X86_INIT_FNS) {
		if ((init_fn->flags & INIT_DETECTED) && init_fn->setup_arch) {
			pr_info("Running setup_arch for %s ...\n", init_fn->name);
			init_fn->setup_arch();
		}
	}
}
