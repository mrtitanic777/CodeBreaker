// The XamAvatar* calls that remain when this runtime has no avatar system (no AvatarAssetPack.toc: the Microsoft
// asset data is never shipped and this runtime has no avatar app; XamAvatarInitialize answers 0x80070002,
// xam_system.cpp). The
// console's answers for a LIVE-era profile without asset data, from XAM 17559 (xam.xex 2.0.17559.0; its decompilation
// is read, never copied):
//
//   - The manifests are the profile's (the avatar data service, app 0xF3). XamAvatarGetManifestLocalUser (message
//     0x600003, 0x81982518), ...ByXuid (the handler for message 0x600004, 0x81982588; its wrapper 0x816749C0 sends
//     0x600003 like LocalUser's, with the 0x90-byte request only that handler reads: unsettled, the behaviour here is
//     the handler's), ...SignedOutProfile (0x600014, 0x81982660) all end in
//     0x819823C0, which reads the dashboard GPD (FFFE07D1) setting 0x63E80044 of each XUID: a 1000-byte value is
//     copied to the caller's XAVATAR_METADATA and that XUID's result is 0; any other size (a profile that has no avatar
//     yet, or a player whose profile the console cannot read) zeroes the 1000 bytes and gives 0x80990001 (no name in
//     the XDK; the only value the service writes there). XamAvatarSetManifest (0x600009, 0x81982768) writes the
//     setting. ...SignedOutProfileCount (0x600013, 0x819825A8) counts the profiles XamProfileEnumerate lists that
//     XamUserGetIndexFromXUID does not find signed in; ...SignedOutProfile counts down to the index, an index past the
//     end is the enumerator's 0x12 as 0x80070012.
//   - The asset calls belong to the avatar system app (0xF2), which only exists once XamAvatarInitialize loaded the
//     asset package. Its enumerator handlers (0x8196B578, 0x8196B6C0) answer S_OK with a count of 0 when there is
//     nothing to list (and 0x8000FFFF without the app's object), which is what the enumerators do here.
//     GetAssets / GetAssetsResultSize / GenerateMipMaps / LoadAnimation / GetMetadataRandom have no asset to work
//     from: they answer 0x80070002, XamAvatarInitialize's own answer here (the package is not found). XAM 17559 has no
//     code path for that exact case (its app is not loaded), so that code is this runtime's choice, not XAM's.
//   - Ordinals 1509, 1510, 1516, 1517 (and 1520) are the same placeholder in XAM 17559's export table (0x818C18F8,
//     no arguments): E_NOTIMPL. Xenia's table names none of them.
//
// Every call is XAM's two-way shape (the XDK: "If pOverlapped is NULL, the operation is completed synchronously"):
// with an overlapped it completes through it (XMsgCompleteIORequest(ov, hr < 0 ? 0x65B : 0, hr, 0)) and returns
// ERROR_IO_PENDING; without one it leaves hr as the thread's last error and returns it (XGetOverlappedExtendedError).
// The signatures are XAM's own, which differ from the XDK's XAvatar* wrappers (xavatar2.lib).

#include "kernel/kernel.h"
#include "kernel/overlapped.h"
#include "kernel/profile_store.h"
#include "kernel/users.h"
#include "kernel/xam_avatar.h"

#include <cstring>
#include <mutex>
#include <vector>

namespace
{
    constexpr uint32_t kHrNotImplemented = 0x80004001; // E_NOTIMPL
    constexpr uint32_t kHrNotFound = 0x80070002;       // HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND)
    constexpr uint32_t kHrNoMoreFiles = 0x80070012;    // HRESULT_FROM_WIN32(ERROR_NO_MORE_FILES): XAM's enumerator end
    constexpr uint32_t kHrInvalidArg = 0x80070057;     // E_INVALIDARG
    constexpr uint32_t kHrNoSuchUser = 0x80070525;     // what XamUserGetXUID answers for a user who is not signed in
    constexpr uint32_t kHrNoAvatar = 0x80990001;       // XAM 0x819823C0: the profile's setting is not a 1000-byte manifest

