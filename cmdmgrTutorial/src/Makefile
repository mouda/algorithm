AR       := ar
ARFLAGS  := cr
CXX      := g++
CPPFLAGS := -O2 -I../lib

all : ../bin/graphlab
	@echo -n ""

clean :
	rm *.o ../bin/graphlab

../bin/graphlab : main.o sys_cmd.o user_cmd.o ../lib/libgraphlab.a
	$(CXX) $^ -o $@

main.o : main.cpp sys_cmd.h user_cmd.h
	$(CXX) $(CPPFLAGS) -c $< -o $@

sys_cmd.o : sys_cmd.cpp sys_cmd.h ../lib/cmd.h
	$(CXX) $(CPPFLAGS) -c $< -o $@

user_cmd.o : user_cmd.cpp user_cmd.h ../lib/cmd.h
	$(CXX) $(CPPFLAGS) -c $< -o $@

