#CC and CFLAGS are varilables 
CC=g++
CFLAGS =  -c 
OPT= -o2
AR = ar
ARFLAGS = rcv

#DBGFLAGS = -g -D_DEBUG_ON_
#TMEFLAGS = -D_TIME_ON_
DBGFLAGS = 
# -c option ask g++ to compile the source files, but do not link.
# -g option is for debugging

all		: lib/libtm_usage.a bin/mysort 

lib/libtm_usage.a: lib/tm_usage.o
	$(AR) $(ARFLAGS) lib/libtm_usage.a lib/tm_usage.o

lib/tm_usage.o: lib/tm_usage.h lib/tm_usage.cpp
	$(CC) $(CFLAGS) lib/tm_usage.cpp -o lib/tm_usage.o


bin/mysort : bin/main.o bin/mySort.o 
	$(CC) $(DBGFLAGS) $(TMEFLAGS) $(OPT) bin/main.o bin/mySort.o -ltm_usage -Llib -o bin/mysort 

bin/main.o : lib/tm_usage.h src/main.cpp 
	$(CC) $(CFLAGS) $(DBGFLAGS) $(TMEFLAGS)  src/main.cpp -Ilib -o bin/main.o

bin/mySort.o: src/mySort.cpp
	$(CC) $(CFLAGS) $(DBGFLAGS) $(TMEFLAGS) $(OPT)  src/mySort.cpp -o bin/mySort.o





# clean all the .o and executable files
clean:
		rm -rf bin/*.o bin/mysort src/*.gch lib/*.o lib/*.a 

