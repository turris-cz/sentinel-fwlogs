#ifndef _SENTINEL_FWLOGS_PARSER_H_
#define _SENTINEL_FWLOGS_PARSER_H_
#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include <sys/param.h>
#include <arpa/inet.h>

#define ADDRSTRLEN (MAX(INET6_ADDRSTRLEN, INET_ADDRSTRLEN) + 1)

struct packet_data {
	time_t ts;
	const char *proto;
	char source_ip[ADDRSTRLEN];
	char dest_ip[ADDRSTRLEN];
	unsigned source_port, dest_port;
};

// Returns maximum required szie of data needed to parse packet_data. This can be
// used to limit amount of data copied from kernel.
size_t max_packet_size();

// Parse provided packet to packet_data structure
// Returns true on success and false otherwise. packet_data has valid content only
// if true was returned.
bool parse_packet(const void *data, size_t data_size, struct packet_data *packet_data);

#endif

