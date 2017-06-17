CXX = g++

CFLAGS=-std=c++98 -g 
INCLUDE_PATH=-I ../geos/include

TEST_ANGLE=test_angle
TEST_PT=test_point_locator

TARGETS=$(TEST_ANGLE) $(TEST_PT)

all: $(TARGETS)

$(TEST_ANGLE):
	g++ -o $(TEST_ANGLE) $(INCLUDE_PATH) $(CFLAGS) $(TEST_ANGLE).cpp -L../geos/libs -lgeos

$(TEST_PT):
	g++ -o $(TEST_PT) $(INCLUDE_PATH) $(CFLAGS) $(TEST_PT).cpp -L../geos/libs -lgeos

clean:
	rm -f $(TARGETS)
