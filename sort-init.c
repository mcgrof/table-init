#include <stdio.h> /* for NULL and printf */
#include <string.h> /* for memmove */
#include "init.h"

#define DEBUG 1

static struct x86_init_fn *
find_dependents_of(struct x86_init_fn *start,
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


void sort_table(struct x86_init_fn *start,
		struct x86_init_fn *finish) {

	struct x86_init_fn *p, *q, tmp;

	for (p = start; p < finish; p++) {
again:
		q = find_dependents_of(start, finish, p);
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
void check_table_entries(struct x86_init_fn *start,
			 struct x86_init_fn *finish)
{
	struct x86_init_fn *p, *q, *x;

	/* Simple cyclic dependency checker. */
	for (p = start; p < finish; p++) {
		if (!p->depend)
			continue;
		q = find_dependents_of(start, finish, p);
		x = find_dependents_of(start, finish, q);
		if (p == x) {
			printf("CYCLIC DEPENDENCY FOUND! %pS depends on %pS and vice-versa. BREAKING IT.\n",
			       p->detect, q->detect);
			/* Heavy handed way..*/
			x->depend = 0;
		}
	}

	for (p = start; p < finish; p++) {
		q = find_dependents_of(p, finish, p);
		if (!p->depend)
			continue;
		if (q && q > p) {
			printf("EXECUTION ORDER INVALID! %pS should be called before %pS!\n",
			       p->detect, q->detect);
		}
	}
}
#else
inline void check_table_entries(struct x86_init_fn *start,
				struct x86_init_fn *finish)
{
}
#endif
