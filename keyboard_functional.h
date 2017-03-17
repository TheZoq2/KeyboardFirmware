#ifndef H_KEYBOARD_FUNCTIONAL
#define H_KEYBOARD_FUNCTIONAL

#include "bounded_array.h"
#include <functional>

using Keycode = uint16_t;

/*
 * List of modifier keys on the keyboard
 */
enum Modifier
{
    MOD_CTRL,
    MOD_SHIFT,
    MOD_RIGHT_ALT,
    MOD_ALT,
    MOD_FN,
    MOD_RAISE,
    MOD_LOWER,
    MOD_GUI,
    //Helper for getting the amount of modifiers
    MODIFIER_AMOUNT
};

using ModifierList = BoundedArray<bool, MODIFIER_AMOUNT>;
ModifierList init_modifier_list()
{
    auto result = ModifierList();

    for(uint8_t i = 0; i < MODIFIER_AMOUNT; i++)
    {
        result.push(false);
    }
    return result;
}


template<std::size_t WIDTH, std::size_t HEIGHT>
using Keymap = BoundedArray< BoundedArray<Keycode, HEIGHT>, WIDTH>;

template<std::size_t WIDTH, std::size_t HEIGHT>
Keymap<WIDTH, HEIGHT> init_keymap(const Keycode map[HEIGHT][WIDTH])
{
    auto result = Keymap<WIDTH, HEIGHT>();

    for(std::size_t x = 0; x < WIDTH; ++x)
    {
        auto inner_result = BoundedArray<Keycode, HEIGHT>();
        for(std::size_t y = 0; y < HEIGHT; ++y)
        {
            inner_result.push(map[y][x]);
        }
        result.push(inner_result);
    }

    return result;
}


/*
 * The start of modifier keys in keycodes
*/
static const uint16_t MODIFIER_OFFSET = 256;

Keycode modifier_keycode(const Modifier modifier)
{
    return modifier + MODIFIER_OFFSET;
}
Modifier modifier_from_keycode(const Keycode code)
{
    if(code < MODIFIER_OFFSET)
    {
        exception("Tried to go from keycode to modifier for a keycode that is not a modifier");
    }

    Keycode decoded = Modifier(code - MODIFIER_OFFSET);
    if(decoded >= MODIFIER_AMOUNT)
    {
        exception("Tried to go from keycode to modifier but got a too high modifier value");
    }
    return static_cast<Modifier>(decoded);
}

/**
 Struct containing the current state of the keyboard. These are things
 like the current layer we are operating in
*/
struct KeyboardState
{
    uint8_t layer;
};


/**
  Struct for representing a raw pressed key without any association
  with a specified function
*/
struct KeyCoordinate
{
    uint8_t x;
    uint8_t y;

    KeyCoordinate(){};
    KeyCoordinate(uint8_t x, uint8_t y)
    {
        this->x = x;
        this->y = y;
    }

    bool operator==(const KeyCoordinate& other) const
    {
        return other.x == this->x && other.y == this->y;
    }
    bool operator!=(const KeyCoordinate& other) const
    {
        return !(other == *this);
    }
};

/*
  Merges key coordinates of array1 with array2 with the coordinates of array2.
  The coordinaes of array2 are modified using `function` before being put
  in the result.
*/
template<std::size_t SIZE>
BoundedArray<KeyCoordinate, SIZE*2> merge_coordinates(
            BoundedArray<KeyCoordinate, SIZE> array1,
            BoundedArray<KeyCoordinate, SIZE> array2,
            std::function<KeyCoordinate(KeyCoordinate)> function
        )
{
    BoundedArray<KeyCoordinate, SIZE*2> result;

    for(std::size_t i = 0; i < array1.size(); ++i)
    {
        result.push(array1[i]);
    }

    for(std::size_t i = 0; i < array2.size(); i++)
    {
        result.push(function(array2[i]));
    }

    return result;
}


/*
  Translates a list of coordinates into key codes
*/
template<std::size_t SIZE, std::size_t WIDTH, std::size_t HEIGHT>
BoundedArray<Keycode, SIZE> translate_coordinates(
        const BoundedArray<KeyCoordinate, SIZE> coordinates,
        const Keymap<WIDTH, HEIGHT> map
    )
{
    auto result = BoundedArray<Keycode, SIZE>();
    for(std::size_t i = 0; i < coordinates.size(); i++)
    {
        result.push(map[coordinates.x][coordinates.y]);
    }

    result;
}


#endif
