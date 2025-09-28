CC=g++
CFLAGS=-c -g -Wall -I /usr/local/include/boost-1_37/
LDFLAGS=-L /usr/local/lib 
SOURCES=viterbi.cpp test.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=viterbi

all: $(SOURCES) $(EXECUTABLE)
	

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -fr *.o *~ $(EXECUTABLE)
