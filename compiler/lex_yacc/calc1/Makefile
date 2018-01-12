useful := $(wildcard *.lex *.l *.y) Makefile
rmfiles := $(filter-out $(useful),$(wildcard *))

calc: y.tab.o lex.yy.o
	$(CC) $^ -o $@

y.tab.o: y.tab.c

lex.yy.o: lex.yy.c

lex.yy.c: calc1.l
	flex $<

y.tab.c: calc1.y
	bison -y -d $<


.PHONY: clean

clean_test:
	echo $(RM) -rf $(rmfiles)

clean:
	$(RM) -rf $(rmfiles)

