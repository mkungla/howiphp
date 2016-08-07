AC_PREREQ([2.69])
AC_COPYRIGHT([Copyright (c) 2010 - 2016 Marko Kungla - License (MIT)])

AC_INIT([howiphp], [3.1.0], [https://github.com/okramlabs/howiphp/issues], [], [https://github.com/okramlabs/howiphp])

AC_DEFINE([HOWIPHP_VENDOR], ["Okram Labs"], [Set package vendor string])
AC_DEFINE([HOWIPHP_VENDOR_SITE], ["https://okramlabs.com"], [Set package vendor website])
AC_DEFINE([HOWIPHP_LICENSE], ["MIT"], [Set package license])
AC_DEFINE([HOWIPHP_COPY], ["2010 - 2016"], [Set package copy year])
AC_DEFINE([HOWIPHP_AUTHOR], ["Marko Kungla"], [Set package author])

AC_PREFIX_PROGRAM
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_SRCDIR([src/howiphp.h])
AC_CONFIG_HEADERS([config.h])

AC_PROG_CXX
AC_PROG_CC
AC_PROG_INSTALL

AM_INIT_AUTOMAKE([subdir-objects foreign -Wall -Werror])

AC_CHECK_HEADERS([arpa/inet.h float.h netdb.h stdlib.h sys/ioctl.h sys/time.h unistd.h])
AC_CHECK_FUNCS([gettimeofday inet_ntoa memset modf socket strchr strcspn strpbrk])

AC_FUNC_ERROR_AT_LINE
AC_FUNC_MALLOC

AC_CONFIG_FILES([Makefile])

AC_OUTPUT
