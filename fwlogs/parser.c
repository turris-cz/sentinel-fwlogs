#include "parser.h"
#include <strings.h>
#include <netinet/ip.h>
#include <netinet/ip6.h>
#include <netinet/udp.h>
#include <netinet/tcp.h>
#include <libnetfilter_log/libnetfilter_log.h>
#include "log.h"


size_t max_packet_size() {
	return \
		MAX(sizeof(struct ip), sizeof(struct ip6_hdr)) +
		MAX(sizeof(struct tcphdr), sizeof(struct udphdr));
}

#include "protocol2str.c"


static bool handle_ipv4(const char *payload, size_t payload_size, struct packet_data *dt) {
	const struct ip *p = (struct ip*)payload;

	dt->proto = protocol2str(p->ip_p);
	inet_ntop(AF_INET, &p->ip_src, dt->source_ip, sizeof(dt->source_ip));
	inet_ntop(AF_INET, &p->ip_dst, dt->dest_ip, sizeof(dt->dest_ip));

	const void *phdr = payload + (4 * p->ip_hl);
	switch (p->ip_p) {
		case IPPROTO_UDP: {
			if (payload_size < (sizeof(struct ip) + sizeof(struct udphdr)))
				return false;
			const struct udphdr *udp = (struct udphdr*)phdr;
			dt->source_port = ntohs(udp->source);
			dt->dest_port = ntohs(udp->dest);
			break;
		}
		case IPPROTO_TCP: {
			if (payload_size < (sizeof(struct ip) + sizeof(struct tcphdr)))
				return false;
			const struct tcphdr *tcp = (struct tcphdr*)phdr;
			dt->source_port = ntohs(tcp->source);
			dt->dest_port = ntohs(tcp->dest);
			break;
		}
		default:
			dt->source_port = 0;
			dt->dest_port = 0;
	}
	return true;
}

static bool handle_ipv6(const void *payload, size_t payload_size, struct packet_data *dt) {
	if (payload_size < sizeof(struct ip6_hdr))
		return false;
	// TODO what if there is more than one header? We do not go trough them but we
	// should support them
	const struct ip6_hdr *p = (struct ip6_hdr*)payload;

	dt->proto = protocol2str(p->ip6_nxt);

	inet_ntop(AF_INET6, &p->ip6_src, dt->source_ip, sizeof(dt->source_ip));
	inet_ntop(AF_INET6, &p->ip6_dst, dt->dest_ip, sizeof(dt->dest_ip));
	
	// TODO for TCP and UDP ports (where are those?)
	dt->source_port = 0;
	dt->dest_port = 0;
	return true;
}


bool parse_packet(const void *data, size_t data_size, struct packet_data *packet_data) {
	if (data_size < sizeof(struct ip))
		goto invalid_size;

	struct ip *p_ip = (struct ip*)data;
	switch (p_ip->ip_v) {
		case 4:
			if (!handle_ipv4(data, data_size, packet_data))
				goto invalid_size;
			break;
		case 6:
			if (!handle_ipv6(data, data_size, packet_data))
				goto invalid_size;
			break;
		default:
			DEBUG("Received packet with unknown IP version: %d", p_ip->ip_v);
			return 0;
	}
	// Note: libnetfilter_log provides nflog_get_timestamp. It seems that info is
	// not either provided by kernel or that it is just implemented invalidly. It
	// always fails to receive timestamp. So instead we just use current time.
	// That is not exact because of buffering in kernel but should be good enough.
	packet_data->ts = time(NULL);
	return true;

invalid_size:
	DEBUG("Received packet has smaller size than expected IP header. Ignoring");
	return false;
}