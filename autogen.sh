#!/usr/bin/env bash

aclocal
autoheader
libtoolize
automake --copy --add-missing
autoconf

