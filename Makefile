CFLAGS += -O2 -g
CFLAGS += -std=gnu99 -Wall -Werror
CFLAGS += -I include/ -I arch/x86/include/

CFLAGS +=-Wl,-Tcustom.lds.S

ifeq ($(V),1)
	Q=
	NQ=@true
else
	Q=@
	NQ=@echo
endif

all: main parse-bzimage

%.o: %.c *.h
	$(NQ) '  CC  ' $@
	$(Q)$(CC) -c $(CPPFLAGS) $(CFLAGS) -o $@ $<

parse-bzimage: parse-bzimage.c
	$(NQ) '  CC  ' $@
	$(Q)$(CC) -o $@ $<

# Simply placing alpha before beta will ensure
# alpha runs prior to beta. This is expected given
# SORT() for the init tables work on the order level
# defined only, ie:
#
#                 order level
#                      |
#                      V
# *(SORT(.tbl.init_fns.*))
#
# The name of the struct places no effect to the actual linker order.
OBJS =  sort-init.o \
	start_kernel.o \
	x86.o \
	memory.o \
	kasan.o\
	arch/x86/kernel/init.o \
	pci.o \
	beta.o \
	alpha.o \
	driver.o \
	xen.o \
	xen-driver.o \
	main.o

main: $(OBJS)
	$(NQ) '  CC  ' $@
	$(Q)$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $^

clean:
	rm -f main parse-bzimage *.o
