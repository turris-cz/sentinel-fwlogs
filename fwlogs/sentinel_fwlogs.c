#include <stdlib.h>
#include <stdio.h>
#include "log.h"
#include "config.h"
#include "fwlog.h"
#include "parser.h"


static bool callback(uint8_t *payload, size_t payload_len, void *data) {
	sender_t sender = data;

	struct packet_data dt;
	if (parse_packet(payload, payload_len, &dt)) {
		debug("Proto: %s | Source: %s :%d | Destination: %s :%d | Time: %ld",
				dt.proto, dt.source_ip, dt.source_port, dt.dest_ip, dt.dest_port,
				dt.ts);
		if (!sender_send(sender, &dt))
			return false;
	}
	return true;
}


int main(int argc, char **argv) {
	struct config *conf = parse_args(argc, argv);

	sender_t sender = sender_new(conf->socket, conf->topic);
	if (!sender)
		critical("Unable to initialize ZMQ socket, probably invalid socket was provided");

	fwlog_run(conf->nflog_group, FWLOG_LIMIT, &callback, sender);

	sender_destroy(sender);
	return 0;
}
