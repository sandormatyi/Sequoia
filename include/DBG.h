#pragma once

#define _ENABLE_SERIAL 1


#if _ENABLE_SERIAL == 1
#define DBG Serial.printf
#else
int _dummyprintf(const char*, ...);
#define DBG _dummyprintf
#endif

// function from the sdFat library (SdFatUtil.cpp)
// licensed under GPL v3
// Full credit goes to William Greiman.
int FreeRam();