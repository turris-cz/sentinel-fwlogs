#ifndef _SENTINEL_FWLOGS_CONFIG_H_
#define  _SENTINEL_FWLOGS_CONFIG_H_
#include <stdbool.h>
#include <stdint.h>

struct config {
	const char *config_file;
	const char *socket;
	const char *topic;
	uint16_t nflog_group;
};


// Parse arguments and load configuration file
// Returned pointer is to statically allocated (do not call free on it).
struct config *parse_args(int argc, char **argv);

#endif
