#!/bin/sh

aclocal
libtoolize --force --copy
autoheader
automake -ac
autoconf
