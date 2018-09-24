TOP-DIR=..

-include config.make

include $(TOP-DIR)/cet.make

CFLAGS=-g -fcf-protection -mshstk

EXES=quick_test mprotect_bench fork_bench shstk_alloc shstk_ptrace \
     shstk_thp shstk_dirty_cow shstk_swap_out ibt_bitmap

quick_test-OBJS=quick_test.o
mprotect_bench-OBJS=mprotect_bench.o
fork_bench-OBJS=fork_bench.o
shstk_alloc-OBJS=shstk_alloc.o
shstk_ptrace-OBJS=shstk_ptrace.o
shstk_thp-OBJS=shstk_thp.o
shstk_dirty_cow-OBJS=shstk_dirty_cow.o
shstk_swap_out-OBJS=shstk_swap_out.o
ibt_bitmap-OBJS=ibt_bitmap.o

shstk_dirty_cow-LIBS=-lpthread

all: $(EXES)
	for f in $(EXES); do \
	  status=; \
	  readelf -n $$f | grep -q "IBT, SHSTK" && ./$$f && status=OK; \
	  if [ x$$status != xOK ]; then \
	    echo $$f: Failed; exit 1; \
	  fi; \
	done

include $(TOP-DIR)/glibc.make

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

shstk_swap_out: $(shstk_swap_out-OBJS)
	$(build-dynamic)

ibt_bitmap: $(ibt_bitmap-OBJS)
	$(build-dynamic)
