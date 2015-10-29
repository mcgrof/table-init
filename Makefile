CFLAGS += -O2 -g
CFLAGS += -std=gnu99 -Wall -Werror

CFLAGS +=-Wl,-Tcustom.lds.S

ifeq ($(V),1)
	Q=
	NQ=@true
else
	Q=@
	NQ=@echo
endif

all: main

%.o: %.c *.h
	$(NQ) '  CC  ' $@
	$(Q)$(CC) -c $(CPPFLAGS) $(CFLAGS) -o $@ $<


OBJS =  sort-init.o \
	memory.o \
	kasan.o\
	init.o \
	pci.o \
	driver.o \
	xen.o \
	xen-driver.o \
	main.o

main: $(OBJS)
	$(NQ) '  CC  ' $@
	$(Q)$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $^

clean:
	rm -f main *.o
