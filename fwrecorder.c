#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <argp.h>
#include <time.h>
#include <arpa/inet.h>
#include <libnetfilter_log/libnetfilter_log.h>
#define DEFLOG log_fwrecorder // logger used in default in logc macros
#include <logc.h>
#include <logc_util.h>
#include <logc_argp.h>
#include "fwlog.h"

APP_LOG(fwrecorder)

static struct {
	uint16_t nflog_group;
} conf = {
	.nflog_group = DEFAULT_NFLOG_GROUP,
};

static error_t parse_opt(int key, char *arg, struct argp_state *state);

const static struct argp_option argp_options[] = {
	{"nflog-group", 'g', "group", 0, "Netfilter log group", 0},
	{NULL}
};

const static struct argp argp_parser = {
	.options = argp_options,
	.parser = parse_opt,
	.doc = "Firewall logs recorder. This saves timestamped received raw packets to current working directory.",
	.children = (struct argp_child[]){{&logc_argp_parser, 0, "Logging", 2}, {NULL}},
};


static error_t parse_opt(int key, char *arg, struct argp_state *state) {
	switch (key) {
		case 'g': {
			char *endptr;
			long int group = strtol(arg, &endptr, 0);
			if (*endptr != '\0')
				argp_error(state, "nflog-group has to be valid number but was: %s", arg);
			if (group < 0 || group >= (1L<<16))
				argp_error(state, "nflog-group must be in range froma 0 to 2^16-1 but was: %s", arg);
			conf.nflog_group = group;
			break; }
		default:
			return ARGP_ERR_UNKNOWN;
	}
	return 0;
}

char *generate_filename() {
	static unsigned seq = 0;
	char *filename = NULL;
	do {
		free(filename);
		STD_IGNORE(asprintf(&filename, "fwrecorder-%d.packet", seq++));
	} while (!access(filename, F_OK));
	errno = 0;
	return filename;
}

static bool callback(uint8_t *payload, size_t payload_len, void *data) {
	char *filename = generate_filename();
	int fd;
	STD_FATAL(fd = open(filename, O_WRONLY | O_CREAT | O_EXCL, S_IRWXU | S_IRWXG | S_IRWXO));
	do {
		ssize_t written = write(fd, payload, payload_len);
		if (written == -1)
			CRITICAL("Unable to write payload");
		payload_len -= written;
	} while (payload_len > 0);
	close(fd);
	INFO("Recorded packet: %s", filename);

	free(filename);
	return true;
}


int main(int argc, char **argv) {
	logc_argp_log = log_fwrecorder; // assign our log to be configured by logc_argp
	argp_parse(&argp_parser, argc, argv, 0, NULL, NULL);

	fwlog_run(conf.nflog_group, 0, callback, NULL);

	return 0;
}
