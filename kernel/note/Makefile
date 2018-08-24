include ../../cet.make

CC=gcc
CFLAGS=-g -O2 -Wa,-mx86-used-note=no
ASFLAGS=-g -Wa,-mx86-used-note=no
LD=ld
LDFLAGS=

EXES=ibt1 ibt2 ibt3 shstk1 shstk2 shstk3

OBJS=start.o syscall.o

all: $(EXES)
	./ibt1
	./shstk1
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

ibt1: $(OBJS) ibt.o note1.o
	$(LD) $(LDFLAGS) -o $@ $^

ibt2: $(OBJS) ibt.o note2.o
	$(LD) $(LDFLAGS) -o $@ $^

ibt3: $(OBJS) ibt.o note3.o
	$(LD) $(LDFLAGS) -o $@ $^

shstk1: $(OBJS) foo.o shstk.o note1.o
	$(LD) $(LDFLAGS) -o $@ $^

shstk2: $(OBJS) foo.o shstk.o note2.o
	$(LD) $(LDFLAGS) -o $@ $^

shstk3: $(OBJS) foo.o shstk.o note3.o
	$(LD) $(LDFLAGS) -o $@ $^

note1.o note2.o note3.o: note.S

clean:
	rm -f $(OBJS) $(EXES) *.o
