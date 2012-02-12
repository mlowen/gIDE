#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.60)
AC_INIT(gIDE, 0.1, gide@mlowen.com)
AC_CONFIG_SRCDIR([src/main.cc])
AM_INIT_AUTOMAKE
AM_CONFIG_HEADER([config.h])
AC_PREFIX_DEFAULT("/usr")

# Checks for programs.
AC_PROG_CXX
AC_PROG_LIBTOOL
AC_PROG_INSTALL

# Checks for libraries.
PKG_CHECK_MODULES(GTKMM, gtkmm-2.4 >= 2.12.5)
AC_SUBST(GTKMM_CFLAGS)
AC_SUBST(GTKMM_LIBS)

PKG_CHECK_MODULES(GTKSOURCEVIEWMM, gtksourceviewmm-2.0 >= 1.9.2)
AC_SUBST(GTKSOURCEVIEWMM_CFLAGS)
AC_SUBST(GTKSOURCEVIEWMM_LIBS)

PKG_CHECK_MODULES(GIOMM, giomm-2.4 >= 2.16.0)
AC_SUBST(GIOMM_CFLAGS)
AC_SUBST(GIOMM_CFLAGS)

PKG_CHECK_MODULES(XMLPP, libxml++-2.6 >= 2.20.0)
AC_SUBST(XMLPP_CFLAGS)
AC_SUBST(XMLPP_LIBS)

PKG_CHECK_MODULES(GLADEMM, libglademm-2.4 >= 2.6.6)
AC_SUBST(GLADEMM_CFLAGS)
AC_SUBST(GLADEMM_LIBS)

PKG_CHECK_MODULES(VTEMM, libvtemm-1.0 >= 0.20.2)
AC_SUBST(VTEMM_CFLAGS)
AC_SUBST(VTEMM_LIBS)

PKG_CHECK_MODULES(GCONFMM, gconfmm-2.6 >= 2.22.0)
AC_SUBST(GCONFMM_CFLAGS)
AC_SUBST(GCONFMM_LIBS)

#PKG_CHECK_MODULES(GLADEUI, gladeui-1.0 >= 3.4.5)
#AC_SUBST(GLADEUI_CFLAGS)
#AC_SUBST(GLADEUI_LIBS)

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile
                 pixmaps/Makefile
                 src/Makefile
                 src/console/Makefile
                 src/document/Makefile
                 src/project/Makefile
                 templates/Makefile
                 ui/Makefile])
AC_OUTPUT
