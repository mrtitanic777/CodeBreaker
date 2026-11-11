// XAM profile settings (XamUserReadProfileSettings).
//
// Setting IDs encode type, maximum size and number: (type << 28) | (size << 16) | id. Settings are owned by the
// dashboard (title FFFE07D1) except the three title-specific ones (ids 0x3FFD-0x3FFF), which belong to each title.
// A setting the profile has never stored reads back with source XSOURCE_DEFAULT and its default value, except the
// title-specific ones, which have no default (XSOURCE_NO_VALUE, no data). Defaults
// come from the XDK documentation (Gamer Profile Constants and the option enumerations), where every documented
// default is the enumeration's first value except controller vibration (ON = 3); the voice volume default (100)
// is the value XHV2 uses for itself when the profile cannot be read (its volume is a 0-100 percentage).
//
// Layouts (XDK xonline.h / xbox.h):
//   XUSER_READ_PROFILE_SETTING_RESULT (8): dwSettingsLen, pSettings
//   XUSER_PROFILE_SETTING (0x28): source +0, dwUserIndex or xuid +8, dwSettingId +0x10, XUSER_DATA +0x18
//   XUSER_DATA (0x10): type (BYTE) +0, value +8 (LONG/LONGLONG/double/float/FILETIME, or cbData +8, pointer +0xC)
// Variable-size values (UNICODE, BINARY) are stored after the settings array, each in a slot of its maximum size.

#include "kernel/kernel.h"
#include "kernel/enumerator.h"
#include "kernel/overlapped.h"
#include "kernel/profile_location.h"
#include "kernel/profile_store.h"
#include "kernel/spa.h"
#include "kernel/users.h"
#include "settings.h"
#include "xui/image.h"

#include <algorithm>
#include <cstring>
#include <format>
#include <map>
#include <optional>
#include <vector>

namespace
{
    constexpr uint32_t kDashboardTitleId = 0xFFFE07D1;

    constexpr uint32_t kErrorInvalidParameter = 87;
    constexpr uint32_t kErrorInsufficientBuffer = 122;

    constexpr uint32_t kSourceNoValue = 0;
    constexpr uint32_t kSourceDefault = 1;
    constexpr uint32_t kSourceTitle = 2;

    constexpr uint8_t kTypeInt32 = 1;
    constexpr uint8_t kTypeUnicode = 4;
    constexpr uint8_t kTypeBinary = 6;

    constexpr uint32_t kResultHeaderSize = 8;
    constexpr uint32_t kSettingSize = 0x28;

    uint8_t SettingType(uint32_t id) { return uint8_t(id >> 28); }
    uint32_t SettingMaxSize(uint32_t id) { return (id >> 16) & 0xFFF; }
    bool HasExtendedData(uint32_t id) { return SettingType(id) == kTypeUnicode || SettingType(id) == kTypeBinary; }
    bool TitleSpecific(uint32_t id) { return (id & 0x3FFF) >= 0x3FFD; }

    // Settings a title may write (XDK xonline.h): controller vibration and the three title-specific settings.
    bool TitleWritable(uint32_t id) { return id == 0x10040003 || TitleSpecific(id); }

    // XAM 17559's defaults for a setting the profile has never stored (0x81915638): the setting's type with a zero
    // value, except the four in its table at 0x8162FA20 (non-string types only).
    int32_t DefaultInt32(uint32_t id)
    {
        switch (id)
        {
        case 0x1004000E: return 100; // XPROFILE_OPTION_VOICE_VOLUME
        case 0x10040003: return 3;   // XPROFILE_OPTION_CONTROLLER_VIBRATION: ON
        case 0x10040022: return 1;   // XPROFILE_GAMER_ACTION_AUTO_AIM: ON
        case 0x10040053: return 3;
        default: return 0;
        }
    }

    // Writes one XUSER_PROFILE_SETTING of the profile `xuid`. `extended` is the slot for variable-size data.
    void WriteSetting(uint32_t entry, uint64_t xuid, uint32_t owner, uint32_t id, uint32_t extended)
    {
        auto& mem = guest::g_memory;
        const auto stored = kernel::profile::ReadSetting(xuid, owner, id);
        mem.Write32(entry + 0x10, id);
        mem.Write8(entry + 0x18, SettingType(id));
        if (stored)
        {
            mem.Write32(entry + 0x00, kSourceTitle);
            if (HasExtendedData(id))
            {
                const size_t size = std::min<size_t>(stored->extended.size(), SettingMaxSize(id));
                std::memcpy(mem.Translate(extended), stored->extended.data(), size);
                mem.Write32(entry + 0x20, uint32_t(size));
                mem.Write32(entry + 0x24, extended);
            }
            else
                std::memcpy(mem.Translate(entry + 0x20), stored->value, 8);
            return;
        }

        if (TitleSpecific(id))
        {
            // Never written: XSOURCE_NO_VALUE with the setting's type and no data. The XDK's ProfileSettings sample
            // reads it this way (it checks for XSOURCE_NO_VALUE, switches on the type, and handles a NULL pbData).
            mem.Write32(entry + 0x00, kSourceNoValue);
            return;
        }
        mem.Write32(entry + 0x00, kSourceDefault);
        if (SettingType(id) == kTypeInt32)
            mem.Write32(entry + 0x20, uint32_t(DefaultInt32(id)));
    }
}

namespace
{
    // The offline XUID of the console's profile with this offline XUID, or with this online XUID while it is logged
    // on to Xbox LIVE; nullopt for anyone else.
    std::optional<uint64_t> LocalProfileXuid(uint64_t xuid)
    {
        for (const auto& p : kernel::users::Profiles())
            if (p.xuid == xuid)
                return p.xuid;
        for (uint32_t user = 0; user < kernel::users::kMaxUsers; user++)
        {
            const auto p = kernel::users::SignedIn(user);
            if (p && p->onlineXuid == xuid && xuid != 0)
                return p->xuid;
        }
        return std::nullopt;
    }

