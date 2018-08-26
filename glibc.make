ARCH?=$(shell uname -m)

ifneq (,$(GLIBC-SOURCE-DIR)$(GLIBC-BUILD-DIR))

ifeq (x86_64,$(ARCH))
ifneq ($(findstring mx32,$(CC)),)
LD-SO=$(GLIBC-BUILD-DIR)/elf/ld-linux-x32.so.2
else
LD-SO=$(GLIBC-BUILD-DIR)/elf/ld-linux-x86-64.so.2
endif
else
LD-SO=$(GLIBC-BUILD-DIR)/elf/ld-linux.so.2
endif

GLIBC-PATH=$(GLIBC-BUILD-DIR):.

ifeq (yes,$(NEED-LIBM))
GLIBC-PATH:=$(GLIBC-PATH):$(GLIBC-BUILD-DIR)/math
LIBM-STATIC=$(GLIBC-BUILD-DIR)/math/libm.a
LIBM-DYNAMIC=$(GLIBC-BUILD-DIR)/math/libm.so
endif

ifeq (yes,$(NEED-LIBPTHREAD))
GLIBC-PATH:=$(GLIBC-PATH):$(GLIBC-BUILD-DIR)/nptl
LIBPTHREAD-STATIC=$(GLIBC-BUILD-DIR)/nptl/libpthread.a
LIBPTHREAD-DYNAMIC=$(GLIBC-BUILD-DIR)/nptl/libpthread.so
LIBPTHREAD-DYNAMIC+=$(GLIBC-BUILD-DIR)/nptl/libpthread_nonshared.a
endif

LIBGCC-STATIC=$(shell $(CC) --print-file-name=libgcc.a)
LIBGCC-EH-DYNAMIC=$(LIBGCC-STATIC)
LIBGCC-EH-DYNAMIC+=-Wl,--push-state,--as-needed
LIBGCC-EH-DYNAMIC+=$(shell $(CC) --print-file-name=libgcc_s.so)
LIBGCC-EH-DYNAMIC+=-Wl,--pop-state
LIBGCC-EH-STATIC=$(LIBGCC-STATIC)
LIBGCC-EH-STATIC+=$(shell $(CC) --print-file-name=libgcc_eh.a)

RPATH-LINK=-rpath

define build-static
$(CC) -static -nostdlib -nostartfiles -o $@ \
$(GLIBC-BUILD-DIR)/csu/crt1.o $(GLIBC-BUILD-DIR)/csu/crti.o \
`$(CC) --print-file-name=crtbegin.o` \
$($(@F)-LDFLAGS) $^ \
-Wl,--start-group \
$(LIBM-STATIC) \
$(LIBGCC-EH-STATIC) \
$(GLIBC-BUILD-DIR)/libc.a  \
-Wl,--end-group \
`$(CC) --print-file-name=crtend.o` \
$(GLIBC-BUILD-DIR)/csu/crtn.o
endef

define build-dynamic
$(CC) -Busr/local/bin/ -nostdlib -nostartfiles -o $@ \
-Wl,-dynamic-linker=$(LD-SO) -Wl,-z,nocombreloc \
$(GLIBC-BUILD-DIR)/csu/crt1.o $(GLIBC-BUILD-DIR)/csu/crti.o \
`$(CC) --print-file-name=crtbegin.o` -Wl,$(RPATH-LINK)=$(GLIBC-PATH) \
$($(@F)-LDFLAGS) $^ \
$(LIBM-DYNAMIC) $(LIBPTHREAD-DYNAMIC) $(GLIBC-BUILD-DIR)/libc.so.6 \
$(LD-SO) $(GLIBC-BUILD-DIR)/libc_nonshared.a $(LIBGCC-EH-DYNAMIC) \
`$(CC) --print-file-name=crtend.o` $(GLIBC-BUILD-DIR)/csu/crtn.o
endef
else
define build-static
$(CC) -static -o $@ $^ $($(@F)-LDFLAGS)
endef

define build-dynamic
$(CC) -o $@ $^ $($(@F)-LDFLAGS)
endef
endif

clean::
	rm -f *.o $(EXES)