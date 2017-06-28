# The modules to implement subsystems are either built into
# static libraries or DSOs

PKTIO_LOOPBACK := modules/libpktio-loopback.so
PKTIO_LOOPBACK_SOURCES := modules/pktio-loopback.c
ALL_MODULES += $(PKTIO_LOOPBACK)

PKTIO_SOCKET := modules/libpktio-socket.so
PKTIO_SOCKET_SOURCES := modules/pktio-socket.c
ALL_MODULES += $(PKTIO_SOCKET)

SCHEDULER_DEFAULT := modules/libscheduler-default.so
SCHEDULER_DEFAULT_SOURCES := modules/scheduler-default.c
ALL_MODULES += $(SCHEDULER_DEFAULT)

ALL_MODULES_STATIC := $(patsubst %.so,%.a, $(ALL_MODULES))
ALL_MODULES_OVERRIDE := $(patsubst %.so,%-override.so, $(ALL_MODULES))
ALL_MODULES_STATIC_OVERRIDE := $(patsubst %.so,%.a, $(ALL_MODULES_OVERRIDE))

%-override.E: %.c %-override.h
	$(CC) $(CFLAGS) --include "$*-override.h" -fPIC -E -o $@ $<

%-override.po: %.c %-override.h
	$(CC) $(CFLAGS) --include "$*-override.h" -fPIC -c -o $@ $<

%-override.o: %.c %-override.h
	$(CC) $(CFLAGS) --include "$*-override.h" -c -o $@ $<

# default scheduler module
modules/libscheduler-default.a: \
	$(patsubst %.c,%.o, $(SCHEDULER_DEFAULT_SOURCES))
	$(AR) rcs $@ $^

modules/libscheduler-default.so: \
	$(patsubst %.c,%.po, $(SCHEDULER_DEFAULT_SOURCES))
	$(CC) $(CFLAGS) -shared -o $@ $^

# module is configured to override subsystem APIs.
modules/libscheduler-default-override.so: \
	$(patsubst %.c,%-override.po, $(SCHEDULER_DEFAULT_SOURCES))
	$(CC) $(CFLAGS) -shared -o $@ $^

modules/libscheduler-default-override.a: \
	$(patsubst %.c,%-override.o, $(SCHEDULER_DEFAULT_SOURCES))
	$(AR) rcs $@ $^

# loopback pktio module
modules/libpktio-loopback.a: \
	$(patsubst %.c,%.o, $(PKTIO_LOOPBACK_SOURCES))
	$(AR) rcs $@ $^

modules/libpktio-loopback.so: \
	$(patsubst %.c,%.po, $(PKTIO_LOOPBACK_SOURCES))
	$(CC) $(CFLAGS) -shared -o $@ $^

modules/libpktio-loopback-override.so: \
	$(patsubst %.c,%-override.po, $(PKTIO_LOOPBACK_SOURCES))
	$(CC) $(CFLAGS) -shared -o $@ $^

modules/libpktio-loopback-override.a: \
	$(patsubst %.c,%-override.o, $(PKTIO_LOOPBACK_SOURCES))
	$(AR) rcs $@ $^

# socket pktio module
modules/libpktio-socket.a: \
	$(patsubst %.c,%.o, $(PKTIO_SOCKET_SOURCES))
	$(AR) rcs $@ $^

modules/libpktio-socket.so: \
	$(patsubst %.c,%.po, $(PKTIO_SOCKET_SOURCES))
	$(CC) $(CFLAGS) -shared -o $@ $^

# module is configured to override subsystem APIs.
modules/libpktio-socket-override.so: \
	$(patsubst %.c,%-override.po, $(PKTIO_SOCKET_SOURCES))
	$(CC) $(CFLAGS) -shared -o $@ $^

modules/libpktio-socket-override.a: \
	$(patsubst %.c,%-override.o, $(PKTIO_SOCKET_SOURCES))
	$(AR) rcs $@ $^
