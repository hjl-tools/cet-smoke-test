TOP-DIR?=.
include $(TOP-DIR)/cet.config

ifeq (,$(shell grep shstk /proc/cpuinfo))
CET-ENABLED=no
else
CET-ENABLED=yes
endif

ARCH?=$(shell uname -m)
TARGET?=$(ARCH)

CC=gcc
LD=ld
ifeq (x86_64,$(ARCH))
ifeq (x32,$(TARGET))
CC+=-mx32
LD+= -m elf32_x86_64
endif
else
CC+=-m32
LD+= -m elf_i386
endif
