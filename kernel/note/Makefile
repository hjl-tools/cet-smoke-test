include ../../cet.make

CC=gcc
CFLAGS=-g -O2 -Wa,-mx86-used-note=no
ASFLAGS=-g -Wa,-mx86-used-note=no
LD=ld
LDFLAGS=

EXES=ibt1 ibt2 ibt3

OBJS=start.o syscall.o ibt.o

all: $(EXES)
	./ibt1
ifeq (yes,$(CET-ENABLED))
	if ./ibt2; then exit 1; fi
	if ./ibt3; then exit 1; fi
else
	./ibt2
	./ibt3
endif

ibt1: $(OBJS) note1.o
	$(LD) $(LDFLAGS) -o $@ $^

ibt2: $(OBJS) note2.o
	$(LD) $(LDFLAGS) -o $@ $^

ibt3: $(OBJS) note3.o
	$(LD) $(LDFLAGS) -o $@ $^

note1.o note2.o note3.o: note.S

clean:
	rm -f $(OBJS) $(EXES) *.o
