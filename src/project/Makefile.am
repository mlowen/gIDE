INCLUDES = \
 $(GTKMM_CFLAGS) \
 $(GIOMM_CFLAGS) \
 $(XMLPP_CFLAGS) \
 $(GLADEMM_CFLAGS)

noinst_LTLIBRARIES = libproject.la

noinst_HEADERS = \
 project.hh \
 add_dialog.hh \
 create_dialog.cc \
 template.hh \
 template_manager.hh \
 properties_dialog.hh

libproject_la_SOURCES = \
 project.cc \
 add_dialog.cc \
 create_dialog.cc \
 template.cc \
 template_manager.cc \
 properties_dialog.cc

libproject_la_LIBADD = \
 $(GTKMM_LIBS) \
 $(GIOMM_LIBS) \
 $(XMLPP_LIBS) \
 $(GLADEMM_LIBS)
