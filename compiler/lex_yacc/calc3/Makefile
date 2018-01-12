useful := $(wildcard *.lex *.l calc3*.c calc3.h *.y) Makefile
rmfiles := $(filter-out $(useful),$(wildcard *))
CC := gcc

all: y.tab.h calc3a calc3b calc3g

calc3a: calc3a.o y.tab.o lex.yy.o
	$(CC) $^ -o $@


calc3b: calc3b.o y.tab.o lex.yy.o
	$(CC) $^ -o $@


calc3g: calc3g.o y.tab.o lex.yy.o
	$(CC) $^ -o $@


lex.yy.o: lex.yy.c

lex.yy.c: calc3.l
	flex $<

y.tab.c y.tab.h: calc3.y
	bison -y -d $<


.PHONY: clean

clean_test:
	echo $(RM) -rf $(rmfiles)

clean:
	$(RM) -rf $(rmfiles)

