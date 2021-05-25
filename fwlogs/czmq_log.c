#include "log.h"
#include <czmq_logc.h>


__attribute__((constructor))
static void log_constructor() {
	logc_czmq_init();
}

__attribute__((destructor))
static void log_destructor() {
	logc_czmq_cleanup();
}
