include ../cet.make

all .DEFAULT:
ifeq (yes,$(CET-ENABLED))
	for d in *; do \
	  if [ -d $$d ]; then \
	    make -C $$d $@; \
	  fi; \
	done
else
	echo CET is not available.
endif
