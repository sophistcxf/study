CXX = g++

CFLAGS=-std=c++98 -g 
INCLUDE_PATH=-I ../include

TEST_MPI=test_mpich

TARGETS=$(TEST_MPI)

all: $(TARGETS)

$(TEST_MPI):
	g++ -o $(TEST_MPI) $(INCLUDE_PATH) $(CFLAGS) $(TEST_MPI).cpp -L ../lib/mpich/lib/ -lmpich -Wl,-rpath ../lib/mpich/lib

clean:
	rm -f $(TARGETS)
