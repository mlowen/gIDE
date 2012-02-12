INCLUDES = \
 $(GTKMM_CFLAGS) \
 $(GIOMM_CFLAGS) \
 $(GTKSOURCEVIEWMM_CFLAGS) \
 $(GLADEMM_CFLAGS)

noinst_LTLIBRARIES = libdocument.la

noinst_HEADERS = \
 document.hh \
 document_label.hh \
 source_document.hh

libdocument_la_SOURCES = \
 document.cc \
 document_label.cc \
 source_document.cc

libdocument_la_LIBADD = \
 $(GTKMM_LIBS) \
 $(GIOMM_LIBS) \
 $(GTKSOURCEVIEWMM_LIBS) \
 $(GLADEMM_LIBS)
