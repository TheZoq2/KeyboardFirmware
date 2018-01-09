#ifndef H_LAYOUTS
#define H_LAYOUTS
#include "keyboard_functional.h"

const uint8_t LAYER_AMOUNT = 3;

const uint8_t WIDTH = 6;
const uint8_t FULL_WIDTH = WIDTH * 2;
const uint8_t HEIGHT = 4;

enum class KeyboardState
{
    NORMAL,
    LOWERED,
    RAISED,
    WM_LAYER,
    HARDWARE_VIM,
    SPECIAL
};

const Z::Keycode DEFAULT_LAYER[HEIGHT][FULL_WIDTH] = {
              {KEY_TAB, KEY_Q,                    KEY_W, KEY_E,          KEY_R, KEY_T,         KEY_Y, KEY_U,                       KEY_I, KEY_O,                   KEY_P, KEY_BACKSPACE},
              {KEY_ESC, KEY_A,                    KEY_S, KEY_D,          KEY_F, KEY_G,         KEY_H, KEY_J,                       KEY_K, KEY_L,           KEY_SEMICOLON, KEY_ENTER},
    {MODIFIERKEY_SHIFT, KEY_Z,                    KEY_X, KEY_C,          KEY_V, KEY_B,         KEY_N, KEY_M,                   KEY_COMMA, KEY_PERIOD,          KEY_SLASH, KEY_QUOTE},

     {MODIFIERKEY_CTRL, MODIFIERKEY_ALT, KEY_LEFT_BRACE, Z::FN_WM, Z::FN_LOWER, KEY_SPACE, Z::FN_VIM, Z::FN_RAISE, MODIFIERKEY_RIGHT_ALT, KEY_RIGHT_BRACE,          0x0F, Z::FN_SPECIAL}
};

const Z::Keycode LOWER_LAYER[HEIGHT][FULL_WIDTH] = {
            {KEY_TILDE, KEY_1,                    KEY_2, KEY_3,          KEY_4, KEY_5,                 KEY_6, KEY_7,                       KEY_8, KEY_9,                   KEY_0, KEY_BACKSPACE},
              {KEY_ESC, KEY_MINUS,                KEY_S, KEY_D,          KEY_F, KEY_G,                 KEY_H, KEY_EQUAL,          KEY_LEFT_BRACE, KEY_RIGHT_BRACE, KEY_SEMICOLON, KEY_QUOTE},
    {MODIFIERKEY_SHIFT, KEY_Z,                    KEY_X, KEY_C,          KEY_V, KEY_B,                 KEY_N, KEY_M,                   KEY_COMMA, KEY_PERIOD,          KEY_SLASH, KEY_NON_US_BS},

     {MODIFIERKEY_CTRL, MODIFIERKEY_ALT, KEY_LEFT_BRACE, Z::FN_WM, Z::FN_LOWER, KEY_SPACE, MODIFIERKEY_SHIFT, Z::FN_RAISE, MODIFIERKEY_RIGHT_ALT, KEY_RIGHT_BRACE,          0x0F, Z::FN_SPECIAL}
};

const Z::Keycode WM_LAYER[HEIGHT][FULL_WIDTH] = {
            {KEY_TILDE, KEY_1,                    KEY_2, KEY_3,          KEY_4, KEY_5,                 KEY_6, KEY_7,                       KEY_8, KEY_9,                   KEY_0, KEY_BACKSPACE},
              {KEY_TAB, KEY_Q,                    KEY_S, KEY_T,          KEY_R, KEY_G,                 KEY_H, KEY_J,                       KEY_K, KEY_L,           KEY_SEMICOLON, KEY_ENTER},
    {MODIFIERKEY_SHIFT, KEY_Z,                    KEY_X, KEY_C,          KEY_V, KEY_B,                 KEY_N, KEY_M,                   KEY_COMMA, KEY_PERIOD,          KEY_SLASH, KEY_NON_US_BS},

     {MODIFIERKEY_CTRL, MODIFIERKEY_ALT, KEY_LEFT_BRACE, Z::FN_WM, Z::FN_LOWER, KEY_SPACE, MODIFIERKEY_SHIFT, Z::FN_RAISE, MODIFIERKEY_RIGHT_ALT, KEY_RIGHT_BRACE,          0x0F, Z::FN_SPECIAL}
};


