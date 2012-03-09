#CC and CFLAGS are varilables 
CC=g++
CFLAGS =  -c -g
# -c option ask g++ to compile the source files, but do not link.
# -g option is for debugging

all		: mysort 

mysort : main.o mySort.o 
	$(CC) main.o mySort.o -o mysort

main.o : main.cpp 
	$(CC) $(CFLAGS) main.cpp

mysort.o: mysort.h mysort.cpp
	$(CC) $(CFLAGS) mySort.h mySort.cpp


# clean all the .o and executable files
clean:
		rm -rf *.o mysort *.gch 

