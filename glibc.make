ifneq (,$(GLIBC-SOURCE-DIR)$(GLIBC-BUILD-DIR))

ifeq (x86_64,$(ARCH))
ifeq (x32,$(TARGET))
LD-SO=$(GLIBC-BUILD-DIR)/elf/ld-linux-x32.so.2
else
LD-SO=$(GLIBC-BUILD-DIR)/elf/ld-linux-x86-64.so.2
endif
else
LD-SO=$(GLIBC-BUILD-DIR)/elf/ld-linux.so.2
endif

GLIBC-PATH=$(GLIBC-BUILD-DIR):.

LIBDL-GLIBC-PATH=:$(GLIBC-BUILD-DIR)/dlfcn
LIBDL-STATIC=$(GLIBC-BUILD-DIR)/dlfcn/libdl.a
LIBDL-DYNAMIC=$(GLIBC-BUILD-DIR)/dlfcn/libdl.so

LIBM-GLIBC-PATH=:$(GLIBC-BUILD-DIR)/math
LIBM-STATIC=$(GLIBC-BUILD-DIR)/math/libm.a
LIBM-DYNAMIC=$(GLIBC-BUILD-DIR)/math/libm.so

LIBPTHREAD-GLIBC-PATH=:$(GLIBC-BUILD-DIR)/nptl
LIBPTHREAD-STATIC=$(GLIBC-BUILD-DIR)/nptl/libpthread.a
LIBPTHREAD-DYNAMIC=$(GLIBC-BUILD-DIR)/nptl/libpthread.so
ifneq ($(wildcard $(GLIBC-BUILD-DIR)/nptl/libpthread_nonshared.a),)
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
$($(@F)-LDFLAGS) $^
$(if $(findstring -lm,$($(@F)-LIBS)),$(LIBM-STATIC)) \
$(if $(findstring -lpthread,$($(@F)-LIBS)),$(LIBPTHREAD-STATIC)) \
-Wl,--start-group \
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
`$(CC) --print-file-name=crtbegin.o` \
-Wl,$(RPATH-LINK)=$(GLIBC-PATH)$(if $(findstring -lpthread,$($(@F)-LIBS)),$(LIBPTHREAD-GLIBC-PATH))$(if $(findstring -lm,$($(@F)-LIBS)),$(LIBM-GLIBC-PATH))$(if $(findstring -ldl,$($(@F)-LIBS)),$(LIBDL-GLIBC-PATH)) \
$($(@F)-LDFLAGS) $^ \
$(if $(findstring -ldl,$($(@F)-LIBS)),$(LIBDL-DYNAMIC)) \
$(if $(findstring -lm,$($(@F)-LIBS)),$(LIBM-DYNAMIC)) \
$(if $(findstring -lpthread,$($(@F)-LIBS)),$(LIBPTHREAD-DYNAMIC)) \
$(GLIBC-BUILD-DIR)/libc.so.6 \
$(LD-SO) $(GLIBC-BUILD-DIR)/libc_nonshared.a $(LIBGCC-EH-DYNAMIC) \
`$(CC) --print-file-name=crtend.o` $(GLIBC-BUILD-DIR)/csu/crtn.o
endef
else
define build-static
$(CC) -static $($(@F)-LDFLAGS) -o $@ $^ $($(@F)-LIBS)
endef

define build-dynamic
$(CC) $($(@F)-LDFLAGS) -o $@ $^ $($(@F)-LIBS)
endef
endif

clean::
	rm -f *.o $(EXES)
