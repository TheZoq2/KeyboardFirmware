#include "keyboard_functional.h"

using namespace Z;

/*
  Returns the keycode for a functionkey
*/
Keycode Z::function_keycode(const FunctionKey modifier)
{
    return modifier | static_cast<uint16_t>(KeySignature::FUNCTION);
}
/*
  Returns the function key that corresponds to a specific keycode
*/
Z::FunctionKey Z::function_from_keycode(const Keycode key)
{
    if((get_key_type(key) != KeyType::FN) || key >= FN_AMOUNT)
    {
        exception("Key passed to modifier_from_keycode is not a valid modifier");
    }
    return (FunctionKey) key;
}

/*
  Returns the type of a key
*/
Z::KeyType Z::get_key_type(const Keycode key)
{
    uint16_t signature = key & KEYTYPE_MASK;

    switch(signature)
    {
        case static_cast<uint16_t>(KeySignature::FUNCTION):
            return KeyType::FN;
        case static_cast<uint16_t>(KeySignature::MODIFIER):
            return KeyType::MODIFIER;
        case static_cast<uint16_t>(KeySignature::MEDIA):
            return KeyType::MEDIA;
        case static_cast<uint16_t>(KeySignature::SYSTEM):
            return KeyType::SYSTEM;
        case static_cast<uint16_t>(KeySignature::STANDARD):
            return KeyType::STANDARD;
        default:
            DEBUG_PRINT("Unrecognised key signature ");
            DEBUG_PRINTLN(signature);
            return KeyType::UNRECOGNISED;
    }
}



/*
  Initializes a list of modifiers
*/
Z::FunctionKeyList Z::init_function_key_list()
{
    auto result = FunctionKeyList();

    for(uint8_t i = 0; i < FN_AMOUNT; i++)
    {
        result.push(false);
    }
    return result;
}
