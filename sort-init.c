#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <asm/x86_init_fn.h>

static struct x86_init_fn *x86_init_fn_find_dep(struct x86_init_fn *start,
						struct x86_init_fn *finish,
						struct x86_init_fn *q)
{
	struct x86_init_fn *p;

	if (!q)
		return NULL;

	for (p = start; p < finish; p++)
		if (p->detect == q->depend)
			return p;

	return NULL;
}


void x86_init_fn_sort(struct x86_init_fn *start,
		      struct x86_init_fn *finish) {

	struct x86_init_fn *p, *q, tmp;

	for (p = start; p < finish; p++) {
again:
		q = x86_init_fn_find_dep(start, finish, p);
		/* We are bit sneaky here. We use the memory address to figure
		 * out if the node we depend on is past our point, if so, swap.
		 */
		if (q > p) {
			tmp = *p;
			memmove(p, q, sizeof(*p));
			*q = tmp;
			goto again;
		}
	}

}

#ifdef DEBUG
void x86_init_fn_check(struct x86_init_fn *start, struct x86_init_fn *finish)
{
	struct x86_init_fn *p, *q, *x;

	/* Simple cyclic dependency checker. */
	for (p = start; p < finish; p++) {
		if (!p->depend)
			continue;
		q = x86_init_fn_find_dep(start, finish, p);
		x = x86_init_fn_find_dep(start, finish, q);
		if (p == x) {
			pr_info("CYCLIC DEPENDENCY FOUND! %pS depends on %pS and vice-versa. BREAKING IT.\n",
			       p->name, q->name);
			/* Heavy handed way..*/
			x->depend = 0;
		}
	}

	/*
	 * Validate sorting semantics.
	 *
	 * p depends on q so:
	 * 	- q must run first, so q < p. If q > p that's an issue
	 * 	  as its saying p must run prior to q. We already sorted
	 * 	  this table, this is a problem.
	 *
	 * 	- q's order level must be <= than p's as it should run first
	 */
	for (p = start; p < finish; p++) {
		if (!p->depend)
			continue;
		/*
		 * Be pedantic and do a full search on the entire table,
		 * if we need further validation, after this is called
		 * one could use an optimized version which just searches
		 * on x86_find_dependents_of(p, finish, p), as we would have
		 * guarantee on proper ordering both at the dependency level
		 * and by order level.
		 */
		q = x86_find_dependents_of(start, finish, p);
		if (q && q > p) {
			pr_info("EXECUTION ORDER INVALID! %s should be called before %s!\n",
			       p->name, q->name);
		}

		/*
		 * Technically this would still work as the memmove() would
		 * have forced the dependency to run first, however we want
		 * strong semantics, so lets avoid these.
		 */
		if (q && q->order_level > p->order_level) {
			pr_info("INVALID ORDER LEVEL! %s should have an order level <= be called before %s!\n",
			       p->name, q->name);
		}
	}
}
#else
inline void x86_init_fn_check(struct x86_init_fn *start,
			      struct x86_init_fn *finish)
{
}
#endif
