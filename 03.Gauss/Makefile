CXX_EXTRA_FLAGS=-std=c++17 -I ../external/lazycsv/include -I ../external/eigen $(shell pkgconf --cflags gtest 2>/dev/null)
LD_EXTRA_OPT=$(shell pkgconf --libs openblas 2>/dev/null)

include ../Makefile.inc

GTEST_LIBS=$(shell pkgconf --libs gtest gtest_main 2>/dev/null)

release : gauss

gauss  : util.r.o Gauss_main.r.o Gauss_solve.r.o
	$(LD) -o $@ $^ $(LD_EXTRA_OPT)

debug   : util.d.o Gauss_main.d.o Gauss_solve.d.o
	$(LD) -o $@ $^ $(LD_EXTRA_OPT)

utest   : Gauss_test.d.o util.d.o Gauss_solve.d.o
	$(LD) -o $@ $^ $(GTEST_LIBS)

run_utest : utest
	./$^

run_itest : release
	echo "Integration tests..."
	./integration_test.sh

all: depends run_utest run_itest

include Makefile.hdrdep
