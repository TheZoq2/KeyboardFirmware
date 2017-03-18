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
    #define DEBUG_PRINT(msg) std::cout << msg
#endif


void exception(char const* message);
#endif
