#include <stdio.h>
#include <pure-interface.h>

/* Default scheduler module to implement the scheduler subsystem:
 * should work in both statically linked and DSO builds.
 */
static int default_scheduler_api_one(void)
{
	printf("default scheduler: api_one().\n");
	return 0;
}

static const char *default_scheduler_api_two(int input)
{
	printf("default scheduler: api_two(%d).\n", input);
	return NULL;
}

static int default_scheduler_init(void)
{
	printf("default scheduler module init.\n");
	return 0;
}

static int default_scheduler_term(void)
{
	printf("default scheduler module term.\n");
	return 0;
}

scheduler_module_t default_scheduler = {
	.name = "default scheduler",
	.init = default_scheduler_init,
	.term = default_scheduler_term,
	.api_one = default_scheduler_api_one,
	.api_two = default_scheduler_api_two,
};

MODULE_CONSTRUCTOR(default_scheduler)
{
	module_constructor(&default_scheduler);
	...
	subsystem_register_module(scheduler, &default_scheduler);
}
