#include <stdio.h>
#include "pure-interface.h"

/* Subsystem pktio demonstrates mutliple implementations support
 * in dynamic loadable module forms (DSOs), and subsystem API
 * stubs can route the calls to either implementation in runtime.
 */

#define PKTIO_SUBSYSTEM_VERSION 0x00010000UL
SUBSYSTEM(pktio, "pktio public APIs", PKTIO_SUBSYSTEM_VERSION);

int SUBSYSTEM_API_STUB(pktio, api_one)(void)
{
	int result = 0;
	pktio_module_t *module = NULL;

	printf("pktio stub: api_one().\n");
	subsystem_lock(read, pktio);

	/* pktio API one only route to the active
	 * implementation.
	 */
	module = subsystem_active_module(pktio, module);

	if (module != NULL && module->api_one != NULL)
		result = module->api_one();

	subsystem_unlock(read, pktio);
	return result;
}

const char *SUBSYSTEM_API_STUB(pktio, api_two)(int input)
{
	const char *result = NULL;
	pktio_module_t *module = NULL;

	printf("pktio stub: api_two(%d).\n", input);
	subsystem_lock(read, pktio);

	/* pktio API two iterates all registered modules
	 * until the implementation returns not-NULL result.
	 */
	subsystem_foreach_module(pktio, module) {
		if (module->api_two != NULL) {
			result = module->api_two(input);
			if (result != NULL)
				break;
		}
	}

	subsystem_unlock(read, pktio);
	return result;
}

SUBSYSTEM_CONSTRUCTOR(pktio)
{
	subsystem_constructor(pktio);

	/* Further initialization per subsystem */
	printf("pktio subsystem constructor.\n");
}