    constexpr uint32_t kManifestSize = 1000; // XAVATAR_METADATA
    constexpr uint32_t kDashboardTitle = 0xFFFE07D1;
    constexpr uint32_t kSettingAvatar = 0x63E80044; // XPROFILE_GAMERCARD_AVATAR_INFO_1
    // XamAvatarGetManifestsByXuid's request holds 16 XUIDs (128 bytes, XAM 0x816749C0); more would run past it.
    constexpr uint32_t kMaxXuids = 16;
    // XamAvatarGetMetadataRandom: more than this many avatars is a mistake, not a crowd.
    constexpr uint32_t kMaxRandom = 1024;
    // The XEX privilege (system flag bit) XamAvatarGetManifestsByXuid requires (XexCheckExecutablePrivilege(0x1D)).
    constexpr uint32_t kPrivilegeManifestsByXuid = 0x1D;
    constexpr uint32_t kXexHeaderSystemFlags = 0x00030000;

    // Ends a call as XAM's avatar wrappers do (see the top of the file).
    void Finish(PPCContext& ctx, uint32_t overlapped, uint32_t hr)
    {
        if (overlapped)
        {
            kernel::CompleteOverlapped(overlapped, int32_t(hr) < 0 ? 0x65B : 0, hr, 0);
            ctx.r3.u64 = kernel::kErrorIoPending;
            return;
        }
        auto& mem = guest::g_memory;
        const uint32_t thread = mem.Read32(ctx.r13.u32 + kernel::pcr::kCurrentThread);
        mem.Write32(thread + kernel::kthread::kLastError, hr);
        ctx.r3.u64 = hr;
    }

    void NoteOnce(std::once_flag& flag)
    {
        std::call_once(flag, [] { Log("XAM: avatar calls without an avatar system: manifests come from the profiles, "
                                      "assets are not found, enumerations are empty"); });
    }
    std::once_flag g_noted;

    // 0x819823C0's per-XUID step: the profile's manifest into `out` (1000 bytes of guest memory), true when it has one,
    // else the bytes zeroed. A stored value longer than 1000 bytes is read as its first 1000 (XAM wants exactly 1000).
    bool CopyManifest(uint64_t xuid, uint32_t out)
    {
        auto& mem = guest::g_memory;
        const auto setting = kernel::profile::ReadSetting(xuid, kDashboardTitle, kSettingAvatar);
        if (!setting || setting->extended.size() < kManifestSize)
        {
            std::memset(mem.Translate(out), 0, kManifestSize);
            return false;
        }
        std::memcpy(mem.Translate(out), setting->extended.data(), kManifestSize);
        return true;
    }

    // The profiles on this console nobody is signed in with, in XamProfileEnumerate's order.
    std::vector<kernel::users::Profile> SignedOutProfiles()
    {
        std::vector<kernel::users::Profile> result;
        for (const kernel::users::Profile& profile : kernel::users::Profiles())
            if (!kernel::users::UserOfXuid(profile.xuid))
                result.push_back(profile);
        return result;
    }
}

namespace kernel::avatar
{
    uint32_t ManifestsByXuid(uint32_t count, uint32_t xuids, uint32_t results, uint32_t metadata)
    {
        if (count < 1 || count > kMaxXuids || !xuids || !results || !metadata)
            return kHrInvalidArg;
        auto& mem = guest::g_memory;
        const auto& profiles = kernel::users::Profiles();
        for (uint32_t i = 0; i < count; i++)
        {
            const uint64_t xuid = mem.Read64(xuids + 8 * i);
            uint64_t stored = 0;
            for (const kernel::users::Profile& profile : profiles)
                if (xuid != 0 && (profile.xuid == xuid || (profile.onlineXuid != 0 && profile.onlineXuid == xuid)))
                    stored = profile.xuid;
            bool found = false;
            if (stored != 0)
                found = CopyManifest(stored, metadata + i * kManifestSize);
            else
                std::memset(mem.Translate(metadata + i * kManifestSize), 0, kManifestSize);
            mem.Write32(results + 4 * i, found ? 0 : kHrNoAvatar);
        }
        return 0;
    }
}

