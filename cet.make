ifeq (,$(shell grep shstk /proc/cpuinfo))
CET-ENABLED=no
else
CET-ENABLED=yes
endif
