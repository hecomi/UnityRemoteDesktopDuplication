#include <chrono>
#include <algorithm>
#include "Assembler.h"
#include "Log.h"


namespace uPacketFragmentation
{


void Frame::AddFragmentData(const void *pData, uint32_t)
{
    const auto &header = *reinterpret_cast<const PacketHeader*>(pData);

    if (header.version != GetPacketHeaderVersion())
    {
        DebugLog("Packet version (%u) is wrong.", header.version);
        return;
    }

    if (header.type != static_cast<uint32_t>(PacketType::Fragment))
    {
        DebugLog("Packet type (%u) is wrong.", header.type);
        return;
    }

    if (!buf_)
    {
        Initialize(header);
    }

    if (header.fragmentCount != fragmentCount_ ||
        header.timestamp != timestamp_ ||
        header.totalSize != totalSize_ ||
        header.fragmentIndex > fragmentCount_)
    {
        DebugLog("Detected a wrong packet (count: %u / %u, time: %u / %u, size: %u / %u, index: %u).",
            header.fragmentCount, fragmentCount_, 
            header.timestamp, timestamp_, 
            header.totalSize, totalSize_, 
            header.fragmentIndex);
        return;
    }

    bool &flag = receivedfragmentIndices_.at(header.fragmentIndex);
    if (flag)
    {
        DebugLog("Received a same fragmentIndex data.");
        return;
    }
    flag = true;

    const auto offset = header.offsetSize;
    if (offset + header.fragmentSize > totalSize_)
    {
        DebugLog("Received data exceeds the buffer size.");
        return;
    }

    auto *pDst = buf_.get() + offset;
    const auto *pSrc = reinterpret_cast<const char*>(pData) + sizeof(PacketHeader);
    memcpy(pDst, pSrc, header.fragmentSize);

    receivedSize_ += header.fragmentSize;
}


bool Frame::IsCompleted() const
{
    if (receivedfragmentIndices_.empty()) return false;

    for (const auto &pair : receivedfragmentIndices_)
    {
        if (!pair.second) return false;
    }

    if (receivedSize_ != totalSize_)
    {
        DebugLog("Receive size is wrong.");
        return false;
    }

    return true;
}


void Frame::Initialize(const PacketHeader &header)
{
    totalSize_ = header.totalSize;
    timestamp_ = header.timestamp;
    fragmentCount_ = header.fragmentCount;

    for (uint32_t i = 0; i < fragmentCount_; ++i)
    {
        receivedfragmentIndices_.emplace(i, false);
    }

    buf_ = std::make_unique<char[]>(totalSize_);
}


// ---


void PacketAssembler::AddData(const void* pData, uint32_t size)
{
    constexpr uint32_t headerSize = sizeof(PacketHeader);

    if (size < headerSize)
    {
        DebugLog("Received data size (%u) is smaller than header (%u).", 
            size, headerSize);
        return;
    }

    const auto &header = *reinterpret_cast<const PacketHeader*>(pData);

    if (header.version != GetPacketHeaderVersion())
    {
        DebugLog("Packet version is wrong.");
        return;
    }

    if (header.fragmentSize + headerSize != size)
    {
        DebugLog("Packet size is wrong (size: %u, fragment: %u, header: %u).", 
            size, header.fragmentSize, headerSize);
        return;
    }

    if (header.type != static_cast<uint32_t>(PacketType::Fragment))
    {
        DebugLog("Header type is not supported (%u).", header.type);
        return;
    }

    const auto index = header.frameIndex;

    if (index > latestFrameIndex_ || latestFrameIndex_ == -1)
    {
        latestFrameIndex_ = index;
        latestTimestamp_ = header.timestamp;
    }

    if (oldestFrameIndex_ == -1)
    {
        oldestFrameIndex_ = index;
    }

    auto it = frames_.find(index);
    if (it == frames_.end())
    {
        auto frame = std::make_unique<Frame>();
        frame->AddFragmentData(pData, size);
        frames_.emplace(index, std::move(frame));
    }
    else
    {
        const auto &frame = it->second;
        frame->AddFragmentData(pData, size);
    }

    CheckAllPackets();
}


void PacketAssembler::CheckAllPackets()
{
    using namespace std::chrono;

    eventType_ = PacketEventType::None;
    lossType_ = PacketLossType::None;

    if (frames_.empty()) return;

    auto it = frames_.find(oldestFrameIndex_);
    if (it == frames_.end())
    {
        DebugLog("packet loss.");

        eventType_ = PacketEventType::PacketLoss;
        lossType_ = PacketLossType::NotReceived;

        if (latestFrameIndex_ > oldestFrameIndex_)
        {
            ++oldestFrameIndex_;
        }

        return;
    }

    const auto &packet = it->second;
    if (packet->IsCompleted())
    {
        assembledFrameIndices_.push_back(oldestFrameIndex_);
        eventType_ = PacketEventType::FrameCompleted;
        ++oldestFrameIndex_;
    }
    else
    {
        const auto dt = latestTimestamp_ - packet->GetTimestamp();
        if (dt > 100)
        {
            DebugLog("packet timeout (> 100 ms, %u / %u).", 
                packet->GetReceivedSize(), packet->GetSize());

            eventType_ = PacketEventType::PacketLoss;
            lossType_ = PacketLossType::Timeout;
            frames_.erase(oldestFrameIndex_);

            if (latestFrameIndex_ > oldestFrameIndex_)
            {
                ++oldestFrameIndex_;
            }

            return;
        }
    }
}


uint64_t PacketAssembler::GetAssembledFrameIndex() const
{
    return !assembledFrameIndices_.empty() ? 
        *assembledFrameIndices_.begin() : 
        -1;
}


const char * PacketAssembler::GetFrameData(uint64_t index) const
{
    const auto it = frames_.find(index);
    return (it != frames_.end()) ? 
        it->second->GetData() : 
        nullptr;
}


uint32_t PacketAssembler::GetFrameSize(uint64_t index) const
{
    const auto it = frames_.find(index);
    return (it != frames_.end()) ? 
        it->second->GetSize() : 
        0;
}


void PacketAssembler::RemoveFrame(uint64_t index)
{
    const auto it = frames_.find(index);
    if (it != frames_.end())
    {
        frames_.erase(it);
    }

    auto &indices = assembledFrameIndices_;
    indices.erase(
        std::remove(indices.begin(), indices.end(), index),
        indices.end());
}


}