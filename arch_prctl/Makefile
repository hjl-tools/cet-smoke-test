TOP-DIR=..

include $(TOP-DIR)/sde.config
include $(TOP-DIR)/cet.make

CFLAGS=-g -O2
ASFLAGS=-g
LDFLAGS=-T note.t

EXES=status shstk_alloc

OBJS=start.o syscall.o

all: note.t $(EXES)
	for f in $(EXES); do \
	  status=; \
	  readelf -n $$f | grep -q "IBT, SHSTK" \
	    && $(RUN) ./$$f && status=OK; \
	  if [ x$$status != xOK ]; then \
	    echo $$f: Failed; exit 1; \
	  fi; \
	done

include ../glibc.make

status: $(OBJS) status.o note.o
	$(LD) $(LDFLAGS) -o $@ $^

shstk_alloc: $(OBJS) shstk_alloc.o note.o
	$(LD) $(LDFLAGS) -o $@ $^

note.t:
	$(LD) --verbose | \
	  sed > $@T \
	      -e '/^=========/,/^=========/!d;/^=========/d' \
	      -e 's@^.*\*(\.jcr).*$$@& \
		 /DISCARD/ : { *(.note.gnu.property) }@'
	test -s $@T
	mv -f $@T $@

clean::
	rm -f note.t
