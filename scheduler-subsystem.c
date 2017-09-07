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
	scheduler_module_t *module = NULL;

	/* API stub routes the API call to the default(active)
	 * implementation module. */
	module = subsystem_active_module(scheduler, module);

	if (module != NULL && module->api_one != NULL)
		result = module->api_one();
	...
}

const char *SUBSYSTEM_API_STUB(scheduler, api_two)(int input)
{
	scheduler_module_t *module = NULL;

	/* API stub iterates the API call through all
	 * implementation module. */
	subsystem_foreach_module(scheduler, module) {
		if (module->api_two != NULL)
			result = module->api_two(input);
	}
	...
}

SUBSYSTEM_CONSTRUCTOR(scheduler)
{
	subsystem_constructor(scheduler);

	/* Further initialization per subsystem */
	printf("scheduler subsystem constructor.\n");
}
