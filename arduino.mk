BOARD_TAG    = teensyLC
ARDUINO_LIBS = 

ARDUINO_DIR=/home/frans/bin/arduino-1.6.12

LOCAL_CPP_SRCS = keyboard_functional.cpp debug.cpp keyboard_arduino.cpp

CXXFLAGS += -DIS_ARDUINO

USB_TYPE = USB_SERIAL_HID

#include ../../Teensy.mk
include ~/Arduino/Arduino-Makefile/Teensy.mk
