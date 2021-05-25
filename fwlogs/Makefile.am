bin_PROGRAMS += sentinel-fwlogs

fwlogs_sources = \
	%reldir%/log.c \
	%reldir%/czmq_log.c \
	%reldir%/config.c \
	%reldir%/fwlog.c \
	%reldir%/parser.c \
	%reldir%/sender.c

sentinel_fwlogs_SOURCES = \
	%reldir%/sentinel_fwlogs.c \
	${fwlogs_sources}

sentinel_fwlogs_CFLAGS = \
	${czmq_CFLAGS} \
	${msgpack_CFLAGS} \
	${libconfig_CFLAGS} \
	${logc_CFLAGS} \
	${logc_czmq_CFLAGS} \
	${libnetfilter_log_CFLAGS} \
	${CODE_COVERAGE_CFLAGS}
sentinel_fwlogs_LDADD = \
	${czmq_LIBS} \
	${msgpack_LIBS} \
	${libconfig_LIBS} \
	${logc_LIBS} \
	${logc_czmq_LIBS} \
	${libnetfilter_log_LIBS} \
	${CODE_COVERAGE_LIBS}

m4_files = %reldir%/protocol2str.c
$(sentinel_fwlogs_OBJECTS): $(m4_files)
include $(srcdir)/m4.am

linted_files += $(sentinel_fwlogs_SOURCES)
