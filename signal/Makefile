TOP-DIR=..

-include config.make

include $(TOP-DIR)/cet.make

CFLAGS=-g -fcf-protection

EXES=sigreturn

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
