#include "debug.h"

void exception(char const* message)
{
    DEBUG_PRINT("An exception occured: ");
    DEBUG_PRINTLN(message);

#ifdef IS_ARDUINO
    //exit(1);
    //abort();
    while(true)
    {
        delay(1000);
        Serial.print("Arduino has crashed with message");
        Serial.println(message);
    }
#else
    throw "Bounded array access out of bounds";
#endif
}
