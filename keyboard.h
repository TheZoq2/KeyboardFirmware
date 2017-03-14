#ifndef H_KEYBOARD
#define H_KEYBOARD

#include <Arduino.h>

#include <stdint.h>
#include "map.h"

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

        static const int FN_AMOUNT;
        static const uint8_t FN_KEYS[];
        static const uint8_t FN_VALUES[];

        //The delay between setting the row HIGH and reading the value
        //in microseconds
        static const int READ_DELAY; 

        static const uint8_t MAX_PRESSED_KEYS = 128;
        static const uint8_t MAX_SENT_KEYS = 6;

        //The minimum amount of time between two keypresses of the same key
        //in loop iterations
        static const int16_t RECENT_KEY_COOLDOWN;

        ////////////////////////////////////////
        void updateSentKeys();
        void sendKeys();
        void handleModifier(int keyCode);
        int translateFN(int keycode);

        bool isKeyRecent(int keycode);
        void addRecentKey(int keycode);
        void updateRecentKeys();
        ////////////////////////////////////////

        //The keys that are currently being pressed
        uint8_t pressedAmount;
        uint8_t pressedKeys[MAX_PRESSED_KEYS];
        uint8_t sentKeys[MAX_SENT_KEYS];

        uint8_t recentKeys[MAX_PRESSED_KEYS];
        int16_t recentKeyTimes[MAX_PRESSED_KEYS];
        uint8_t recentKeyAmount;

        uint8_t currentModifier;
    };
};
#endif
