# CC and CFLAGS are varilables 
CC=g++
CFLAGS =  -c 
# -c option ask g++ to compile the source files, but do not link.
# -g option is for debugging version
# -O2 option is for optimized version
DBGFLAGS = -g -D_DEBUG_ON_
OPTFLAGS = -O2


# DEBUG Version
demo_dbg	: mergeSort_dbg main_dbg
			$(CC) $(DBGFLAGS) mergeSort.o main.o -ltm_usage -L../lib -o ../bin/demo_dbg
main_dbg 	   	:  ../lib/tm_usage.h  global.h main.cpp 
			$(CC) $(CFLAGS)  main.cpp   -I../lib
mergeSort_dbg	: mergeSort.cpp mergeSort.h
			$(CC) $(CFLAGS) $(DBGFLAGS) mergeSort.h mergeSort.cpp 
			
# optimized version
demo_opt	: mergeSort_opt main_opt
			$(CC) $(OPTFLAGS) mergeSort.o main.o -ltm_usage -L../lib -o ../bin/demo_opt
main_opt 	   	:  ../lib/tm_usage.h  global.h main.cpp 
			$(CC) $(CFLAGS)  main.cpp   -I../lib
mergeSort_opt	: mergeSort.cpp mergeSort.h
			$(CC) $(CFLAGS) $(OPTFLAGS) mergeSort.h mergeSort.cpp 

# clean all the .o and executable files
clean:
		rm -rf *.o demo
