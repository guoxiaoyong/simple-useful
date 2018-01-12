all: pass strcmp.so

pass: pass.c

strcmp.so: strcmp.c
	gcc -fPIC -shared -o $@ $^
