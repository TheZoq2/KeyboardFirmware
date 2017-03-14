#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include "bounded_array.h"

TEST_CASE( "Simple bounded array", "[bounded_array]" ) {
    BoundedArray<int, 10> array;

    REQUIRE(true);
}
