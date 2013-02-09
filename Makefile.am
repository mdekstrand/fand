sbin_PROGRAMS = fand

noinst_HEADERS = layout.h sensor.h

LIBS = $(config_LIBS) $(daemon_LIBS)
AM_CPPFLAGS = -DSYSCONFDIR='"$(sysconfdir)"' \
	$(config_CFLAGS) $(daemon_CFLAGS)

fand_SOURCES = fand.c layout.c sensor.c util.c
