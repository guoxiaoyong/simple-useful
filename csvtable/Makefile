all: kblock block csvtable

%:%.cc
	g++ -O2 -std=c++0x -o $@ $<

%:%.c
	gcc -ggdb3 -O0 -o $@ $<

useful := $(wildcard *.cc *.h) Makefile param.conf table.csv
rmfiles := $(filter-out $(useful),$(wildcard *))

.PHONY: clean clean_test


clean_test:
	echo $(RM) $(rmfiles)

clean:
	$(RM) $(rmfiles)
