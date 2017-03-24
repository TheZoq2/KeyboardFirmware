SOURCES=map.h keyboard_functional.cpp debug.cpp 

TEST_SOURCE=unit.cpp

X86_SOURCE=board_x86.cpp

test:
	g++ $(SOURCES) $(TEST_SOURCE) --std=c++11 -o test.out -Wall -g -Werror=return-type
	./test.out -b

x86:
	g++ $(SOURCES) $(X86_SOURCE) --std=c++11 -o x86.out -Wall -g -Werror=return-type


upload:
	make -f arduino.mk
	make -f arduino.mk reset

monitor:
	make -f arduino.mk monitor
