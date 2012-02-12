SUBDIRS = \
 console \
 document \
 project

INCLUDES = \
 $(GTKMM_CFLAGS) \
 $(GTKSOURCEVIEWMM_CFLAGS) \
 $(GIOMM_CFLAGS) \
 $(GLADEMM_CFLAGS) \
 $(VTEMM_CFLAGS) \
 $(XMLPP_CFLAGS) \
 $(GCONFMM_CFLAGS)

AM_CPPFLAGS = \
 -DINSTALL_PREFIX=\"$(prefix)\"

bin_PROGRAMS = gide

gide_SOURCES = \
 main.cc \
 gide.cc \
 preferences_dialog.cc \
 configuration_manager.cc

noinst_HEADERS = \
 gide.hh \
 options.hh \
 preferences_dialog.hh \
 configuration_manager.hh

gide_LDADD = \
 $(GTKMM_LIBS) \
 $(GTKSOURCEVIEWMM_LIBS) \
 $(GIOMM_LIBS) \
 $(XMLPP_LIBS) \
 $(GLADEMM_LIBS) \
 $(VTEMM_LIBS) \
 $(GCONFMM_LIBS) \
 console/libconsole.la \
 document/libdocument.la \
 project/libproject.la
