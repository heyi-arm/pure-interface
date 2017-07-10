# The modules to implement subsystems are either built into
# static libraries or DSOs

PKTIO_LOOPBACK := modules/libpktio-loopback.so
pktio_loopback_sources := modules/pktio-loopback.c
pktio_loopback_override_sources := modules/pktio-loopback-override.c
ALL_MODULES += $(PKTIO_LOOPBACK)

PKTIO_SOCKET := modules/libpktio-socket.so
pktio_socket_sources := modules/pktio-socket.c
pktio_socket_override_sources := modules/pktio-socket-override.c
ALL_MODULES += $(PKTIO_SOCKET)

# choose loopback pktio as the active implementation
modules/pktio-loopback%o: CFLAGS += -DIM_ACTIVE_MODULE

SCHEDULER_DEFAULT := modules/libscheduler-default.so
scheduler_default_sources := modules/scheduler-default.c
scheduler_default_override_sources := modules/scheduler-default-override.c
ALL_MODULES += $(SCHEDULER_DEFAULT)

# choose default scheduler as the active implementation
modules/scheduler-default%o: CFLAGS += -DIM_ACTIVE_MODULE

ALL_MODULES_STATIC := $(patsubst %.so,%.a, $(ALL_MODULES))
ALL_MODULES_DYNAMIC := $(patsubst modules/lib%.so,-l%, $(ALL_MODULES))
ALL_MODULES_OVERRIDE := $(patsubst %.so,%-override.so, $(ALL_MODULES))
ALL_MODULES_STATIC_OVERRIDE := $(patsubst %.so,%.a, $(ALL_MODULES_OVERRIDE))

%-override.E: %.c %-override.h
	$(CC) $(CFLAGS) --include "$*-override.h" -fPIC -E -o $@ $<

# in case lib%-override.h exists
%-override.po: %.c %-override.h
	$(CC) $(CFLAGS) --include "$*-override.h" -fPIC -c -o $@ $<

# in case lib%-override.h does not exist, rule order matters here
%-override.po: %.po
	$(CP) -f $< $@

%-override.o: %.c %-override.h
	$(CC) $(CFLAGS) --include "$*-override.h" -c -o $@ $<

%-override.o: %.o
	$(CP) -f $< $@

PC := %
.SECONDEXPANSION:

modules/lib%.a: $$(patsubst $$(PC).c,$$(PC).o, \
		$$($$(subst -,_,$$*_sources)))
	$(AR) rcs $@ $^

modules/lib%.so: $$(patsubst $$(PC).c,$$(PC).po, \
		 $$($$(subst -,_,$$*_sources)))
	$(CC) $(CFLAGS) -shared -o $@ $^
