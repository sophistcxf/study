CXX = g++

CFLAGS= -g 
INCLUDE_PATH=-I ../../include -I ../../../mpich/include/mpich-x86_64
LIB_PATH=-L ../../lib

TEST1=test_object_pool

TARGETS=$(TEST1)

all: $(TARGETS)

$(TEST1):
	g++ -o $(TEST1) $(INCLUDE_PATH) $(CFLAGS) $(TEST1).cpp $(LIB_PATH) -lboost_system -lboost_thread -lboost_filesystem -lpthread -lrt -L ../../../mpich/lib/mpich/lib/ -lboost_mpi -Wl,-rpath ../../../mpich/lib/mpich/lib/

clean:
	rm -f $(TARGETS)
