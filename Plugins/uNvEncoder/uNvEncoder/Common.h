#pragma once

#include <chrono>
#include <functional>
#include <string>
#include <sstream>
#include <thread>
#include <wrl/client.h>


namespace uNvEncoder
{


template <class T>
using ComPtr = Microsoft::WRL::ComPtr<T>;


struct IUnityInterfaces * GetUnity();
struct ID3D11Device * GetUnityDevice();
void ThrowError(const std::string &error);


#define UNVENC_DEBUG_ON


class ScopedTimer final
{
public:
    using StartFunc = std::function<void()>;
    using EndFunc = std::function<void(const std::chrono::microseconds &)>;
    ScopedTimer(const StartFunc &startFunc, const EndFunc &endFunc);
    ~ScopedTimer();

private:
    const EndFunc func_;
    const std::chrono::time_point<std::chrono::steady_clock> start_;
};

#ifdef UNVENC_DEBUG_ON
#define UNVENC_FUNC_SCOPED_TIMER \
    ScopedTimer _timer_##__COUNTER__( \
    [] \
    { \
        std::stringstream ss; \
        const auto threadId = std::this_thread::get_id(); \
        ss << threadId << ": " << __FUNCTION__ << "@" << __FILE__ << ":" << __LINE__ << " => {" << std::endl; \
        ::OutputDebugStringA(ss.str().c_str()); \
    }, \
    [](const std::chrono::microseconds &us) \
    { \
        std::stringstream ss; \
        const auto threadId = std::this_thread::get_id(); \
        ss << threadId << ": " << "} " << __FUNCTION__ << "@" << __FILE__ << ":" << __LINE__ << " => " << us.count() << " [us]" << std::endl; \
        ::OutputDebugStringA(ss.str().c_str()); \
    });
#else
#define UNVENC_FUNC_SCOPED_TIMER
#endif


}
