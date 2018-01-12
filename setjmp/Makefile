src := $(wildcard *.c)
useful := $(src) Makefile
rmfiles := $(filter-out $(useful),$(wildcard *))
exe := $(patsubst %.c,%,$(src))

CC := gcc
CFLAGS := -Wall 

all: $(exe)


.PHONY: clean

clean:
	$(RM) $(rmfiles)
