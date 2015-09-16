#ifndef H_KEYBOARD
#define H_KEYBOARD

#include <Arduino.h>

#include <stdint.h>
        
namespace Z
{
    class Board
    {
    public:
        void setup();
    
        void update();
    private:
        static const uint8_t ROW_PINS[];
        static const uint8_t ROW_AMOUNT;
    
        static const uint8_t COL_PINS[];
        static const uint8_t COL_AMOUNT;

        static const int KEYMAP[6][15];
        
        //The delay between setting the row HIGH and reading the value
        //in microseconds
        static const int READ_DELAY; 

        static const uint8_t MAX_PRESSED_KEYS = 128;
        static const uint8_t MAX_SENT_KEYS = 6;
        
        ////////////////////////////////////////
        void updateSentKeys();
        void sendKeys();
        void handleModifier(int keyCode);
        ////////////////////////////////////////

        //The keys that are currently being pressed
        uint8_t pressedAmount;
        uint8_t pressedKeys[MAX_PRESSED_KEYS];
        uint8_t sentKeys[MAX_SENT_KEYS];
        
        uint8_t currentModifier;

    };
};
#endif
