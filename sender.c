#include "sender.h"
#include <czmq.h>
#include <msgpack.h>
#include "log.h"

struct sender {
	char *topic;
	zsock_t *sock;
};


sender_t sender_new(const char *socket, const char *topic) {
	sender_t sender = malloc(sizeof *sender);
	sender->topic = strdup(topic);
	sender->sock = zsock_new(ZMQ_PUSH);
	if (zsock_connect(sender->sock, "%s", socket) == 0)
		return sender;

	zsock_destroy(&sender->sock);
	free(sender);
	return NULL;
}

#define PACK_STR(packer, str) do { \
	msgpack_pack_str(packer, strlen(str));\
	msgpack_pack_str_body(packer, str, strlen(str)); \
	} while(0)

bool sender_send(sender_t sender, struct packet_data *data) {
	// TODO possibly make it so we use same sbuffer and packer
	msgpack_sbuffer sbuf;
	msgpack_packer pk;
	msgpack_sbuffer_init(&sbuf);
	msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

	msgpack_pack_map(&pk, 6);
	PACK_STR(&pk, "ts");
	msgpack_pack_long_long(&pk, data->ts);
	PACK_STR(&pk, "protocol");
	PACK_STR(&pk, data->proto);
	PACK_STR(&pk, "ip");
	PACK_STR(&pk, data->source_ip);
	PACK_STR(&pk, "port");
	msgpack_pack_unsigned_int(&pk, data->source_port);
	PACK_STR(&pk, "local_ip");
	PACK_STR(&pk, data->dest_ip);
	PACK_STR(&pk, "local_port");
	msgpack_pack_unsigned_int(&pk, data->dest_port);

	zmsg_t *msg = zmsg_new();
	if (zmsg_addstr(msg, sender->topic) ||
			zmsg_addmem(msg, sbuf.data, sbuf.size) ||
			zmsg_send(&msg, sender->sock)) {
		// TODO reconnect? And reason?
		ERROR("Submit to ZMQ failed");
		msgpack_sbuffer_destroy(&sbuf);
		zmsg_destroy(&msg);
		return false;
	}

	msgpack_sbuffer_destroy(&sbuf);
	return true;
}

void sender_destroy(sender_t sender) {
	zsock_destroy(&sender->sock);
	free(sender->topic);
	free(sender);
}
