sbin_PROGRAMS = fand

dist_doc_DATA = README.md

noinst_HEADERS = layout.h fan.h sensor.h util.h

LIBS = $(config_LIBS) $(daemon_LIBS)
AM_CPPFLAGS = -DSYSCONFDIR='"$(sysconfdir)"' \
	$(config_CFLAGS) $(daemon_CFLAGS)

fand_SOURCES = fand.c layout.c fan.c sensor.c util.c
