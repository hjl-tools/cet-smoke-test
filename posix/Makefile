TOP-DIR=..

-include config.make

include $(TOP-DIR)/cet.make

CFLAGS=-g -O2 -fcf-protection

EXES=tst-cet-legacy-1 tst-cet-legacy-1a

tst-cet-legacy-1-OBJS=tst-cet-legacy-1.o
tst-cet-legacy-1a-OBJS=tst-cet-legacy-1a.o

tst-cet-legacy-1-LIBS=tst-cet-legacy-mod-1.so
tst-cet-legacy-1a-LIBS=tst-cet-legacy-mod-1.so

tst-cet-legacy-mod-1.o: CFLAGS=-g -O2 -fcf-protection=none

all: $(EXES)
	for f in $(EXES); do \
	  status=; \
	  readelf -n $$f | grep -q "IBT, SHSTK" && ./$$f && status=OK; \
	  if [ x$$status != xOK ]; then \
	    echo $$f: Failed; exit 1; \
	  fi; \
	done

include $(TOP-DIR)/glibc.make

tst-cet-legacy-1: $(tst-cet-legacy-1-OBJS) $(tst-cet-legacy-1-LIBS)
	$(build-dynamic)

tst-cet-legacy-1a: $(tst-cet-legacy-1a-OBJS) $(tst-cet-legacy-1a-LIBS)
	$(build-dynamic)

tst-cet-legacy-mod-1.so: tst-cet-legacy-mod-1.o
	$(build-shared-object)
