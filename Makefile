CXX      = g++-9
CXXFLAGS = -std=c++17 -Wall -g3
OBJS     = $(SRCOBJS) $(TPOBJS)
SRCOBJS  = $(patsubst %.cpp, %.o, $(wildcard */*.cpp))
TPOBJS   = $(patsubst %.cpp, %.o, $(wildcard */*/*.cpp))
PROGRAM  = Compiler

all: $(PROGRAM)

$(PROGRAM): $(OBJS)
	$(CXX) $(OBJS) $(CXXFLAGS) -o $(PROGRAM)

clean:; rm -f *.o */*.o */*/*.o $(PROGRAM)