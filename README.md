# pure-interface

### This project experiments Modular Programming Framework

Modular Programming Framework supports runtime selectable
implementations for variant software subsystems.

Multiple implementations of the same subsystem can be built
into individual static libraries or loadable DSOs, and use
constructor functions to register themselves.

A subsystem can choose one active implementation and provide
APIs to switch between implementations.

Alternatively, subsystem can load multiple implementations
and determine the APIs route in runtime.

Also in need to pursue extreme performance the subsystem
can choose one specific implementation module and build it
to override subsystem API symbols directly, thus eliminate
one level indirection of API calls through function pointers.

This framework tries to minimizes dependencies to the linked
list and rwlock facilities only.

### Abstractions: (to write a software library)

* The software library can be partitioned into multiple subsystems
* Each subsystem represents a view of the component and provides APIs to
  manipulate the component in that 'view'
  * Example: scheduler is the component, we can:
  * Create subsystem(scheduler) to provide public APIs, and
  * Create subsystem(scheduler_internal) to provide accelerated internal access APIs
* Each subsystem can have multiple implementations, subsystem implementations
  are written as modules
  * Modules can be built into static libraries or DSOs (loadable modules)
* Subsystem APIs can route the call to either implementation modules in runtime
  * This API route has overheads as read locks and pointer references
  * Modules built in static libraries or DSOs can override subsystem APIs

### Sample: pure-interface software library

* Two subsystems (both have two APIs):
  * Scheduler has one implementation: scheduler-default
  * Pktio has two implementations: pktio-loop and pktio-socket

* Build targets (to demonstrate):
  * main (with DSOs)
    * make main
    * run ./main
    * The dynamic module loader loads all modules through libdl.so APIs
      and the subsystem API stubs route the calls to the loaded modules
  * main-preload (with override DSOs)
    * make main-preload
    * make modules/libscheduler-default-override.so
    * run PRE_LOAD=modules/libscheduler-default-override.so ./main-preload
    * The preloaded DSO will override the scheduler subsystem API symbols
      directly and eliminate one level indirection of API calls through
      function pointers.
  * main-static
    * make main-static
    * run ./main-static
    * The static built modules are compiled in and the subsystem API stubs
      route the calls to the static modules.
  * main-static-overload
    * make main-static-override
    * run ./main-static-override
    * The static built module will override the scheduler subsystem API
      symbols directly and eliminate one level indirection of API calls
      through function pointers.
