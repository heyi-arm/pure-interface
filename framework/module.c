#include <stdio.h>
#include <errno.h>
#include "module.h"

#define MODULE_FRAMEWORK_VERSION 0x00010000UL
SUBSYSTEM(module, "module framework", MODULE_FRAMEWORK_VERSION);

/* Keep it simple, allow one registration session at a time. */
static struct {
	rwlock_t lock;
	subsystem_t *subsystem;
	module_base_t *module;
} registration = {
	.lock = RW_LOCK_UNLOCKED,
	.subsystem = NULL,
	.module = NULL,
};

#define REGISTRATION_SANITY_CHECK(subsystem, module)		\
do {								\
	if (subsystem == NULL || module == NULL)		\
		return -ENOENT;					\
								\
	if (!list_node_detached(&module->list)) {		\
		printf("module %s was already registered.\n",	\
			module->name);				\
		return -EAGAIN;					\
	}							\
} while (0)

/* Module is linked statically or dynamically, and are loaded by
 * program loader (execve) or dynamic linker/loader (ld.so)
 *
 * subsystem_register_module() should complete the whole registration
 * session and link the module into subsystem's module array.
 */
static int linker_register_module(
	subsystem_t *subsystem, module_base_t *module)
{
	REGISTRATION_SANITY_CHECK(subsystem, module);

	/* Allow one registration session at a time */
	rwlock_write_lock(&registration.lock);

	/* Block the subsystem API calls in load new
	 * implementation modules. */
	rwlock_write_lock(&subsystem->lock);
	module->handler = NULL; /* no DSO handler */
	list_add_tail(&subsystem->modules, &module->list);
	rwlock_write_unlock(&subsystem->lock);

	rwlock_write_unlock(&registration.lock);
	return 0;
}

static int (*do_register_module)(subsystem_t *, module_base_t *)
		= &linker_register_module;

static int loader_register_module(
	subsystem_t *subsystem, module_base_t *module)
{
	REGISTRATION_SANITY_CHECK(subsystem, module);

	/* Registration session lock must be held by
	 * module_loader_start(). */
	if (rwlock_write_trylock(&registration.lock) == 0) {
		registration.subsystem = subsystem;
		registration.module = module;
		return 0;
	}

	rwlock_write_unlock(&registration.lock);
	return -EACCES;
}

void module_loader_start(void)
{
	rwlock_write_lock(&registration.lock);

	if (registration.module != NULL ||
	    registration.subsystem != NULL) {
		printf("module loader start warn, A previous "
		       "registration did not complete yet.\n");
	}

	registration.module = NULL;
	registration.subsystem = NULL;
	do_register_module = &loader_register_module;
}

void module_loader_end(void)
{
	if (registration.module != NULL ||
	    registration.subsystem != NULL) {
		printf("module loader end warn, A previous "
		       "registration did not complete yet.\n");
	}

	registration.module = NULL;
	registration.subsystem = NULL;
	do_register_module = &linker_register_module;

	rwlock_write_unlock(&registration.lock);
}

int module_install_dso(void *dso, bool active)
{
	/* Bottom halves of the registration, context exclusion
	 * is guaranteed by module_loader_start()
	 */
	if (0 == rwlock_write_trylock(&registration.lock)) {
		subsystem_t *subsystem = registration.subsystem;
		module_base_t *module = registration.module;

		if (subsystem != NULL && module != NULL) {
			rwlock_write_lock(&subsystem->lock);

			module->handler = dso;
			list_add_tail(&subsystem->modules, &module->list);

			/* install as active implementation */
			if (active) /* warn: replaceable */
				subsystem->active = &module->list;

			rwlock_write_unlock(&subsystem->lock);
		}

		registration.subsystem = NULL;
		registration.module = NULL;
		return 0;
	}

	rwlock_write_unlock(&registration.lock);
	return -EACCES;
}

int module_abandon_dso(void)
{
	/* Bottom halves of the registration, context exclusion
	 * is guaranteed by module_loader_start()
	 */
	if (0 == rwlock_write_trylock(&registration.lock)) {
		registration.subsystem = NULL;
		registration.module = NULL;
		return 0;
	}

	rwlock_write_unlock(&registration.lock);
	return -EACCES;
}

int __subsystem_register_module(
	subsystem_t *subsystem, module_base_t *module)
{
	return do_register_module(subsystem, module);
}
