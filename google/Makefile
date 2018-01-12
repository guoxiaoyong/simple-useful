CXXFLAGS := -I$(HOME)/.local/include 
LDFLAGS := -L$(HOME)/.local/lib -L$(HOME)/.local/lib64
CXX := g++

all: gflags_demo glog_demo

gflags_demo: gflags_demo.cc
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -lgflags -o $@ $^

glog_demo: glog_demo.cc
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -lglog -o $@ $^
