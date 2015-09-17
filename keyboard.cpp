#include "keyboard.h"

//The pins that the rows are connected to
const uint8_t Z::Board::ROW_PINS[] = {3, 4, 5, 6, 1, 2};
const uint8_t Z::Board::ROW_AMOUNT = 6;

const uint8_t Z::Board::COL_PINS[] = {10, 11, 12, 19, 14, 15, 16, 17, 18, 20, 21, 22, 23, 9};
const uint8_t Z::Board::COL_AMOUNT = 15;

const int Z::Board::READ_DELAY = 5;

const int KEY_SHIFT = -1;
const int KEY_CTRL = -2;
const int KEY_ALT = -3;
const int KEY_GUI = -4;

const int KEY_FN = -100;

//REMEMBER TO CHANGE THE SIZE OF THIS IN THE DECLARATION ASWELL
const int Z::Board::KEYMAP[Z::Board::ROW_AMOUNT][Z::Board::COL_AMOUNT] = 
    {
        {KEY_ESC, KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_F11, KEY_F12, KEY_DELETE},
        {KEY_TILDE, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9, KEY_0, KEY_MINUS, KEY_EQUAL, KEY_BACKSPACE},
        {KEY_TAB, KEY_Q, KEY_W, KEY_E, KEY_R, KEY_T, KEY_Y, KEY_U, KEY_I, KEY_O, KEY_P, KEY_LEFT_BRACE, KEY_RIGHT_BRACE, KEY_BACKSLASH},
        {KEY_CAPS_LOCK, KEY_A, KEY_S, KEY_D, KEY_F, KEY_G, KEY_H, KEY_J, KEY_K, KEY_L, KEY_SEMICOLON, KEY_QUOTE, 0, KEY_ENTER},
        {KEY_SHIFT, KEYPAD_ASTERIX, KEY_Z, KEY_X, KEY_C, KEY_V, KEY_B, KEY_N, KEY_M, KEY_COMMA, KEY_PERIOD, KEY_SLASH, KEY_SHIFT, KEY_HOME},
        {KEY_CTRL, KEY_FN, KEY_GUI, KEY_ALT, 0, 0, KEY_SPACE, 0, 0, KEY_ALT, KEY_FN, KEY_GUI, KEY_CTRL, KEY_END},
    };

int Z::Board::FN_AMOUNT = 4;
int Z::Board::FN_KEYS[] = {KEY_K, KEY_J, KEY_H, KEY_L};
int Z::Board::FN_VALUES[] = {KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT};
Z::Map<int, int> Z::Board::FN_MAP(Z::Board::FN_KEYS, Z::Board::FN_VALUES, FN_AMOUNT);

void Z::Board::setup()
{
    //Alternative mappings for when the FN key is pressed
    //Z::Board::FN_MAP[KEY_K] = KEY_UP;
    //Z::Board::FN_MAP[KEY_J] = KEY_DOWN;
    //Z::Board::FN_MAP[KEY_H] = KEY_LEFT;
    //Z::Board::FN_MAP[KEY_L] = KEY_RIGHT;
    //

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

    bool fnPressed = false;

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
                }
                else if(keyCode == KEY_FN)
                {
                    fnPressed = true;
                }
                else
                {
                    handleModifier(keyCode);
                }
            }
        }

        digitalWrite(ROW_PINS[i], HIGH); //Reset the pin;
    }

    //If the FN key was pressed, we need to remap those keystrokes.
    if(fnPressed)
    {
        for(int i = 0; i < pressedAmount; i++)
        {
            int* fnAlternative = FN_MAP.getValue(pressedKeys[i]);
            if(fnAlternative != NULL)
            {
                pressedKeys[i] = *fnAlternative;
            }
        }
    }

    Serial.println(KEY_H);
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

