#ifndef __ANDROID__

#include <cstdio>
#include <cstdarg>
#include <fstream>
#include <Windows.h>
#include "Log.h"

#define DEBUG_LOG


namespace uPacketFragmentation
{


void DebugLog(const char *format, ...)
{
#ifdef DEBUG_LOG
    static char buf[1024];
    constexpr size_t n = sizeof(buf);
    memset(buf, 0, n);
    va_list args;
    va_start(args, format);
    vsnprintf_s(buf, n, _TRUNCATE, format, args);
    va_end(args);
    ::OutputDebugStringA(buf);
    ::OutputDebugStringA("\n");
#endif
}


}


#endif
