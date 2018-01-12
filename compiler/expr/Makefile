CC = gcc
CFLAGS = -ggdb3 -Wall -Wextra -Werror
CXX = g++
CXXFLAGS = -ggdb3
LDFLAGS = -lm

src := $(wildcard *.c *.cpp)
useful := $(src) Makefile
rmfiles := $(filter-out $(useful),$(wildcard *))

all: recur stack sim

recur: recur.c

stack: stack.cpp

clean:
	$(RM) $(rmfiles)


