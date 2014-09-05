bin_PROGRAMS= groupby
check_PROGRAMS= groupby_test

groupby_SOURCES= main.cpp groupby.cpp

groupby_test_SOURCES= groupby_test.cpp groupby.cpp
groupby_test_CPPFLAGS= -I$(top_builddir)/gtest
groupby_test_LDADD= $(top_builddir)/gtest/libgtest_main.la

TESTS= groupby_test
