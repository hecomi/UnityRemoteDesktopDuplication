#pragma once

#include <vector>
#include <atomic>
#include <memory>
#include <d3d11.h>
#include <wrl/client.h>
#include "nvEncodeAPI.h"
#include "Common.h"


namespace uNvEncoder
{


struct NvencDesc
{
    ComPtr<ID3D11Device> d3d11Device; 
    uint32_t width = 1920; 
    uint32_t height = 1080;
    DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM;
    uint32_t frameRate = 60;
    uint32_t bitRate = 2'000'000;
    uint32_t maxFrameSize = 2'000'000 / 60;
};


struct NvencEncodedData
{
    uint64_t index = 0;
    std::unique_ptr<uint8_t[]> buffer;
    uint32_t size = 0;
};


class Nvenc final
{
public:
    explicit Nvenc(const NvencDesc &desc);
    ~Nvenc();
    void Initialize();
    void Finalize();
    bool IsValid() const { return encoder_ != nullptr; }
    void Reconfigure(const NvencDesc &desc);
    void Encode(const ComPtr<ID3D11Texture2D> &source, bool forceIdrFrame);
    void GetEncodedData(std::vector<NvencEncodedData> &data);

private:
    void ThrowErrorIfNotInitialized();

    void OpenEncodeSession();
    void InitializeEncoder();
    void CreateInitializeParams();
    void DestroyEncoder();
    void CreateCompletionEvents();
    void DestroyCompletionEvents();
    void CreateBitstreamBuffers();
    void DestroyBitstreamBuffers();
    void CreateInputTextures();
    void RegisterResources();
    void UnregisterResources();

    void CopyToInputTexture(int index, const ComPtr<ID3D11Texture2D> &texture);
    bool EncodeInputTexture(int index, bool forceIdrFrame);
    void MapInputResource(int index);
    void UnmapInputResource(int index);
    bool WaitForCompletion(int index, DWORD duration);
    void EndEncode();
    void SendEOS();

    unsigned long GetResourceCount() const { return static_cast<unsigned long>(resources_.size()); }
    unsigned long GetInputIndex() const { return inputIndex_ % GetResourceCount(); }
    unsigned long GetOutputIndex() const { return outputIndex_ % GetResourceCount(); }

    NvencDesc desc_;
    NV_ENC_INITIALIZE_PARAMS initParams_ = { NV_ENC_INITIALIZE_PARAMS_VER };
    NV_ENC_CONFIG encConfig_ = { NV_ENC_CONFIG_VER };;
    bool isInitialized_ = false;
    void *encoder_ = nullptr;
    uint64_t inputIndex_ = 0U;
    uint64_t outputIndex_ = 0U;

    struct Resource
    {
        ComPtr<ID3D11Texture2D> inputTexture_ = nullptr;
        HANDLE inputTextureSharedHandle_ = nullptr;
        NV_ENC_REGISTERED_PTR registeredResource_ = nullptr;
        NV_ENC_INPUT_PTR inputResource_ = nullptr;
        NV_ENC_OUTPUT_PTR bitstreamBuffer_ = nullptr;
        void *completionEvent_ = nullptr;
        std::atomic<bool> isEncoding_ = false;
    };
    std::vector<Resource> resources_;

public:
    static void LoadModule();
    static void UnloadModule();

private:
    static HMODULE s_module;
    static NV_ENCODE_API_FUNCTION_LIST s_nvenc;
    static uint32_t s_referenceCount;
};


}