    // XamUserReadProfileSettingsEx's work (XAM 17559: XamUserReadProfileSettings is the Ex call with Flags 0); returns
    // the call's result. Flags bit 4 only changes the entries of explicit XUIDs, bits 1-2 an extra read path.
    uint32_t ReadProfileSettings(uint32_t titleId, uint32_t user, uint32_t xuidCount, uint32_t xuids, uint32_t count,
        uint32_t ids, uint32_t flags, uint32_t sizePointer, uint32_t results, uint32_t overlapped)
    {
    auto& mem = guest::g_memory;
    if (titleId == 0)
        titleId = kernel::TitleId();
    if (flags)
        Fatal("XamUserReadProfileSettingsEx: flags {:X} (only 0 is known)", flags);

    if (count < 1 || count > 32 || ids == 0 || sizePointer == 0 || xuidCount > 16 || (xuidCount && xuids == 0) ||
        (mem.Read32(sizePointer) != 0 && results == 0))
    {
        return kErrorInvalidParameter;
    }

    const uint32_t profiles = xuidCount ? xuidCount : 1;
    uint32_t extendedSize = 0;
    for (uint32_t i = 0; i < count; i++)
    {
        const uint32_t id = mem.Read32(ids + i * 4);
        if (HasExtendedData(id))
            extendedSize += SettingMaxSize(id);
    }
    const uint32_t settingsSize = profiles * count * kSettingSize;
    const uint32_t needed = kResultHeaderSize + settingsSize + profiles * extendedSize;
    if (mem.Read32(sizePointer) < needed)
    {
        mem.Write32(sizePointer, needed);
        return kErrorInsufficientBuffer;
    }

    if (titleId != kernel::TitleId() && titleId != kDashboardTitleId)
        Fatal("XamUserReadProfileSettings: settings of another title ({:08X})", titleId);
    if (xuidCount)
    {
        // XAM reads any profile on the console by its offline XUID, signed in or not (the profile list does), and a
        // profile logged on to Xbox LIVE also by its online XUID.
        for (uint32_t i = 0; i < xuidCount; i++)
        {
            const uint64_t xuid = mem.Read64(xuids + i * 8);
            if (!LocalProfileXuid(xuid))
                Fatal("XamUserReadProfileSettings: XUID {:016X} is not a profile on this console", xuid);
        }
    }
    else if (user >= kernel::users::kMaxUsers || !kernel::UserSignedIn(user))
    {
        // XAM's handler (app 0xFB, message 0xB000C) asks XamUserGetXUID first: E_INVALIDARG for an index above 3,
        // 0x80070525 for one nobody is signed in to; the request completes with ERROR_FUNCTION_FAILED.
        Log("XamUserReadProfileSettings: user {} has no profile (ERROR_FUNCTION_FAILED)", user);
        if (overlapped)
        {
            kernel::CompleteOverlapped(overlapped, 0x65B, user >= kernel::users::kMaxUsers ? 0x80070057 : 0x80070525, 0);
            return kernel::kErrorIoPending;
        }
        return 0x65B;
    }

    std::memset(mem.Translate(results), 0, needed);
    mem.Write32(results + 0, profiles * count);
    mem.Write32(results + 4, results + kResultHeaderSize);
    uint32_t extended = results + kResultHeaderSize + settingsSize;
    for (uint32_t p = 0; p < profiles; p++)
    {
        for (uint32_t i = 0; i < count; i++)
        {
            const uint32_t id = mem.Read32(ids + i * 4);
            const uint32_t entry = results + kResultHeaderSize + (p * count + i) * kSettingSize;
            const uint64_t asked = xuidCount ? mem.Read64(xuids + p * 8) : 0;
            const uint64_t xuid = xuidCount ? *LocalProfileXuid(asked) : kernel::users::Xuid(user);
            if (xuidCount)
                mem.Write64(entry + 0x08, asked);
            else
                mem.Write32(entry + 0x08, user);
            WriteSetting(entry, xuid, TitleSpecific(id) ? titleId : kDashboardTitleId, id, extended);
            if (HasExtendedData(id))
                extended += SettingMaxSize(id);
        }
    }

    if (overlapped)
    {
        kernel::CompleteOverlappedImmediate(overlapped, kernel::kErrorSuccess);
        return kernel::kErrorIoPending;
    }
    return kernel::kErrorSuccess;
    }
}

// DWORD XamUserReadProfileSettings(DWORD TitleId, DWORD UserIndex, DWORD XuidCount, const XUID* Xuids,
//     DWORD SettingCount, const DWORD* SettingIds, PDWORD ResultsSize, PXUSER_READ_PROFILE_SETTING_RESULT Results,
//     PXOVERLAPPED Overlapped)
// Called twice: with *ResultsSize = 0 to learn the size (ERROR_INSUFFICIENT_BUFFER), then with a buffer.
GUEST_IMPORT(XamUserReadProfileSettings)
{
    ctx.r3.u64 = ReadProfileSettings(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32, ctx.r8.u32, 0,
        ctx.r9.u32, ctx.r10.u32, kernel::StackArg32(ctx, 8));
}

// DWORD XamUserReadProfileSettingsEx(DWORD TitleId, DWORD UserIndex, DWORD XuidCount, const XUID* Xuids,
//     DWORD SettingCount, const DWORD* SettingIds, DWORD Flags, PDWORD ResultsSize,
//     PXUSER_READ_PROFILE_SETTING_RESULT Results, PXOVERLAPPED Overlapped)
GUEST_IMPORT(XamUserReadProfileSettingsEx)
{
    ctx.r3.u64 = ReadProfileSettings(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32, ctx.r8.u32,
        ctx.r9.u32, ctx.r10.u32, kernel::StackArg32(ctx, 8), kernel::StackArg32(ctx, 9));
}