// DWORD XamAvatarGetManifestLocalUser(DWORD UserIndex, X_AVATAR_METADATA* Metadata, PXOVERLAPPED Overlapped) (XAM
// 0x81674950: its avatar data service, 0xF3 message 0x600003): the signed-in user's avatar, the 1000-byte manifest a
// console keeps in the profile's dashboard GPD as setting 0x63E80044. The sign-ins are this runtime's (kernel/users.h):
// the manifest comes from the profile. A user index from 4 is E_INVALIDARG and nobody signed in at the index ERROR_NO_SUCH_USER (what XamUserGetXUID,
// the handler's first step, answers), no Metadata (XAM would fault) E_INVALIDARG. A profile without an avatar: the
// bytes zeroed and 0x80990001.
GUEST_IMPORT(XamAvatarGetManifestLocalUser)
{
    NoteOnce(g_noted);
    const uint32_t user = ctx.r3.u32, metadata = ctx.r4.u32, overlapped = ctx.r5.u32;
    uint32_t hr = kHrInvalidArg;
    if (user < kernel::users::kMaxUsers && metadata)
    {
        if (const uint64_t xuid = kernel::users::Xuid(user))
            hr = CopyManifest(xuid, metadata) ? 0 : kHrNoAvatar;
        else
            hr = kHrNoSuchUser;
    }
    Finish(ctx, overlapped, hr);
}

// DWORD XamAvatarGetManifestsByXuid(DWORD UserIndex, DWORD XuidCount, const XUID* Xuids, DWORD* Results,
// X_AVATAR_METADATA* Metadata, PXOVERLAPPED Overlapped) (XAM 0x816749C0: 0xF3 message 0x600004): for each XUID, its
// avatar in Metadata[i] and a result in Results[i] (0, or 0x80990001 with the bytes zeroed: see the top of the file).
// Needs the executable privilege 0x1D: without it XAM does nothing and returns 0 (the overlapped is never
// completed). A profile on this console gives its stored manifest; every other player (a friend or a stranger on
// Xbox LIVE: this console has no profile service to ask) comes back empty, 0x80990001. The request holds 16 XUIDs, so
// a count of 0 or above 16 is E_INVALIDARG here (XAM would overrun its request). The call itself is S_OK.
GUEST_IMPORT(XamAvatarGetManifestsByXuid)
{
    NoteOnce(g_noted);
    const uint32_t count = ctx.r4.u32, xuids = ctx.r5.u32, results = ctx.r6.u32, metadata = ctx.r7.u32,
        overlapped = ctx.r8.u32;
    if (((kernel::XexHeaderField(kernel::ExecutableXexHeader(), kXexHeaderSystemFlags) >> kPrivilegeManifestsByXuid) & 1) == 0)
    {
        Log("XamAvatarGetManifestsByXuid: the executable lacks privilege {:#x}: nothing done (as XAM)", kPrivilegeManifestsByXuid);
        ctx.r3.u64 = 0;
        return;
    }
    Finish(ctx, overlapped, kernel::avatar::ManifestsByXuid(count, xuids, results, metadata));
}

// DWORD XamAvatarGetAssetsResultSize(WORD ComponentMask, DWORD* ResultBufferSize, DWORD* GpuBufferSize) (XAM
// 0x81674A70: a synchronous in-process call to the avatar app, 0xF2 message 0x600005; the result is the call's, no
// overlapped): the sizes a GetAssets call for these components needs. No asset package: both sizes 0 and not found.
GUEST_IMPORT(XamAvatarGetAssetsResultSize)
{
    NoteOnce(g_noted);
    auto& mem = guest::g_memory;
    if (ctx.r4.u32)
        mem.Write32(ctx.r4.u32, 0);
    if (ctx.r5.u32)
        mem.Write32(ctx.r5.u32, 0);
    ctx.r3.u64 = kHrNotFound;
}

