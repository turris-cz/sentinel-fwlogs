#ifndef _SENTINEL_FWLOGS_FWLOG_H_
#define _SENTINEL_FWLOGS_FWLOG_H_
#include <stdint.h>
#include <stddef.h>

#define FWLOG_LIMIT (1 << 0) // Request only max_packet_size() to be copied from kernel


typedef void (*fwlog_callback)(uint8_t *payload, size_t payload_len, void *data)
	__attribute__((nonnull(1)));

// This function connects to Firewall log and calls callback for every packet
// received from given log_group.
void fwlog_run(uint16_t log_group, int flags, fwlog_callback callback, void *data)
	__attribute__((nonnull(3)));

#endif
