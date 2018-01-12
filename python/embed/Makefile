CXX := clang++
CXXFLAGS := -Wall
CXXFLAGS += -shared
CXXFLAGS += -std=c++11
CXXFLAGS += -fPIC
CXXFLAGS += $(shell python-config --includes)
CXXFLAGS += $(shell python -m pybind11 --includes)

all: main1 main2

clean:
	$(RM) main1 main2
