#pragma once

#ifdef __ANDROID__

#include <utility>
#include "android/log.h"


namespace uPacketFragmentation
{

template <class T>
void DebugLog(const T &str)
{
    __android_log_print(ANDROID_LOG_DEBUG, "uPacketFragmentation", "%s", str);
}

template <class ... Args>
void DebugLog(const Args &... args)
{
    __android_log_print(ANDROID_LOG_DEBUG, "uPacketFragmentation", args...);
}

}

#else

namespace uPacketFragmentation
{

void DebugLog(const char *format, ...);

}

#endif
