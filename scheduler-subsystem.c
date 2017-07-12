#include <stdio.h>
#include "pure-interface.h"

/* Subsystem scheduler demonstrates the support for API symbols
 * override in static library or preload DSO builds of the
 * implementation module, to reduce overhead of locks and API
 * function pointers.
 */

#define SCHEDULER_SUBSYSTEM_VERSION 0x00010000UL
SUBSYSTEM(scheduler, "scheduler public APIs", SCHEDULER_SUBSYSTEM_VERSION);

int SUBSYSTEM_API_STUB(scheduler, api_one)(void)
{
	int result = 0;
	scheduler_module_t *module = NULL;

	printf("scheduler stub: api_one().\n");
	subsystem_lock(read, scheduler);

	/* scheduler API one only route to the active
	 * implementation.
	 */
	module = subsystem_active_module(scheduler, module);

	if (module != NULL && module->api_one != NULL)
		result = module->api_one();

	subsystem_unlock(read, scheduler);
	return result;
}

const char *SUBSYSTEM_API_STUB(scheduler, api_two)(int input)
{
	const char *result = NULL;
	scheduler_module_t *module = NULL;

	printf("scheduler stub: api_two(%d).\n", input);
	subsystem_lock(read, scheduler);

	/* scheduler API two iterates all registered modules
	 * until the implementation returns not-NULL result.
	 */
	subsystem_foreach_module(scheduler, module) {
		if (module->api_two != NULL) {
			result = module->api_two(input);
			if (result != NULL)
				break;
		}
	}

	subsystem_unlock(read, scheduler);
	return result;
}

SUBSYSTEM_CONSTRUCTOR(scheduler)
{
	subsystem_constructor(scheduler);

	/* Further initialization per subsystem */
	printf("scheduler subsystem constructor.\n");
}
