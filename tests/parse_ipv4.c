#include <check.h>
#include <stdint.h>
#include "../fwlogs/parser.h"
// Note: data here are real live recorded packets with utility fwrecorder

void unittests_add_suite(Suite*);

static struct packet_data dt;


// This is connection with curl
static const uint8_t tcp_80_data[] = {
	0x45, 0x00, 0x00, 0x3c, 0x92, 0x57, 0x40, 0x00, 0x40, 0x06, 0x22, 0x7f,
	0xc0, 0xa8, 0x02, 0x01, 0xc0, 0xa8, 0x02, 0x94, 0xdb, 0x5c, 0x00, 0x50,
	0x0e, 0x10, 0xa1, 0xf7, 0x00, 0x00, 0x00, 0x00, 0xa0, 0x02, 0x72, 0x10,
	0x32, 0x59, 0x00, 0x00, 0x02, 0x04, 0x05, 0xb4, 0x04, 0x02, 0x08, 0x0a,
	0xe8, 0x31, 0xa9, 0xca, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x03, 0x07
};
static const size_t tcp_80_len = sizeof(tcp_80_data) / sizeof(*tcp_80_data);

// This is DNS with nslookup
static const uint8_t udp_53_data[] = {
	0x45, 0x00, 0x00, 0x34, 0x7c, 0x43, 0x40, 0x00, 0x40, 0x11, 0x38, 0x90,
	0xc0, 0xa8, 0x02, 0x01, 0xc0, 0xa8, 0x02, 0x94, 0x93, 0x08, 0x00, 0x35,
	0x00, 0x20, 0x0b, 0x9b, 0xf0, 0x9c, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x03, 0x6e, 0x69, 0x63, 0x02, 0x63, 0x7a, 0x00,
	0x00, 0x1c, 0x00, 0x01
};
static const size_t udp_53_len = sizeof(udp_53_data) / sizeof(*udp_53_data);

// This is ping
static const uint8_t icmp_data[] = {
	0x45, 0x00, 0x00, 0x54, 0x73, 0x2c, 0x40, 0x00, 0x40, 0x01, 0x41, 0x97,
	0xc0, 0xa8, 0x02, 0x01, 0xc0, 0xa8, 0x02, 0x94, 0x08, 0x00, 0x15, 0x68,
	0x2f, 0x08, 0x00, 0x02, 0x12, 0xbe, 0x43, 0x5f, 0x64, 0x6d, 0x0e, 0x00,
	0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13,
	0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
	0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b,
	0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37
};
static const size_t icmp_len = sizeof(icmp_data) / sizeof(*icmp_data);

static struct packets {
	const uint8_t *data;
	size_t len;
	struct packet_data expected;
} packets[] = {
	{
		.data = tcp_80_data,
		.len = tcp_80_len,
		.expected = (struct packet_data){
			.proto = "TCP",
			.source_ip = "192.168.2.1",
			.dest_ip = "192.168.2.148",
			.source_port = 56156,
			.dest_port = 80,
		}
	},
	{
		.data = udp_53_data,
		.len = udp_53_len,
		.expected = (struct packet_data){
			.proto = "UDP",
			.source_ip = "192.168.2.1",
			.dest_ip = "192.168.2.148",
			.source_port = 37640,
			.dest_port = 53,
		}
	},
	{
		.data = icmp_data,
		.len = icmp_len,
		.expected = (struct packet_data){
			.proto = "ICMP",
			.source_ip = "192.168.2.1",
			.dest_ip = "192.168.2.148",
			.source_port = 0,
			.dest_port = 0,
		}
	},
};


// This is connection with curl to port 80
START_TEST(ipv4_parse) {
	ck_assert(parse_packet(packets[_i].data, packets[_i].len, &dt));

	// We ignore time as we can't match it exactly anyway
	ck_assert_str_eq(dt.proto, packets[_i].expected.proto);
	ck_assert_str_eq(dt.source_ip, packets[_i].expected.source_ip);
	ck_assert_str_eq(dt.dest_ip, packets[_i].expected.dest_ip);
	ck_assert_int_eq(dt.source_port, packets[_i].expected.source_port);
	ck_assert_int_eq(dt.dest_port, packets[_i].expected.dest_port);
}
END_TEST


// We are using truncated tcp_80_data in these invalid packets but in general it
// should not matter what data we pass. The important part is data length.
static const uint8_t invalid_ip_data[] = {
	0x45, 0x00, 0x00, 0x3c, 0x92, 0x57, 0x40
};
static const size_t invalid_ip_len =
	sizeof(invalid_ip_data) / sizeof(*invalid_ip_data);

START_TEST(invalid_ip) {
	ck_assert(!parse_packet(invalid_ip_data, invalid_ip_len, &dt));
}
END_TEST

static const uint8_t invalid_tcp_data[] = {
	0x45, 0x00, 0x00, 0x3c, 0x92, 0x57, 0x40, 0x00, 0x40, 0x06, 0x22, 0x7f,
	0xc0, 0xa8, 0x02, 0x01, 0xc0, 0xa8, 0x02, 0x94, 0xdb, 0x5c, 0x00, 0x50,
};
static const size_t invalid_tcp_len =
	sizeof(invalid_tcp_data) / sizeof(*invalid_tcp_data);

START_TEST(invalid_tcp) {
	struct packet_data d;
	ck_assert(!parse_packet(invalid_tcp_data, invalid_tcp_len, &dt));
}
END_TEST

static const uint8_t invalid_ip_version_data[] = {
	0x35, 0x00, 0x00, 0x3c, 0x92, 0x57, 0x40, 0x00, 0x40, 0x06, 0x22, 0x7f,
	0xc0, 0xa8, 0x02, 0x01, 0xc0, 0xa8, 0x02, 0x94, 0xdb, 0x5c, 0x00, 0x50,
};
static const size_t invalid_ip_version_len =
	sizeof(invalid_ip_version_data) / sizeof(*invalid_ip_version_data);

START_TEST(invalid_ip_version) {
	ck_assert(!parse_packet(invalid_ip_version_data, invalid_ip_version_len, &dt));
}
END_TEST



__attribute__((constructor))
static void suite() {
	Suite *suite = suite_create("parse ipv4");

	TCase *valid_case = tcase_create("valid");
	tcase_add_loop_test(valid_case, ipv4_parse, 0,
		sizeof(packets) / sizeof(struct packets));
	suite_add_tcase(suite, valid_case);

	TCase *invalid_case = tcase_create("invalid");
	tcase_add_test(invalid_case, invalid_ip);
	tcase_add_test(invalid_case, invalid_tcp);
	tcase_add_test(invalid_case, invalid_ip_version);
	suite_add_tcase(suite, invalid_case);

	unittests_add_suite(suite);
}
