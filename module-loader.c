#include <stdio.h>
#include <dlfcn.h>
#include "module.h"

void dynamic_module_loader(const char *modules[])
{
	module_loader_start();
	...
	for (name = &modules[0]; *name != NULL; name++) {
		handler = dlopen(*name, RTLD_NOW | RTLD_GLOBAL);
		if (handler != NULL)
			/* Install the DSO handler */
			module_install(handler, false /* Select as default or not */);
		else
			module_abandon();
	}
	module_loader_end();
}
