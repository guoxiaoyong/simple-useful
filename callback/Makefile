CC := g++
CXX := g++
CXXFLAGS := -std=c++11 -ggdb3

all: main test_polymorphism

main: main.o callback.o

test_polymorphism: test_polymorphism.cc

callback.o: callback.cc callback.h


clean:
	$(RM) -f *.o main test_polymorphism
