AR       := ar
ARFLAGS  := cr
CXX      := g++
#CPPFLAGS := -O2 -I../lib
CPPFLAGS := -g  -I../lib -D_TIME_ON_
all : ../bin/graphlab 
	@echo -n ""

clean :
	rm *.o ../bin/graphlab

../bin/graphlab : main.o sys_cmd.o user_cmd.o graphMgr.o ../lib/libgraphlab.a utility.o
	$(CXX) $^ -o $@

main.o : main.cpp sys_cmd.h user_cmd.h
	$(CXX) $(CPPFLAGS) -c $< -o $@

sys_cmd.o : sys_cmd.cpp sys_cmd.h ../lib/cmd.h
	$(CXX) $(CPPFLAGS) -c $< -o $@

user_cmd.o : user_cmd.cpp user_cmd.h ../lib/cmd.h
	$(CXX) $(CPPFLAGS) -c $< -o $@

utility.o : utility.cpp utility.h
	$(CXX) $(CPPFLAGS) -c $< -o $@

graphMgr.o : graphMgr.cpp graphMgr.h
	$(CXX) $(CPPFLAGS) -c $< -o $@

