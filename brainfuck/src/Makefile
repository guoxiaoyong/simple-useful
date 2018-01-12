bfsrc := $(wildcard *.b)
useful := $(bfsrc) $(wildcard *.html) Makefile

rmfiles := $(filter-out $(useful),$(wildcard *))


clean:
	$(RM) $(rmfiles)
