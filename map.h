#ifndef H_MAP
#define H_MAP

#include <stdint.h>

namespace Z
{
    template<class T_KEY, class T_VAL>
    class Map
    {
    public:
        Map(T_KEY* keys, T_VAL* values, uint16_t amount)
        {
            this->keys = keys;
            this->values = values;
    
            this->amount = amount;
        }
        
        T_VAL* getValue(T_KEY key)
        {
            for(uint16_t i = 0; i < amount; i++)
            {
                if(keys[i] == key)
                {
                    return &values[i];
                }
            }
            return nullptr;
        }
    private:
        uint16_t amount;
    
        T_KEY* keys;
        T_VAL* values;
    };
}
#endif
