// XAM network storage (cloud saved games, XAM 17559 message area 0x2D, handlers 0x816A5E00). Cloud storage is an Xbox
// LIVE service: this console never set it up and has no network device, so it answers as such a console does and the
// dashboard's Storage shows no cloud device (owner: cloud storage is not shown).

#include "kernel/kernel.h"

#include <cstring>

namespace
{
    constexpr uint32_t kHrInvalidArg = 0x80070057, kHrNotFound = 0x80070490;
}

// HRESULT XamNetworkStorageGetNetworkDevice(PDWORD DeviceId) (XAM 0x816A6730 -> message 0x2D001 -> 0x816A6EA0): the
// cloud device's ID once XAM has created it, else 0x80070490.
GUEST_IMPORT(XamNetworkStorageGetNetworkDevice)
{
    ctx.r3.u64 = ctx.r3.u32 == 0 ? kHrInvalidArg : kHrNotFound;
}

// BOOL XamNetworkStorageDeviceHasCacheFile(DWORD DeviceId) (XAM 0x816A6790 -> 0x2D002 -> 0x816A6490): TRUE for the
// cloud device, or when the device's root holds NetworkStorageCache.bin (the cloud cache a console keeps on a drive
// after cloud storage was used). Neither exists here.
GUEST_IMPORT(XamNetworkStorageDeviceHasCacheFile)
{
    ctx.r3.u64 = 0;
}

// HRESULT XamNetworkStorageGetStatus(DWORD UserIndex, PDWORD Status, PDWORD UserStatus) (XAM 0x816A6920 -> 0x2D005 ->
// 0x816A73B8). Status from XCONFIG 7.0x0E (0x816A7058): never set up 1, "Setting Disabled" 3; enabled: 4 without
// the cloud device (0x816A7330), else 2 or 4 by the cache file. UserStatus: 5 when Status is 4; 4 unless the user (or,
// for an index of 4 or more, every user with the cloud privilege) is signed in to Xbox LIVE; else from the LIVE hive
// and the user's privilege (6 / 3 / 1-2). The ports' own LIVE hive (kernel/xam_live.cpp) has no cloud-storage keys:
// a user on LIVE gets 6, as indexes of 4 or more (assumed, not traced through XamNetworkStorageIsEnabledInLiveHive).
GUEST_IMPORT(XamNetworkStorageGetStatus)
{
    auto& mem = guest::g_memory;
    const uint32_t user = ctx.r3.u32, statusPtr = ctx.r4.u32, userStatusPtr = ctx.r5.u32;
    if (statusPtr == 0)
    {
        ctx.r3.u64 = kHrInvalidArg;
        return;
    }
    const std::vector<uint8_t> setting = kernel::XConfigRead(7, 0x0E);
    uint8_t disabled[0x14] = {};
    std::memcpy(disabled, "Setting Disabled", 16);
    const bool isDisabled = setting.size() == 0x14 && std::memcmp(setting.data(), disabled, 0x14) == 0;
    bool isZero = true;
    for (uint8_t b : setting)
        isZero = isZero && b == 0;
    const uint32_t status = isDisabled ? 3 : isZero ? 1 : 4;
    mem.Write32(statusPtr, status);
    if (userStatusPtr)
        mem.Write32(userStatusPtr, status == 4 ? 5 : user < 4 && !kernel::UserOnLive(user) ? 4 : 6);
    ctx.r3.u64 = 0;
}

// BOOL XamNetworkStorageHasUserEnabledStorage(DWORD UserIndex) (XAM 0x816A4E70): TRUE only when the user has the
// cloud privilege and XamNetworkStorageGetStatus says 2 (enabled, cache file present); the cloud is never set up here
// (owner: cloud storage hidden), so FALSE.
GUEST_IMPORT(XamNetworkStorageHasUserEnabledStorage)
{
    ctx.r3.u64 = 0;
}
