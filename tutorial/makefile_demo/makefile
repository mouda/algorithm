#CC and CFLAGS are varilables 
CC=g++
CFLAGS =  -c -g
# -c option ask g++ to compile the source files, but do not link.
# -g option is for debugging

all		: demo_stl  demo_is

demo_stl	: main.o stlsort.o
			$(CC) main.o stlsort.o -o demo_stl

demo_is		: main.o insertionsort.o
			$(CC) main.o insertionsort.o -o demo_is

main.o		: main.cpp replaceable_sorter.h
			$(CC) $(CFLAGS) main.cpp

stlsort.o	: stlsort.cpp
			$(CC) $(CFLAGS) stlsort.cpp

insertionsort.o : insertionsort.cpp
			$(CC) $(CFLAGS) insertionsort.cpp

# clean all the .o and executable files
clean:
		rm -rf *.o demo_is demo_stl

