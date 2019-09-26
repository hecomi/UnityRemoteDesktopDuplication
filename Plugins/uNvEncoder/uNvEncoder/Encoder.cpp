#include "Encoder.h"


namespace uNvEncoder
{


Encoder::Encoder(const EncoderDesc &desc)
    : desc_(desc)
{
    try
    {
        CreateDevice();
        CreateNvenc();
        StartThread();
    }
    catch (const std::exception& e)
    {
        error_ = e.what();
    }
}


Encoder::~Encoder()
{
    try
    {
        StopThread();
        DestroyNvenc();
        DestroyDevice();
    }
    catch (const std::exception& e)
    {
        error_ = e.what();
    }
}


bool Encoder::IsValid() const
{
    return device_ && nvenc_ && nvenc_->IsValid();
}


NvencDesc Encoder::CreateNvencDesc() const
{
    NvencDesc desc = { 0 };
    desc.d3d11Device = device_;
    desc.width = desc_.width;
    desc.height = desc_.height;
    desc.format = desc_.format;
    desc.frameRate = desc_.frameRate;
    desc.bitRate = desc_.bitRate;
    desc.maxFrameSize = desc_.maxFrameSize;
    return desc;
}


void Encoder::Reconfigure(const EncoderDesc &encDesc)
{
    if (!IsValid()) return;

    desc_ = encDesc;
    nvenc_->Reconfigure(CreateNvencDesc());
}


void Encoder::CreateDevice()
{
    ComPtr<IDXGIDevice1> dxgiDevice;
    if (FAILED(GetUnityDevice()->QueryInterface(IID_PPV_ARGS(&dxgiDevice)))) 
    {
        ThrowError("Failed to get IDXGIDevice1.");
        return;
    }

    ComPtr<IDXGIAdapter> dxgiAdapter;
    if (FAILED(dxgiDevice->GetAdapter(&dxgiAdapter))) 
    {
        ThrowError("Failed to get IDXGIAdapter.");
        return;
    }

    constexpr auto driverType = D3D_DRIVER_TYPE_UNKNOWN;
    constexpr auto flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
    constexpr D3D_FEATURE_LEVEL featureLevelsRequested[] =
    {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_3,
        D3D_FEATURE_LEVEL_9_2,
        D3D_FEATURE_LEVEL_9_1
    };
    constexpr UINT numLevelsRequested = sizeof(featureLevelsRequested) / sizeof(D3D_FEATURE_LEVEL);
    D3D_FEATURE_LEVEL featureLevelsSupported;

    D3D11CreateDevice(
        dxgiAdapter.Get(),
        driverType,
        nullptr,
        flags,
        featureLevelsRequested,
        numLevelsRequested,
        D3D11_SDK_VERSION,
        &device_,
        &featureLevelsSupported,
        nullptr);
}


void Encoder::DestroyDevice()
{
    device_ = nullptr;
}


void Encoder::CreateNvenc()
{
    nvenc_ = std::make_unique<Nvenc>(CreateNvencDesc());
    nvenc_->Initialize();
}


void Encoder::DestroyNvenc()
{
    nvenc_->Finalize();
    nvenc_.reset();
}


void Encoder::StartThread()
{
    encodeThread_ = std::thread([&]
    {
        while (!shouldStopEncodeThread_)
        {
            WaitForEncodeRequest();
            UpdateGetEncodedData();
        }
    });
}


void Encoder::StopThread()
{
    shouldStopEncodeThread_ = true;
    encodeCond_.notify_one();

    if (encodeThread_.joinable())
    {
        encodeThread_.join();
    }
}


bool Encoder::Encode(const ComPtr<ID3D11Texture2D> &source, bool forceIdrFrame)
{
    try
    {
        nvenc_->Encode(source, forceIdrFrame);
    }
    catch (const std::exception& e)
    {
        error_ = e.what();
        return false;
    }

    RequestGetEncodedData();
    return true;
}


bool Encoder::Encode(HANDLE sharedHandle, bool forceIdrFrame)
{
    ComPtr<ID3D11Texture2D> source;
    if (FAILED(GetUnityDevice()->OpenSharedResource(
        sharedHandle,
        __uuidof(ID3D11Texture2D),
        &source)))
    {
        return false;
    }

    return Encode(source, forceIdrFrame);
}


void Encoder::WaitForEncodeRequest()
{
    std::unique_lock<std::mutex> encodeLock(encodeMutex_);
    encodeCond_.wait(encodeLock, [&] 
    { 
        return isEncodeRequested || shouldStopEncodeThread_; 
    });
    isEncodeRequested = false;
}


void Encoder::RequestGetEncodedData()
{
    std::lock_guard<std::mutex> lock(encodeMutex_);
    isEncodeRequested = true;
    encodeCond_.notify_one();
}


void Encoder::UpdateGetEncodedData()
{
    std::vector<NvencEncodedData> data;

    try
    {
        nvenc_->GetEncodedData(data);
    }
    catch (const std::exception& e)
    {
        error_ = e.what();
        return;
    }

    std::lock_guard<std::mutex> dataLock(encodeDataListMutex_);
    for (auto &ed : data)
    {
        encodedDataList_.push_back(std::move(ed));
    }
}


void Encoder::CopyEncodedDataList()
{
    std::lock_guard<std::mutex> lock(encodeDataListMutex_);

    encodedDataListCopied_.clear();
    std::swap(encodedDataListCopied_, encodedDataList_);
}


const std::vector<NvencEncodedData> & Encoder::GetEncodedDataList() const
{
    return encodedDataListCopied_;
}


}
