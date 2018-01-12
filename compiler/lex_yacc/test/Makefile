CC = gcc
LDLIBS = -lm

useful := $(wildcard *.lex *.y) Makefile
rmfiles := $(filter-out $(useful),$(wildcard *))


calc: y.tab.o lex.yy.o
	$(CC) -o $@ -lm $^

y.tab.o: y.tab.c

lex.yy.o: lex.yy.c

lex.yy.c: calc.lex
	flex $<

y.tab.c: calc.y
	bison -y -d $<


.PHONY: clean

clean_test:
	echo $(RM) -rf $(rmfiles)

clean:
	$(RM) -rf $(rmfiles)

