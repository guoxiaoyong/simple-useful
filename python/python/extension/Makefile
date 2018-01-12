CXX := g++
CXXFLAGS := -Wall
CXXFLAGS += -shared
CXXFLAGS += -std=c++11
CXXFLAGS += -fPIC
CXXFLAGS += $(shell python-config --libs)
CXXFLAGS += $(shell python -m pybind11 --includes)

%.so: %.cc
	g++ $(CXXFLAGS) $^ -o $@

all: simple.so cpp_object.so

