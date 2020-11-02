#include "DBG.h"

#include "WProgram.h"
#include <stdlib.h>

#if _ENABLE_SERIAL == 0
int _dummyprintf(const char*, ...)
{
  return 0;
}
#endif

#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char* sbrk(int incr);
#else  // __ARM__
extern char* __brkval;
extern char __bss_end;
#endif // __arm__

int FreeRam()
{
  char top;
#ifdef __arm__
  return &top - reinterpret_cast<char*>(sbrk(0));
#else  // __arm__
  return __brkval ? &top - __brkval : &top - &__bss_end;
#endif // __arm__
}