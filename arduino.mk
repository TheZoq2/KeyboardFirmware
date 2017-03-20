#This file must contain the following variables
# ARDUINO_DIR=path_to_arduino_home

include ./paths.mk

BOARD_TAG    = teensyLC
ARDUINO_LIBS = 

LOCAL_CPP_SRCS = keyboard_functional.cpp debug.cpp keyboard_arduino.cpp

CXXFLAGS += -DIS_ARDUINO

USB_TYPE = USB_SERIAL_HID

#include ../../Teensy.mk
include ~/Arduino/Arduino-Makefile/Teensy.mk
