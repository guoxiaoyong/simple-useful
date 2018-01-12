CC = g++
CXX = g++
CFLAGS = -I. -O3 -W -Wall
CXXFLAGS = -I. `pkg-config sdl --cflags` 
LDLIBS =  `pkg-config opencv --libs` `pkg-config sdl --libs` -lSDL_mixer -lSDL_image 

vdrum: vdrum.o timer.o

timer.o: timer.c

vdrum.o: vdrum.cpp


clean:
	rm -rf *.*~ Makefile~ *.*.orig vdrum *.o
