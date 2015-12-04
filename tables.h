#ifndef _LINUX_TABLES_H
#define _LINUX_TABLES_H

/**
 * DOC: linker tables - for when #ifdefs are harmful
 *
 * Overuse of C #ifdefs can be problematic for certain types of code.  Linux
 * provides a rich array of features, but all these features take up valuable
 * space in a kernel image. The traditional solution to this problem has been
 * for each feature to have its own Kconfig entry and for the respective code
 * to be wrapped around #ifdefs, allowing the feature to be compiled in only
 * if desired.
 *
 * The problem with this is that over time it becomes very difficult and time
 * consuming to compile, let alone test, all possible versions of Linux. Code
 * that is not typically used tends to suffer from bit-rot over time. It can
 * become difficult to predict which combinations of compile-time options will
 * result in code that can compile and link correctly.
 *
 * To solve this problem linker tables can be used on Linux, it enables you to
 * always force compiling of features that one wishes to avoid bit-rot by while
 * still enabling you to disable linking the feature code into the final kernel
 * image if the features have been disabled via Kconfig. The code is derivative
 * of gPXE table's solution embraced on gPXE >= 5 and can optionally be used in
 * conjunction with Linux's own custom init dependency solution, originally
 * developed for use in IOMMU. For more details on history and to see how this
 * code evolved refer to the userspace mockup solution on github [0]. To allow
 * for easy testing of extensions that code can be used and extended prior to
 * inclusion in Linux. Contrary to gPXE's solution which stives to force
 * compilation of *everything* Linux's solution allows for developers to be
 * selective over where one should use linker tables.
 *
 * [0] https://github.com/mcgrof/table-init/
 *
 * To use linker tables, features should be implemented in separate C files,
 * and should always be compiled -- they should not be guarded with a C code
 * #ifdef CONFIG_FOO statements. By making code always compile, you avoid
 * the problem of bit-rot in rarely-used code.
 *
 * For instance, let's assume we want to always force compilation of both FOO
 * and BAR code in the kernel but avoid linking it.  When you enable the
 * features via Kconfig you'd end up with:
 *
 *   #define CONFIG_FOO 1
 *
 * You typically would then just use this on your Makefile to selectively
 * compile and link the features:
 *
 * obj-$(CONFIG_FOO) += foo.o
 *
 * If using linker tables you'd instead use:
 *
 * XXX: is this true?
 * obj-y += foo.o
 *
 * The reason why this works is due to the magic behind linker tables.
 * Traditionally, we would implement integration of these features
 * through C code as follows:
 *
 *	foo_init();
 *
 * You'd then have a foo.h which would have:
 *
 *  #ifdef CONFIG_FOO
 *  void foo(void);
 *  #else
 *  static inline void foo(void) { }
 *  #endif
 *
 * With linker tables this is no longer necessary as your init routines would
 * be implicit, you'd instead call:
 *
 *	call_init_fns();
 *
 * call_init_fns() would call all functions present in this init table and if
 * and only if foo.o gets linked in, then its initialisation function will be
 * called.
 *
 * The linker script takes care of assembling the tables for us. All of our
 * table sections have names of the format .tbl.NAME.NN. NAME designates the
 * data structure stored in the table. For instance if you had defined a
 * struct init_fn to data type your init sequences you would have a respective
 * init_fns table name declared as reference for these init sequences. NN is a
 * two-digit decimal number used to impose an "order level" upon the tables if
 * required. NN=00 is reserved for the symbol indicating "table start", and
 * NN=99 is reserved for the symbol indicating "table end". In order for the
 * call_init_fns() to work behind the scenes the custom linker script would
 * need to define the beginning of the table, the end of the table, and in
 * between it should use SORT() to give order-level effect. To account for
 * all of your struct init_fn init sequences, in your linker script you would
 * have:
 *
 *   .tbl            : {
 *	__tbl_start_init_fns = .;
 * 	*(SORT(.tbl.init_fns.*))
 *	__tbl_end_init_fns = .;
 *
 *	Other init tables can go here as well for different
 *	structures.
 *   }
 *
 * As an example, suppose that we want to create a "frobnicator"
 * feature framework, and allow for several independent modules to
 * provide frobnicating services. Then we would create a frob.h
 * header file containing e.g.
 *
 *   struct frobnicator {
 *      const char *name;
 *	void (*frob) (void);
 *   };
 *
 *   #define FROBNICATORS __table (struct frobnicator, "frobnicators")
 *   #define __frobnicator __table_entry (FROBNICATORS, 01)
 *
 * Any module providing frobnicating services would look something
 * like
 *
 *   #include "frob.h"
 *
 *   static void my_frob (void) {
 *	... Do my frobnicating
 *   }
 *
 *   struct frob my_frobnicator __frobnicator = {
 *	.name = "my_frob",
 *	.frob = my_frob,
 *   };
 *
 * The central frobnicator code (frob.c) would use the frobnicating
 * modules as follows
 *
 *   #include "frob.h"
 *
 *   void frob_all(void) {
 *	struct frob *frob;
 *
 *	for_each_table(frob, FROBNICATORS) {
 *         printf("Calling frobnicator \"%s\"\n", frob->name);
 *	   frob->frob();
 *	}
 *   }
 */

