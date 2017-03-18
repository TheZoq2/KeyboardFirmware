BOARD_TAG    = teensyLC
ARDUINO_LIBS =

LOCAL_CPP_SRCS = keyboard_functional.cpp debug.cpp

CXXFLAGS += -DIS_ARDUINO

#include ../../Teensy.mk
include ~/Arduino/Arduino-Makefile/Teensy.mk
