#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include "keycodes.h"

#include "bounded_array.h"
#include "keyboard_functional.h"

TEST_CASE( "Simple bounded array", "[bounded_array]" ) {
    BoundedArray<int, 5> array;
    REQUIRE_FALSE(array.contains(1));
    REQUIRE(array.size() == 0);
    array.push(1);
    array.push(2);
    array.push(3);
    REQUIRE_THROWS(array[4]);
    REQUIRE(array.size() == 3);
    array.push(4);
    array.push(5);

    REQUIRE(array.contains(1) == true);
    REQUIRE(array.contains(8) == false);


    for(std::size_t i = 0; i < 5; i++)
    {
        REQUIRE(i+1 == array[i]);
    }

    REQUIRE_THROWS(array.push(6));
    REQUIRE_THROWS(array[500]);

    array.reset();
    REQUIRE(array.size() == 0);
    CHECK_NOTHROW(array.push(1));
    REQUIRE_THROWS(array[2]);
}


TEST_CASE("Keyboard coordinate equality", "[keyboard_coordinate]")
{
    auto coordinate1 = KeyCoordinate(0,0);
    auto coordinate2 = KeyCoordinate(0,0);
    auto coordinate3 = KeyCoordinate(0,1);
    auto coordinate4 = KeyCoordinate(1,1);
    auto coordinate5 = KeyCoordinate(1,0);

    REQUIRE(coordinate1 == coordinate2);
    REQUIRE(coordinate2 != coordinate3);
    REQUIRE(coordinate2 != coordinate4);
    REQUIRE(coordinate2 != coordinate5);
}

TEST_CASE("Keyboard combination function", "[keyboard]")
{
    auto coordinates1 = BoundedArray<KeyCoordinate, 10>();
    coordinates1.push(KeyCoordinate(0, 0));
    coordinates1.push(KeyCoordinate(1, 0));
    coordinates1.push(KeyCoordinate(2, 0));
    coordinates1.push(KeyCoordinate(3, 0));

    auto coordinates2 = BoundedArray<KeyCoordinate, 10>();
    coordinates2.push(KeyCoordinate(0, 0));
    coordinates2.push(KeyCoordinate(1, 0));
    coordinates2.push(KeyCoordinate(2, 0));
    coordinates2.push(KeyCoordinate(5, 0));

    auto combine_function = [](KeyCoordinate coord)
    {
        coord.y += 1;
        return coord;
    };

    auto combined = merge_coordinates(coordinates1, coordinates2, combine_function);

    //Contains a coordinate shared by both
    REQUIRE(combined.contains(KeyCoordinate(0,0)));
    //Contains a coordinate shared by both and translated
    REQUIRE(combined.contains(KeyCoordinate(0,1)));
    //Contains a coordinate not in the first array
    REQUIRE(combined.contains(KeyCoordinate(5,1)));
    //Does not contain a coordinate only contained in the second array
    REQUIRE_FALSE(combined.contains(KeyCoordinate(5,0)));
    //Does not contain a coordinate that shouldn't be in any of the parts
    REQUIRE_FALSE(combined.contains(KeyCoordinate(6,1)));
}


TEST_CASE("Keyboard modifier offsets", "[keyboard]")
{
    FunctionKey mod = FN_LOWER;
    Keycode code = function_keycode(mod);
    FunctionKey new_mod = function_from_keycode(code);

    REQUIRE(mod == new_mod);
    REQUIRE_THROWS(function_from_keycode(0x1000)); //Wrong keymask
    REQUIRE_THROWS(function_from_keycode(FN_AMOUNT)); //Modifier index too large
}



TEST_CASE("Simple coverate tests", "[keyboard]")
{
    init_function_key_list();
}


