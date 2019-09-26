#pragma once

#include <memory>
#include <vector>


namespace uPacketFragmentation
{


struct Fragment
{
    std::unique_ptr<char[]> buf = nullptr;
    uint32_t size = 0;
};


class PacketFragmenter final
{
public:
    void SetMaxPacketSize(uint32_t size) { maxPacketSize_ = size; }
    void FragmentData(const void *pData, uint32_t size);
    uint32_t GetFragmentCount() const { return static_cast<uint32_t>(fragments_.size()); }
    uint32_t GetFragmentSize(uint32_t index) const;
    const char * GetFragmentData(uint32_t index) const;

private:
    uint32_t maxPacketSize_ = 1472;
    uint64_t frameIndex_ = 0;
    std::vector<Fragment> fragments_;
};


}
