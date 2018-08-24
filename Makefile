include cet.make

all:
ifneq (yes,$(CET-ENABLED))
	echo Shadow stack is not available.
endif