TEST_CASE("Keymap lookup", "[keyboard]")
{
    //Ensuring that the keymap works as expected
    const Keycode raw_keymap[3][4] = {
        {1,4,7,10},
        {2,5,8,11},
        {3,6,9,12}
    };

    auto keymap = init_keymap<4,3>(raw_keymap);

    REQUIRE(keymap[0][0] == 1);
    REQUIRE(keymap[2][2] == 9);
    REQUIRE(keymap[2][1] == 8);
    REQUIRE_THROWS(keymap[0][3]);
    REQUIRE_THROWS(keymap[4][0]);


    //Testing the keymap lookup function
    auto pressed_coords = BoundedArray<KeyCoordinate, 4*3>();
    pressed_coords.push(KeyCoordinate(1,0));
    pressed_coords.push(KeyCoordinate(3,2));

    auto pressed_keys = translate_coordinates<4, 3>(pressed_coords, keymap);

    //Ensure the specified keys are in the list
    REQUIRE(pressed_keys.contains(4));
    REQUIRE(pressed_keys.contains(12));

    REQUIRE(pressed_keys.size() == pressed_coords.size());
}


TEST_CASE("Key type test", "[keyboard]")
{
    CHECK(get_key_type(MODIFIERKEY_RIGHT_ALT) == KeyType::MODIFIER);
    CHECK(get_key_type(KEY_A) == KeyType::STANDARD);
    CHECK(get_key_type(KEY_SYSTEM_POWER_DOWN) == KeyType::SYSTEM);
    CHECK(get_key_type(KEY_MEDIA_MUTE) == KeyType::MEDIA);
    CHECK(get_key_type(0x0f00) == KeyType::UNRECOGNISED);
    CHECK(get_key_type(FunctionKey::FN_RAISE) == KeyType::FN);
}



TEST_CASE("Keytype translation", "[keyboard]")
{
    auto keys = BoundedArray<Keycode, 10>();

    keys.push(KEY_A);
    keys.push(KEY_B);
    keys.push(MODIFIERKEY_RIGHT_ALT);
    keys.push(FunctionKey::FN_RAISE);

    auto keytypes = keycodes_to_keytypes(keys);


    REQUIRE(keytypes.standard_keys.contains(KEY_A));
    REQUIRE(keytypes.standard_keys.contains(KEY_B));
    REQUIRE(keytypes.standard_keys.size() == 2);

    REQUIRE(keytypes.function_keys[FunctionKey::FN_RAISE] == true);
    REQUIRE(keytypes.function_keys[FunctionKey::FN_LOWER] == false);

    REQUIRE(keytypes.modifiers.contains(MODIFIERKEY_RIGHT_ALT));
    REQUIRE(keytypes.modifiers.size() == 1);
}




TEST_CASE("Generating keyboard packets", "[keyboard packet generation]")
{
    //No previous keys and not too many keys
    auto keys = BoundedArray<Keycode, 10>();
    keys.push(KEY_A);
    keys.push(KEY_B);
    keys.push(KEY_C);

    auto old_packet = KeyPacket();

    auto keytypes = keycodes_to_keytypes(keys);
    auto packet = keytypes_to_packet(keytypes, old_packet);

    CHECK(packet.keys.contains(KEY_A));
    CHECK(packet.keys.contains(KEY_B));
    CHECK(packet.keys.contains(KEY_C));
    CHECK(packet.keys.size() == 3);
    CHECK(packet.modifiers == 0);


    //Too many keys for one packet, some modifiers, some old, some old removed
    keys.reset();
    keys.push(KEY_A);
    keys.push(KEY_C);
    keys.push(KEY_D);
    keys.push(KEY_E);
    keys.push(KEY_F);
    keys.push(KEY_G);
    keys.push(KEY_H);
    keys.push(MODIFIERKEY_SHIFT);
    keys.push(MODIFIERKEY_CTRL);

    keytypes = keycodes_to_keytypes(keys);
    packet = keytypes_to_packet(keytypes, packet);

    CHECK(packet.keys.contains(KEY_A));
    CHECK(packet.keys.contains(KEY_C));
    CHECK_FALSE(packet.keys.contains(KEY_B));
    CHECK(packet.keys.size() == 6);
    CHECK(packet.modifiers == (MODIFIERKEY_SHIFT | MODIFIERKEY_CTRL));
}
