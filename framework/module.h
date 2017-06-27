/**
 * @file
 *
 * Modular programming framework to support runtime selectable
 * implementations for variant software subsystems.
 *
 * Multiple implementations of the same subsystem can be built
 * into individual static libraries or loadable DSOs, and use
 * constructor functions to register themselves.
 *
 * A subsystem can choose one active implementation and provide
 * APIs to switch between implementations.
 *
 * Alternatively, subsystem can load multiple implementations
 * and determine the APIs route in runtime.
 *
 * this framework tries to minimizes dependencies to the linked
 * list and rwlock facilities only.
 */

#ifndef MODULE_H
#define MODULE_H

#include "list.h"
#include "rwlock.h"

typedef struct {
	rwlock_t lock;
	uint32_t version;
	const char *description;
	struct list_head modules;
	struct list_head *active;
} subsystem_t;

/* Subsystem instance name */
#define subsystem(name) name ## _subsystem

/* The trick is to use macro SUBSYSTEM() for both subsystem
 * declaration and definition. ARGC() macro chooses either
 * SUBSYSTEM_DEFINE() or SUBSYSTEM_DECLARE() depends on argument
 * number,
 */
#define _ARGC(_0, _1, _2, _3, ...) _3
#define  ARGC(...) _ARGC(__VA_ARGS__, DEFINE, 2, DECLARE, 0)

#define _OVERLOAD(M, S, ...) M ## _ ## S(__VA_ARGS__)
#define  OVERLOAD(M, S, ...) _OVERLOAD(M, S, __VA_ARGS__)

#define SUBSYSTEM_DEFINE(name, _description, _version)		\
	subsystem_t name ## _subsystem = {			\
		.lock = RW_LOCK_UNLOCKED(lock),			\
		.version = _version,				\
		.description = _description,			\
	}

#define SUBSYSTEM_DECLARE(name) subsystem_t name ## _subsystem
#define SUBSYSTEM(...) OVERLOAD(SUBSYSTEM, ARGC(__VA_ARGS__), __VA_ARGS__)

/* Subsystem API prototype name */
#define api_proto(subsystem, api) subsystem ##_## api ## _proto_t

/* Subsystem API declaration */
#define SUBSYSTEM_API(name, _return, api, ...) 			\
	extern _return name ##_## api(__VA_ARGS__);		\
	typedef _return (*name ##_## api ## _proto_t)		\
			(__VA_ARGS__)

/* Subsystem API stubs are weak */
#define SUBSYSTEM_API_STUB(name, api) 				\
	__attribute__((weak)) name ##_## api

/* In case subsystem API implementations are built as static
 * libraries or preload DSOs, one implementation can use this
 * macro to override the APIs weak stubs.
 */
#define SUBSYSTEM_API_OVERRIDE(name, api, _alias)		\
	__attribute__((alias(#_alias))) name ##_## api

#define subsystem_constructor(name) 				\
	do {							\
		rwlock_init(&name ## _subsystem.lock);	\
		INIT_LIST_HEAD(&name ## _subsystem.modules);	\
		name ## _subsystem.active = NULL;		\
	} while(0)

#define SUBSYSTEM_CONSTRUCTOR(name) 				\
	static void __attribute__((constructor(101)))		\
		name ## _subsystem_constructor(void)

#define subsystem_lock(access, name)				\
	rwlock_ ##access## _lock(&name ## _subsystem.lock)

#define subsystem_unlock(access, name)				\
	rwlock_ ##access## _unlock(&name ## _subsystem.lock)

#define subsystem_foreach_module(name, mod)			\
	list_for_each_entry(mod, &name ## _subsystem.modules, list)

#define MODULE_CLASS(subsystem)					\
	struct subsystem ## _module {				\
		struct list_head list;				\
		void *handler; /* DSO */			\
		const char *name;				\
		int (*init_local)(void);			\
		int (*term_local)(void);			\
		int (*init_global)(void);			\
		int (*term_global)(void);			\

/* Base class to all inherited subsystem module classes */
typedef MODULE_CLASS(base) } module_base_t;

/* Module constructors should be late than subsystem constructors,
 * in statically linked scenarios (both subsystems and modules are
 * linked statically). thus the priority 102 compared to the above
 * subsystem constructor priority 101.
 */
#define MODULE_CONSTRUCTOR(name) 				\
	static void __attribute__((constructor(102)))		\
		name ## _module_constructor(void)

/* Subsystem Modules Registration
 *
 * subsystem_register_module() are called by all modules in their
 * constructors, whereas the modules could be:
 *
 * 1) Linked statically or dynamically, and are loaded by program
 *    loader (execve) or dynamic linker/loader (ld.so)
 *
 *    subsystem_register_module() should complete the whole
 *    registration session and link the module into subsystem's
 *    module array.
 *
 * 2) Loaded by a module loader in runtime with libdl APIs
 *
 *    The whole registration session needs to be split to aim the
 *    module loader to properly handle dlopen() returns, and save
 *    the DSO handler into module's data structure.
 *
 *    The module loader should program in this way:
 *	module_loader_start();
 *	......
 * 	for each module
 *		handler = dlopen(module)
 *		-- the module constructor calls register_module()
 *		if (handler is valid)
 *			install_dso(handler);
 *		else
	 		abandon_dso();
 *      ......
 *	module_loader_end();
 */

extern void module_loader_start(void);
extern void module_loader_end(void);

extern int module_install_dso(void *);
extern int module_abandon_dso(void);

extern int __subsystem_register_module(subsystem_t *, module_base_t *);

/* Macro to allow polymorphism on module data structures */
#define subsystem_register_module(name, module)			  \
({								  \
	module_base_t *base = (module_base_t *)module;		  \
	__subsystem_register_module(&name ## _subsystem, base); \
})

#endif
