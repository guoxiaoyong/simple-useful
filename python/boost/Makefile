 
PYTHON_INCLUDE := $(shell python-config --include)
 
 
# compile mesh classes
TARGET = hello_ext
 
$(TARGET).so: $(TARGET).o
	g++ -shared -Wl,--export-dynamic $^ -lboost_python $(shell python-config --libs) -o $@
 
$(TARGET).o: $(TARGET).cpp
	g++ $(PYTHON_INCLUDE) -fPIC -c $^


.PHONY: clean

useful := $(wildcard *.cpp) Makefile
rmfiles := $(filter-out $(useful),$(wildcard *))

clean:
	$(RM) $(rmfiles)
