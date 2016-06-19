GEOS_INCLUDE=../../../../third/geos/include/
GEOS_LIB_PATH=../../../../third/geos/libs/

debug:
	g++ -g -I$(GEOS_INCLUDE) -o test test.cpp ../buffer_filter.cpp $(GEOS_LIB_PATH)/libgeos.a

release:
	g++ -I$(GEOS_INCLUDE) -o test test.cpp ../buffer_filter.cpp $(GEOS_LIB_PATH)/libgeos.a

so:
	g++ -I$(GEOS_INCLUDE) -o test test.cpp -L../ -lbuffer_filter -Wl,-rpath ../
