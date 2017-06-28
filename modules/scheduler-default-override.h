#ifndef SCHEDULER_DEFAULT_OVERRIDE_H
#define SCHEDULER_DEFAULT_OVERRIDE_H

#include "module.h"

/* The header file serves to override the subsystem API symbols
 * directly with module implementations in static linked or pre-
 * load DSO builds.
 *
 * The inclusion of this header should be controlled by build
 * system as it has the knowledge of compiler-time configurations
 * of which implementation module will be built into static
 * library or preload DSO and will override the subsystem APIs.
 */

extern int SUBSYSTEM_API_OVERRIDE(
	scheduler, api_one, default_scheduler_api_one)(void);

extern const char *SUBSYSTEM_API_OVERRIDE(
	scheduler, api_two, default_scheduler_api_two)(int);

#endif
