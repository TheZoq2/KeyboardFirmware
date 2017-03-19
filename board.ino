//#include <Keyboard.h>

//#include "keyboard.h"
#include "keyboard_arduino.h"
#include "layouts.h"

using namespace Z;

const int dummy = 0;

// Teensy 3.0 has the LED on pin 13
const int ledPin = 13;

const uint8_t WIDTH = 6;
const uint8_t HEIGHT = 4;



//uint8_t COL_PINS_RAW[WIDTH] = {7, 6, 8, 10, 11, 12};
uint8_t COL_PINS_RAW[WIDTH] = {12, 11, 10, 8, 6, 7};
uint8_t ROW_PINS_RAW[HEIGHT] = {2, 3, 4, 5};
auto COL_PINS =BoundedArray<uint8_t, WIDTH>(COL_PINS_RAW);
auto ROW_PINS =BoundedArray<uint8_t, HEIGHT>(ROW_PINS_RAW);

void setup() 
{
    // initialize the digital pin as an output.
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, HIGH);

    Serial.begin(115200);
    //keyboard.setup();

    init_pins<WIDTH, HEIGHT>(ROW_PINS, COL_PINS);

    delay(10); //Wait a while for the pull-up resistors to get ready
}

void loop() 
{
    auto old_packet = KeyPacket();
    while(true)
    {
        auto read_keys = read_pressed_keys(ROW_PINS, COL_PINS);

        auto keymap = init_keymap<WIDTH, HEIGHT>(default_layout);

        auto translated = translate_coordinates<WIDTH, HEIGHT>(read_keys, keymap);

        auto keytypes = keycodes_to_keytypes<WIDTH * HEIGHT>(translated);

        auto packet = keytypes_to_packet<WIDTH*HEIGHT>(keytypes, old_packet);
        old_packet = packet;

        //for(uint8_t i = 0; i < packet.keys.size(); ++i)
        //{
        //    set_key(i, packet.keys[i]);
        //}
        //for(uint8_t i = packet.keys.size(); i < KEYS_PER_PACKET; ++i)
        //{
        //    set_key(i, 0);
        //}
        //Keyboard.set_modifier(packet.modifiers);
        //Keyboard.send_now();
        send_packet(packet);
    }
}
