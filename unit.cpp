#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

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
