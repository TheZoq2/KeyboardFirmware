#ifndef H_KEYBOARD_FUNCTIONAL
#define H_KEYBOARD_FUNCTIONAL

#include "debug.h"
#include "bounded_array.h"
#include <functional>

#ifndef KEYCODE_MASK
    #define KEYCODE_MASK 0xff700
#endif

//See keycodes.h for details

namespace Z
{
    //The amount of possible modifier keys
    const uint8_t MODIFIER_AMOUNT=8;
    const uint8_t KEYS_PER_PACKET=6;
    const uint16_t KEYTYPE_MASK = 0xFF00;
    //const uint16_t KEYCODE_MASK = 0x00FF;


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
    template<size_t WIDTH, size_t HEIGHT>
    using Keymap = BoundedArray< BoundedArray<Keycode, HEIGHT>, WIDTH>;

    template<size_t WIDTH, size_t HEIGHT>
    Keymap<WIDTH, HEIGHT> init_keymap(const Keycode map[HEIGHT][WIDTH])
    {
        auto result = Keymap<WIDTH, HEIGHT>();

        for(size_t col_i = 0; col_i < WIDTH; ++col_i)
        {
            auto column = BoundedArray<Keycode, HEIGHT>();
            for(size_t row_i = 0; row_i < HEIGHT; ++row_i)
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
    template<size_t LAYER_AMOUNT, size_t WIDTH, size_t HEIGHT>
    class KeyboardState
    {
        public:
            const Keymap<WIDTH, HEIGHT> get_current_keymap() const
            {
                return keymaps[layer];
            }

        private:
            uint8_t layer;
            BoundedArray<Keymap<WIDTH, HEIGHT>, LAYER_AMOUNT> keymaps;
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
    template<size_t SIZE>
    BoundedArray<KeyCoordinate, SIZE*2> merge_coordinates(
                BoundedArray<KeyCoordinate, SIZE> array1,
                BoundedArray<KeyCoordinate, SIZE> array2,
                std::function<KeyCoordinate(KeyCoordinate)> function
            )
    {
        BoundedArray<KeyCoordinate, SIZE*2> result;

        for(size_t i = 0; i < array1.size(); ++i)
        {
            result.push(array1[i]);
        }

        for(size_t i = 0; i < array2.size(); i++)
        {
            result.push(function(array2[i]));
        }

        return result;
    }


    /*
      Translates a list of coordinates into key codes
    */
    template<size_t WIDTH, size_t HEIGHT>
    BoundedArray<Keycode, WIDTH*HEIGHT> translate_coordinates(
            const BoundedArray<KeyCoordinate, WIDTH*HEIGHT> coordinates,
            const Keymap<WIDTH, HEIGHT> map
        )
    {
        auto result = BoundedArray<Keycode, WIDTH*HEIGHT>();
        for(size_t i = 0; i < coordinates.size(); i++)
        {
            result.push(map[coordinates[i].x][coordinates[i].y]);
        }

        return result;
    }



    /*
      A list of different types of keys that are pressed
    */
    template<size_t KEY_AMOUNT>
    struct KeyTypes
    {
        FunctionKeyList function_keys;
        BoundedArray<Keycode, KEY_AMOUNT> standard_keys;
        BoundedArray<Keycode, MODIFIER_AMOUNT> modifiers;

        KeyTypes()
        {
            this->function_keys = init_function_key_list();
        }
    };

    /*
      Creates a list of modifiers and keys from a list of keycodes

      TODO: Optimze this to not copy the keys
    */
    template<size_t KEY_AMOUNT>
    KeyTypes<KEY_AMOUNT> keycodes_to_keytypes(const BoundedArray<Keycode, KEY_AMOUNT> keys)
    {
        KeyTypes<KEY_AMOUNT> result;
        for(size_t i = 0; i < keys.size(); ++i)
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


    /*
      A keyboard packet that can be sent to the computer
    */
    struct KeyPacket
    {
        uint16_t modifiers;
        BoundedArray<Keycode, KEYS_PER_PACKET> keys;

        KeyPacket()
        {
            modifiers = 0;
        }
    };

    /*
      Generate a KeyPacket from a KeyType list aswell as the last packet that
      was sent
    */
    template<size_t KEY_AMOUNT>
    KeyPacket keytypes_to_packet(const KeyTypes<KEY_AMOUNT> keytypes, const KeyPacket old_packet)
    {
        KeyPacket result;

        //Building the modifiers
        for(size_t i = 0; i < keytypes.modifiers.size(); ++i)
        {
            result.modifiers |= keytypes.modifiers[i];
        }

        //Check which keys should be sent again
        BoundedArray<Keycode, KEY_AMOUNT> keys_not_sent;

        for(size_t i = 0; i < keytypes.standard_keys.size(); ++i)
        {
            if(old_packet.keys.contains(keytypes.standard_keys[i]))
            {
                result.keys.push(keytypes.standard_keys[i]);
            }
            else
            {
                keys_not_sent.push(keytypes.standard_keys[i]);
            }
        }

        //If there are still spots left in the keys_to_send list
        int16_t next_key = keys_not_sent.size() - 1;
        while(result.keys.size() < KEYS_PER_PACKET && next_key >= 0)
        {
            result.keys.push(keys_not_sent[next_key]);
            next_key--;
        }

        return result;
    }
}

#endif
