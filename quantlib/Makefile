CFLAGS := -I$(HOME)/.local/include
LDFLAGS := -L$(HOME)/.local/lib
CXX := g++


vanilla: vanilla.cc
	$(CXX) $(CFLAGS) $(LDFLAGS) $^ -o $@ -lQuantLib



bls: bls.cc
	$(CXX) $(CFLAGS) $(LDFLAGS) $^ -o $@ -lQuantLib

asian: asian.cc
	$(CXX) $(CFLAGS) $(LDFLAGS) $^ -o $@ -lQuantLib

ql: ql.cc
	$(CXX) $(CFLAGS) $(LDFLAGS) $^ -o $@ -lQuantLib
