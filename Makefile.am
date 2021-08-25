bin_PROGRAMS = 
EXTRA_DIST =
linted_files =

include fwlogs/Makefile.am

if FWRECORDER
include recorder/Makefile.am
endif


if ENABLE_LINTERS
include lint.am
endif

if ENABLE_TESTS
DISTCHECK_CONFIGURE_FLAGS = --enable-tests --enable-fwrecorder
include tests/Makefile.am
endif
