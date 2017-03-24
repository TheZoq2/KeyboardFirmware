//#include <Keyboard.h>

//#include "keyboard.h"
#include "keyboard_arduino.h"
#include "layouts.h"

#include <HardwareSerial.h>

using namespace Z;

const int dummy = 0;

// Teensy 3.0 has the LED on pin 13
const int ledPin = 13;



//This is ugly but it fixes an undefined reference to an STD function. 
//Perhaps something should be logged
namespace std
{
    void __throw_bad_function_call()
    {

    }
}



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
    //Serial3.setRX(7);
    Serial1.begin(9600);
    Serial3.begin(9600);
    //keyboard.setup();

    init_pins<WIDTH, HEIGHT>(ROW_PINS, COL_PINS);

    delay(10); //Wait a while for the pull-up resistors to get ready
}

void loop() 
{
    auto old_packet = KeyPacket();
    auto current_state = KeyboardState::NORMAL;

    while(true)
    {
        auto self_keys = read_pressed_keys(ROW_PINS, COL_PINS);
//#define IS_SLAVE
#ifdef IS_SLAVE
        auto bytes = coords_to_bytes(self_keys);
        send_uart_bytes(bytes);
        digitalWrite(ledPin, HIGH);
#else
        //auto read_keys = read_pressed_keys(ROW_PINS, COL_PINS);
        auto read_bytes = read_uart_byte_stream<(WIDTH * HEIGHT)*2 + 1>();
        auto decoded_coordinates = decode_coordinates_from_bytes<WIDTH * HEIGHT>(read_bytes);
        if(decoded_coordinates.error != CoordsFromBytesError::SUCCESS)
        {
            Serial.println("Got invalid bytes");
            continue;
        }
        auto other_read_keys = decoded_coordinates.keys;

        auto keymap = get_current_keymap(current_state);

        auto full_coordinates = merge_coordinates(self_keys, other_read_keys, 
                [](KeyCoordinate coord) {
                    return KeyCoordinate(coord.x + WIDTH, coord.y);
                });

        auto translated = translate_coordinates<FULL_WIDTH, HEIGHT>(full_coordinates, keymap);

        auto keytypes = keycodes_to_keytypes(translated);
        keytypes.modifiers.push(get_state_modifier(current_state));

        current_state = get_new_state(current_state, keytypes);

        auto packet = keytypes_to_packet(keytypes, old_packet);
        old_packet = packet;

        send_packet(packet);

        //state_manager.update_current_layer(keytypes);

        digitalWrite(ledPin, HIGH);
#endif
    }
}
