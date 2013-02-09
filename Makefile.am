sbin_PROGRAMS = fand

noinst_HEADERS = layout.h sensor.h

LIBS = $(config_LIBS)
AM_CPPFLAGS = -DSYSCONFDIR='"$(sysconfdir)"'

fand_SOURCES = fand.c layout.c sensor.c memory.c
