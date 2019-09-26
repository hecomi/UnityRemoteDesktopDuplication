#include <map>
#include "IUnityInterface.h"
#include "Assembler.h"
#include "Fragmenter.h"

using namespace uPacketFragmentation;


namespace
{
    uint32_t g_assemblerId = 0U;
    std::map<int, std::unique_ptr<PacketAssembler>> g_assemblers;

    uint32_t g_fragmenterId = 0U;
    std::map<int, std::unique_ptr<PacketFragmenter>> g_fragmenters;
}


extern "C"
{


UNITY_INTERFACE_EXPORT int UNITY_INTERFACE_API UpfCreateFragmenter()
{
    const auto id = g_fragmenterId++;
    auto fragmenter = std::make_unique<PacketFragmenter>();
    g_fragmenters.emplace(id, std::move(fragmenter));
    return id;
}


UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API UpfDestroyFragmenter(uint32_t id)
{
    g_fragmenters.erase(id);
}


const std::unique_ptr<PacketFragmenter> & GetFragmenter(uint32_t id)
{
    static std::unique_ptr<PacketFragmenter> invalid = nullptr;
    const auto it = g_fragmenters.find(id);
    return (it != g_fragmenters.end()) ? it->second : invalid;
}


UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API UpfFragmenterIsValid(uint32_t id)
{
    const auto it = g_fragmenters.find(id);
    return it != g_fragmenters.end();
}


UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API UpfFragmenterSetMaxPacketSize(uint32_t id, uint32_t size)
{
    if (const auto &fragmenter = GetFragmenter(id))
    {
        fragmenter->SetMaxPacketSize(size);
    }
}


UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API UpfFragmenterFragmentData(uint32_t id, void *pData, uint32_t size)
{
    if (const auto &fragmenter = GetFragmenter(id))
    {
        fragmenter->FragmentData(pData, size);
    }
}


UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API UpfFragmenterGetFragmentCount(uint32_t id)
{
    if (const auto &fragmenter = GetFragmenter(id))
    {
        fragmenter->GetFragmentCount();
    }
}


UNITY_INTERFACE_EXPORT const void * UNITY_INTERFACE_API UpfFragmenterGetFragmentData(uint32_t id, uint32_t index)
{
    if (const auto &fragmenter = GetFragmenter(id))
    {
        return fragmenter->GetFragmentData(index);
    }
    return nullptr;
}


UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API UpfFragmenterGetFragmentSize(uint32_t id, uint32_t index)
{
    if (const auto &fragmenter = GetFragmenter(id))
    {
        return fragmenter->GetFragmentSize(index);
    }
    return 0;
}


// ---


UNITY_INTERFACE_EXPORT int UNITY_INTERFACE_API UpfCreateAssembler()
{
    const auto id = g_assemblerId++;
    auto assembler = std::make_unique<PacketAssembler>();
    g_assemblers.emplace(id, std::move(assembler));
    return id;
}


UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API UpfDestroyAssembler(uint32_t id)
{
    g_assemblers.erase(id);
}


const std::unique_ptr<PacketAssembler> & GetAssembler(uint32_t id)
{
    static std::unique_ptr<PacketAssembler> invalid = nullptr;
    const auto it = g_assemblers.find(id);
    return (it != g_assemblers.end()) ? it->second : invalid;
}


UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API UpfAssemblerIsValid(uint32_t id)
{
    const auto it = g_assemblers.find(id);
    return it != g_assemblers.end();
}


UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API UpfAssemblerAddData(uint32_t id, const void *pData, int size)
{
    if (const auto &assembler = GetAssembler(id))
    {
        assembler->AddData(pData, static_cast<uint32_t>(size));
    }
}


UNITY_INTERFACE_EXPORT PacketEventType UNITY_INTERFACE_API UpfAssemblerGetPacketEventType(uint32_t id)
{
    if (const auto &assembler = GetAssembler(id))
    {
        return assembler->GetPacketEventType();
    }
    return PacketEventType::None;
}


UNITY_INTERFACE_EXPORT PacketLossType UNITY_INTERFACE_API UpfAssemblerGetPacketLossType(uint32_t id)
{
    if (const auto &assembler = GetAssembler(id))
    {
        return assembler->GetPacketLossType();
    }
    return PacketLossType::None;
}


UNITY_INTERFACE_EXPORT uint64_t UNITY_INTERFACE_API UpfAssemblerGetAssembledFrameIndex(uint32_t id)
{
    if (const auto &assembler = GetAssembler(id))
    {
        return assembler->GetAssembledFrameIndex();
    }
    return -1;
}


UNITY_INTERFACE_EXPORT const char * UNITY_INTERFACE_API UpfAssemblerGetFrameData(uint32_t id, uint64_t index)
{
    if (const auto &assembler = GetAssembler(id))
    {
        return assembler->GetFrameData(index);
    }
    return nullptr;
}


UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API UpfAssemblerGetFrameSize(uint32_t id, uint64_t index)
{
    if (const auto &assembler = GetAssembler(id))
    {
        return assembler->GetFrameSize(index);
    }
    return 0;
}


UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API UpfAssemblerRemoveFrame(uint32_t id, uint64_t index)
{
    if (const auto &assembler = GetAssembler(id))
    {
        return assembler->RemoveFrame(index);
    }
}


}
