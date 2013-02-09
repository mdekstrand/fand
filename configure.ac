AC_PREREQ(2.67)
AC_INIT(fand, 0.1, michael@elehack.net)
AM_INIT_AUTOMAKE([foreign silent-rules])
AM_MAINTAINER_MODE

AC_PROG_CC
AC_PROG_CC_C_O
AC_PROG_CC_C99
AC_PROG_INSTALL

PKG_CHECK_MODULES(config, libconfuse, [], [AC_ERROR([must install libconfuse])])
PKG_CHECK_MODULES(daemon, libdaemon, [], [AC_ERROR([must install libdaemon])])

AC_CHECK_HEADER([getopt.h], [], [AC_ERROR([must have GNU getopt])])
AC_CHECK_HEADER([syslog.h], [], [AC_ERROR([what, you don't have a syslog?])])

AC_DEFINE_UNQUOTED([FAND], ["${PACKAGE}"], [The fand package name])
AC_DEFINE_UNQUOTED([FAND_VERSION], ["${PACKAGE_VERSION}"],
                                   [The fand version])

AH_VERBATIM([_GNU_SOURCE],
[/* Inhabit our GNU happy place */
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif])

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
