#ifndef H_DEBUG
#define H_DEBUG


#ifdef IS_ARDUINO
    //Printing error messages
    #define DEBUG_PRINTLN(msg) Serial.println(msg)
    #define DEBUG_PRINT(msg) Serial.print(msg)
#else
    #include <iostream>
    //Printing error messages
    #define DEBUG_PRINTLN(msg) std::cout << msg << std::endl
    #define DEBUG_PRINT(msg) std::cout << msg << std::endl
#endif


void exception(char* message)
{
    DEBUG_PRINT("An exception occured: ");
    DEBUG_PRINTLN(message);

#ifdef IS_ARDUINO
    exit(1);
#else
    throw "Out of bounds";
#endif
}
#endif
