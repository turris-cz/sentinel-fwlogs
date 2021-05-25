bin_PROGRAMS += fwrecorder

fwrecorder_SOURCES = \
	%reldir%/fwrecorder.c \
	$(srcdir)/fwlogs/log.c \
	$(srcdir)/fwlogs/fwlog.c \
	$(srcdir)/fwlogs/parser.c

fwrecorder_CFLAGS = \
	-iquote '$(builddir)/fwlogs' \
	${logc_CFLAGS} \
	${libnetfilter_log_CFLAGS} \
	${CODE_COVERAGE_CFLAGS}
fwrecorder_LDADD = \
	${logc_LIBS} \
	${libnetfilter_log_LIBS} \
	${CODE_COVERAGE_LIBS}

linted_files += $(fwrecorder_SOURCES)
