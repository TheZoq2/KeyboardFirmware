#ifndef H_LAYOUTS
#define H_LAYOUTS
#include "keyboard_functional.h"

const uint8_t LAYER_AMOUNT = 3;

const uint8_t WIDTH = 6;
const uint8_t FULL_WIDTH = WIDTH * 2;
const uint8_t HEIGHT = 4;

enum Layers
{
    NORMAL,
    LOWERED,
    RAISED
};

const Z::Keycode DEFAULT_LAYER[HEIGHT][FULL_WIDTH] = {
    {KEY_TAB,           KEY_Q, KEY_W, KEY_E, KEY_R, KEY_T, KEY_Y, KEY_U, KEY_I,        KEY_O,      KEY_P,         KEY_BACKSPACE},
    {KEY_ESC,           KEY_A, KEY_S, KEY_D, KEY_F, KEY_G, KEY_H, KEY_J, KEY_K,        KEY_L,      KEY_SEMICOLON, KEY_ENTER},
    {MODIFIERKEY_SHIFT, KEY_QUOTE,    KEY_Z, KEY_X, KEY_C, KEY_V, KEY_B, KEY_N, KEY_M, KEY_COMMA, KEY_PERIOD, KEY_SLASH},

    {MODIFIERKEY_CTRL,  MODIFIERKEY_ALT, KEY_LEFT_BRACE, MODIFIERKEY_GUI, Z::FN_LOWER, KEY_SPACE, KEY_BACKSPACE, Z::FN_RAISE, MODIFIERKEY_RIGHT_ALT, KEY_RIGHT_BRACE, 0x0F, 0x0F}
};

const Z::Keycode LOWER_LAYER[HEIGHT][FULL_WIDTH] = {
    {
        KEY_TILDE,           KEY_1, KEY_2,  KEY_2, KEY_4, KEY_5, KEY_5, KEY_7, KEY_8,        KEY_9,      KEY_0,         KEY_BACKSPACE},
    {KEY_ESC,           KEY_A, KEY_S,  KEY_D, KEY_F, KEY_G, KEY_H, KEY_EQUAL, KEY_RIGHT_BRACE,        KEY_LEFT_BRACE,      KEY_SEMICOLON, KEY_QUOTE},
    {MODIFIERKEY_SHIFT, KEY_NON_US_BS, KEY_Z, KEY_X, KEY_C, KEY_V, KEY_B, KEY_N, KEY_M, KEY_COMMA, KEY_PERIOD, KEY_SLASH},

    {MODIFIERKEY_CTRL,  MODIFIERKEY_ALT, KEY_LEFT_BRACE, MODIFIERKEY_GUI, Z::FN_LOWER, KEY_SPACE, KEY_BACKSPACE, Z::FN_RAISE, MODIFIERKEY_RIGHT_ALT, KEY_RIGHT_BRACE, 0x0F, 0x0F}
};

Z::KeyboardStateManager<LAYER_AMOUNT, FULL_WIDTH, HEIGHT> init_state_manager()
{
    //auto layer_change_function = [](uint8_t current_layer, Z::KeyTypes<FULL_WIDTH * HEIGHT> keys)
    //{
    //    (void)(current_layer); //Avoid the unused parameter warning

    //    if(keys.function_keys.contains(Z::FunctionKey::FN_LOWER))
    //    {
    //        return LOWERED;
    //    }
    //    else if(keys.function_keys.contains(Z::FunctionKey::FN_RAISE))
    //    {
    //        return RAISED;
    //    }
    //    else
    //    {
    //        return NORMAL;
    //    }
    //};

    //Z::Keymap<FULL_WIDTH, HEIGHT> keymaps[LAYER_AMOUNT] = {
    //    Z::init_keymap<FULL_WIDTH, HEIGHT>(DEFAULT_LAYER),
    //    Z::init_keymap<FULL_WIDTH, HEIGHT>(LOWER_LAYER),
    //    Z::init_keymap<FULL_WIDTH, HEIGHT>(DEFAULT_LAYER) //TODO: Add a raised layer
    //};
    //auto keymap_array = BoundedArray<Z::Keymap<FULL_WIDTH, HEIGHT>, LAYER_AMOUNT>(keymaps);
    return Z::KeyboardStateManager<LAYER_AMOUNT, FULL_WIDTH, HEIGHT>(
            //keymap_array 
        );
            //layer_change_function
}
#endif
