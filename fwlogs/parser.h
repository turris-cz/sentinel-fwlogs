#ifndef _SENTINEL_FWLOGS_PARSER_H_
#define _SENTINEL_FWLOGS_PARSER_H_
#include <time.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/param.h>
#include <arpa/inet.h>

#define ADDRSTRLEN (MAX(INET6_ADDRSTRLEN, INET_ADDRSTRLEN) + 1)

struct packet_data {
	time_t ts;
	const char *proto;
	char source_ip[ADDRSTRLEN];
	char dest_ip[ADDRSTRLEN];
	uint16_t source_port, dest_port;
};

// Returns maximum required szie of data needed to parse packet_data. This can be
// used to limit amount of data copied from kernel.
size_t max_packet_size();

// Parse provided packet to packet_data structure
// Returns true on success and false otherwise. packet_data has valid content only
// if true was returned.
bool parse_packet(const void *packet, size_t packet_len,
	struct packet_data *packet_data) __attribute__((nonnull));

#endif

