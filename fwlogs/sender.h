#ifndef _SENTINEL_FWLOGS_SENDER_H_
#define _SENTINEL_FWLOGS_SENDER_H_
#include <stdbool.h>
#include "parser.h"

struct sender;
typedef struct sender* sender_t;

// Allocate and initialize new sender
// Returns sender_t instance or NULL in case of of invalid socket
sender_t sender_new(const char *socket, const char *topic)
	__attribute__((malloc,nonnull));

// Send packet_data with given sender
// Returns true on success and false otherwise
bool sender_send(sender_t, struct packet_data*) __attribute__((nonnull));

// Free sender instance resources
void sender_destroy(sender_t) __attribute__((nonnull));

#endif

