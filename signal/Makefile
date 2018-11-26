TOP-DIR=..

-include config.make

include $(TOP-DIR)/cet.make

CFLAGS=-g -fcf-protection

EXES=

# Skip this on CET SDV since ia32 isn't available.
ifeq (,$(shell egrep "model name.*i7-7700" /proc/cpuinfo))
ifneq (x32,$(TARGET))
EXES+=sigreturn
endif
endif

sigreturn-OBJS=sigreturn.o

all: $(EXES)
	for f in $(EXES); do \
	  status=; \
	  readelf -n $$f | grep -q "IBT, SHSTK" && ./$$f && status=OK; \
	  if [ x$$status != xOK ]; then \
	    echo $$f: Failed; exit 1; \
	  fi; \
	done

include $(TOP-DIR)/glibc.make

sigreturn: $(sigreturn-OBJS)
	$(build-dynamic)
