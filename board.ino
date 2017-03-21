//#include <Keyboard.h>

//#include "keyboard.h"
#include "keyboard_arduino.h"
#include "layouts.h"

using namespace Z;

const int dummy = 0;

// Teensy 3.0 has the LED on pin 13
const int ledPin = 13;

const uint8_t WIDTH = 6;
const uint8_t FULL_WIDTH = WIDTH * 2;
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
    digitalWrite(ledPin, LOW);

    Serial.begin(115200);
    Serial3.setTX(20);
    Serial3.setRX(7);
    Serial3.begin(9600);
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
//#define IS_SLAVE
#ifdef IS_SLAVE
        auto bytes = coords_to_bytes(read_keys);
        send_uart_bytes(bytes);
        digitalWrite(ledPin, HIGH);
        delay(10);
#else
        //auto read_keys = read_pressed_keys(ROW_PINS, COL_PINS);
        //auto read_bytes = read_uart_byte_stream<(WIDTH * HEIGHT)*2 + 1>();
        //auto decode_result = decode_coordinates_from_bytes<WIDTH * HEIGHT>(read_bytes);
        //if(decode_result.error != CoordsFromBytesError::SUCCESS)
        //{
        //    Serial.println("Got invalid bytes");
        //    continue;
        //}
        //digitalWrite(ledPin, HIGH);
        //Serial.println("Doing loop stuff");
        //auto other_read_keys = decode_result.keys;
        auto other_read_keys = read_keys;

        auto keymap = init_keymap<WIDTH, HEIGHT>(DEFAULT_LAYER);

        auto translated = translate_coordinates<WIDTH, HEIGHT>(other_read_keys, keymap);

        auto keytypes = keycodes_to_keytypes<WIDTH * HEIGHT>(translated);

        auto packet = keytypes_to_packet<WIDTH*HEIGHT>(keytypes, old_packet);
        old_packet = packet;

        send_packet(packet);

        digitalWrite(ledPin, HIGH);

        Serial.println(KEY_A);
#endif
    }
}
