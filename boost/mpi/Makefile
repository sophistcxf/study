CXX = g++

CFLAGS= -g 
INCLUDE_PATH=-I ../../include -I ../../../mpich/include/mpich-x86_64
LIB_PATH=-L ../../lib

TEST_MPI=test_mpi

TARGETS=$(TEST_MPI)

all: $(TARGETS)

$(TEST_MPI):
	g++ -o $(TEST_MPI) $(INCLUDE_PATH) $(CFLAGS) $(TEST_MPI).cpp $(LIB_PATH) -lboost_system -lboost_thread -lboost_filesystem -lpthread -lrt -L ../../../mpich/lib/mpich/lib/ -lboost_mpi -Wl,-rpath ../../../mpich/lib/mpich/lib/

clean:
	rm -f $(TARGETS)
