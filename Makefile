SOURCES=debug.h bounded_array.h map.h

TEST_SOURCE=unit.cpp

test:
	g++ $(SOURCES) $(TEST_SOURCE) --std=c++11
