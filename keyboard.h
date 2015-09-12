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
        
        //The delay between setting the row HIGH and reading the value
        //in microseconds
        static const int READ_DELAY; 
        
    };
};
#endif