// DWORD XamUserWriteProfileSettings(DWORD TitleId, DWORD UserIndex, DWORD SettingCount,
//                                   const XUSER_PROFILE_SETTING* Settings, PXOVERLAPPED Overlapped)
// Stores the values in the profile (the title's GPD for title-specific settings, the dashboard's for vibration);
// later reads return them with source XSOURCE_TITLE.
GUEST_IMPORT(XamUserWriteProfileSettings)
{
    auto& mem = guest::g_memory;
    const uint32_t titleId = ctx.r3.u32 ? ctx.r3.u32 : kernel::TitleId();
    const uint32_t user = ctx.r4.u32, count = ctx.r5.u32, settings = ctx.r6.u32, overlapped = ctx.r7.u32;
    constexpr uint32_t kErrorNoSuchUser = 1317;
    auto finish = [&](uint32_t error)
        {
            if (overlapped)
            {
                kernel::CompleteOverlappedWin32(overlapped, error, 0);
                ctx.r3.u64 = kernel::kErrorIoPending;
            }
            else
                ctx.r3.u64 = error;
        };
    if (count < 1 || settings == 0)
        return finish(kErrorInvalidParameter);
    if (user >= 4 || !kernel::UserSignedIn(user))
        return finish(kErrorNoSuchUser);
    if (titleId != kernel::TitleId())
        Fatal("XamUserWriteProfileSettings: settings of another title ({:08X})", titleId);

    for (uint32_t i = 0; i < count; i++)
    {
        const uint32_t entry = settings + i * kSettingSize;
        const uint32_t id = mem.Read32(entry + 0x10);
        const uint8_t type = mem.Read8(entry + 0x18);
        // The dashboard writes its own system settings too (dash 0x9225B2A0: the billing notice it last showed,
        // 7008004F + 61180050, after GetUserBillingNotifications); they go to the dashboard's GPD.
        if (!TitleWritable(id) && titleId != kDashboardTitleId)
            Fatal("XamUserWriteProfileSettings: setting {:08X} is not writable by titles (behaviour not established)", id);
        if (type != SettingType(id))
            Fatal("XamUserWriteProfileSettings: setting {:08X} written with data type {}", id, type);
        kernel::profile::Setting value;
        value.type = type;
        std::memcpy(value.value, mem.Translate(entry + 0x20), 8);
        if (HasExtendedData(id))
        {
            const uint32_t size = mem.Read32(entry + 0x20), data = mem.Read32(entry + 0x24);
            if (size > SettingMaxSize(id) || (size && data == 0))
                return finish(kErrorInvalidParameter);
            value.extended.assign(mem.Translate(data), mem.Translate(data) + size);
            std::memset(value.value + 4, 0, 4); // the pointer is not stored
        }
        kernel::profile::WriteSetting(kernel::users::Xuid(user), TitleSpecific(id) ? titleId : kDashboardTitleId, id,
            value);
    }
    finish(kernel::kErrorSuccess);
}

// HRESULT XamProfileLoadAccountInfo(DWORD DeviceId, XUID Xuid, PXAMACCOUNTINFO Info) (XAM 0x816DB020 -> message 0x23004
// -> 0x81686978): mounts the profile package Xuid on the device, reads its Account file (0x194 bytes), unobfuscates it
// with the console's key (XeKeysUnObfuscate) into the 0x17C-byte XAMACCOUNTINFO and checks it; failures: the mount's
// error, a short read or a bad Account 0x80070570 (Info zeroed). The dash's Storage reads it for each profile item.
// Here: the profile on the device that holds it (kernel/profile_location.h, kernel/profile_store.h ReadAccount); a
// profile not on that device answers 0x80070002 (the mount's code was not traced).
GUEST_IMPORT(XamProfileLoadAccountInfo)
{
    auto& mem = guest::g_memory;
    const uint32_t device = ctx.r3.u32, info = ctx.r5.u32;
    const uint64_t xuid = ctx.r4.u64;
    // The profile's package must be on that device (kernel/profile_location.h; a configured profile without one is
    // the hard drive's).
    uint32_t where = kernel::profile_location::Find(xuid).device;
    if (where == 0)
        where = 1;
    if (device != where)
    {
        Log("XamProfileLoadAccountInfo({}, {:016X}): the profile is on device {}: 80070002", device, xuid, where);
        ctx.r3.u64 = 0x80070002;
        return;
    }
    const auto account = kernel::profile::ReadAccount(xuid);
    if (!account || account->info.size() < 0x17C)
    {
        Log("XamProfileLoadAccountInfo({:016X}): no readable Account: 80070002", xuid);
        ctx.r3.u64 = 0x80070002;
        return;
    }
    std::memcpy(mem.Translate(info), account->info.data(), 0x17C);
    ctx.r3.u64 = 0;
}

