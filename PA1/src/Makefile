#CC and CFLAGS are varilables 
CC=g++
CFLAGS =  -c -g
# -c option ask g++ to compile the source files, but do not link.
# -g option is for debugging

all		: mysort 

mysort : main.o insertionsort.o mergeSort.o heapSort.o
	$(CC) main.o insertionsort.o mergeSort.o heapSort.o -o mysort

main.o		: main.cpp 
	$(CC) $(CFLAGS) main.cpp

insertionsort.o : insertionsort.cpp insertionsort.h
	$(CC) $(CFLAGS) insertionsort.cpp

mergeSort.o : mergeSort.h mergeSort.cpp
	$(CC) $(CFLAGS) mergeSort.h mergeSort.cpp			

heapSort.o : heapSort.h heapSort.cpp
	$(CC) $(CFLAGS) heapSort.h heapSort.cpp


# clean all the .o and executable files
clean:
		rm -rf *.o mysort *.gch 

