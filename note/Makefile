-include config.make
include ../sde.config

CC=gcc
CFLAGS=-g -O2
ASFLAGS=-g
LDFLAGS=-Wl,-T,note.t

EXES=ibt1 ibt2 ibt3 ibt4 ibt5 shstk1 shstk2 shstk3 shstk4 shstk5

ibt1-OBJS=bar.o ibt.o note1.o
ibt2-OBJS=bar.o ibt.o note2.o
ibt3-OBJS=bar.o ibt.o note3.o
ibt4-OBJS=bar.o ibt.o note4.o
ibt5-OBJS=bar.o ibt.o note5.o

ibt1-LDFLAGS=$(LDFLAGS)
ibt2-LDFLAGS=$(LDFLAGS)
ibt3-LDFLAGS=$(LDFLAGS)
ibt4-LDFLAGS=$(LDFLAGS)
ibt5-LDFLAGS=$(LDFLAGS)

shstk1-OBJS=foo.o shstk.o note1.o
shstk2-OBJS=foo.o shstk.o note2.o
shstk3-OBJS=foo.o shstk.o note3.o
shstk4-OBJS=foo.o shstk.o note4.o
shstk5-OBJS=foo.o shstk.o note5.o

shstk1-LDFLAGS=$(LDFLAGS)
shstk2-LDFLAGS=$(LDFLAGS)
shstk3-LDFLAGS=$(LDFLAGS)
shstk4-LDFLAGS=$(LDFLAGS)
shstk5-LDFLAGS=$(LDFLAGS)

ibt.o: CFLAGS +=-fcf-protection=branch

all: note.t $(EXES)
	$(RUN) ./ibt1
	$(RUN) ./shstk1
	$(RUN) ./ibt5
	$(RUN) ./shstk5
	if $(RUN) ./ibt2; then exit 1; fi
	if $(RUN) ./ibt3; then exit 1; fi
	if $(RUN) ./ibt4; then exit 1; fi
	if $(RUN) ./shstk2; then exit 1; fi
	if $(RUN) ./shstk3; then exit 1; fi
	if $(RUN) ./shstk4; then exit 1; fi

include ../glibc.make

note1.o note2.o note3.o: note.S

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

ibt1: $(ibt1-OBJS)
	$(build-dynamic)

ibt2: $(ibt3-OBJS)
	$(build-dynamic)

ibt3: $(ibt3-OBJS)
	$(build-dynamic)

ibt4: $(ibt4-OBJS)
	$(build-dynamic)

ibt5: $(ibt5-OBJS)
	$(build-dynamic)

shstk1: $(shstk1-OBJS)
	$(build-dynamic)

shstk2: $(shstk2-OBJS)
	$(build-dynamic)

shstk3: $(shstk3-OBJS)
	$(build-dynamic)

shstk4: $(shstk4-OBJS)
	$(build-dynamic)

shstk5: $(shstk5-OBJS)
	$(build-dynamic)