/*
 * __table - Declares a linker table
 *
 * @type: data type
 * @name: table name
 * 
 * Declares a linker table.
 */
#define __table(type, name) (type, name)

/*
 * __table_type() - get linker table data type
 *
 * @table: linker table
 *
 * Gives you the linker table data type.
 */
#define __table_type(table) __table_extract_type table
#define __table_extract_type(type, name) type

/*
 * __table_name - get linker table name
 *
 * @table: linker table
 *
 * Gives you the table name.
 */
#define __table_name(table) __table_extract_name table
#define __table_extract_name(type, name) name

/*
 * __table_section - get linker table section name
 *
 * @table: linker table
 * @idx: order level, this is the sub-table index
 *
 * Declares the section name.
 */
#define __table_section(table, idx) \
	".tbl." __table_name (table) "." __table_str (idx)
#define __table_str(x) #x

/*
 * __table_alignment - get linker table alignment
 *
 * @table: linker table
 *
 * Gives you the linker table alignment.
 */
#define __table_alignment( table ) __alignof__ (__table_type(table))

/*
 * __table_entry - declare a linker table entry
 *
 * @table: linker table
 * @idx: order level, the sub-table index
 *
 * Example usage:
 *
 *   #define FROBNICATORS __table (struct frobnicator, "frobnicators")
 *   #define __frobnicator __table_entry(FROBNICATORS, 01)
 *
 *   struct frobnicator my_frob __frobnicator = {
 *      ...
 *   };
 */
#define __table_entry(table, idx)					\
	__attribute__ ((__section__(__table_section(table, idx)),	\
			__aligned__(__table_alignment(table))))

/*
 * __table_entries - get start of linker table entries
 *
 * @table: linker table
 * @idx: order level, the sub-table index
 *
 * This gives you the start of the respective linker table entries
 */
#define __table_entries(table, idx) ( {					\
	static __table_type(table) __table_entries[0]			\
		__table_entry(table, idx); 				\
	__table_entries; } )

/*
 * table_start - get start of linker table
 *
 * @table: linker table
 *
 * This gives you the start of the respective linker table.
 *
 * Example usage:
 *
 *   #define FROBNICATORS __table (struct frobnicator, "frobnicators")
 *
 *   struct frobnicator *frobs = table_start(FROBNICATORS);
 */
#define table_start(table) __table_entries(table, 00)

/*
 * table_end - get end of linker table
 *
 * @table: linker table
 *
 * This gives you the end of the respective linker table.
 *
 * Example usage:
 *
 *   #define FROBNICATORS __table (struct frobnicator, "frobnicators")
 *
 *   struct frobnicator *frobs_end = table_end(FROBNICATORS);
 */
#define table_end(table) __table_entries(table, 99)

/*
 * table_num_entries - get number of entries in linker table
 *
 * @table: linker table
 *
 * This gives you the number of entries in linker table.
 *
 * Example usage:
 *
 *   #define FROBNICATORS __table(struct frobnicator, "frobnicators")
 *
 *   unsigned int num_frobs = table_num_entries(FROBNICATORS);
 */
