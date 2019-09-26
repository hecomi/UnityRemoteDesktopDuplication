#include <d3d11.h>
#include <IUnityInterface.h>
#include <IUnityGraphicsD3D11.h>
#include "Common.h"


namespace uNvEncoder
{


extern IUnityInterfaces *g_unity;


IUnityInterfaces * GetUnity()
{
    return g_unity;
}


ID3D11Device * GetUnityDevice()
{
    return GetUnity()->Get<IUnityGraphicsD3D11>()->GetDevice();
}


void ThrowError(const std::string &error)
{
    ::OutputDebugStringA((error + "\n").c_str());
    throw std::exception(error.c_str());
}


ScopedTimer::ScopedTimer(const StartFunc &startFunc, const EndFunc &endFunc)
    : func_(endFunc)
    , start_(std::chrono::high_resolution_clock::now())
{
    startFunc();
}


ScopedTimer::~ScopedTimer()
{
    using namespace std::chrono;
    const auto end = high_resolution_clock::now();
    const auto time = duration_cast<microseconds>(end - start_);
    func_(time);
}


}
