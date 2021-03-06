SOURCES=map.h keyboard_functional.cpp debug.cpp 

TEST_SOURCE=unit.cpp

test:
	g++ $(SOURCES) $(TEST_SOURCE) --std=c++11 -o test.out -Wall -g -Werror=return-type
	./test.out -b


upload:
	make -f arduino.mk
	make -f arduino.mk reset

monitor:
	make -f arduino.mk monitor
