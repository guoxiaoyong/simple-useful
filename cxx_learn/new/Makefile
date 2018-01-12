all: new.s new_array.s oasize

CXXFLAGS := -std=c++11

%.s: %.cc
	g++ -std=c++11 -O0 -S -m32 -masm=intel -o - $^ | grep -v cfi | c++filt > $@

