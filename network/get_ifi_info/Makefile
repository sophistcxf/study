CXX=g++
CFLAGS=-g
APP=print_ifi_info

all: ifi.o error.o
	$(CXX) $(CFLAGS) -o $(APP) print_ifi_info.cpp ifi.o error.o sock_inet_ntop.o

ifi.o:
	$(CXX) $(CFLAGS) -c -o ifi.o ifi.cpp

error.o:
	$(CXX) $(CFLAGS) -c -o error.o error.cpp

sock_inet_ntop.o:
	$(CXX) $(CFLAGS) -c -o sock_inet_ntop.o sock_inet_ntop.cpp

clean:
	rm -f ifi.o error.o $(APP)
