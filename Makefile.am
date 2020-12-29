bin_PROGRAMS = sentinel-fwlogs

fwlogs_sources = \
	log.c \
	config.c \
	fwlog.c \
	parser.c \
	sender.c

sentinel_fwlogs_SOURCES = \
	sentinel_fwlogs.c \
	${fwlogs_sources}

sentinel_fwlogs_CFLAGS = \
	${czmq_CFLAGS} \
	${msgpack_CFLAGS} \
	${libconfig_CFLAGS} \
	${logc_CFLAGS} \
	${libnetfilter_log_CFLAGS} \
	${CODE_COVERAGE_CFLAGS}
sentinel_fwlogs_LDADD = \
	${czmq_LIBS} \
	${msgpack_LIBS} \
	${libconfig_LIBS} \
	${logc_LIBS} \
	${libnetfilter_log_LIBS} \
	${CODE_COVERAGE_LIBS}

m4_files = protocol2str.c
$(sentinel_fwlogs_OBJECTS): $(m4_files)
include $(srcdir)/m4.am


if FWRECORDER
bin_PROGRAMS += fwrecorder

fwrecorder_SOURCES = \
	fwrecorder.c \
	log.c \
	fwlog.c \
	parser.c

fwrecorder_CFLAGS = \
	${logc_CFLAGS} \
	${libnetfilter_log_CFLAGS} \
	${CODE_COVERAGE_CFLAGS}
fwrecorder_LDADD = \
	${logc_LIBS} \
	${libnetfilter_log_LIBS} \
	${CODE_COVERAGE_LIBS}
endif


linted_files = $(sentinel_fwlogs_SOURCES) $(fwrecorder_SOURCES)
include $(srcdir)/lint.am

include $(srcdir)/tests/Makefile.am
