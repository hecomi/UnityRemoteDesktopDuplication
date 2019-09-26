#include <cstdint>
#include <IUnityInterface.h>
#include <IUnityRenderingExtensions.h>
#include "NvPipe.h"


using namespace uNvPipe;


extern "C"
{


UNITY_INTERFACE_EXPORT Encoder::Id UNITY_INTERFACE_API uNvPipeCreateEncoder()
{
    return Encoder::CreateInstance();
}


UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API uNvPipeDeleteEncoder(Encoder::Id id)
{
    Encoder::DeleteInstance(id);
}


UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API uNvPipeIsEncoderValid(Encoder::Id id)
{
    if (const auto &encoder = Encoder::GetInstance(id))
    {
        return encoder->IsValid();
    }
    return false;
}


UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API uNvPipeInitializeEncoder(Encoder::Id id)
{
    if (const auto &encoder = Encoder::GetInstance(id))
    {
        return encoder->Initialize();
    }
    return false;
}


UNITY_INTERFACE_EXPORT const char * UNITY_INTERFACE_API uNvPipeEncoderGetError(Encoder::Id id)
{
    if (const auto &encoder = Encoder::GetInstance(id))
    {
        return encoder->GetError();
    }
    return NvPipe_GetError(nullptr);
}


UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API uNvPipeSetEncoderWidth(Encoder::Id id, uint32_t width)
{
    if (const auto &encoder = Encoder::GetInstance(id))
    {
        encoder->SetWidth(width);
    }
}


UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API uNvPipeSetEncoderHeight(Encoder::Id id, uint32_t height)
{
    if (const auto &encoder = Encoder::GetInstance(id))
    {
        encoder->SetHeight(height);
    }
}


UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API uNvPipeSetEncoderFormat(Encoder::Id id, NvPipe_Format format)
{
    if (const auto &encoder = Encoder::GetInstance(id))
    {
        encoder->SetFormat(format);
    }
}


UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API uNvPipeSetEncoderCodec(Encoder::Id id, NvPipe_Codec codec)
{
    if (const auto &encoder = Encoder::GetInstance(id))
    {
        encoder->SetCodec(codec);
    }
}


UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API uNvPipeSetEncoderCompression(Encoder::Id id, NvPipe_Compression compression)
{
    if (const auto &encoder = Encoder::GetInstance(id))
    {
        encoder->SetCompression(compression);
    }
}


UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API uNvPipeSetEncoderBitrate(Encoder::Id id, float bitrate)
{
    if (const auto &encoder = Encoder::GetInstance(id))
    {
        encoder->SetBitrate(bitrate);
    }
}


UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API uNvPipeSetEncoderFps(Encoder::Id id, uint32_t fps)
{
    if (const auto &encoder = Encoder::GetInstance(id))
    {
        encoder->SetFps(fps);
    }
}


UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API uNvPipeEncoderEncode(Encoder::Id id, const void *pData, bool forceIframe)
{
    if (const auto &encoder = Encoder::GetInstance(id))
    {
        return encoder->Encode(pData, forceIframe);
    }
    return false;
}


UNITY_INTERFACE_EXPORT uint64_t UNITY_INTERFACE_API uNvPipeGetEncoderEncodedSize(Encoder::Id id)
{
    if (const auto &encoder = Encoder::GetInstance(id))
    {
        return encoder->GetEncodedSize();
    }
    return 0;
}


UNITY_INTERFACE_EXPORT const uint8_t * UNITY_INTERFACE_API uNvPipeGetEncoderEncodedData(Encoder::Id id)
{
    if (const auto &encoder = Encoder::GetInstance(id))
    {
        return encoder->GetEncodedData();
    }
    return nullptr;
}


// ---


UNITY_INTERFACE_EXPORT Decoder::Id UNITY_INTERFACE_API uNvPipeCreateDecoder()
{
    return Decoder::CreateInstance();
}


UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API uNvPipeDeleteDecoder(Decoder::Id id)
{
    Decoder::DeleteInstance(id);
}


UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API uNvPipeIsDecoderValid(Decoder::Id id)
{
    if (const auto &decoder = Decoder::GetInstance(id))
    {
        return decoder->IsValid();
    }
    return false;
}


UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API uNvPipeInitializeDecoder(Decoder::Id id)
{
    if (const auto &decoder = Decoder::GetInstance(id))
    {
        return decoder->Initialize();
    }
    return false;
}


UNITY_INTERFACE_EXPORT const char * UNITY_INTERFACE_API uNvPipeDecoderGetError(Decoder::Id id)
{
    if (const auto &decoder = Decoder::GetInstance(id))
    {
        return decoder->GetError();
    }
    return nullptr;
}


UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API uNvPipeSetDecoderWidth(Decoder::Id id, uint32_t width)
{
    if (const auto &decoder = Decoder::GetInstance(id))
    {
        decoder->SetWidth(width);
    }
}


UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API uNvPipeSetDecoderHeight(Decoder::Id id, uint32_t height)
{
    if (const auto &decoder = Decoder::GetInstance(id))
    {
        decoder->SetHeight(height);
    }
}


UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API uNvPipeSetDecoderFormat(Decoder::Id id, NvPipe_Format format)
{
    if (const auto &decoder = Decoder::GetInstance(id))
    {
        decoder->SetFormat(format);
    }
}


UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API uNvPipeSetDecoderCodec(Decoder::Id id, NvPipe_Codec codec)
{
    if (const auto &decoder = Decoder::GetInstance(id))
    {
        decoder->SetCodec(codec);
    }
}


UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API uNvPipeDecoderDecode(Decoder::Id id, const void *pData, uint32_t size)
{
    if (const auto &decoder = Decoder::GetInstance(id))
    {
        return decoder->Decode(pData, size);
    }
    return false;
}


UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API uNvPipeGetDecoderDecodedSize(Decoder::Id id)
{
    if (const auto &decoder = Decoder::GetInstance(id))
    {
        return decoder->GetDecodedSize();
    }
    return 0;
}


UNITY_INTERFACE_EXPORT const uint8_t * UNITY_INTERFACE_API uNvPipeGetDecoderDecodedData(Decoder::Id id)
{
    if (const auto &decoder = Decoder::GetInstance(id))
    {
        return decoder->GetDecodedData();
    }
    return nullptr;
}


void UNITY_INTERFACE_API OnTextureUpdate(int eventId, void *pData)
{
    const auto event = static_cast<UnityRenderingExtEventType>(eventId);

    if (event == kUnityRenderingExtEventUpdateTextureBeginV2 ||
        event == kUnityRenderingExtEventUpdateTextureEndV2)
    {
        auto *pParams = static_cast<UnityRenderingExtTextureUpdateParamsV2*>(pData);
        const auto id = static_cast<Decoder::Id>(pParams->userData);
        if (const auto &decoder = Decoder::GetInstance(id))
        {
            decoder->OnTextureUpdate(eventId, pData);
        }
    }
}


UNITY_INTERFACE_EXPORT UnityRenderingEventAndData UNITY_INTERFACE_API uNvPipeDecoderGetTextureUpdateCallback()
{
    return OnTextureUpdate;
}


}
