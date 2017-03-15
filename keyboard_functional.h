#ifndef H_KEYBOARD_FUNCTIONAL
#define H_KEYBOARD_FUNCTIONAL

#include "bounded_array.h"
#include <functional>

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


class Modifiers
{
    bool shift;
    bool fn;

};

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


#endif
