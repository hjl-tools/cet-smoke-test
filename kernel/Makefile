TOP-DIR=..

include $(TOP-DIR)/cet.make

all .DEFAULT:
ifeq (yes,$(CET-ENABLED))
	for d in *; do \
	  if [ -e $$d/Makefile ]; then \
	    make -C $$d $@; \
	  fi; \
	done
else
	echo CET is not available.
endif
