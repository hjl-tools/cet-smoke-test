include cet.make

all .DEFAULT:
ifeq (yes,$(CET-ENABLED))
	for d in *; do \
	  if [ -e $$d/Makefile ]; then \
	    status=; \
	    make -C $$d $@ && status=OK; \
	    if [ x$$status != xOK ]; then \
	       echo $$d: Failed; exit 1; \
	    fi; \
	  fi; \
	done
else
	echo CET is not available.
endif
