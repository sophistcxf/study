GEOS_INCLUDE=../../geos/include/
GEOS_LIB_PATH=../../geos/libs/

all:
	g++ -fPIC -shared -I $(GEOS_INCLUDE) -o libbuffer_filter.so buffer_filter.cpp $(GEOS_LIB_PATH)/libgeos.a
