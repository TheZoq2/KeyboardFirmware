#include "keyboard.h"

//The pins that the rows are connected to
const uint8_t Z::Board::ROW_PINS[] = {5, 4, 3, 0, 1, 2};
const uint8_t Z::Board::ROW_AMOUNT = 6;

const uint8_t Z::Board::COL_PINS[] = {22, 23, 21, 20};
const uint8_t Z::Board::COL_AMOUNT = 4;

const int Z::Board::READ_DELAY = 5;

const int KEY_SHIFT = -1;
const int KEY_CTRL = -2;
const int KEY_ALT = -3;
const int KEY_GUI = -4;

const int KEY_FN = -100;

//REMEMBER TO CHANGE THE SIZE OF THIS IN THE DECLARATION ASWELL
const int Z::Board::KEYMAP[Z::Board::ROW_AMOUNT][Z::Board::COL_AMOUNT] = 
    {
        {KEY_ESC, KEY_F1, KEY_F2, KEY_F3},
        {KEY_TILDE, KEY_1, KEY_2, KEY_3},
        {KEY_TAB, KEY_Q, KEY_W, KEY_E},
        {KEY_CAPS_LOCK, KEY_A, KEY_S, KEY_D},
        {KEY_SHIFT, KEYPAD_ASTERIX, KEY_Z, KEY_X},
        {KEY_CTRL, KEY_FN, KEY_GUI, KEY_ALT},
    };

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
    //Reset the pressed key list
    pressedAmount = 0;
    currentModifier = 0;

    //Loop through the rows turning them on one by one
    for(uint8_t i = 0; i < ROW_AMOUNT; ++i)
    {
        //Turn the row on
        digitalWrite(ROW_PINS[i], LOW); //Low because the teensy has a built in pull-up resistor

        delayMicroseconds(READ_DELAY);
        //Looping through the collumns and checking if any of them are LOW
        for(uint8_t n = 0; n < COL_AMOUNT; ++n)
        {
            int signal = digitalRead(COL_PINS[n]);

            if(signal == LOW)
            {
                //Converting the matrix signal to a keypress
                int keyCode = Z::Board::KEYMAP[i][n];
                
                //If this is a regular keypress
                if(keyCode > 0)
                {
                    //Add the key
                    pressedKeys[pressedAmount] = keyCode;

                    //Increment the amount of pressed keys
                    pressedAmount++;

                    Serial.println("Got regular key");
                }
                else
                {
                    handleModifier(keyCode);
                }
            }
        }

        digitalWrite(ROW_PINS[i], HIGH); //Reset the pin;
    }

    updateSentKeys();
    sendKeys();
}


/////////////////////////////////////////////////////////////////////////////////////
//                      Private memebers
/////////////////////////////////////////////////////////////////////////////////////

void Z::Board::updateSentKeys()
{
    uint8_t keysToSend[MAX_PRESSED_KEYS];
    uint8_t keysToSendAmount = 0;
    //Checking which of the pressed keys are already being sent
    for(uint8_t i = 0; i < pressedAmount; ++i)
    {
        bool beingSent = false;
        for(uint8_t n = 0; n < MAX_SENT_KEYS; ++n)
        {
            if(pressedKeys[i] == sentKeys[n])
            {
                beingSent = true;
            }
        }

        if(!beingSent)
        {
            keysToSend[keysToSendAmount] = pressedKeys[i];
            keysToSendAmount++;
        }
    }

    //Go through the list of sent keys and add any pressed keys not already on it to the list where 
    //there is room
    for(uint8_t i = 0; i < MAX_SENT_KEYS; ++i)
    {
        if(sentKeys[i] == 0) //We can send another key
        {
            if(keysToSendAmount > 0)
            {
                sentKeys[i] = keysToSend[keysToSendAmount - 1];
                keysToSendAmount--;
            }
        }
    }

    //Finally, go through all the keys that are being sent and checking if any of them should not 
    //be sent because it was being sent last frame. I suspect that you need to send the 0 byte between
    //sending new keys
    for(uint8_t i = 0; i < MAX_SENT_KEYS; ++i)
    {
        bool isPressed = false;
        for(uint8_t n = 0; n < pressedAmount; n++)
        {
            if(sentKeys[i] == pressedKeys[n])
            {
                isPressed = true;
            }
        }

        //The key isn't pressed anymore, remove it from the list of sent keys
        if(!isPressed) 
        {
            sentKeys[i] = 0;
        }
    }
}
void Z::Board::sendKeys()
{
    //Tell the keyboard class what keys to send
    Keyboard.set_key1(sentKeys[0]);
    Keyboard.set_key2(sentKeys[1]);
    Keyboard.set_key3(sentKeys[2]);
    Keyboard.set_key4(sentKeys[3]);
    Keyboard.set_key5(sentKeys[4]);
    Keyboard.set_key6(sentKeys[5]);

    //TODO: Modifier keys
    Keyboard.set_modifier(currentModifier);
    
    //Send the message
    Keyboard.send_now();
}
void Z::Board::handleModifier(int keyCode)
{
    switch(keyCode)
    {
        case(KEY_SHIFT):
        {
            currentModifier = currentModifier | MODIFIERKEY_SHIFT;

            break;
        }
        case(KEY_CTRL):
        {
            currentModifier = currentModifier | MODIFIERKEY_CTRL;

            break;
        }
        case(KEY_ALT):
        {
            currentModifier = currentModifier | MODIFIERKEY_ALT;
            break;
        }
        case(KEY_GUI):
        {
            currentModifier = currentModifier | MODIFIERKEY_GUI;
            break;
        }
    }
}

