#ifndef H_KEYBOARD_FUNCTIONAL
#define H_KEYBOARD_FUNCTIONAL

#include "debug.h"
#include "bounded_array.h"
#include <functional>

//See keycodes.h for details

//The amount of possible modifier keys
const uint8_t MODIFIER_AMOUNT=8;
const uint16_t KEYTYPE_MASK = 0xFF00;
const uint16_t KEYCODE_MASK = 0x00FF;


enum class KeySignature : uint16_t
{
    FUNCTION = 0x0000,
    MODIFIER = 0xE000,
    MEDIA = 0xE400,
    SYSTEM = 0xE200,
    STANDARD = 0xF000,
};
enum class KeyType
{
    FN,
    MODIFIER,
    MEDIA,
    SYSTEM,
    STANDARD,
    UNRECOGNISED
};

using Keycode = uint16_t;

/*
 * List of modifier keys on the keyboard
 */
enum FunctionKey
{
    FN_RAISE,
    FN_LOWER,
    //Helper for getting the amount of modifiers
    FN_AMOUNT
};

using FunctionKeyList = BoundedArray<bool, FN_AMOUNT>;
FunctionKeyList init_function_key_list();


/*
 * Maps are stored in a column-row structure to allow
 * [x][y] to be [horizontal][vertical]
 *
 * [
 *   x1:[y1,y2,y3...]
 *   x2:[y1,y2,y3...]
 *   ...
 * ]
*/
template<std::size_t WIDTH, std::size_t HEIGHT>
using Keymap = BoundedArray< BoundedArray<Keycode, HEIGHT>, WIDTH>;

template<std::size_t WIDTH, std::size_t HEIGHT>
Keymap<WIDTH, HEIGHT> init_keymap(const Keycode map[HEIGHT][WIDTH])
{
    auto result = Keymap<WIDTH, HEIGHT>();

    for(std::size_t col_i = 0; col_i < WIDTH; ++col_i)
    {
        auto column = BoundedArray<Keycode, HEIGHT>();
        for(std::size_t row_i = 0; row_i < HEIGHT; ++row_i)
        {
            column.push(map[row_i][col_i]);
        }
        result.push(column);
    }

    return result;
}




Keycode function_keycode(const FunctionKey modifier);
KeyType get_key_type(const Keycode key);
FunctionKey function_from_keycode(const Keycode key);

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
template<std::size_t WIDTH, std::size_t HEIGHT>
BoundedArray<Keycode, WIDTH*HEIGHT> translate_coordinates(
        const BoundedArray<KeyCoordinate, WIDTH*HEIGHT> coordinates,
        const Keymap<WIDTH, HEIGHT> map
    )
{
    auto result = BoundedArray<Keycode, WIDTH*HEIGHT>();
    for(std::size_t i = 0; i < coordinates.size(); i++)
    {
        result.push(map[coordinates[i].x][coordinates[i].y]);
    }

    return result;
}



template<std::size_t KEY_AMOUNT>
struct Keytypes
{
    FunctionKeyList function_keys;
    BoundedArray<Keycode, KEY_AMOUNT> standard_keys;
    BoundedArray<Keycode, MODIFIER_AMOUNT> modifiers;

    Keytypes()
    {
        this->function_keys = init_function_key_list();
    }
};

/*
  Creates a list of modifiers and keys from a list of keycodes

  TODO: Optimze this to not copy the keys
*/
template<std::size_t KEY_AMOUNT>
Keytypes<KEY_AMOUNT> keycodes_to_keytypes(const BoundedArray<Keycode, KEY_AMOUNT> keys)
{
    Keytypes<KEY_AMOUNT> result;
    for(std::size_t i = 0; i < keys.size(); ++i)
    {
        auto key_type = get_key_type(keys[i]);
        if(key_type == KeyType::FN)
        {
            result.function_keys[function_from_keycode(keys[i])] = true;
        }
        else if(key_type == KeyType::MODIFIER)
        {
            result.modifiers.push(keys[i]);
        }
        else if(key_type == KeyType::STANDARD)
        {
            result.standard_keys.push(keys[i]);
        }
    }
    return result;
}



#endif
