-include config.make
include ../sde.config

CC=gcc
CFLAGS=-g -O2 -fcf-protection

EXES=ucontext-1

ucontext-1-OBJS=ucontext-1.o

all: $(EXES)
	for f in $(EXES); do \
	  status=; \
	  readelf -n $$f | grep -q "IBT, SHSTK" \
	    && $(RUN) ./$$f && status=OK; \
	  if [ x$$status != xOK ]; then \
	    echo $$f: Failed; exit 1; \
	  fi; \
	done

include ../glibc.make

ucontext-1: $(ucontext-1-OBJS)
	$(build-dynamic)
