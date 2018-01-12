CXXFLAGS := -std=c++11 -ggdb3

src := $(wildcard *.hpp *.cpp)
xml := $(wildcard *.xml)
obj := $(patsubst %.cpp,%.o,$(src))
exe := $(patsubst %.cpp,%,$(src))

useful := $(src) $(xml) Makefile README.md
rmfiles := $(filter-out $(useful),$(wildcard *))

all: $(exe)

.PHONY: clean clean_test

clean_test:
	echo $(RM) $(rmfiles)

clean:
	$(RM) $(rmfiles)
