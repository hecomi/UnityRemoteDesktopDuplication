#pragma once

#include <cstdio>
#include <vector>
#include <memory>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <d3d11.h>
#include "Common.h"
#include "Nvenc.h"


namespace uNvEncoder
{


struct NvencEncodedData;


struct EncoderDesc
{
    int width; 
    int height;
    int frameRate;
    int bitRate;
    int maxFrameSize;
    DXGI_FORMAT format;
};


class Encoder final
{
public:
    explicit Encoder(const EncoderDesc &desc);
    ~Encoder();
    bool IsValid() const;
    void Reconfigure(const EncoderDesc &desc);
    bool Encode(const ComPtr<ID3D11Texture2D> &source, bool forceIdrFrame);
    bool Encode(HANDLE sharedHandle, bool forceIdrFrame);
    void CopyEncodedDataList();
    const std::vector<NvencEncodedData> & GetEncodedDataList() const;
    const EncoderDesc & GetDesc() const { return desc_; }
    bool HasError() const { return !error_.empty(); }
    const std::string & GetError() const { return error_; }
    void ClearError() { error_.clear(); }

private:
    NvencDesc CreateNvencDesc() const;
    void CreateDevice();
    void DestroyDevice();
    void CreateNvenc();
    void DestroyNvenc();
    void StartThread();
    void StopThread();
    void WaitForEncodeRequest();
    void RequestGetEncodedData();
    void UpdateGetEncodedData();

    EncoderDesc desc_;
    ComPtr<ID3D11Device> device_;
    std::unique_ptr<class Nvenc> nvenc_;
    std::vector<NvencEncodedData> encodedDataList_;
    std::vector<NvencEncodedData> encodedDataListCopied_;
    std::thread encodeThread_;
    std::condition_variable encodeCond_;
    std::mutex encodeMutex_;
    std::mutex encodeDataListMutex_;
    bool shouldStopEncodeThread_ = false;
    bool isEncodeRequested = false;
    std::string error_;
};


}
