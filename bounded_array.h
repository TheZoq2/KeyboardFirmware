#ifndef H_BOUNDED_ARRAY
#define H_BOUNDED_ARRAY

#include "debug.h"

/*
  Stack allocated array containing a fixed amount of elements.
*/
template <typename T, std::size_t MAX_LENGTH>
class BoundedArray
{
    public:
        BoundedArray(){}

        /**
          Appends a single element to the end of the currently stored array
        */
        void push(T element)
        {
            if(this->length >= MAX_LENGTH)
            {
                exception("Bounded array insertion would overflow");
            }

            this->elements[this->length] = element;
            length++;
        }

        const T& operator[](std::size_t index) const
        {
            if(index < length)
            {
                return elements[index];
            }
            else
            {
                exception("Bounded array access out of bounds");
            }
        }

        std::size_t size() const
        {
            return length;
        }

        void reset()
        {
            this->length = 0;
        }
    private:
        T elements[MAX_LENGTH];

        std::size_t length = 0;
};

#endif
