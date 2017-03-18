#include "debug.h"

void exception(char const* message)
{
    DEBUG_PRINT("An exception occured: ");
    DEBUG_PRINTLN(message);

#ifdef IS_ARDUINO
    //exit(1);
    abort();
#else
    throw "Bounded array access out of bounds";
#endif
}