const Z::Keycode VIM_LAYER[HEIGHT][FULL_WIDTH] = {
              {KEY_TAB, KEY_Q,                    KEY_W, KEY_E,               KEY_R, KEY_T,         KEY_Y, KEY_PAGE_UP,              KEY_HOME, KEY_O,                   KEY_P, KEY_BACKSPACE},
              {KEY_ESC, KEY_END,                KEY_ESC, KEY_PAGE_DOWN,       KEY_F, KEY_G,      KEY_LEFT, KEY_DOWN,                   KEY_UP, KEY_RIGHT,       KEY_SEMICOLON, KEY_ENTER},
    {MODIFIERKEY_SHIFT, KEY_Z,                    KEY_X, KEY_C,               KEY_V, KEY_B,         KEY_N, KEY_M,                   KEY_COMMA, KEY_PERIOD,          KEY_SLASH, KEY_QUOTE},

     {MODIFIERKEY_CTRL, MODIFIERKEY_ALT, KEY_LEFT_BRACE, Z::FN_WM,      Z::FN_LOWER, KEY_SPACE, Z::FN_VIM, Z::FN_RAISE, MODIFIERKEY_RIGHT_ALT, KEY_RIGHT_BRACE,          0x0F, Z::FN_SPECIAL}
};

const Z::Keycode SPECIAL_LAYER[HEIGHT][FULL_WIDTH] = {
            {KEY_TILDE, KEY_F1,                    KEY_F2, KEY_F3,          KEY_F4, KEY_5,                 KEY_F6, KEY_F7,                       KEY_F8, KEY_F9,                   KEY_F10, KEY_DELETE},
              {KEY_TAB, KEY_Q,                    KEY_S, KEY_T,          KEY_R, KEY_G,                 KEY_H, KEY_J,                       KEY_K, KEY_L,           KEY_SEMICOLON, KEY_ENTER},
    {MODIFIERKEY_SHIFT, KEY_Z,                    KEY_X, KEY_C,          KEY_V, KEY_B,                 KEY_N, KEY_M,                   KEY_COMMA, KEY_PERIOD,          KEY_SLASH, KEY_NON_US_BS},

     {MODIFIERKEY_CTRL, MODIFIERKEY_ALT, KEY_LEFT_BRACE, Z::FN_WM, Z::FN_LOWER, KEY_SPACE, MODIFIERKEY_SHIFT, Z::FN_RAISE, MODIFIERKEY_RIGHT_ALT, KEY_RIGHT_BRACE,          0x0F, Z::FN_SPECIAL}
};



/*
  Function that determines the current state based on the old state aswell as what
  keys have been pressed
*/
KeyboardState get_new_state(
        const KeyboardState old_state, 
        const Z::KeyTypes<FULL_WIDTH*HEIGHT>& pressed_keys
    )
{
    //Prevent unused variable warning for now
    (void)(old_state);
    if(pressed_keys.contains_functionkey(Z::FunctionKey::FN_LOWER))
    {
        return KeyboardState::LOWERED;
    }
    else if(pressed_keys.contains_functionkey(Z::FunctionKey::FN_RAISE))
    {
        return KeyboardState::RAISED;
    }
    else if(pressed_keys.contains_functionkey(Z::FunctionKey::FN_WM))
    {
        return KeyboardState::WM_LAYER;
    }
    else if(pressed_keys.contains_functionkey(Z::FunctionKey::FN_VIM))
    {
        return KeyboardState::HARDWARE_VIM;
    }
    else if(pressed_keys.contains_functionkey(Z::FunctionKey::FN_SPECIAL))
    {
        return KeyboardState::SPECIAL;
    }
    else
    {
        return KeyboardState::NORMAL;
    }
}


/*
  Function that returns the current keymap based on what state the keyboard is in
*/
Z::Keymap<FULL_WIDTH, HEIGHT> get_current_keymap(KeyboardState state)
{
    switch(state) 
    {
        case KeyboardState::NORMAL:
        {
            return Z::init_keymap<FULL_WIDTH, HEIGHT>(DEFAULT_LAYER);
        }
        case KeyboardState::LOWERED:
        {
            return Z::init_keymap<FULL_WIDTH, HEIGHT>(LOWER_LAYER);
        }
        case KeyboardState::RAISED:
        {
            return Z::init_keymap<FULL_WIDTH, HEIGHT>(LOWER_LAYER);
        }
        case KeyboardState::WM_LAYER:
        {
            return Z::init_keymap<FULL_WIDTH, HEIGHT>(WM_LAYER);
        }
        case KeyboardState::HARDWARE_VIM:
        {
            return Z::init_keymap<FULL_WIDTH, HEIGHT>(VIM_LAYER);
        }
        case KeyboardState::SPECIAL:
        {
            return Z::init_keymap<FULL_WIDTH, HEIGHT>(SPECIAL_LAYER);
        }
        default:
            exception("Unimplemented keyboard state");
            return Z::init_keymap<FULL_WIDTH, HEIGHT>(DEFAULT_LAYER);
    }
}

/*
  Returns the modifiers that should always be active when the state is active
*/
uint16_t get_state_modifier(KeyboardState state)
{
    switch(state) 
    {
        case KeyboardState::WM_LAYER:
        {
            return MODIFIERKEY_GUI;
        }
        case KeyboardState::RAISED:
        {
            return MODIFIERKEY_SHIFT;
        }
        default:
            return 0;
    }
}

#endif
