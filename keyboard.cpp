#include "keyboard.h"

//The pins that the rows are connected to
const uint8_t Z::Board::ROW_PINS[] = {5, 4, 3, 0, 1, 2};
const uint8_t Z::Board::ROW_AMOUNT = 6;

const uint8_t Z::Board::COL_PINS[] = {22, 23, 21, 20};
const uint8_t Z::Board::COL_AMOUNT = 4;

const int Z::Board::READ_DELAY = 5;

void Z::Board::setup()
{
    //Setup all rows as ouptus
    for(uint8_t i = 0; i < ROW_AMOUNT; i++)
    {
        pinMode(ROW_PINS[i], OUTPUT);
        digitalWrite(ROW_PINS[i], HIGH);
    }

    //Setup all columns as inputs
    for(uint8_t i = 0; i < COL_AMOUNT; i++)
    {
        pinMode(COL_PINS[i], INPUT_PULLUP);
        digitalWrite(COL_PINS[i], INPUT_PULLUP);
    }
}

void Z::Board::update()
{
    Keyboard.set_key1(0);
    //Loop through the rows turning them on one by one
    for(uint8_t i = 0; i < ROW_AMOUNT; i++)
    {
        //Turn the row on
        digitalWrite(ROW_PINS[i], LOW); //Low because the teensy has a built in pull-up resistor

        delayMicroseconds(READ_DELAY);
        //Looping through the collumns and checking if any of them are LOW
        for(uint8_t n = 0; n < COL_AMOUNT; n++)
        {
            int signal = digitalRead(COL_PINS[n]);

            //Serial.println(signal);

            
            if(signal == LOW)
            {
                Serial.print(i);
                Serial.print(",");
                Serial.print(n);
                Serial.println(" closed");
            }
        }

        digitalWrite(ROW_PINS[i], HIGH); //Reset the pin;
    }
    Keyboard.send_now();
}