#define table_num_entries(table)					\
	((unsigned int) (table_end(table) -				\
			 table_start(table)))

/*
 * for_each_table_entry - iterate through all entries within a linker table
 *
 * @pointer: entry pointer
 * @table: linker table
 *
 * Example usage:
 *
 *   #define FROBNICATORS __table(struct frobnicator, "frobnicators")
 *
 *   struct frobnicator *frob;
 *
 *   for_each_table_entry(frob, FROBNICATORS) {
 *     ...
 *   }
 */
#define for_each_table_entry(pointer, table)				\
	for (pointer = table_start(table);				\
	     pointer < table_end(table);				\
	     pointer++)

/**
 * for_each_table_entry_reverse - iterate through linker table in reverse order
 *
 * @pointer: entry pointer
 * @table: linker table
 *
 * Example usage:
 *
 *   #define FROBNICATORS __table(struct frobnicator, "frobnicators")
 *
 *   struct frobnicator *frob;
 *
 *   for_each_table_entry_reverse(frob, FROBNICATORS) {
 *     ...
 *   }
 */
#define for_each_table_entry_reverse(pointer, table)			\
	for (pointer = (table_end(table) - 1 );				\
	     pointer >= table_start(table);				\
	     pointer--)

/*
 *
 * Intel's C compiler chokes on several of the constructs used in this
 * file. The workarounds are ugly, so we use them only for an icc
 * build.
 */
#define ICC_ALIGN_HACK_FACTOR 128
#ifdef __ICC

/*
 * icc miscompiles zero-length arrays by inserting padding to a length
 * of two array elements. We therefore have to generate the
 * __table_entries() symbols by hand in asm.
 */
#undef __table_entries
#define __table_entries(table, idx) ( {					\
	extern __table_type(table)					\
		__table_temp_sym(idx, __LINE__) []			\
		__table_entry(table, idx) 				\
		asm (__table_entries_sym(table, idx));			\
	__asm__( ".ifndef %c0\n\t"					\
		  ".section " __table_section(table, idx) "\n\t"	\
		  ".align %c1\n\t"					\
	          "\n%c0:\n\t"						\
		  ".previous\n\t" 					\
		  ".endif\n\t"						\
		  : : "i" (__table_temp_sym(idx, __LINE__ )),		\
		      "i" (__table_alignment(table)));			\
	__table_temp_sym ( idx, __LINE__ ); } )
#define __table_entries_sym(table, idx)					\
	"__tbl_" __table_name(table) "_" #idx
#define __table_temp_sym(a, b)						\
	___table_temp_sym(__table_, a, _, b)
#define ___table_temp_sym(a, b, c, d) a ## b ## c ## d

/*
 * icc ignores __attribute__ (( aligned (x) )) when it is used to
 * decrease the compiler's default choice of alignment (which may be
 * higher than the alignment actually required by the structure).  We
 * work around this by forcing the alignment to a large multiple of
 * the required value (so that we are never attempting to decrease the
 * default alignment) and then postprocessing the object file to
 * reduce the alignment back down to the "real" value.
 *
 */
#undef __table_alignment
#define __table_alignment(table) \
	(ICC_ALIGN_HACK_FACTOR * __alignof__(__table_type(table)))

/*
 * Because of the alignment hack, we must ensure that the compiler
 * never tries to place multiple objects within the same section,
 * otherwise the assembler will insert padding to the (incorrect)
 * alignment boundary.  Do this by appending the line number to table
 * section names.
 *
 * Note that we don't need to worry about padding between array
 * elements, since the alignment is declared on the variable (i.e. the
 * whole array) rather than on the type (i.e. on all individual array
 * elements).
 */
#undef __table_section
#define __table_section(table, idx) \
	".tbl." __table_name(table) "." __table_str(idx) \
	"." __table_xstr(__LINE__)
#define __table_xstr(x) __table_str(x)

#endif /* __ICC */

#endif /* _LINUX_TABLES_H */
