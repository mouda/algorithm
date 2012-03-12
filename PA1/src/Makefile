#CC and CFLAGS are varilables 
CC=g++
CFLAGS =  -c 
#DBGFLAGS = -g -D_DEBUG_ON_
TMEFLAGS = -D_TIME_ON_
DBGFLAGS = 
# -c option ask g++ to compile the source files, but do not link.
# -g option is for debugging

all		: mysort 

mysort : main.o mySort.o 
	$(CC) $(DBGFLAGS) $(TMEFLAGS) main.o mySort.o -ltm_usage -L../lib -o mysort 

main.o : ../lib/tm_usage.h main.cpp 
	$(CC) $(CFLAGS) $(DBGFLAGS) $(TMEFLAGS) main.cpp -I../lib

mysort.o: mysort.h mysort.cpp
	$(CC) $(CFLAGS) $(DBGFLAGS) $(TMEFLAGS) mySort.h mySort.cpp


# clean all the .o and executable files
clean:
		rm -rf *.o mysort *.gch 