// DWORD XamAvatarGetAssets(const X_AVATAR_METADATA* Metadata, WORD ComponentMask, DWORD LoadFlags, ..., PXOVERLAPPED
// Overlapped) (XAM 0x81674AD8: 0xF2 message 0x600006, the overlapped is the sixth argument): loads the avatar's
// assets into the caller's buffers. There is no asset package: not found (the buffers stay as they are).
GUEST_IMPORT(XamAvatarGetAssets)
{
    NoteOnce(g_noted);
    Finish(ctx, ctx.r8.u32, kHrNotFound);
}

// DWORD XamAvatarGenerateMipMaps(..., PXOVERLAPPED Overlapped) (XAM 0x81674CC0: 0xF2 message 0x600007, four arguments
// then the overlapped): mipmaps for the textures GetAssets loaded; there are none: not found.
GUEST_IMPORT(XamAvatarGenerateMipMaps)
{
    NoteOnce(g_noted);
    Finish(ctx, ctx.r7.u32, kHrNotFound);
}

// DWORD XamAvatarSetManifest(DWORD UserIndex, const X_AVATAR_METADATA* Metadata, PXOVERLAPPED Overlapped) (XAM
// 0x81674DE0: {user, the 1000 bytes} to its avatar data service, 0xF3 message 0x600009): the avatar editor's Save. As
// XamAvatarGetManifestLocalUser, the profile is this runtime's: the manifest becomes the dashboard GPD's setting
// 0x63E80044 (binary, 1000 bytes). Nobody signed in at UserIndex: ERROR_NO_SUCH_USER, an index from 4 or no Metadata:
// E_INVALIDARG. Whatever else XAM's avatar app does on a change (telling titles, caching the profile's assets) is not
// done; the dashboard reads the avatar again when it starts after the editor.
GUEST_IMPORT(XamAvatarSetManifest)
{
    NoteOnce(g_noted);
    auto& mem = guest::g_memory;
    const uint32_t user = ctx.r3.u32, metadata = ctx.r4.u32, overlapped = ctx.r5.u32;
    uint32_t hr = kHrInvalidArg;
    if (user < kernel::users::kMaxUsers && metadata)
    {
        if (const uint64_t xuid = kernel::users::Xuid(user))
        {
            kernel::profile::Setting setting;
            setting.type = 6; // binary
            setting.value[2] = kManifestSize >> 8;
            setting.value[3] = kManifestSize & 0xFF;
            const uint8_t* data = mem.Translate(metadata);
            setting.extended.assign(data, data + kManifestSize);
            kernel::profile::WriteSetting(xuid, kDashboardTitle, kSettingAvatar, setting);
            Log("XamAvatarSetManifest(user {}): profile {:016X}'s avatar saved", user, xuid);
            hr = 0;
        }
        else
            hr = kHrNoSuchUser;
    }
    Finish(ctx, overlapped, hr);
}

// xam ordinals 1509 (0x5E5) and 1510 (0x5E6), between XamAvatarSetManifest and XamAvatarLoadAnimation, and 1516 (0x5EC)
// and 1517 (0x5ED), between XamAvatarGetMetadataRandom and ...GetMetadataSignedOutProfileCount: Xenia's table names
// none of them, and XAM 17559 exports the same placeholder for them (0x818C18F8, shared with ordinal 1520 and 15 more;
// it takes nothing and returns E_NOTIMPL): the avatar functions an earlier XAM had there. The early NXE dashboards
// bind them by ordinal at start-up (7357: 1500 ... 1521) but never call them.
GUEST_IMPORT(xam_Ordinal_1509)
{
    ctx.r3.u64 = kHrNotImplemented;
}

GUEST_IMPORT(xam_Ordinal_1510)
{
    ctx.r3.u64 = kHrNotImplemented;
}

GUEST_IMPORT(xam_Ordinal_1516)
{
    ctx.r3.u64 = kHrNotImplemented;
}

