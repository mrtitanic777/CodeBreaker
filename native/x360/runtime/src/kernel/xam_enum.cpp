// XAM enumerators: XamEnumerate over runtime-created enumerators (content, achievements, ...).
//
// Each call hands out min(remaining, items per enumerate) items packed at the item size. Synchronously the count goes
// to *ItemCount; asynchronously (ItemCount NULL, XOVERLAPPED given) it goes to InternalHigh. When nothing is left the
// result is ERROR_NO_MORE_FILES, which an asynchronous request reports as ERROR_FUNCTION_FAILED with extended error
// 0x80070012 (TU46 tests exactly that value to stop enumerating).
//
// Enumerators created by the title through XamCreateEnumeratorHandle (served by XAM apps) are not supported yet.

#include "kernel/enumerator.h"
#include "kernel/objects.h"
#include "kernel/overlapped.h"

#include <cstring>
#include <map>
#include <mutex>

namespace
{
    constexpr uint32_t kBodySize = 0x18;
    constexpr uint32_t kErrorInvalidHandle = 6;
    constexpr uint32_t kErrorNoMoreFiles = 0x12;
    constexpr uint32_t kErrorInvalidParameter = 0x57;

    struct HostEnumerator
    {
        uint32_t itemSize = 0;
        uint32_t itemsPerEnumerate = 1;
        uint32_t unsupportedFlags = 0;
        bool wholeItems = false;
        std::vector<std::vector<uint8_t>> items;
        size_t next = 0;
    };

    std::mutex g_enumMutex;
    std::map<uint32_t, HostEnumerator> g_enumerators; // object -> state
}

namespace kernel
{
    uint32_t CreateHostEnumerator(const EnumeratorInfo& info, uint32_t itemSize, std::vector<std::vector<uint8_t>> items)
    {
        auto& mem = guest::g_memory;
        const uint32_t object = CreateOwnedObject(kBodySize);
        mem.Write32(object + 0x00, info.appId);
        mem.Write32(object + 0x04, info.message);
        mem.Write32(object + 0x08, info.closeMessage);
        mem.Write32(object + 0x0C, info.userIndex);
        mem.Write32(object + 0x10, info.itemsPerEnumerate);
        mem.Write32(object + 0x14, info.flags);
        {
            std::lock_guard lock(g_enumMutex);
            g_enumerators[object] = { itemSize, info.itemsPerEnumerate, info.unsupportedEnumerateFlags,
                                      info.wholeItems, std::move(items), 0 };
        }
        SetObjectCleanup(object, [object]
            {
                std::lock_guard lock(g_enumMutex);
                g_enumerators.erase(object);
            });
        return CreateHandle(object);
    }
}

namespace
{
    // XamEnumerate's body; returns its result.
    uint32_t Enumerate(uint32_t handle, uint32_t flags, uint32_t buffer, uint32_t bufferSize, uint32_t countPtr,
                       uint32_t overlapped)
    {
    auto& mem = guest::g_memory;
    auto finish = [&](uint32_t error, uint32_t count) -> uint32_t
        {
            if (countPtr)
                mem.Write32(countPtr, count);
            if (overlapped)
            {
                kernel::CompleteOverlappedWin32(overlapped, error, count);
                return kernel::kErrorIoPending;
            }
            return error;
        };

    const uint32_t object = kernel::LookupHandle(handle);
    std::unique_lock lock(g_enumMutex);
    auto it = g_enumerators.find(object);
    if (object == 0)
        return finish(kErrorInvalidHandle, 0);
    if (it == g_enumerators.end())
        Fatal("XamEnumerate on enumerator {:08X} (app {:02X}, message {:08X}): enumerators served by XAM apps are not "
              "implemented", handle, mem.Read32(object), mem.Read32(object + 4));
    if (buffer == 0)
        return finish(kErrorInvalidParameter, 0);

    HostEnumerator& e = it->second;
    if (flags & e.unsupportedFlags)
        Fatal("XamEnumerate on enumerator {:08X} (message {:08X}): flags {:X} are not implemented", handle,
              mem.Read32(object + 4), flags & e.unsupportedFlags);
    const uint32_t count = uint32_t(std::min<size_t>(e.items.size() - e.next, e.itemsPerEnumerate));
    if (count == 0)
        return finish(kErrorNoMoreFiles, 0);
    if (!e.wholeItems && uint64_t(count) * e.itemSize > bufferSize)
        Fatal("XamEnumerate: {}-byte buffer for {} items of {} bytes", bufferSize, count, e.itemSize);
    for (uint32_t i = 0; i < count; i++)
        std::memcpy(mem.Translate(buffer + i * e.itemSize), e.items[e.next + i].data(), e.itemSize);
    e.next += count;
    return finish(kernel::kErrorSuccess, count);
    }
}

// DWORD XamEnumerate(HANDLE Enumerator, DWORD Flags, PVOID Buffer, DWORD BufferSize, PDWORD ItemCount,
//                    PXOVERLAPPED Overlapped)
GUEST_IMPORT(XamEnumerate)
{
    ctx.r3.u64 = Enumerate(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32, ctx.r8.u32);
}

// DWORD XamProfileEnumerate(HANDLE Enumerator, DWORD Flags, PVOID Buffer, PXOVERLAPPED Overlapped): XAM 17559 calls
// XamEnumerate(Enumerator, Flags, Buffer, 1, Overlapped ? NULL : &count, Overlapped) (the count stays in XAM); its
// profile handler writes one 0x188-byte record whatever the size, so the host enumerator gets the record size.
GUEST_IMPORT(XamProfileEnumerate)
{
    ctx.r3.u64 = Enumerate(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, kernel::kProfileEnumRecordSize, 0, ctx.r6.u32);
}
