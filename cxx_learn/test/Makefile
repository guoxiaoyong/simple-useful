all: result.s ga.s ma.s

result.s: test.cc
	g++ -S -m32 -O0 -masm=intel $^ 
	cat test.s | grep -v cfi | grep -v .LF | c++filt > result.s


ga.s: g.cc
	g++ -std=c++11 -S -m32 -O0 -masm=intel $^ 
	cat g.s | grep -v cfi | grep -v .LF | c++filt > ga.s


ma.s: move.cc
	g++ -std=c++11 -S  -O0 -masm=intel $^ 
	cat move.s | grep -v cfi | grep -v .LF | c++filt > ma.s