// DWORD XamProfileCreateEnumerator(DWORD DeviceId, PHANDLE Enumerator): the console's profiles. XAM 17559 creates the
// enumerator (user 0xFF, app 0xFE, messages 0x23001 / 0x23003, one item per call) and its handler 0x23002 lists the
// profiles on the device (0 = every device). A profile is on the device that holds its package
// (kernel/profile_location.h); a configured profile with no package yet counts as the hard drive's. Records (kernel::kProfileEnumRecordSize): offline XUID, the Account's XAMACCOUNTINFO (a configured profile
// with no Account file: its gamertag and, once our Xbox LIVE made it an account, the LIVE flag and online XUID;
// everything else zero), device ID.
GUEST_IMPORT(XamProfileCreateEnumerator)
{
    constexpr uint32_t kHardDriveId = 1, kErrorInvalidParameter = 0x57;
    const uint32_t device = ctx.r3.u32, handlePtr = ctx.r4.u32;
    if (handlePtr == 0)
    {
        ctx.r3.u64 = kErrorInvalidParameter;
        return;
    }
    std::vector<std::vector<uint8_t>> items;
    {
        for (const kernel::users::Profile& profile : kernel::users::Profiles())
        {
            uint32_t where = kernel::profile_location::Find(profile.xuid).device;
            if (where == 0)
                where = kHardDriveId;
            if (device != 0 && device != where)
                continue;
            std::vector<uint8_t> item(kernel::kProfileEnumRecordSize, 0);
            for (int i = 0; i < 8; i++)
                item[i] = uint8_t(profile.xuid >> (56 - 8 * i));
            const auto account = profile.copied ? kernel::profile::ReadAccount(profile.xuid) : std::nullopt;
            if (account && account->info.size() == 0x17C)
                std::memcpy(item.data() + 8, account->info.data(), 0x17C);
            else
            {
                for (size_t i = 0; i < profile.gamertag.size() && i < 15; i++)
                    item[8 + 8 + 2 * i + 1] = uint8_t(profile.gamertag[i]);
                if (profile.liveEnabled) // an Xbox LIVE account from our server (live_accounts.txt)
                {
                    item[8] |= 0x20;
                    for (int i = 0; i < 8; i++)
                        item[8 + 0x28 + i] = uint8_t(profile.onlineXuid >> (56 - 8 * i));
                }
            }
            item[0x187] = uint8_t(where);
            items.push_back(std::move(item));
        }
    }
    const size_t count = items.size();
    const kernel::EnumeratorInfo info{ 0xFE, 0x23001, 0x23003, 0xFF, 1, 0 };
    guest::g_memory.Write32(handlePtr, kernel::CreateHostEnumerator(info, kernel::kProfileEnumRecordSize, std::move(items)));
    Log("XamProfileCreateEnumerator(device {}): {} profiles", device, count);
    ctx.r3.u64 = kernel::kErrorSuccess;
}

namespace
{
    // XamReadTile's results (HRESULTs; the overlapped's extended error).
    constexpr uint32_t kHrFail = 0x80004005, kHrAbort = 0x80004004, kHrFileNotFound = 0x80070002,
        kHrInvalidArg = 0x80070057, kHrInsufficientBuffer = 0x8007007A;

    // One read of a local profile's tile (XAM 0x8191BD50): the profile's package is mounted (XamProfileOpen: a
    // profile the console does not have fails with ERROR_FILE_NOT_FOUND) and the tile's file read whole. Types:
    // 4/5 tile_64.png / tile_32.png, 0xA/0xB pp_64.png / pp_32.png. *BufferSize is in/out: too small gets the file's
    // size and ERROR_INSUFFICIENT_BUFFER (nothing copied); success gets the bytes read. A missing file fails with
    // CreateFile's ERROR_FILE_NOT_FOUND; an empty one with E_FAIL (0x8191D108).
    uint32_t ReadLocalTile(uint32_t type, uint64_t xuid, uint32_t buffer, uint32_t sizePtr)
    {
        auto& mem = guest::g_memory;
        const char* name = type == 4 ? "tile_64.png" : type == 5 ? "tile_32.png" : type == 0xA ? "pp_64.png" : "pp_32.png";
        const auto& profiles = kernel::users::Profiles();
        if (std::none_of(profiles.begin(), profiles.end(), [&](const kernel::users::Profile& p) { return p.xuid == xuid; }))
            return kHrFileNotFound;
        const auto file = kernel::profile::ReadPackageFile(xuid, name);
        if (!file)
            return kHrFileNotFound;
        const uint32_t size = uint32_t(file->size());
        if (mem.Read32(sizePtr) < size)
        {
            mem.Write32(sizePtr, size);
            return kHrInsufficientBuffer;
        }
        if (size)
            std::memcpy(mem.Translate(buffer), file->data(), size);
        mem.Write32(sizePtr, size);
        return size ? kernel::kErrorSuccess : kHrFail;
    }
}

