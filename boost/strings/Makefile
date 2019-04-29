CXX = g++

CFLAGS= -g 
INCLUDE_PATH=-I ../../include -I ../../../mpich/include/mpich-x86_64 -I ../../third/linux/include/
LIB_PATH=-L ../../lib

all: $(TARGETS)

test_replace:
	g++ -o test_replace $(INCLUDE_PATH) $(CFLAGS) test_replace.cpp 

test_split:
	g++ -o test_split $(INCLUDE_PATH) $(CFLAGS) test_split.cpp

clean:
	rm -f $(TARGETS)
