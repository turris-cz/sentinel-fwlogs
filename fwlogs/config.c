#include "config.h"
#include <unistd.h>
#include <string.h>
#include <argp.h>
#include <logc_argp.h>
#include <libconfig.h>
#include "log.h"

static struct config conf = {
	.config_file = DEFAULT_CONFIG_FILE,
	.socket = DEFAULT_SOCKET_URI,
	.topic = DEFAULT_TOPIC,
	.nflog_group = DEFAULT_NFLOG_GROUP,
};

enum {
	OPT_TOPIC = 256,
};

static error_t parse_opt(int key, char *arg, struct argp_state *state);

const static struct argp_option argp_options[] = {
	{"config", 'f', "path",  0, "Path to config file"},
	{"nflog-group", 'g', "group", 0, "Netfilter log group", 0},
	{"socket", 's', "socket", 0, "Local socket to push data to Sentinel Proxy", 0},
	{"topic", OPT_TOPIC, "topic", 0, "Topic for communication with Sentinel Proxy", 0},
	{NULL}
};

const static struct argp argp_parser = {
	.options = argp_options,
	.parser = parse_opt,
	.doc = "Sentinel firewall logs collector",
	.children = (struct argp_child[]){{&logc_argp_parser, 0, "Logging", 2}, {NULL}},
};


static error_t parse_opt(int key, char *arg, struct argp_state *state) {
	switch (key) {
		case 'f':
			conf.config_file = arg;
			break;
		case 'g': {
			char *endptr;
			long int group = strtol(arg, &endptr, 0);
			if (*endptr != '\0')
				argp_error(state, "nflog-group has to be valid number but was: %s", arg);
			if (group < 0 || group >= (1L<<16))
				argp_error(state, "nflog-group must be in range from 0 to 2^16-1 but was: %s", arg);
			conf.nflog_group = group;
			break; }
		case 's':
			conf.socket = arg;
			break;
		case OPT_TOPIC:
			conf.topic = arg;
			break;
		default:
			return ARGP_ERR_UNKNOWN;
	}
	return 0;
}

static void load_config_file() {
	if (access(conf.config_file, R_OK)) {
		debug("Skipping config file load (%s)", conf.config_file);
		return;
	}
	config_t cfg;
	config_init(&cfg);
	if(!config_read_file(&cfg, conf.config_file))
		critical("Load of config file failed: %s: %s:%d - %s",
				conf.config_file,
				config_error_file(&cfg),
				config_error_line(&cfg),
				config_error_text(&cfg)
		);

	if (conf.nflog_group != DEFAULT_NFLOG_GROUP) {
		const config_setting_t *nflog_group = config_lookup(&cfg, "nflog-group");
		if (nflog_group && config_setting_type(nflog_group) == CONFIG_TYPE_INT)
			conf.nflog_group = config_setting_get_int(nflog_group);
	}
	if (strcmp(conf.socket, DEFAULT_SOCKET_URI)) {
		const config_setting_t *socket = config_lookup(&cfg, "socket");
		if (socket && config_setting_type(socket) == CONFIG_TYPE_STRING)
			conf.socket = config_setting_get_string(socket);
	}
	if (strcmp(conf.topic, DEFAULT_TOPIC)) {
		const config_setting_t *topic = config_lookup(&cfg, "topic");
		if (topic && config_setting_type(topic) == CONFIG_TYPE_STRING)
			conf.topic = config_setting_get_string(topic);
	}

	config_destroy(&cfg);
}


struct config *parse_args(int argc, char **argv) {
	logc_argp_log = log_sentinel_fwlogs; // set our log to be configured by logc_argp
	argp_parse(&argp_parser, argc, argv, 0, NULL, NULL);
	load_config_file();
	return &conf;
}
