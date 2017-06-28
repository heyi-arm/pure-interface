#include <stdio.h>
#include <dlfcn.h>
#include "module.h"

void dynamic_module_loader(const char *modules[])
{
	void *handler = NULL;
	const char **name = NULL;

	module_loader_start();

	/* Use RTLD_NOW to avoid lazy resolution, since typically
	 * modules do not have external references at all.
	 */
	for (name = &modules[0]; *name != NULL; name++) {
		handler = dlopen(*name, RTLD_NOW | RTLD_GLOBAL);
		if (handler != NULL)
			module_install_dso(handler);
		else
			module_abandon_dso();
	}

	module_loader_end();
}
