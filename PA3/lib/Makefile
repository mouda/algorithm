AR       := ar
ARFLAGS  := cr
CXX      := g++
CPPFLAGS := -O2

all : libgraphlab.a
	@echo -n ""

clean :
	rm *.a *.o

libgraphlab.a : cmd.o get_opt.o tm_usage.o
	$(AR) $(ARFLAGS) $@ $^ 

cmd.o : cmd.cpp cmd.h ascii.h vt100.h
	$(CXX) $(CPPFLAGS) -c $< -o $@

get_opt.o : get_opt.cpp get_opt.h
	$(CXX) $(CPPFLAGS) -c $< -o $@

tm_usage.o : tm_usage.cpp tm_usage.h
	$(CXX) $(CPPFLAGS) -c $< -o $@

