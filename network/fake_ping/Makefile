CXX=g++
CFLAGS=-g
APP=ping

all: ping.o error.o
	$(CXX) $(CFLAGS) -o $(APP) fake_ping.cpp ping.o error.o

ping.o:
	$(CXX) $(CFLAGS) -c -o ping.o ping.cpp

error.o:
	$(CXX) $(CFLAGS) -c -o error.o error.cpp

clean:
	rm -f ping.o error.o $(APP)