// DWORD XamReadTile(XTILETYPE TileType, DWORD TitleId, QWORD ImageId, DWORD UserIndex, PBYTE Buffer,
//                   PDWORD BufferSize, PXOVERLAPPED Overlapped)
// XAM 17559 (0x81670B68) posts app 0xFB message 0xB0030; its handler (0x8191E428 -> 0x8191E078 -> 0x8191D108) does the
// work at once and completes the request with XMsgCompleteIORequest(Overlapped, hr < 0 ? ERROR_FUNCTION_FAILED : 0,
// hr, 0). Returns ERROR_IO_PENDING with an overlapped structure, else 0 or ERROR_FUNCTION_FAILED (the HRESULT is the
// thread's last error). Only local profiles' tiles are implemented: types 2/3 (gamer picture) with an offline XUID
// become 0xA/0xB; 0xA/0xB (the profile's picture, the dash's profile list: XamReadTile(0xA, 0, XUID, 0xFE, ...)) read
// pp_64/pp_32.png and, if that fails other than with ERROR_INSUFFICIENT_BUFFER or E_ABORT, retry as 4/5 (tile_64/32.png).
// Types 0 (achievement picture, ImageId = its image id) and 1 (game icon, image 0x8000) of a title (0 = the caller's):
// the signed-in user's <TitleID>.gpd section 2 (images) entry, a PNG (0x8190FB88); a profile stores the pictures of
// earned achievements only. Not there: XAM tries its tile cache, which this console has empty: 0x80070002. (The
// dash's own icon, type 1 of FFFE07D1, is XAM's default image: not implemented.)
// ImageId is the profile's offline XUID; otherwise UserIndex < 4 names the signed-in profile (XAM asks
// XamUserGetXUID; the offline XUID is used here), else E_INVALIDARG. Gamer pictures (2/3) of online XUIDs (0009...:
// friends, other players) come from XAM's tile cache, which XamPrepareGamerTiles fills from Xbox LIVE: here our
// LIVE's `profile` (picture / picture_small, sent by each profile at sign-in); a player without one misses
// (ERROR_FILE_NOT_FOUND; the dash shows its default picture). The other types (title tiles, avatars ...) stop.
GUEST_IMPORT(XamReadTile)
{
    uint32_t type = ctx.r3.u32;
    const uint32_t titleId = ctx.r4.u32, user = ctx.r6.u32, buffer = ctx.r7.u32, sizePtr = ctx.r8.u32;
    const uint32_t overlapped = ctx.r9.u32;
    const uint64_t imageId = ctx.r5.u64;
    const bool online = (imageId >> 48) == 0x0009;
    if (type == 0 || type == 1)
    {
        auto& mem = guest::g_memory;
        const uint32_t title = titleId ? titleId : kernel::TitleId();
        if (type == 1 && title == 0xFFFE07D1)
            Fatal("XamReadTile(1, FFFE07D1): XAM's default dashboard image is not implemented");
        const auto profile = user < 4 ? kernel::users::SignedIn(user) : std::nullopt;
        uint32_t hr = kHrFileNotFound;
        if (profile)
        {
            const uint64_t id = type == 1 ? 0x8000 : imageId;
            if (const auto png = kernel::profile::ReadEntry(profile->xuid, title, 2, id))
            {
                const uint32_t size = uint32_t(png->size());
                if (mem.Read32(sizePtr) < size)
                    hr = kHrInsufficientBuffer;
                else
                {
                    std::memcpy(mem.Translate(buffer), png->data(), size);
                    hr = kernel::kErrorSuccess;
                }
                mem.Write32(sizePtr, size);
            }
        }
        Log("XamReadTile(type {:X}, title {:08X}, image {:X}, user {:X}): {:08X}", type, title, imageId, user, hr);
        const uint32_t result = int32_t(hr) < 0 ? 0x65B : kernel::kErrorSuccess;
        if (overlapped)
        {
            kernel::CompleteOverlapped(overlapped, result, hr, 0);
            ctx.r3.u64 = kernel::kErrorIoPending;
            return;
        }
        const uint32_t thread = mem.Read32(ctx.r13.u32 + kernel::pcr::kCurrentThread);
        mem.Write32(thread + kernel::kthread::kLastError, hr);
        ctx.r3.u64 = result;
        return;
    }
    if ((type == 2 || type == 3) && online)
    {
        // The tile cache: the player's picture from our Xbox LIVE (their profile sends both sizes at sign-in).
        auto& mem = guest::g_memory;
        uint32_t hr = kHrFileNotFound;
        if (const auto png = kernel::LiveGamerPicture(imageId, type == 3))
        {
            const uint32_t size = uint32_t(png->size());
            if (mem.Read32(sizePtr) < size)
                hr = kHrInsufficientBuffer;
            else
            {
                std::memcpy(mem.Translate(buffer), png->data(), size);
                hr = kernel::kErrorSuccess;
            }
            mem.Write32(sizePtr, size);
        }
        Log("XamReadTile(type {:X}, {:016X}): {:08X}{}", type, imageId, hr,
            hr == kHrFileNotFound ? " (not in the tile cache: no picture on our Xbox LIVE)" : "");
        const uint32_t result = int32_t(hr) < 0 ? 0x65B : kernel::kErrorSuccess;
        if (overlapped)
        {
            kernel::CompleteOverlapped(overlapped, result, hr, 0);
            ctx.r3.u64 = kernel::kErrorIoPending;
            return;
        }
        const uint32_t thread = mem.Read32(ctx.r13.u32 + kernel::pcr::kCurrentThread);
        mem.Write32(thread + kernel::kthread::kLastError, hr);
        ctx.r3.u64 = result;
        return;
    }
    if ((type == 2 || type == 3) && !online)
        type = type == 3 ? 0xB : 0xA;
    if ((type != 4 && type != 5 && type != 0xA && type != 0xB) || online)
        Fatal("XamReadTile(type {:X}, title {:08X}, image {:016X}, user {:X}): only local profiles' pictures are "
            "implemented", ctx.r3.u32, titleId, imageId, user);

    uint64_t xuid = 0;
    if ((imageId >> 60) == 0xE)
        xuid = imageId;
    else if (user < 4)
        if (const auto profile = kernel::users::SignedIn(user))
            xuid = profile->xuid;
    uint32_t hr = kHrInvalidArg;
    if (xuid != 0)
    {
        hr = ReadLocalTile(type, xuid, buffer, sizePtr);
        if (int32_t(hr) < 0 && hr != kHrInsufficientBuffer && hr != kHrAbort && (type == 0xA || type == 0xB))
            hr = ReadLocalTile(type == 0xA ? 4 : 5, xuid, buffer, sizePtr);
    }
    Log("XamReadTile(type {:X}, {:016X}, user {:X}): {:08X}, {} bytes", ctx.r3.u32, imageId, user, hr,
        int32_t(hr) < 0 ? 0 : guest::g_memory.Read32(sizePtr));

    const uint32_t result = int32_t(hr) < 0 ? 0x65B : kernel::kErrorSuccess; // ERROR_FUNCTION_FAILED
    if (overlapped)
    {
        kernel::CompleteOverlapped(overlapped, result, hr, 0);
        ctx.r3.u64 = kernel::kErrorIoPending;
        return;
    }
    const uint32_t thread = guest::g_memory.Read32(ctx.r13.u32 + kernel::pcr::kCurrentThread);
    guest::g_memory.Write32(thread + kernel::kthread::kLastError, hr);
    ctx.r3.u64 = result;
}

