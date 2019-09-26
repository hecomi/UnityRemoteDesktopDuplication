#pragma once

#include <cstdint>
#include <memory>
#include <map>
#include <deque>
#include <functional>
#include "Common.h"


namespace uPacketFragmentation
{


class Frame final
{
public:
    void AddFragmentData(const void *pData, uint32_t size);
    bool IsCompleted() const;
    uint64_t GetTimestamp() const { return timestamp_; }
    uint32_t GetSize() const { return totalSize_; }
    uint32_t GetReceivedSize() const { return receivedSize_; }
    const char * GetData() const { return buf_.get(); }

private:
    void Initialize(const PacketHeader &header);

    std::unique_ptr<char[]> buf_;
    uint32_t totalSize_ = 0;
    uint32_t receivedSize_ = 0;
    uint64_t timestamp_ = 0;
    uint32_t fragmentCount_ = 0;
    std::map<uint32_t, bool> receivedfragmentIndices_;
};


enum class PacketEventType
{
    None = -1,
    FrameCompleted = 0,
    PacketLoss = 1,
};


enum class PacketLossType
{
    None = -1,
    NotReceived = 0,
    Timeout = 1,
};


class PacketAssembler final
{
public:
    void AddData(const void *pData, uint32_t size);
    PacketEventType GetPacketEventType() const { return eventType_; }
    PacketLossType GetPacketLossType() const { return lossType_; }
    uint64_t GetAssembledFrameIndex() const;
    const char * GetFrameData(uint64_t index) const;
    uint32_t GetFrameSize(uint64_t index) const;
    void RemoveFrame(uint64_t index);

private:
    void CheckAllPackets();

    std::map<uint64_t, std::unique_ptr<Frame>> frames_;
    std::deque<uint64_t> assembledFrameIndices_;
    uint64_t oldestFrameIndex_ = -1;
    uint64_t latestFrameIndex_ = -1;
    uint64_t latestTimestamp_ = -1;
    PacketEventType eventType_ = PacketEventType::None;
    PacketLossType lossType_ = PacketLossType::None;
};


}
