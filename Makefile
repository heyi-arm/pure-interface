CP := cp
AR := ar
CC := gcc
PWD := $(shell pwd)
CFLAGS := -Wall -Werror -g -I$(PWD) -I$(PWD)/framework

# The interface library compiles with modular framework and
# all subsystems and its own dynamic module loader (policies),
# while the modular framework only provides mechanisms to
# support variant dynamic module loader's implementation

INTERFACE_LIBRARY := libpure-interface.so

INTERFACE_SOURCES := module-loader.c       \
		     pktio-subsystem.c     \
		     scheduler-subsystem.c \
		     framework/module.c    \
		     framework/rwlock.c

$(INTERFACE_LIBRARY): $(patsubst %.c,%.po, $(INTERFACE_SOURCES))
	$(CC) $(CFLAGS) -shared -o $@ $^ -ldl

%.E: %.c
	$(CC) $(CFLAGS) -E -o $@ $<

%.po: %.c
	$(CC) $(CFLAGS) -fPIC -c -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

include modules/Makefile

empty:=
comma:= \",\"
space:= $(empty) $(empty)

all-modules.c: $(ALL_MODULES)
	echo "#include <stddef.h>" > $@
	echo "const char *all_modules[] = " \
		"{ $(subst $(space),$(comma),\"$^\"),NULL };" >> $@
empty-modules.c:
	echo "#include <stddef.h>" > $@
	echo "const char *all_modules[] = { NULL };" >> $@

main-plugin: main.o all-modules.o $(INTERFACE_LIBRARY)
	$(CC) $< all-modules.o -L"${PWD}" \
	  -Wl,-R"${PWD}" -lpure-interface -o $@

# In dynamic link use, constructor order was not implemented
# by __attribute__((constructor(priority))), seems problem with
# ld-linux.so!
#
# Workaround with command line link order, always put interface
# libraries (subsystems) at last and constructors will be invoked
# in reverse order.
main-dynamic: main.o empty-modules.o $(INTERFACE_LIBRARY) $(ALL_MODULES)
	$(CC) $< empty-modules.o -L"${PWD}" -L"${PWD}/modules" \
	  -Wl,-R"${PWD}/modules" -Wl,--no-as-needed $(ALL_MODULES) \
	  -Wl,-R"${PWD}" -Wl,--as-needed -lpure-interface -o $@

main-preload: main.o empty-modules.o $(INTERFACE_LIBRARY) $(ALL_MODULES_OVERRIDE)
	$(CC) $< empty-modules.o -L"${PWD}" \
	  -Wl,-R"${PWD}" -lpure-interface -o $@

main-static: main.o empty-modules.o \
	     $(INTERFACE_LIBRARY) $(ALL_MODULES_STATIC)
	$(CC) $< empty-modules.o -Wl,--whole-archive \
	  $(ALL_MODULES_STATIC) -Wl,--no-whole-archive \
	  -L"${PWD}" -Wl,-R"${PWD}" -lpure-interface -o $@

main-static-override: main.o empty-modules.o \
		      $(INTERFACE_LIBRARY) $(ALL_MODULES_STATIC_OVERRIDE)
	$(CC) $< empty-modules.o -Wl,--whole-archive \
	  $(ALL_MODULES_STATIC_OVERRIDE) -Wl,--no-whole-archive \
	  -L"${PWD}" -Wl,-R"${PWD}" -lpure-interface -o $@

clean:
	rm -f all-modules.c empty-modules.c
	rm -f main-plugin main-dynamic main-preload
	rm -f main-static main-static-override
	find $(PWD) -regex ".*\.\(po\|so\|o\|E\|a\)$$" -delete

.PHONY: all clean
