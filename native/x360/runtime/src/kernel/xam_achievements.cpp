// XAM achievements: definitions from the title's SPA, earned state from the profile.
//
// Definitions come from the SPA (kernel/spa.h).
//
// XACHIEVEMENT_DETAILS (0x24, XDK xbox.h): dwId, pwszLabel, pwszDescription, pwszUnachieved, dwImageId, dwCred,
// ftAchieved (u64), dwFlags (type in bits 0-2, SHOWUNACHIEVED 0x8, ACHIEVED_ONLINE 0x10000, ACHIEVED 0x20000).
// ftAchieved is zero for achievements earned offline. Earned achievements come from the profile's title GPD
// (written by XUserWriteAchievements, kernel/xam_msg.cpp).

#include "kernel/enumerator.h"
#include "kernel/objects.h"
#include "kernel/overlapped.h"
#include "kernel/profile_store.h"
#include "kernel/spa.h"
#include "kernel/users.h"
#include "settings.h"

#include <cstring>
#include <mutex>
#include <vector>

namespace
{
    constexpr uint32_t kDetailsSize = 0x24;
    constexpr uint32_t kDetailsStrings = 0x7; // LABEL | DESCRIPTION | UNACHIEVED
    // The characters (with the terminator) a label, description and unachieved text take, and what a caller allows per
    // item for them: TRON allocates 500 bytes an item with strings (0x24 + 2 * (32 + 100 + 100)).
    constexpr uint32_t kLabelLength = 32, kDescriptionLength = 100, kUnachievedLength = 100;
    constexpr uint32_t kStringsSize = 2 * (kLabelLength + kDescriptionLength + kUnachievedLength);

    constexpr uint32_t kErrorInvalidParameter = 0x57;
    constexpr uint32_t kErrorNoSuchUser = 0x525;

    void PutBe32(std::vector<uint8_t>& v, size_t at, uint32_t x)
    {
        v[at] = uint8_t(x >> 24);
        v[at + 1] = uint8_t(x >> 16);
        v[at + 2] = uint8_t(x >> 8);
        v[at + 3] = uint8_t(x);
    }
}

namespace
{
    // Another title's achievements (the dash's hub shows the last played game's), as XAM serves them from the
    // profile's <TitleID>.gpd (enumerate 0xB000A -> 0x81907F78 -> 0x8190F2B0 -> 0x81929FF8): section 1's sync list
    // (id 0x100000000, {be64 entry id, be64 sync id}) walked from its end, i.e. the most recently changed (unlocked)
    // first; StartingIndex entries skipped; each entry (0x1C header: struct size, id, image id, gamerscore, flags,
    // unlock FILETIME; then label / description / unachieved strings) becomes an XACHIEVEMENT_DETAILS without strings:
    // id, image id, gamerscore, ftAchieved = the unlock time, flags as stored (e.g. 0x00130009: type 1,
    // SHOWUNACHIEVED, ACHIEVED_ONLINE, ACHIEVED, 0x100000). Not verified: whether XAM's converter (vfunc 0x20 of the
    // achievement store) masks the flags. A title without a GPD stops. The list is read at creation (XAM: per call).
    void CreateProfileAchievementEnumerator(PPCContext& ctx, uint32_t titleId, uint32_t user, uint32_t start,
                                            uint32_t count, uint32_t sizePtr, uint32_t handlePtr)
    {
        auto& mem = guest::g_memory;
        constexpr uint16_t kSectionAchievement = 1;
        constexpr uint64_t kSyncListId = 0x100000000;
        const uint64_t owner = kernel::users::Xuid(user);
        const auto syncList = kernel::profile::ReadEntry(owner, titleId, kSectionAchievement, kSyncListId);
        if (!syncList)
            Fatal("XamUserCreateAchievementEnumerator: the profile has no achievement list for title {:08X}", titleId);
        auto be32 = [](const uint8_t* p) {
            return uint32_t(p[0]) << 24 | uint32_t(p[1]) << 16 | uint32_t(p[2]) << 8 | p[3];
        };
        std::vector<std::vector<uint8_t>> items;
        const size_t listed = syncList->size() / 16;
        for (size_t i = listed, skipped = 0; i-- > 0;)
        {
            if (skipped < start)
            {
                skipped++;
                continue;
            }
            const uint8_t* e = syncList->data() + i * 16;
            const uint64_t id = uint64_t(be32(e)) << 32 | be32(e + 4);
            const auto entry = kernel::profile::ReadEntry(owner, titleId, kSectionAchievement, id);
            if (!entry || entry->size() < 0x1C)
                Fatal("XamUserCreateAchievementEnumerator: title {:08X} achievement {:X} is missing or truncated",
                    titleId, id);
            const uint8_t* a = entry->data();
            std::vector<uint8_t> item(kDetailsSize, 0);
            PutBe32(item, 0x00, be32(a + 4));          // dwId
            PutBe32(item, 0x10, be32(a + 8));          // dwImageId
            PutBe32(item, 0x14, be32(a + 12));         // dwCred
            std::memcpy(&item[0x18], a + 0x14, 8);     // ftAchieved
            PutBe32(item, 0x20, be32(a + 16));         // dwFlags
            items.push_back(std::move(item));
        }
        Log("XamUserCreateAchievementEnumerator: title {:08X}: {} achievements from the profile (from {}, {} per "
            "enumerate)", titleId, items.size(), start, count);
        mem.Write32(sizePtr, kDetailsSize * count);
        kernel::EnumeratorInfo info;
        info.appId = 0xFB; // XGI
        info.message = 0xB000A;
        info.closeMessage = 0xB000B;
        info.userIndex = user;
        info.itemsPerEnumerate = count;
        mem.Write32(handlePtr, kernel::CreateHostEnumerator(info, kDetailsSize, std::move(items)));
        ctx.r3.u64 = kernel::kErrorSuccess;
    }
}

