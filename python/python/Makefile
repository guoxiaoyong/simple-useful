CXX := g++
CXXFLAGS := -Wall
CXXFLAGS += -std=c++11
CXXFLAGS += $(shell python -m pybind11 --includes)
CXXFLAGS += -ggdb3
LDFLAGS := $(shell python-config --libs)

all: pybind11_test pybind11_example

pybind11_example: pybind11_example.cc
	g++ $(CXXFLAGS) $(LDFLAGS) $^ -o $@

pybind11_test: pybind11_test.cc
	g++ $(CXXFLAGS) $(LDFLAGS) $^ -o $@



