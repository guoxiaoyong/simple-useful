CC := g++
CXX := g++

CXXFLAGS := -ggdb3
LDFLAGS := -lboost_system -lpthread -lboost_thread-mt -lpthread

src := $(sort $(wildcard *.cc))
exe := $(patsubst %.cc,%,$(src))

useful := $(src) Makefile
rmfiles := $(filter-out $(useful),$(wildcard *))

all: $(exe)


.PHONY: clean

clean:
	$(RM) $(rmfiles)
