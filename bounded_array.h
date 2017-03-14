#ifndef H_BOUNDED_ARRAY
#define H_BOUNDED_ARRAY

#include "debug.h"


/*
  Stack allocated array containing a fixed amount of elements.
*/
template <typename T, std::size_t LENGTH>
class BoundedArray
{
    public:
        BoundedArray();

        const T& operator[](std::size_t index)
        {
            if(index < current_capacity)
            {
                return elements[index];
            }
            else
            {
                exception("Bounded array access out of bounds");
            }
        }
    private:
        T elements[LENGTH];

        std::size_t current_capacity = 0;
};

#endif
