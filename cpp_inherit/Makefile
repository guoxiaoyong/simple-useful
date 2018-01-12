RTTI := #-fno-rtti
DUMP := -fdump-class-hierarchy

CXX := g++
CXXFLAGS := -std=c++11 -ggdb3 $(RTTI) $(DUMP)

target := bind object_slice normal_inheritance virtual_inheritance con_des virtual_method virtual_method1 virtual_method2

all: $(target)

clean:
	$(RM) -rf $(target) *.o *.class *.dSYM
