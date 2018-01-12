
useful := cppstyle.cpp cppstyle.lex Makefile

rmfiles := $(filter-out $(useful),$(wildcard *))

cppstyle: cppstyle.lex
	flex cppstyle.lex
	gcc -ggdb3 lex.yy.c -lfl -o cppstyle

install:
	cp cppstyle.exe /usr/local/bin/


.PHONY: clean

clean_test:
	echo $(RM) $(rmfiles)

clean:
	$(RM) $(rmfiles)


