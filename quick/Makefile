-include config.make
include ../cet.make

CC=gcc
CFLAGS=-g -fcf-protection

EXES=quick_test mprotect_bench shstk_alloc

quick_test-OBJS=quick_test.o
mprotect_bench-OBJS=mprotect_bench.o
shstk_alloc-OBJS=shstk_alloc.o

all: $(EXES)
	for f in $(EXES); do \
	  status=; \
	  readelf -n $$f | grep -q "IBT, SHSTK" && ./$$f && status=OK; \
	  if [ x$$status != xOK ]; then \
	    echo $$f: Failed; exit 1; \
	  fi; \
	done

include ../glibc.make

quick_test: $(quick_test-OBJS)
	$(build-dynamic)

mprotect_bench: $(mprotect_bench-OBJS)
	$(build-dynamic)

shstk_alloc: $(shstk_alloc-OBJS)
	$(build-dynamic)
