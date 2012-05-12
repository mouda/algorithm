AR       := ar
ARFLAGS  := cr
CXX      := g++
#CPPFLAGS := -O2 -I../lib
CPPFLAGS := -g  -I../lib

all : ../bin/graphlab ../bin/test 
	@echo -n ""

clean :
	rm *.o ../bin/graphlab

../bin/graphlab : main.o sys_cmd.o user_cmd.o graphMgr.o ../lib/libgraphlab.a
	$(CXX) $^ -o $@

../bin/test :  test.o graphMgr.o
	$(CXX) $^ -o $@

main.o : main.cpp sys_cmd.h user_cmd.h
	$(CXX) $(CPPFLAGS) -c $< -o $@

test.o: test.cpp graphMgr.h
	$(CXX) $(CPPFLAGS) -c test.cpp -o test.o

sys_cmd.o : sys_cmd.cpp sys_cmd.h ../lib/cmd.h
	$(CXX) $(CPPFLAGS) -c $< -o $@

user_cmd.o : user_cmd.cpp user_cmd.h ../lib/cmd.h
	$(CXX) $(CPPFLAGS) -c $< -o $@

graphMgr.o : graphMgr.cpp graphMgr.h
	$(CXX) $(CPPFLAGS) -c $< -o $@