GUEST_IMPORT(xam_Ordinal_1517)
{
    ctx.r3.u64 = kHrNotImplemented;
}

// DWORD XamAvatarLoadAnimation(const XAVATAR_ASSET_ID* AssetId, DWORD, DWORD, PXOVERLAPPED Overlapped) (XAM 0x81674D48:
// 0xF2 message 0x60000F, the overlapped is the fourth argument): an animation asset into the avatar app. There is no
// asset package: not found.
GUEST_IMPORT(XamAvatarLoadAnimation)
{
    NoteOnce(g_noted);
    Finish(ctx, ctx.r6.u32, kHrNotFound);
}

// DWORD XamAvatarBeginEnumAssets(DWORD, DWORD MaxCount, DWORD ComponentMask, BYTE BodyType, DWORD, PXOVERLAPPED
// Overlapped) (XAM 0x816750B0: 0xF2 message 0x60000C; the avatar editor calls it with its user, 0x32, 0x1FFFFFF, a body
// type, 1 and no overlapped): starts listing the installed assets. There are none to list: S_OK,
// and XamAvatarEnumAssets reports 0 items (an enumerator with nothing in it, so the caller ends it as usual).
GUEST_IMPORT(XamAvatarBeginEnumAssets)
{
    NoteOnce(g_noted);
    Finish(ctx, ctx.r8.u32, 0);
}

// DWORD XamAvatarEndEnumAssets(PXOVERLAPPED Overlapped) (XAM 0x816751E0: 0xF2 message 0x60000E, no buffer; the only
// argument is the overlapped, the avatar editor passes 0): ends the listing. S_OK.
GUEST_IMPORT(XamAvatarEndEnumAssets)
{
    NoteOnce(g_noted);
    Finish(ctx, ctx.r3.u32, 0);
}

// DWORD XamAvatarEnumAssets(PVOID Items /* 0x2C4 bytes each */, DWORD* Count /* in: room, out: items */, PXOVERLAPPED
// Overlapped) (XAM 0x81675158: 0xF2 message 0x60000D; the avatar editor offers room for 0x32 and stops at a failure
// or a count of 0): the next items of the listing. Nothing to list: *Count = 0 and S_OK (XAM's own answer for an
// empty listing, 0x8196B6C0 with its count below 4).
GUEST_IMPORT(XamAvatarEnumAssets)
{
    NoteOnce(g_noted);
    if (ctx.r4.u32)
        guest::g_memory.Write32(ctx.r4.u32, 0);
    Finish(ctx, ctx.r5.u32, 0);
}

// DWORD XamAvatarGetMetadataRandom(BYTE BodyTypeMask, DWORD AvatarCount, X_AVATAR_METADATA* Metadata, PXOVERLAPPED
// Overlapped) (XAM 0x81675258: 0xF2 message 0x600010; the XDK's XAvatarGetMetadataRandom order): random avatars are
// made from the asset package, which is missing: the AvatarCount manifests are zeroed (empty) and the answer is not
// found. More than 1024 avatars: E_INVALIDARG.
GUEST_IMPORT(XamAvatarGetMetadataRandom)
{
    NoteOnce(g_noted);
    const uint32_t count = ctx.r4.u32, metadata = ctx.r5.u32, overlapped = ctx.r6.u32;
    uint32_t hr = kHrInvalidArg;
    if (count <= kMaxRandom && (metadata || count == 0))
    {
        if (count)
            std::memset(guest::g_memory.Translate(metadata), 0, count * kManifestSize);
        hr = kHrNotFound;
    }
    Finish(ctx, overlapped, hr);
}

