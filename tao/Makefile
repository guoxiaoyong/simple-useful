CXX      = g++
CXXFLAGS = -std=c++11 -ggdb3 -Wall -O0 -DTAO_LINUX -DDEBUG
INCPATH  = -Isrc

cpp_file := $(wildcard src/*.cpp)
obj_file := $(patsubst %.cpp,%.o,$(cpp_file))

all: tao

tao: $(obj_file)
	$(CXX) -o $@ $^ -ldl

.PHONY: clean

clean:
	$(RM) tao $(obj_file)


