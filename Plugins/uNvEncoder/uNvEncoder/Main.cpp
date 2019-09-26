#include <memory>
#include <map>
#include <d3d11.h>
#include <IUnityInterface.h>
#include "Encoder.h"
#include "Nvenc.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")


using namespace uNvEncoder;
using EncoderId = int;


namespace uNvEncoder
{
    IUnityInterfaces *g_unity = nullptr;
}


namespace
{
    std::map<EncoderId, std::unique_ptr<Encoder>> g_encoders;
    EncoderId g_encoderId = 0;
}


extern "C"
{


UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API UnityPluginLoad(IUnityInterfaces* unityInterfaces)
{
    g_unity = unityInterfaces;
}


UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API UnityPluginUnload()
{
    g_unity = nullptr;
}


const std::unique_ptr<Encoder> & GetEncoder(EncoderId id)
{
    static std::unique_ptr<Encoder> invalid;
    const auto it = g_encoders.find(id);
    return (it != g_encoders.end()) ? it->second : invalid;
}


UNITY_INTERFACE_EXPORT EncoderId UNITY_INTERFACE_API uNvEncoderCreate(const EncoderDesc &desc)
{
    const auto id = g_encoderId++;
    auto encoder = std::make_unique<Encoder>(desc);
    g_encoders.emplace(id, std::move(encoder));
    return id;
}


UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API uNvEncoderDestroy(EncoderId id)
{
    g_encoders.erase(id);
}


UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API uNvEncoderReconfigure(int id, const EncoderDesc &desc)
{
    if (const auto &encoder = GetEncoder(id))
    {
        encoder->Reconfigure(desc);
    }
}


UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API uNvEncoderIsValid(EncoderId id)
{
    const auto &encoder = GetEncoder(id);
    return encoder ? encoder->IsValid() : false;
}


UNITY_INTERFACE_EXPORT int UNITY_INTERFACE_API uNvEncoderGetWidth(EncoderId id)
{
    const auto &encoder = GetEncoder(id);
    return encoder ? static_cast<int>(encoder->GetDesc().width) : 0;
}


UNITY_INTERFACE_EXPORT int UNITY_INTERFACE_API uNvEncoderGetHeight(EncoderId id)
{
    const auto &encoder = GetEncoder(id);
    return encoder ? static_cast<int>(encoder->GetDesc().height) : 0;
}


UNITY_INTERFACE_EXPORT DXGI_FORMAT UNITY_INTERFACE_API uNvEncoderGetFormat(EncoderId id)
{
    const auto &encoder = GetEncoder(id);
    return encoder ? encoder->GetDesc().format : DXGI_FORMAT_UNKNOWN;
}


UNITY_INTERFACE_EXPORT int UNITY_INTERFACE_API uNvEncoderGetFrameRate(EncoderId id)
{
    const auto &encoder = GetEncoder(id);
    return encoder ? static_cast<int>(encoder->GetDesc().frameRate) : 0;
}


UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API uNvEncoderEncode(EncoderId id, ID3D11Texture2D *texture, bool forceIdrFrame)
{
    if (const auto &encoder = GetEncoder(id))
    {
        return encoder->Encode(ComPtr<ID3D11Texture2D>(texture), forceIdrFrame);
    }
    return false;
}


UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API uNvEncoderEncodeSharedHandle(EncoderId id, HANDLE handle, bool forceIdrFrame)
{
    if (const auto &encoder = GetEncoder(id))
    {
        return encoder->Encode(handle, forceIdrFrame);
    }
    return false;
}


UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API uNvEncoderCopyEncodedData(EncoderId id)
{
    if (const auto &encoder = GetEncoder(id))
    {
        encoder->CopyEncodedDataList();
    }
}


UNITY_INTERFACE_EXPORT int UNITY_INTERFACE_API uNvEncoderGetEncodedDataCount(EncoderId id)
{
    const auto &encoder = GetEncoder(id);
    return encoder ? static_cast<int>(encoder->GetEncodedDataList().size()) : 0;
}


UNITY_INTERFACE_EXPORT int UNITY_INTERFACE_API uNvEncoderGetEncodedDataSize(EncoderId id, int index)
{
    const auto &encoder = GetEncoder(id);
    if (!encoder) return 0;

    const auto &list = encoder->GetEncodedDataList();
    if (index < 0 || index >= static_cast<int>(list.size())) return 0;

    return static_cast<int>(list.at(index).size);
}


UNITY_INTERFACE_EXPORT const void * UNITY_INTERFACE_API uNvEncoderGetEncodedDataBuffer(EncoderId id, int index)
{
    const auto &encoder = GetEncoder(id);
    if (!encoder) return nullptr;

    const auto &list = encoder->GetEncodedDataList();
    if (index < 0 || index >= static_cast<int>(list.size())) return nullptr;

    return list.at(index).buffer.get();
}


UNITY_INTERFACE_EXPORT const char * UNITY_INTERFACE_API uNvEncoderGetError(EncoderId id)
{
    const auto &encoder = GetEncoder(id);
    return encoder ? encoder->GetError().c_str() : nullptr;
}


UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API uNvEncoderHasError(EncoderId id)
{
    const auto &encoder = GetEncoder(id);
    return encoder ? encoder->HasError() : false;
}


UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API uNvEncoderClearError(EncoderId id)
{
    if (const auto &encoder = GetEncoder(id))
    {
        encoder->ClearError();
    }
}


}