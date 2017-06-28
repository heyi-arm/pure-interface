#include <stdio.h>
#include <pure-interface.h>

/* loopback pktio module to implement the pktio subsystem:
 * should work in both statically linked and DSO builds.
 */
static int loopback_pktio_init(void)
{
	printf("loopback pktio module init.\n");
	return 0;
}

static int loopback_pktio_term(void)
{
	printf("loopback pktio module term.\n");
	return 0;
}

pktio_module_t loopback_pktio = {
	.name = "loopback pktio",
	.init = loopback_pktio_init,
	.term = loopback_pktio_term,
	.api_one = NULL,
	.api_two = NULL,
};

MODULE_CONSTRUCTOR(loopback_pktio)
{
	INIT_LIST_HEAD(&loopback_pktio.list);

	printf("loopback pktio module register.\n");
	subsystem_register_module(pktio, &loopback_pktio);
}
