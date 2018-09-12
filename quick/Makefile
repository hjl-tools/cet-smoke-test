-include config.make
include ../cet.make

CC=gcc
CFLAGS=-g -fcf-protection

EXES=quick_test mprotect_bench fork_bench shstk_alloc shstk_ptrace \
     shstk_thp shstk_dirty_cow

quick_test-OBJS=quick_test.o
mprotect_bench-OBJS=mprotect_bench.o
fork_bench-OBJS=fork_bench.o
shstk_alloc-OBJS=shstk_alloc.o
shstk_ptrace-OBJS=shstk_ptrace.o
shstk_thp-OBJS=shstk_thp.o
shstk_dirty_cow-OBJS=shstk_dirty_cow.o

shstk_dirty_cow-LIBS=-lpthread

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

fork_bench: $(fork_bench-OBJS)
	$(build-dynamic)

shstk_alloc: $(shstk_alloc-OBJS)
	$(build-dynamic)

shstk_ptrace: $(shstk_ptrace-OBJS)
	$(build-dynamic)

shstk_thp: $(shstk_thp-OBJS)
	$(build-dynamic)

shstk_dirty_cow: $(shstk_dirty_cow-OBJS)
	$(build-dynamic)
