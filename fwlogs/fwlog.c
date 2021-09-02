#include "fwlog.h"
#include <errno.h>
#include <sys/socket.h>
#include <libnetfilter_log/libnetfilter_log.h>
#include <stdio.h>
#include "parser.h"
#include "log.h"

struct local_data {
	void *data;
	fwlog_callback callback;
};

static int local_callback(struct nflog_g_handle *gh, struct nfgenmsg *nfmsg,
		struct nflog_data *nfa, void *data) {
	struct local_data *local_data = data;

	char *payload;
	int payload_len = nflog_get_payload(nfa, &payload);
	local_data->callback(payload, payload_len, local_data->data);
	return 0;
}

void fwlog_run(uint16_t log_group, int flags, fwlog_callback callback, void *data) {
	struct nflog_handle *nflog = nflog_open();
	if (!nflog)
		critical("Unable to open connection to netfilter_log");
	if (nflog_bind_pf(nflog, PF_INET))
		critical("Unable to bind to IPv4 firewall");
	if (nflog_bind_pf(nflog, PF_INET6))
		error("Failed to bind to IPv6 firewall (continuing with IPv4 only)");

	struct nflog_g_handle *nflog_g = nflog_bind_group(nflog, log_group);
	if (flags & FWLOG_LIMIT)
		if (nflog_set_mode(nflog_g, NFULNL_COPY_PACKET, max_packet_size()) == -1)
			critical("Can't request packet copy mode");

	struct local_data local_data = {
		.data = data,
		.callback = callback,
	};
	nflog_callback_register(nflog_g, &local_callback, &local_data);
	int fd = nflog_fd(nflog);

	ssize_t rn;
	char buf[BUFSIZ];
	while ((rn = recv(fd, buf, sizeof(buf), 0)) && rn >= 0) {
		trace("Received data from nflog (len=%lu)", rn);
		if (nflog_handle_packet(nflog, buf, rn) < 0) {
			error("Packet handling failed");
			break;
		}
	}
	if (rn == -1 && errno != EINTR) // Note: we use interrupt for termination
		error("Unable to receive data from netfilter");

	nflog_unbind_group(nflog_g);
	nflog_close(nflog);
}
