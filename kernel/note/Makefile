include ../../cet.make

CC=gcc
CFLAGS=-g -O2
ASFLAGS=-g
LD=ld
LDFLAGS=-T note.t

EXES=ibt1 ibt2 ibt3 ibt4 ibt5 shstk1 shstk2 shstk3 shstk4 shstk5

OBJS=start.o syscall.o

all: note.t $(EXES)
	./ibt1
	./shstk1
	./ibt5
	./shstk5
ifeq (yes,$(CET-ENABLED))
	if ./ibt2; then exit 1; fi
	if ./ibt3; then exit 1; fi
	if ./ibt4; then exit 1; fi
	if ./shstk2; then exit 1; fi
	if ./shstk3; then exit 1; fi
	if ./shstk4; then exit 1; fi
else
	./ibt2
	./ibt3
	./ibt4
	./shstk2
	./shstk3
	./shstk4
endif

ibt1: $(OBJS) ibt.o note1.o
	$(LD) $(LDFLAGS) -o $@ $^

ibt2: $(OBJS) ibt.o note2.o
	$(LD) $(LDFLAGS) -o $@ $^

ibt3: $(OBJS) ibt.o note3.o
	$(LD) $(LDFLAGS) -o $@ $^

ibt4: $(OBJS) ibt.o note4.o
	$(LD) $(LDFLAGS) -o $@ $^

ibt5: $(OBJS) ibt.o note5.o
	$(LD) $(LDFLAGS) -o $@ $^

shstk1: $(OBJS) foo.o shstk.o note1.o
	$(LD) $(LDFLAGS) -o $@ $^

shstk2: $(OBJS) foo.o shstk.o note2.o
	$(LD) $(LDFLAGS) -o $@ $^

shstk3: $(OBJS) foo.o shstk.o note3.o
	$(LD) $(LDFLAGS) -o $@ $^

shstk4: $(OBJS) foo.o shstk.o note4.o
	$(LD) $(LDFLAGS) -o $@ $^

shstk5: $(OBJS) foo.o shstk.o note5.o
	$(LD) $(LDFLAGS) -o $@ $^

note1.o note2.o note3.o: note.S

note.t:
	$(LD) --verbose | \
	  sed > $@T \
	      -e '/^=========/,/^=========/!d;/^=========/d' \
	      -e 's@^.*\*(\.jcr).*$$@& \
		 /DISCARD/ : { *(.note.gnu.property) }@'
	test -s $@T
	mv -f $@T $@

clean:
	rm -f $(OBJS) $(EXES) *.o note.t
