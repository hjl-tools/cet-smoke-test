-include config.make
include ../cet.make

CC=gcc
CFLAGS=-g -O2 -Wa,-mx86-used-note=no
ASFLAGS=-g -Wa,-mx86-used-note=no

EXES=ibt1 ibt2 ibt3 shstk1 shstk2 shstk3

ibt1-OBJS=bar.o ibt.o note1.o
ibt2-OBJS=bar.o ibt.o note2.o
ibt3-OBJS=bar.o ibt.o note3.o

shstk1-OBJS=foo.o shstk.o note1.o
shstk2-OBJS=foo.o shstk.o note2.o
shstk3-OBJS=foo.o shstk.o note3.o

ibt.o: CFLAGS +=-fcf-protection=branch

all: $(EXES)
	./ibt1
ifeq (yes,$(CET-ENABLED))
	if ./ibt2; then exit 1; fi
	if ./ibt3; then exit 1; fi
	if ./shstk2; then exit 1; fi
	if ./shstk3; then exit 1; fi
else
	./ibt2
	./ibt3
	./shstk2
	./shstk3
endif


include ../glibc.make

ibt1: $(ibt1-OBJS)
	$(build-dynamic)

ibt2: $(ibt3-OBJS)
	$(build-dynamic)

ibt3: $(ibt3-OBJS)
	$(build-dynamic)

shstk1: $(shstk1-OBJS)
	$(build-dynamic)

shstk2: $(shstk2-OBJS)
	$(build-dynamic)

shstk3: $(shstk3-OBJS)
	$(build-dynamic)
