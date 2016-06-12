CXX=g++

LIBEVENT_INCLUDE_PATH=../include/libevent/include
LIBEVENT_LIB_PATH=../libs/libevent/Linux/x86_64

CFLAGS=-g -I $(LIBEVENT_INCLUDE_PATH) 
all: test_http

test_http:
	$(CXX) $(CFLAGS) test_http.cpp -lrt $(LIBEVENT_LIB_PATH)/libevent.a 

