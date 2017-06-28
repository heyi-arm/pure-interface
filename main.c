#include <stdio.h>
#include "pure-interface.h"

extern const char *all_modules[];
extern void dynamic_module_loader(const char *[]);

SUBSYSTEM_INITERM_TEMPLATE(pktio, init, printf)
SUBSYSTEM_INITERM_TEMPLATE(scheduler, init, printf)

SUBSYSTEM_INITERM_TEMPLATE(pktio, term, printf)
SUBSYSTEM_INITERM_TEMPLATE(scheduler, term, printf)

int main(int argc, char *argv[])
{
	dynamic_module_loader(all_modules);

	pktio_subsystem_init();
	scheduler_subsystem_init();

	scheduler_api_one();
	scheduler_api_two(10);

	pktio_api_one();
	pktio_api_two(10);

	pktio_subsystem_term();
	scheduler_subsystem_term();
	return 0;
}
