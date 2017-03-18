#ifndef H_BOUNDED_ARRAY
#define H_BOUNDED_ARRAY

#include "debug.h"

/*
  Stack allocated array containing a fixed amount of elements.
*/
template <typename T, size_t MAX_LENGTH>
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

        const T& operator[](const size_t index) const
        {
            if(index < length)
            {
                return elements[index];
            }
            else
            {
                exception("Bounded array access out of bounds");
                return this->elements[0];
            }
        }
        T& operator[](const size_t index)
        {
            if(index < length)
            {
                return elements[index];
            }
            else
            {
                exception("Bounded array access out of bounds");
                return this->elements[0];
            }
        }

        /*
         * Returns the amount of elements in the array
         */
        size_t size() const
        {
            return length;
        }

        /*
         * "Removes" all elements in the array
         *
         * This is done by simply resetting length to 0
        */
        void reset()
        {
            this->length = 0;
        }

        /**
         * Searches for the specified needle in the array
         */
        bool contains(T needle) const
        {
            for(size_t i = 0; i < length; ++i)
            {
                if((*this)[i] == needle)
                {
                    return true;
                }
            }
            return false;
        }
    private:
        T elements[MAX_LENGTH];

        size_t length = 0;
};

#endif
