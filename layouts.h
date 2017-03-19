#ifndef H_LAYOUTS
#define H_LAYOUTS
#include "keyboard_functional.h"

const Keycode default_layout[4][6] = {
    {KEY_TAB, KEY_Q, KEY_W, KEY_E, KEY_R, KEY_T},
    {KEY_ESC, KEY_A, KEY_S, KEY_D, KEY_F, KEY_G},
    {MODIFIERKEY_SHIFT, KEY_Z, KEY_X, KEY_C, KEY_V, KEY_B},
    {MODIFIERKEY_CTRL, MODIFIERKEY_ALT, MODIFIERKEY_GUI, MODIFIERKEY_SHIFT, FN_LOWER, KEY_SPACE}
};

#endif
