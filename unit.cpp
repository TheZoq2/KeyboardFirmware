#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include "bounded_array.h"

TEST_CASE( "Simple bounded array", "[bounded_array]" ) {
    BoundedArray<int, 5> array;
    REQUIRE(array.size() == 0);
    array.push(1);
    array.push(2);
    array.push(3);
    REQUIRE_THROWS(array[4]);
    REQUIRE(array.size() == 3);
    array.push(4);
    array.push(5);

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
