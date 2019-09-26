#pragma once

#include <unordered_map>
#include <array>
#include <atomic>
#include <mutex>
#include <vector>
#include <memory>
#include <NvPipe.h>


namespace uNvPipe
{


template <class T>
class NvPipeBase
{
public:
    using Id = int;

    static Id CreateInstance()
    {
        const auto currentId = id++;
        instances.emplace(currentId, std::make_shared<T>());
        return currentId;
    }

    static void DeleteInstance(Id id)
    {
        instances.erase(id);
    }

    static const std::shared_ptr<T> & GetInstance(Id id)
    {
        static std::shared_ptr<T> invalid = nullptr;
        const auto it = instances.find(id);
        return it != instances.end() ? it->second : invalid;
    }

private:
    static std::unordered_map<Id, std::shared_ptr<T>> instances;
    static Id id;

public:
    virtual bool Initialize() = 0;
    bool IsValid() const { return nvpipe_ != nullptr; }

    const char * GetError() const
    {
        return NvPipe_GetError(nvpipe_.get());
    }

    void SetWidth(uint32_t width) { width_ = width; }
    void SetHeight(uint32_t height) { height_ = height; }
    void SetFormat(NvPipe_Format format) { format_ = format; }
    void SetCodec(NvPipe_Codec codec) { codec_ = codec; }

protected:
    std::shared_ptr<::NvPipe> nvpipe_;

    uint32_t width_ = 1920;
    uint32_t height_ = 1080;
    NvPipe_Format format_ = NVPIPE_RGBA32;
    NvPipe_Codec codec_ = NVPIPE_H264;
};

template <class T> 
typename NvPipeBase<T>::Id NvPipeBase<T>::id = 0;

template <class T> 
std::unordered_map<typename NvPipeBase<T>::Id, std::shared_ptr<T>> NvPipeBase<T>::instances;


class Encoder : public NvPipeBase<Encoder>
{
public:
    bool Initialize() override;
    bool Encode(const void *pData, bool forceIframe);
    void SetCompression(NvPipe_Compression compression) { compression_ = compression; }
    void SetBitrate(float mbps) { bitrateMbps_ = mbps; }
    void SetFps(uint32_t fps) { targetFps_ = fps; }
    const uint8_t * GetEncodedData() const { return data_.data(); }
    uint32_t GetEncodedSize() const { return size_; }

private:
    NvPipe_Compression compression_ = NVPIPE_LOSSY;
    float bitrateMbps_ = 32;
    uint32_t targetFps_ = 90;
    std::vector<uint8_t> data_;
    uint32_t size_ = 0;
};


class Decoder : public NvPipeBase<Decoder>
{
public:
    using DataIndex = unsigned long;

    bool Initialize() override;
    bool Decode(const void *pData, uint32_t size);
    void OnTextureUpdate(int eventId, void *pData);
    const uint8_t * GetDecodedData() const;
    const uint32_t GetDecodedSize() const;

private:
    std::mutex dataMutex_;
    std::array<std::unique_ptr<uint8_t[]>, 2> data_;
};


}
