#ifndef H_KEYBOARD_ARDUINO
#define H_KEYBOARD_ARDUINO

#include "keyboard_functional.h"
#include "constants.h"

using namespace Z;

const uint16_t READ_DELAY = 5; //microseconds

template<size_t WIDTH, size_t HEIGHT>
void init_pins(
        const BoundedArray<uint8_t, HEIGHT> row_pins,
        const BoundedArray<uint8_t, WIDTH> col_pins
    )
{
    digitalWrite(13, HIGH);
    for(uint8_t y = 0; y < HEIGHT; ++y)
    {
        pinMode(row_pins[y], OUTPUT);
        for(uint8_t x = 0; x < WIDTH; ++x)
        {
            pinMode(col_pins[x], INPUT_PULLUP);
        }
    }
    
    digitalWrite(13, HIGH);
}


template<size_t WIDTH, size_t HEIGHT>
BoundedArray<KeyCoordinate, WIDTH*HEIGHT> read_pressed_keys(
        const BoundedArray<uint8_t, HEIGHT> row_pins,
        const BoundedArray<uint8_t, WIDTH> col_pins
    )
{
    auto result = BoundedArray<KeyCoordinate, WIDTH*HEIGHT>();

    for(uint8_t y = 0; y < HEIGHT; ++y)
    {
        digitalWrite(row_pins[y], LOW); //Low because the inputs have pullups

        delayMicroseconds(READ_DELAY);
        //delay(READ_DELAY);

        for(uint8_t x = 0; x < WIDTH; ++x)
        {
            if(digitalRead(col_pins[x]) == LOW)
            {
                result.push(KeyCoordinate(x, y));
            }
        }
        digitalWrite(row_pins[y], HIGH);
    }

    return result;
}


void set_key(const uint8_t index, const uint16_t key);
void send_packet(const KeyPacket packet);

template<size_t KEY_AMOUNT>
void send_coordinates(const BoundedArray<KeyCoordinate, KEY_AMOUNT> coords)
{
    //send the start byte
    Serial3.write(KEY_MESSAGE_START);

    uint8_t checksum = 0;

    for(size_t i = 0; i < coords.size(); ++i)
    {
        checksum += coords[i].x + coords[i].y;
        Serial3.write(coords[i].x);
        Serial3.write(coords[i].y);
    }
    //Write the checksum
    Serial3.write(checksum);
    //Write an end byte
    Serial3.write(KEY_MESSAGE_END);
}


/*
  Reads a stream of bytes from Serial3 and puts all the read bytes
  apart from the start and end bytes in an array

  TODO: Possibly handle array overflow
*/
template<size_t MAX_RECEIVED_BYTES>
BoundedArray<uint8_t, MAX_RECEIVED_BYTES> read_uart_byte_stream()
{
    auto received_bytes = BoundedArray<KeyCoordinate, MAX_RECEIVED_BYTES>();

    bool done = false;
    bool is_reading_keys = false;

    while(!done)
    {
        uint8_t new_byte;
        while(!Serial3.available())
        {}

        new_byte = Serial3.read();

        if(!is_reading_keys && new_byte == KEY_MESSAGE_START)
        {
            is_reading_keys = true;
        }
        else
        {
            if(new_byte == KEY_MESSAGE_END)
            {
                done = true;
                is_reading_keys = false;
            }
            //Something went wrong with the transmission. Start over
            else if(new_byte == KEY_MESSAGE_START)
            {
                is_reading_keys = true;
                received_bytes.reset();
            }
            else
            {
                received_bytes.push(new_byte);
            }
        }
    }

    return received_bytes;
}
#endif


