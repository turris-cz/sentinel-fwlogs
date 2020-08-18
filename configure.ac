AC_INIT([sentinel-fwlogs], [0.1.0], [tech.support@turris.cz])
AC_CONFIG_MACRO_DIRS([m4])
AC_CONFIG_AUX_DIR([aux])

AM_INIT_AUTOMAKE([foreign silent-rules subdir-objects -Wall -Wno-portability])
AM_SILENT_RULES([yes])

AC_PROG_CC
AC_GNU_SOURCE

PKG_CHECK_MODULES([czmq], [libczmq])
PKG_CHECK_MODULES([msgpack], [msgpack])
PKG_CHECK_MODULES([logc], [logc])
AC_LINK_IFELSE(
  [AC_LANG_PROGRAM([#include <argp.h>],[argp_parse(0,1,NULL,0,0,0);])],,
  [argp_standalone_required="yes"]
)
AS_IF([test "$argp_standalone_required" = "yes"],
  AC_CHECK_LIB([argp], [argp_parse], , AC_MSG_ERROR([Unable to find libargp])))


dnl Tests
AC_ARG_ENABLE([tests], AC_HELP_STRING([--disable-tests], [Disable tests]))
AM_CONDITIONAL([ENABLE_TESTS], [test "x$enable_tests" != "xno"])
AS_IF([test "x$enable_tests" != "xno"], [
  PKG_CHECK_MODULES([CHECK], [check >= 0.11])
  AC_REQUIRE_AUX_FILE([tap-driver.sh])
])
AX_VALGRIND_CHECK

dnl Linters
AC_ARG_ENABLE([linters], AC_HELP_STRING([--disable-linters], [Disable linters (cppcheck and luacheck)]))
AM_CONDITIONAL([ENABLE_TESTS], [test "x$enable_linters" != "xno"])
AS_IF([test "x$enable_linters" != "xno"], [
  AC_PATH_PROG([CPPCHECK], [cppcheck])
])

dnl Coverage
AX_CODE_COVERAGE


AC_CONFIG_FILES([Makefile])

AC_OUTPUT
