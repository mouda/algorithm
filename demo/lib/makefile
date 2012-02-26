###########################################################
# File       [Makefile]
# Author     [sleepyoala]
# Synopsis   [an example Makefile to generate tm_usage package]
# Date       [Ver. 1.0 started 2010/02/23]
###########################################################

# CC and CFLAGS are varilables 
CC = g++
CFLAGS = -c -g
# -c option ask g++ to compile the source files, but do not link.
# -g option is for debugging

AR = ar
ARFLAGS = rcv

lib: libtm_usage.a

libtm_usage.a: tm_usage.o
	$(AR) $(ARFLAGS) libtm_usage.a tm_usage.o

tm_usage.o: tm_usage.h tm_usage.cpp
	$(CC) $(CFLAGS) tm_usage.cpp

# clean all the .o and executable files
clean:
	rm -rf *.o *.a

