CFLAGS += -O2 -g
CFLAGS += -std=gnu99 -Wall -Werror

CFLAGS +=-Wl,-Tcustom.lds

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

main: \
	foo.o\
	bar.o\
	init.o \
	main.o
	$(NQ) '  CC  ' $@
	$(Q)$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $^

clean:
	rm -f main *.o
