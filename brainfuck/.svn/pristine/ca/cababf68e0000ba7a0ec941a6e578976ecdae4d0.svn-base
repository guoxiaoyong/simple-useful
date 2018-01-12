useful := $(wildcard *.c *.asm) Makefile brainfuck src
rmfiles := $(filter-out $(useful),$(wildcard *))

CFLAGS:= -O0 -ggdb3 #-DDEBUG_MSG

all: bin2bf div compare gbf bf rot13 bfcond

bf: bf.asm
	nasm -f bin -o bf bf.asm && chmod +x bf

.PHONY: clean install

install:
	cp bf /usr/bin
	cp gbf /usr/bin
	cp bin2bf /usr/bin

clean:
	$(RM) $(rmfiles)