// DWORD XamAvatarGetMetadataSignedOutProfileCount(DWORD* Count, PXOVERLAPPED Overlapped) (XAM 0x816752F0: 0xF3 message
// 0x600013, the pointer goes in the request): how many profiles on this console nobody is signed in with (the
// "friends and family" of the XDK's XAvatarGetMetadataSignedOutProfileCount). No Count: E_INVALIDARG (XAM would fault).
GUEST_IMPORT(XamAvatarGetMetadataSignedOutProfileCount)
{
    NoteOnce(g_noted);
    const uint32_t countPtr = ctx.r3.u32, overlapped = ctx.r4.u32;
    uint32_t hr = kHrInvalidArg;
    if (countPtr)
    {
        guest::g_memory.Write32(countPtr, uint32_t(SignedOutProfiles().size()));
        hr = 0;
    }
    Finish(ctx, overlapped, hr);
}

// DWORD XamAvatarGetMetadataSignedOutProfile(DWORD ProfileIndex, X_AVATAR_METADATA* Metadata, PXOVERLAPPED Overlapped)
// (XAM 0x81675358: 0xF3 message 0x600014): the avatar of the ProfileIndex'th profile counted above, from its stored
// manifest (the bytes zeroed and 0x80990001 when it has none). An index past the count: 0x80070012, nothing written.
GUEST_IMPORT(XamAvatarGetMetadataSignedOutProfile)
{
    NoteOnce(g_noted);
    const uint32_t index = ctx.r3.u32, metadata = ctx.r4.u32, overlapped = ctx.r5.u32;
    uint32_t hr = kHrInvalidArg;
    if (metadata)
    {
        const auto profiles = SignedOutProfiles();
        if (index < profiles.size())
            hr = CopyManifest(profiles[index].xuid, metadata) ? 0 : kHrNoAvatar;
        else
            hr = kHrNoMoreFiles;
    }
    Finish(ctx, overlapped, hr);
}

// The early NXE dashboards (7258 ... 7371) look these up at run time instead (XexGetProcedureAddress); the lookup gives
// the same host functions as the imports (kernel/xam_nxe_dynamic.cpp lists the other exports they ask for).
namespace
{
    kernel::DynamicExportRegistrar g_1502("xam.xex", 1502, __imp__XamAvatarGetManifestLocalUser);
    kernel::DynamicExportRegistrar g_1503("xam.xex", 1503, __imp__XamAvatarGetManifestsByXuid);
    kernel::DynamicExportRegistrar g_1504("xam.xex", 1504, __imp__XamAvatarGetAssetsResultSize);
    kernel::DynamicExportRegistrar g_1505("xam.xex", 1505, __imp__XamAvatarGetAssets);
    kernel::DynamicExportRegistrar g_1506("xam.xex", 1506, __imp__XamAvatarGenerateMipMaps);
    kernel::DynamicExportRegistrar g_1508("xam.xex", 1508, __imp__XamAvatarSetManifest);
    kernel::DynamicExportRegistrar g_1509("xam.xex", 1509, __imp__xam_Ordinal_1509);
    kernel::DynamicExportRegistrar g_1510("xam.xex", 1510, __imp__xam_Ordinal_1510);
    kernel::DynamicExportRegistrar g_1511("xam.xex", 1511, __imp__XamAvatarLoadAnimation);
    kernel::DynamicExportRegistrar g_1512("xam.xex", 1512, __imp__XamAvatarBeginEnumAssets);
    kernel::DynamicExportRegistrar g_1513("xam.xex", 1513, __imp__XamAvatarEndEnumAssets);
    kernel::DynamicExportRegistrar g_1514("xam.xex", 1514, __imp__XamAvatarEnumAssets);
    kernel::DynamicExportRegistrar g_1515("xam.xex", 1515, __imp__XamAvatarGetMetadataRandom);
    kernel::DynamicExportRegistrar g_1516("xam.xex", 1516, __imp__xam_Ordinal_1516);
    kernel::DynamicExportRegistrar g_1517("xam.xex", 1517, __imp__xam_Ordinal_1517);
    kernel::DynamicExportRegistrar g_1518("xam.xex", 1518, __imp__XamAvatarGetMetadataSignedOutProfileCount);
    kernel::DynamicExportRegistrar g_1519("xam.xex", 1519, __imp__XamAvatarGetMetadataSignedOutProfile);
}
