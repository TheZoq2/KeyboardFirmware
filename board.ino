
#include "keyboard.h"

const int dummy = 0;

// Teensy 3.0 has the LED on pin 13
const int ledPin = 13;

Z::Board keyboard;

void setup() 
{
    // initialize the digital pin as an output.
    pinMode(ledPin, OUTPUT);

    Serial.begin(115200);
    keyboard.setup();

    delay(10); //Wait a while for the pull-up resistors to get ready
}

void loop() 
{
    keyboard.update();
}
