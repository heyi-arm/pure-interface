#ifndef PURE_INTERFACE_H
#define PURE_INTERFACE_H

#include "module.h"

/* Subsystems and APIs declaration */
extern SUBSYSTEM(scheduler);

SUBSYSTEM_API(scheduler, int, api_one, void);
SUBSYSTEM_API(scheduler, const char *, api_two, int);

typedef MODULE_CLASS(scheduler)
	api_proto(scheduler, api_one) api_one;
	api_proto(scheduler, api_two) api_two;
} scheduler_module_t;

extern SUBSYSTEM(pktio);

SUBSYSTEM_API(pktio, int, api_one, void);
SUBSYSTEM_API(pktio, const char *, api_two, int);

typedef MODULE_CLASS(pktio)
	api_proto(pktio, api_one) api_one;
	api_proto(pktio, api_two) api_two;
} pktio_module_t;

#endif
