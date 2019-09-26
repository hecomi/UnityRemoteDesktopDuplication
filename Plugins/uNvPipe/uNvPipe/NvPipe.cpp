#include <cstdint>
#include <IUnityRenderingExtensions.h>
#include "NvPipe.h"


namespace uNvPipe
{


bool Encoder::Initialize()
{
    const uint64_t bps = static_cast<uint64_t>(
        static_cast<double>(bitrateMbps_) * 1000 * 1000);

    const uint32_t encodeSize = width_ * height_ * 4;
    data_.resize(encodeSize);

    nvpipe_ = std::shared_ptr<::NvPipe>(
        NvPipe_CreateEncoder(
            format_, 
            codec_, 
            compression_, 
            bps, 
            targetFps_, 
            width_, 
            height_),
        [](auto* pPtr)
        {
            NvPipe_Destroy(pPtr);
        });

    return nvpipe_ != nullptr;
}


bool Encoder::Encode(const void *pData, bool forceIframe)
{
    if (!IsValid() || pData == nullptr) return false;

    size_ = static_cast<uint32_t>(
        NvPipe_Encode(
            nvpipe_.get(),
            pData,
            static_cast<uint64_t>(width_) * 4,
            data_.data(),
            data_.size(),
            width_,
            height_,
            forceIframe));

    return size_ != 0;
}


// ---


bool Decoder::Initialize()
{
    nvpipe_ = std::shared_ptr<::NvPipe>(
        NvPipe_CreateDecoder(
            format_, 
            codec_,
            width_, 
            height_),
        [](auto* pPtr)
        {
            NvPipe_Destroy(pPtr);
        });

    data_[0] = std::make_unique<uint8_t[]>(GetDecodedSize());
    data_[1] = std::make_unique<uint8_t[]>(GetDecodedSize());

    return nvpipe_ != nullptr;
}


bool Decoder::Decode(const void *pData, uint32_t size)
{
    if (!IsValid() || pData == nullptr) return false;

    const auto r = NvPipe_Decode(
        nvpipe_.get(),
        static_cast<const uint8_t*>(pData), 
        size,
        data_[0].get(), 
        width_, 
        height_);

    if (r != GetDecodedSize()) return false;

    {
        std::lock_guard<std::mutex> lock(dataMutex_);
        std::swap(data_[0], data_[1]);
    }

    return true;
}


const uint32_t Decoder::GetDecodedSize() const
{
    return width_ * height_ * 4;
}


const uint8_t * Decoder::GetDecodedData() const
{
    return data_[1].get();
}


void Decoder::OnTextureUpdate(int eventId, void *pData)
{
    const auto event = static_cast<UnityRenderingExtEventType>(eventId);

    if (event == kUnityRenderingExtEventUpdateTextureBeginV2)
    {
        auto *pParams = static_cast<UnityRenderingExtTextureUpdateParamsV2*>(pData);
        if (width_ != pParams->width || 
            height_ != pParams->height)
        {
            return;
        }

        {
            std::lock_guard<std::mutex> lock(dataMutex_);
            pParams->texData = data_[1].get();
        }
    }
}


}