namespace
{
    // The result of XamReadString's message handler (XAM 17559 0x81909380): the HRESULT, with the string written.
    // 1. The running title's own string table (its SPA's XSTR, UTF-8 converted to UTF-16 into the buffer; *Size is
    //    in characters and includes the terminator) when TitleId is 0 or the running title. The string must exist
    //    (E_INVALIDARG if not, or the buffer is NULL, or the id is 0xFFFF); a buffer of 0 characters is
    //    HRESULT_FROM_WIN32(ERROR_INSUFFICIENT_BUFFER), and so is one too small for the string (nothing written);
    //    an empty string, or a 1-character buffer, gets "" and *Size 1.
    // 2. Failing that, for a signed-in UserIndex (< 4): the profile's <TitleID>.gpd string entry (section 5, TitleId 0 =
    //    the running title), its UTF-16 bytes copied whole (a buffer too small: insufficient buffer, nothing copied),
    //    *Size the characters read.
    // On any failure *Size becomes 1 and the buffer "" (the handler does that itself).
    uint32_t ReadString(uint32_t titleId, uint16_t stringId, uint32_t user, uint32_t buffer, uint32_t sizePtr)
    {
        auto& mem = guest::g_memory;
        uint32_t hr = kHrInvalidArg;
        const uint32_t running = kernel::TitleId();
        if (buffer && sizePtr && (titleId == 0 || titleId == running) && stringId != 0xFFFF &&
            !kernel::ExecutableResource(std::format("{:08X}", running)).empty())
        {
            const uint32_t capacity = mem.Read32(sizePtr);
            if (capacity == 0)
                hr = kHrInsufficientBuffer;
            else if (const auto text = kernel::spa::FindString(stringId))
            {
                // MultiByteToWideChar(CP_UTF8) into capacity - 1 characters: all or nothing.
                if (text->empty() || capacity == 1)
                {
                    mem.Write32(sizePtr, 1);
                    mem.Write16(buffer, 0);
                    hr = kernel::kErrorSuccess;
                }
                else if (text->size() > capacity - 1)
                    hr = kHrInsufficientBuffer;
                else
                {
                    for (size_t i = 0; i < text->size(); i++)
                        mem.Write16(buffer + uint32_t(i) * 2, (*text)[i]);
                    mem.Write16(buffer + uint32_t(text->size()) * 2, 0);
                    mem.Write32(sizePtr, uint32_t(text->size()) + 1);
                    hr = kernel::kErrorSuccess;
                }
            }
        }
        if (int32_t(hr) < 0 && buffer && sizePtr && user < 4)
        {
            hr = kHrInvalidArg;
            if (const auto profile = kernel::users::SignedIn(user))
                if (const auto entry = kernel::profile::ReadEntry(profile->xuid, titleId ? titleId : running, 5, stringId))
                {
                    if (mem.Read32(sizePtr) * 2 < entry->size())
                        hr = kHrInsufficientBuffer;
                    else
                    {
                        if (!entry->empty())
                            std::memcpy(mem.Translate(buffer), entry->data(), entry->size());
                        const uint32_t chars = uint32_t(entry->size() / 2);
                        mem.Write32(sizePtr, chars);
                        if (chars)
                            mem.Write16(buffer + (chars - 1) * 2, 0);
                        hr = kernel::kErrorSuccess;
                    }
                }
        }
        if (int32_t(hr) < 0 && sizePtr)
        {
            mem.Write32(sizePtr, 1);
            if (buffer)
                mem.Write16(buffer, 0);
        }
        return hr;
    }
}

// DWORD XamReadString(DWORD TitleId, QWORD StringId, DWORD UserIndex, LPWSTR Buffer, PDWORD BufferSize,
//                     PXOVERLAPPED Overlapped)
// XAM 17559 (0x81670898) posts app 0xFB message 0xB0038 ({TitleId, QWORD StringId, UserIndex, Buffer, BufferSize}, 0x20
// bytes; the handler 0x81909380 uses the string id's low 16 bits) and, like XamReadTile, completes at once: 0, or
// ERROR_FUNCTION_FAILED (0x65B; the HRESULT is the thread's last error) on a failure; with an overlapped, ERROR_IO_PENDING
// and XMsgCompleteIORequest(Overlapped, result, hr, 0). *BufferSize is in characters, the terminator included. Titles
// read the strings of their SPA's string table with it (a leaderboard column's name: Minecraft's Leaderboards screen
// asks for string 0x2E1), 0x8000 = the title's name; the dash asks for a title's name with user 0xFE (no user).
// ReadString above has the rules and where the strings come from.
GUEST_IMPORT(XamReadString)
{
    const uint32_t titleId = ctx.r3.u32, user = ctx.r5.u32, buffer = ctx.r6.u32, sizePtr = ctx.r7.u32;
    const uint32_t overlapped = ctx.r8.u32;
    const uint64_t stringId = ctx.r4.u64;
    const uint32_t hr = ReadString(titleId, uint16_t(stringId), user, buffer, sizePtr);
    Log("XamReadString(title {:08X}, string {:X}, user {:X}): {:08X}", titleId, stringId, user, hr);
    const uint32_t result = int32_t(hr) < 0 ? 0x65B : kernel::kErrorSuccess;
    if (overlapped)
    {
        kernel::CompleteOverlapped(overlapped, result, hr, 0);
        ctx.r3.u64 = kernel::kErrorIoPending;
        return;
    }
    const uint32_t thread = guest::g_memory.Read32(ctx.r13.u32 + kernel::pcr::kCurrentThread);
    guest::g_memory.Write32(thread + kernel::kthread::kLastError, hr);
    ctx.r3.u64 = result;
}

