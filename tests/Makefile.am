TESTS = %reldir%/unittests

check_PROGRAMS = %reldir%/unittests
%canon_reldir%_unittests_SOURCES = \
	$(fwlogs_sources) \
	%reldir%/unittests.c \
	%reldir%/parse_ipv4.c \ 
	%reldir%/parse_ipv6.c
%canon_reldir%_unittests_CFLAGS = \
	$(sentinel_fwlogs_CFLAGS) \
	$(CHECK_FLAGS)
%canon_reldir%_unittests_LDADD = \
	$(sentinel_fwlogs_LDADD) \
	$(CHECK_LIBS)
$(tests_unittests_OBJECTS): $(m4_files)


LOG_DRIVER = \
	env AM_TAP_AWK='$(AWK)' TEST_OUTPUT_TAP="/dev/stdout" \
	'$(SHELL)' '$(srcdir)/.aux/tap-driver.sh'

## Valgrind
VALGRIND_memcheck_FLAGS = \
	--leak-check=full \
	--show-leak-kinds=definite,indirect,possible \
	--track-fds=yes \
	--track-origins=yes \
	--trace-children=yes \
	--child-silent-after-fork=no
@VALGRIND_CHECK_RULES@

# Rules generated for valgrind are for some reason called *-am. This is just an alias
check-valgrind: check-valgrind-am
define check_valgrind_rule
check-valgrind-$(1): check-valgrind-$(1)-am
endef
$(foreach tool,$(valgrind_tools),$(eval $(call check_valgrind_rule,$(tool))))

## Coverage
CODE_COVERAGE_LCOV_OPTIONS := --no-external
CODE_COVERAGE_GENHTML_OPTIONS := --prefix $(dir $(abs_top_builddir))

include $(srcdir)/aminclude_static.am

clean-local: code-coverage-clean
distclean-local: code-coverage-dist-clean