// DWORD XamUserCreateAchievementEnumerator(DWORD TitleId, DWORD UserIndex, XUID Xuid, DWORD DetailFlags,
//                                          DWORD StartingIndex, DWORD ItemCount, PDWORD BufferSize, PHANDLE Enumerator)
// Xuid INVALID_XUID (0) means the requesting user. Items come in SPA order (ascending ID); the XDK leaves the order of
// unearned achievements undefined.
GUEST_IMPORT(XamUserCreateAchievementEnumerator)
{
    auto& mem = guest::g_memory;
    const uint32_t titleId = ctx.r3.u32 ? ctx.r3.u32 : kernel::TitleId();
    const uint32_t user = ctx.r4.u32;
    const uint64_t xuid = ctx.r5.u64;
    const uint32_t detailFlags = ctx.r6.u32, start = ctx.r7.u32, count = ctx.r8.u32;
    const uint32_t sizePtr = ctx.r9.u32, handlePtr = ctx.r10.u32;
    if (count == 0 || sizePtr == 0 || handlePtr == 0 || user >= 4)
    {
        ctx.r3.u64 = kErrorInvalidParameter;
        return;
    }
    if (!kernel::UserSignedIn(user))
    {
        ctx.r3.u64 = kErrorNoSuchUser;
        return;
    }
    if (xuid != 0 && xuid != kernel::users::Xuid(user))
        Fatal("XamUserCreateAchievementEnumerator: achievements of XUID {:016X}", xuid);
    if ((detailFlags & kDetailsStrings) && titleId != kernel::TitleId())
        Fatal("XamUserCreateAchievementEnumerator: strings of another title's achievements ({:08X}), not implemented",
            titleId);
    if (titleId != kernel::TitleId())
    {
        CreateProfileAchievementEnumerator(ctx, titleId, user, start, count, sizePtr, handlePtr);
        return;
    }

    const auto& achievements = kernel::spa::Achievements();
    std::vector<std::vector<uint8_t>> items;
    for (size_t i = start; i < achievements.size() && items.size() < count; i++)
    {
        const kernel::spa::Achievement& a = achievements[i];
        std::vector<uint8_t> item(kDetailsSize, 0);
        PutBe32(item, 0x00, a.id);
        PutBe32(item, 0x10, a.imageId);
        PutBe32(item, 0x14, a.gamerscore);
        // Earned offline: ACHIEVED without ACHIEVED_ONLINE, ftAchieved 0.
        const auto earned = kernel::profile::ReadAchievement(kernel::users::Xuid(user), kernel::TitleId(), a.id);
        PutBe32(item, 0x20, earned ? earned->flags : a.flags);
        if (detailFlags & kDetailsStrings)
        {
            // The label, description and unachieved text (the SPA's strings in the console's language), each
            // NUL-terminated UTF-16BE in the item's tail; pwszLabel / pwszDescription / pwszUnachieved point at them.
            // The XDK's limits: 33, 100 and 100 characters with the terminator.
            const std::pair<uint32_t, uint16_t> strings[] = { { 0x04, a.labelId }, { 0x08, a.descriptionId },
                                                              { 0x0C, a.unachievedId } };
            const uint32_t limits[] = { kLabelLength, kDescriptionLength, kUnachievedLength };
            for (size_t k = 0; k < 3; k++)
            {
                std::u16string text = kernel::spa::String(strings[k].second);
                text.resize(std::min<size_t>(text.size(), limits[k] - 1));
                PutBe32(item, strings[k].first, uint32_t(item.size() - kDetailsSize) + 1);
                for (char16_t c : text)
                {
                    item.push_back(uint8_t(c >> 8));
                    item.push_back(uint8_t(c));
                }
                item.push_back(0);
                item.push_back(0);
            }
        }
        items.push_back(std::move(item));
    }

    const bool strings = detailFlags & kDetailsStrings;
    mem.Write32(sizePtr, (kDetailsSize + (strings ? kStringsSize : 0)) * count);
    kernel::EnumeratorInfo info;
    info.appId = 0xFB; // XGI; messages as Xenia uses for achievement enumerators
    info.message = 0xB000A;
    info.closeMessage = 0xB000B;
    info.userIndex = user;
    info.itemsPerEnumerate = count;
    if (strings)
    {
        info.tailPointers = { 0x04, 0x08, 0x0C };
        info.tailSize = kStringsSize;
    }
    mem.Write32(handlePtr, kernel::CreateHostEnumerator(info, kDetailsSize, std::move(items)));
    ctx.r3.u64 = kernel::kErrorSuccess;
}
