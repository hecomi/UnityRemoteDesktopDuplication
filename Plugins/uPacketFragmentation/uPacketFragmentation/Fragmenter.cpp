#include <chrono>
#include <algorithm>
#include "Fragmenter.h"
#include "Common.h"
#include "Log.h"


namespace uPacketFragmentation
{


void PacketFragmenter::FragmentData(const void *pData, uint32_t size)
{
    using namespace std::chrono;

    fragments_.clear();

    constexpr uint32_t headerSize = sizeof(PacketHeader);
    const uint32_t maxBufSize = maxPacketSize_ - headerSize;
    const uint32_t fragmentCount = size / maxBufSize + 1;
    const uint32_t fragmentSize = size / fragmentCount + 1;

    PacketHeader header = { GetPacketHeaderVersion() };
    header.type = static_cast<uint32_t>(PacketType::Fragment);
    header.totalSize = size;
    header.timestamp = time_point_cast<milliseconds>(system_clock::now()).time_since_epoch().count();
    header.frameIndex = frameIndex_++;
    header.fragmentCount = fragmentCount;

    for (uint32_t i = 0; i < fragmentCount; ++i)
    {
        const auto bufSize = static_cast<size_t>(maxBufSize) + headerSize;
        auto buf = std::make_unique<char[]>(bufSize);

        header.fragmentIndex = i;
        header.offsetSize = i * fragmentSize;
        header.fragmentSize = std::min(fragmentSize, size - header.offsetSize);
        memcpy_s(buf.get(), headerSize, &header, headerSize);

        const auto *pPartialData = static_cast<const char*>(pData) + header.offsetSize;
        memcpy_s(buf.get() + headerSize, maxBufSize, pPartialData, header.fragmentSize);

        Fragment fragment;
        fragment.size = header.fragmentSize + headerSize;
        fragment.buf = std::move(buf);
        fragments_.push_back(std::move(fragment));
    }
}


uint32_t PacketFragmenter::GetFragmentSize(uint32_t index) const
{
    if (index >= GetFragmentCount()) return 0;

    const auto &fragment = fragments_.at(index);
    return fragment.size;
}


const char * PacketFragmenter::GetFragmentData(uint32_t index) const
{
    if (index >= GetFragmentCount()) return nullptr;

    const auto &fragment = fragments_.at(index);
    return fragment.buf.get();
}


}