namespace
{
    // The PNG XamReadTile would return for the same arguments, or nullopt (its rules: achievement and game pictures
    // from the signed-in profile's GPD, gamer pictures of online players from our Xbox LIVE, local profiles' pictures
    // from their package).
    std::optional<std::vector<uint8_t>> TilePng(uint32_t type, uint32_t titleId, uint64_t imageId, uint32_t user)
    {
        if (type == 0 || type == 1)
        {
            const auto profile = user < 4 ? kernel::users::SignedIn(user) : std::nullopt;
            if (!profile)
                return std::nullopt;
            return kernel::profile::ReadEntry(profile->xuid, titleId ? titleId : kernel::TitleId(), 2,
                                              type == 1 ? 0x8000 : imageId);
        }
        const bool online = (imageId >> 48) == 0x0009;
        if ((type == 2 || type == 3) && online)
            return kernel::LiveGamerPicture(imageId, type == 3);
        if (type == 2 || type == 3)
            type = type == 3 ? 0xB : 0xA;
        if (type != 4 && type != 5 && type != 0xA && type != 0xB)
            return std::nullopt;
        uint64_t xuid = 0;
        if ((imageId >> 60) == 0xE)
            xuid = imageId;
        else if (user < 4)
            if (const auto profile = kernel::users::SignedIn(user))
                xuid = profile->xuid;
        if (xuid == 0)
            return std::nullopt;
        const char* name = type == 4 ? "tile_64.png" : type == 5 ? "tile_32.png" : type == 0xA ? "pp_64.png" : "pp_32.png";
        auto file = kernel::profile::ReadPackageFile(xuid, name);
        if (!file && (type == 0xA || type == 0xB))
            file = kernel::profile::ReadPackageFile(xuid, type == 0xA ? "tile_64.png" : "tile_32.png");
        return file;
    }

    // XamReadTileToTexture(Ex): the tile decoded into a linear 32-bit texture the title allocated, ARGB in big-endian
    // order (D3DFMT_LIN_A8R8G8B8), `pitch` bytes per row, at most `height` rows. A tile that is not there leaves the
    // texture as it is and fails with ERROR_FILE_NOT_FOUND's HRESULT (the title keeps its default picture).
    void TileToTexture(PPCContext& ctx, const std::string& what, const std::optional<std::vector<uint8_t>>& png,
                       uint32_t buffer, uint32_t pitch, uint32_t height, uint32_t overlapped)
    {
        auto& mem = guest::g_memory;
        uint32_t hr = 0x80070002;
        if (buffer == 0 || pitch < 4)
            hr = 0x80070057;
        else if (png)
        {
            xui::Image image;
            std::string error;
            if (!xui::DecodePng(png->data(), png->size(), image, error))
                Log("{}: the tile's PNG does not decode ({})", what, error);
            else
            {
                const uint32_t w = std::min(image.width, pitch / 4), h = std::min(image.height, height);
                for (uint32_t y = 0; y < h; y++)
                {
                    uint8_t* row = mem.Translate(buffer + y * pitch);
                    for (uint32_t x = 0; x < w; x++)
                    {
                        const uint8_t* src = &image.rgba[(size_t(y) * image.width + x) * 4];
                        row[x * 4 + 0] = src[3];
                        row[x * 4 + 1] = src[0];
                        row[x * 4 + 2] = src[1];
                        row[x * 4 + 3] = src[2];
                    }
                }
                hr = kernel::kErrorSuccess;
            }
        }
        Log("{} ({} bytes x {} rows): {:08X}", what, pitch, height, hr);
        const uint32_t result = int32_t(hr) < 0 ? 0x65B : kernel::kErrorSuccess;
        if (overlapped)
        {
            kernel::CompleteOverlapped(overlapped, result, hr, 0);
            ctx.r3.u64 = kernel::kErrorIoPending;
            return;
        }
        const uint32_t thread = mem.Read32(ctx.r13.u32 + kernel::pcr::kCurrentThread);
        mem.Write32(thread + kernel::kthread::kLastError, hr);
        ctx.r3.u64 = result;
    }
}

// DWORD XamReadTileToTexture(XTILETYPE TileType, DWORD TitleId, QWORD ImageId, DWORD UserIndex, PVOID Texture,
//                            DWORD Pitch, DWORD Height, PXOVERLAPPED Overlapped)
GUEST_IMPORT(XamReadTileToTexture)
{
    const uint32_t type = ctx.r3.u32, titleId = ctx.r4.u32, user = ctx.r6.u32;
    const uint64_t imageId = ctx.r5.u64;
    TileToTexture(ctx, std::format("XamReadTileToTexture(type {:X}, title {:08X}, image {:X}, user {:X})", type,
                  titleId, imageId, user), TilePng(type, titleId, imageId, user), ctx.r7.u32, ctx.r8.u32, ctx.r9.u32,
                  ctx.r10.u32);
}

// DWORD XamReadTileToTextureEx(XTILETYPE TileType, DWORD TitleId, QWORD ImageId, DWORD UserIndex, BOOL Small,
//                              PCXUSER_DATA PictureKey, PVOID Texture, DWORD Pitch, DWORD Height,
//                              PXOVERLAPPED Overlapped): titles look it up by ordinal (765) at run time. XAPI's
// XUserReadGamerPictureByKey calls it with type 0xC (a gamer picture named by the key a gamercard holds), title 0,
// image 0, user 0xFE. The key names a picture of the user's own profile here: the first signed-in profile's.
GUEST_EXPORT("xam.xex", 765, XamReadTileToTextureEx)
{
    const uint32_t type = ctx.r3.u32, titleId = ctx.r4.u32, user = ctx.r6.u32;
    const uint64_t imageId = ctx.r5.u64;
    const bool small = ctx.r7.u32 != 0;
    std::optional<std::vector<uint8_t>> png;
    if (type == 0xC)
    {
        for (uint32_t u = 0; u < 4 && !png; u++)
            if (kernel::users::SignedIn(u))
                png = TilePng(small ? 0xB : 0xA, 0, 0, u);
    }
    else
        png = TilePng(type, titleId, imageId, user);
    TileToTexture(ctx, std::format("XamReadTileToTextureEx(type {:X}, title {:08X}, image {:X}, user {:X}, {})", type,
                  titleId, imageId, user, small ? "small" : "large"), png, ctx.r9.u32, ctx.r10.u32,
                  kernel::StackArg32(ctx, 8), kernel::StackArg32(ctx, 9));
}

