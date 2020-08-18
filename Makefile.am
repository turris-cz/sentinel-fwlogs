bin_PROGRAMS = sentinel-fwlogs

sentinel_fwlogs_SOURCES = \
	sentinel_fwlogs.c

sentinel_fwlogs_CFLAGS = \
	${czmq_CFLAGS} \
	${msgpack_CFLAGS} \
	${logc_CFLAGS}
sentinel_fwlogs_LDADD = \
	${czmq_LIBS} \
	${libevent_LIBS} \
	${logc_LIBS}


linted_files = $(sentinel_fwlogs_SOURCES)
include $(srcdir)/lint.am

include $(srcdir)/tests/Makefile.am
