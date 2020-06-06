#pragma once

#include "WProgram.h"
#include <stdlib.h>


#define _ENABLE_SERIAL 0
#if _ENABLE_SERIAL == 1
#define DBG Serial.printf
#else
static int _dummyprintf(const char *, ...)
{
    return 0;
}
#define DBG _dummyprintf
#endif


#ifdef __arm__
    // should use uinstd.h to define sbrk but Due causes a conflict
    extern "C" char* sbrk(int incr);
#else  // __ARM__
    extern char *__brkval;
    extern char __bss_end;
#endif  // __arm__

// function from the sdFat library (SdFatUtil.cpp)
// licensed under GPL v3
// Full credit goes to William Greiman.
static int FreeRam() {
    char top;
    #ifdef __arm__
        return &top - reinterpret_cast<char*>(sbrk(0));
    #else  // __arm__
        return __brkval ? &top - __brkval : &top - &__bss_end;
    #endif  // __arm__
}