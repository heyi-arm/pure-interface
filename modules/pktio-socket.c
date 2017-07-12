#include <stdio.h>
#include <pure-interface.h>

/* socket pktio module to implement the pktio subsystem:
 * should work in both statically linked and DSO builds.
 */
static int socket_pktio_api_one(void)
{
	printf("socket pktio: api_one().\n");
	return 0;
}

static const char *socket_pktio_api_two(int input)
{
	printf("socket pktio: api_two(%d).\n", input);
	return NULL;
}

static int socket_pktio_init(void)
{
	printf("socket pktio module init.\n");
	return 0;
}

static int socket_pktio_term(void)
{
	printf("socket pktio module term.\n");
	return 0;
}

pktio_module_t socket_pktio = {
	.name = "socket pktio",
	.init = socket_pktio_init,
	.term = socket_pktio_term,
	.api_one = socket_pktio_api_one,
	.api_two = socket_pktio_api_two,
};

MODULE_CONSTRUCTOR(socket_pktio)
{
	module_constructor(&socket_pktio);

	printf("socket pktio module register.\n");
	subsystem_register_module(pktio, &socket_pktio);
}
