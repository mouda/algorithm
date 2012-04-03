#CC and CFLAGS are varilables 
CC=g++
CFLAGS =  -c 
#OPT= -o2
DBGFLAGS = -g -D_DEBUG_ON_
TMEFLAGS = -D_TIME_ON_
# -c option ask g++ to compile the source files, but do not link.
# -g option is for debugging

all		: 01kanpsack

01kanpsack: main.o knapsack.o 
	$(CC) $(DBGFLAGS) $(TMEFLAGS) $(OPT) main.o knapsack.o -ltm_usage -L../lib -o ../bin/01kanpsack 

main.o : ../lib/tm_usage.h main.cpp 
	$(CC) $(CFLAGS) $(DBGFLAGS) $(TMEFLAGS)  main.cpp -I../lib


knapsack.o :  knapsack.cpp 
	$(CC) $(CFLAGS) $(DBGFLAGS) $(TMEFLAGS) $(OPT)  knapsack.cpp


# clean all the .o and executable files
clean:
		rm -rf *.o ../bin/01kanpsack *.gch 

