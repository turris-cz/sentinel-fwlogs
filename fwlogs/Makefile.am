bin_PROGRAMS += sentinel-fwlogs
EXTRA_DIST += \
	protocols \
	%reldir%/protocol2str.c.m4

fwlogs_sources = \
	%reldir%/config.h \
	%reldir%/config.c \
	%reldir%/czmq_log.c \
	%reldir%/fwlog.h \
	%reldir%/fwlog.c \
	%reldir%/log.h \
	%reldir%/log.c \
	%reldir%/parser.h \
	%reldir%/parser.c \
	%reldir%/sender.h \
	%reldir%/sender.c

sentinel_fwlogs_SOURCES = \
	%reldir%/sentinel_fwlogs.c \
	${fwlogs_sources}

sentinel_fwlogs_CFLAGS = \
	-iquote '$(builddir)/%reldir%' \
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
