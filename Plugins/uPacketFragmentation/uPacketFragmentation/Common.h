#pragma once

#include <cstdint>


namespace uPacketFragmentation
{


constexpr const char * GetVersion() { return "uPacketFragmentation v0.0.1"; }


constexpr uint8_t GetPacketHeaderVersion() { return 0x01; }


enum class PacketType : uint8_t
{
    Fragment = 0x01,
};


struct PacketHeader
{
    uint8_t version;
    uint8_t type;
    uint32_t totalSize;
    uint32_t offsetSize;
    uint64_t timestamp;
    uint64_t frameIndex;
    uint16_t fragmentCount;
    uint16_t fragmentIndex;
    uint32_t fragmentSize;
};


}
