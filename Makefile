SOURCES=debug.h bounded_array.h map.h keyboard_functional.h

TEST_SOURCE=unit.cpp

test:
	g++ $(SOURCES) $(TEST_SOURCE) --std=c++11 -o test.out -Wall -g
	./test.out -b