// DWORD XamReadTileEx(XTILETYPE TileType, DWORD TitleId, QWORD ImageId, DWORD UserIndex, DWORD, DWORD, PBYTE Buffer,
//                     PDWORD BufferSize, PXOVERLAPPED Overlapped) (XAM 17559 0x81670C00: app 0xFB message 0xB003A, its
// 0x38-byte request XamReadTile's with the two extra words at +0x28 / +0x2C; the overlapped is the ninth argument, on
// the stack at +0x54). The 6719 dash reads the signed-in gamer's picture with it, both extra words 0: that is
// XamReadTile. Other values' meaning is not known, and they stop.
GUEST_IMPORT(XamReadTileEx)
{
    if (ctx.r7.u32 != 0 || ctx.r8.u32 != 0)
        Fatal("XamReadTileEx(type {:X}, title {:08X}, image {:X}, user {:X}): extra arguments {:08X} {:08X} are not "
            "implemented", ctx.r3.u32, ctx.r4.u32, ctx.r5.u64, ctx.r6.u32, ctx.r7.u32, ctx.r8.u32);
    ctx.r7.u64 = ctx.r9.u32;
    ctx.r8.u64 = ctx.r10.u32;
    ctx.r9.u64 = guest::g_memory.Read32(ctx.r1.u32 + 0x54);
    XamReadTile_impl(ctx, base);
}

// DWORD XamPrepareGamerTiles(DWORD UserIndex, DWORD Count, const XUID* Xuids) (XAM 0x81670600): posts app 0xFB
// message 0xB0034 (handler 0x8191B630): Count >= 0xFFFF is E_INVALIDARG (the call returns ERROR_FUNCTION_FAILED);
// otherwise the players' gamer pictures (setting 0x4064000F) are queued for download from Xbox LIVE into the tile cache
// and it returns 0. Here: our Xbox LIVE's pictures of those players are fetched in the background (XamReadTile
// reads them; kernel::PrefetchGamerPictures).
GUEST_IMPORT(XamPrepareGamerTiles)
{
    const uint32_t user = ctx.r3.u32, count = ctx.r4.u32, xuids = ctx.r5.u32;
    if (count >= 0xFFFF)
    {
        Log("XamPrepareGamerTiles(user {}, {} players): E_INVALIDARG", user, count);
        ctx.r3.u64 = 0x65B;
        return;
    }
    std::string list;
    std::vector<uint64_t> online;
    for (uint32_t i = 0; i < count && xuids; ++i)
    {
        const uint64_t xuid = guest::g_memory.Read64(xuids + i * 8);
        if (i < 8)
            list += std::format("{}{:016X}", i ? " " : "", xuid);
        if ((xuid >> 48) == 0x0009)
            online.push_back(xuid);
    }
    Log("XamPrepareGamerTiles(user {}, {} players: {}{}): {} fetched from our Xbox LIVE", user, count, list,
        count > 8 ? " ..." : "", online.size());
    kernel::PrefetchGamerPictures(std::move(online));
    ctx.r3.u64 = kernel::kErrorSuccess;
}

// DWORD XamUserReadUserPreference(DWORD UserIndex, DWORD Preference, PDWORD Value, PXOVERLAPPED Overlapped) (XAM
// 0x816B1F68 -> XGI 0xB0082 -> 0x8190A7D0): user preferences are bits of the dash GPD setting 0x10040053 (XAM's
// default 3). Preference is one bit: 1, 0x20, 0x40, 0x80, 0x100, 0x200, or 6 / 0x18 (bits 2-1 / 4-3, where the upper
// bit says "set": present -> the lower bit, absent -> S_FALSE with the raw bits). *Value = setting & Preference.
// UserIndex > 3 or Value NULL: ERROR_INVALID_PARAMETER; another Preference (E_INVALIDARG) or a user not signed in:
// ERROR_FUNCTION_FAILED. With an overlapped the request completes and ERROR_IO_PENDING is returned.
GUEST_IMPORT(XamUserReadUserPreference)
{
    auto& mem = guest::g_memory;
    const uint32_t user = ctx.r3.u32, preference = ctx.r4.u32, valuePtr = ctx.r5.u32, overlapped = ctx.r6.u32;
    constexpr uint32_t kSetting = 0x10040053;
    if (user >= 4 || valuePtr == 0)
    {
        ctx.r3.u64 = 0x57;
        return;
    }
    uint32_t special = 0;
    bool valid = preference == 1 || preference == 0x20 || preference == 0x40 || preference == 0x80 ||
        preference == 0x100 || preference == 0x200;
    if (preference == 6)
        special = 4, valid = true;
    if (preference == 0x18)
        special = 0x10, valid = true;
    uint32_t hr = 0x80070057;
    const auto profile = kernel::users::SignedIn(user);
    if (valid && profile)
    {
        uint32_t bits = uint32_t(DefaultInt32(kSetting));
        if (const auto stored = kernel::profile::ReadSetting(profile->xuid, 0xFFFE07D1, kSetting))
            bits = uint32_t(stored->value[0]) << 24 | uint32_t(stored->value[1]) << 16 | uint32_t(stored->value[2]) << 8 |
                stored->value[3];
        uint32_t value = bits & preference;
        hr = 0;
        if (special)
        {
            if ((value & special) == special)
                value &= ~special;
            else
                hr = 1; // S_FALSE
        }
        mem.Write32(valuePtr, value);
    }
    else if (valid)
        hr = 0x80070525;
    const uint32_t result = int32_t(hr) < 0 ? 0x65B : 0;
    if (overlapped)
    {
        kernel::CompleteOverlapped(overlapped, result, hr, 0);
        ctx.r3.u64 = kernel::kErrorIoPending;
        return;
    }
    ctx.r3.u64 = result;
}
