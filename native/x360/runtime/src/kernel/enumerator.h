#pragma once

#include <cstdint>
#include <vector>

// XAM enumerators (XamEnumerate). The object body is X_KENUMERATOR (0x18 bytes: app id, enumerate message, close
// message, user index, items per enumerate, flags) followed by private data. Enumerators the runtime creates hold a
// snapshot of their items on the host side.
namespace kernel
{
    struct EnumeratorInfo
    {
        uint32_t appId = 0;
        uint32_t message = 0;
        uint32_t closeMessage = 0;
        uint32_t userIndex = 0;
        uint32_t itemsPerEnumerate = 1;
        uint32_t flags = 0;
        // XamEnumerate Flags bits whose meaning the snapshot does not implement (XamEnumerate stops on them).
        uint32_t unsupportedEnumerateFlags = 0;
        // The handler writes its items whatever BufferSize says (XAM's profile handler; its content aggregate one,
        // which the 9199 dashboard asks with BufferSize 1 and a buffer of one item).
        bool wholeItems = false;
    };

    // XamProfileCreateEnumerator's records: be64 offline XUID, XAMACCOUNTINFO (0x17C), be32 device ID.
    constexpr uint32_t kProfileEnumRecordSize = 0x188;

    // Creates an enumerator over `items` (each exactly `itemSize` bytes, big-endian guest layout) and returns a
    // handle to it.
    uint32_t CreateHostEnumerator(const EnumeratorInfo& info, uint32_t itemSize, std::vector<std::vector<uint8_t>> items);
}
