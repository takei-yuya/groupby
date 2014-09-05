#!/usr/bin/env bash

libtoolize
autoscan
sed \
  -e '/AC_INIT/s/FULL-PACKAGE-NAME/groupby/' \
  -e '/AC_INIT/s/VERSION/0.0.0/' \
  -e '/AC_INIT/s/BUG-REPORT-ADDRESS/takei.yuya@gmail.com/' \
  -e '/AC_INIT/aAC_CONFIG_AUX_DIR([config])' \
  -e '/AC_INIT/aAC_CONFIG_MACRO_DIR([m4])' \
  -e 's/AC_PROG_RANLIB/LT_INIT/' \
  -e '/AC_INIT/aAM_INIT_AUTOMAKE([foreign])' \
  configure.scan > configure.ac
