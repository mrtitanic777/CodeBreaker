// XAM party exports (Xbox LIVE parties). Titles look these up dynamically because older dashboards lack them.

#include "kernel/kernel.h"
#include "kernel/overlapped.h"
#include "kernel/users.h"
#include "live/client.h"
#include "log.h"
#include "memory.h"

#include <algorithm>
#include <array>
#include <cstring>
#include <map>
#include <mutex>
#include <optional>

namespace
{
    constexpr uint32_t kPartyNotInParty = 0x807D0003; // X_PARTY_E_NOT_IN_PARTY
    constexpr uint32_t kInvalidArg = 0x80070057;      // E_INVALIDARG

    // The XPARTY_CUSTOM_DATA (16 bytes, kept as the title wrote it) each signed-in user set with XamPartySetCustomData,
    // by online XUID: XPARTY_USER_INFO.CustomData of the user's own entry in XamPartyGetUserList. Our LIVE server does
    // not carry it to the other members, so theirs stay zero.
    std::mutex g_customMutex;
    std::map<uint64_t, std::array<uint8_t, 16>> g_customData;

    std::optional<live::Party> PartyOfAny()
    {
        for (uint32_t user = 0; user < 4; user++)
            if (auto party = live::CurrentParty(user))
                return party;
        return std::nullopt;
    }
}

// HRESULT XamPartyGetUserList(DWORD Unknown, PXPARTY_USER_LIST UserList) (XPartyGetUserList): the console's party on our
// Xbox LIVE, X_PARTY_E_NOT_IN_PARTY when no signed-in user is in one. XPARTY_USER_LIST { DWORD dwUserCount; (pad)
// XPARTY_USER_INFO Users[8] } with XPARTY_USER_INFO (XDK xparty.h; 0x78 bytes): +0 XUID, +8 gamertag CHAR[16], +0x18
// dwUserIndex (local players), +0x1C NatType (XONLINE_NAT_OPEN 1), +0x20 title, +0x24 dwFlags (XPARTY_USER_ISLOCAL 1: the
// value the dashboard's party list uses), +0x28 SessionInfo (XNKID of the game the member shows in presence; the same title's
// only), +0x68 CustomData (not kept). The first argument's meaning is not documented: logged once.
GUEST_EXPORT("xam.xex", 2815, XamPartyGetUserList)
{
    static bool logged = false;
    if (!logged)
    {
        logged = true;
        Log("XamPartyGetUserList first argument = {:08X}", ctx.r3.u32);
    }
    auto& mem = guest::g_memory;
    const uint32_t list = ctx.r4.u32;
    std::optional<live::Party> party;
    for (uint32_t user = 0; user < 4 && !party; user++)
        party = live::CurrentParty(user);
    if (!party || list == 0)
    {
        if (!party)
        {
            std::lock_guard lock(g_customMutex);
            g_customData.clear(); // the custom data goes with the membership
        }
        ctx.r3.u64 = kPartyNotInParty;
        return;
    }
    constexpr uint32_t kEntry = 0x78, kMaxUsers = 8;
    std::memset(mem.Translate(list), 0, 8 + kEntry * kMaxUsers);
    uint32_t count = 0;
    for (const live::PartyMember& m : party->members)
    {
        if (count == kMaxUsers)
            break;
        const uint32_t at = list + 8 + count * kEntry;
        mem.Write64(at, m.xuid);
        std::memcpy(mem.Translate(at + 8), m.gamertag.data(), std::min<size_t>(m.gamertag.size(), 15));
        uint32_t flags = 0, index = 0;
        for (uint32_t user = 0; user < 4; user++)
            if (const auto p = kernel::users::SignedIn(user); p && p->onlineXuid == m.xuid)
            {
                flags |= 1; // XPARTY_USER_ISLOCAL
                index = user;
            }
        mem.Write32(at + 0x18, index);
        mem.Write32(at + 0x1C, 1);
        mem.Write32(at + 0x20, m.titleId);
        mem.Write32(at + 0x24, flags);
        if (m.titleId == kernel::TitleId())
            mem.Write64(at + 0x28, m.sessionId);
        {
            std::lock_guard lock(g_customMutex);
            if (const auto custom = g_customData.find(m.xuid); custom != g_customData.end())
                std::memcpy(mem.Translate(at + 0x68), custom->second.data(), 16);
        }
        count++;
    }
    mem.Write32(list, count);
    ctx.r3.u64 = 0;
}

// HRESULT XamPartySetCustomData(DWORD Caller, DWORD UserIndex, const XPARTY_CUSTOM_DATA* Data) (ordinal 2827; XAM
// 0x816AFD28: XMsgInProcessCall(0xF4, 0x500003, {a1, a2, a3}, 0); the party service's own handler answers and is not in
// the references, so the argument order is Xenia's, read from titles' calls): sets the user's 16 bytes of custom data
// that the party's members see in XamPartyGetUserList (XPARTY_CUSTOM_DATA, two big-endian 64-bit values). A title that
// passes (user, data) without the first argument is told apart by its second argument being a pointer. Only a
// user in a party has data to set: X_PARTY_E_NOT_IN_PARTY (0x807D0003, Xenia's xnet.h, the one party error the
// references name) otherwise, also for a user who is not signed in; a user index from 4 or no data is E_INVALIDARG.
// Kept here and shown in the user's own XamPartyGetUserList entry only. The three arguments are logged once, as the
// first one's meaning is not documented.
GUEST_EXPORT("xam.xex", 2827, XamPartySetCustomData)
{
    auto& mem = guest::g_memory;
    static bool logged = false;
    if (!logged)
    {
        logged = true;
        Log("XamPartySetCustomData arguments = {:08X} {:08X} {:08X}", ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
    }
    uint32_t user = ctx.r4.u32, data = ctx.r5.u32;
    if (user >= 0x1000 && ctx.r3.u32 < 4)
    {
        user = ctx.r3.u32;
        data = ctx.r4.u32;
    }
    if (user > 3 || data == 0)
    {
        ctx.r3.u64 = kInvalidArg;
        return;
    }
    const auto profile = kernel::users::SignedIn(user);
    if (!profile || !live::CurrentParty(user))
    {
        if (profile)
        {
            std::lock_guard lock(g_customMutex);
            g_customData.erase(profile->onlineXuid);
        }
        ctx.r3.u64 = kPartyNotInParty;
        return;
    }
    std::array<uint8_t, 16> custom;
    std::memcpy(custom.data(), mem.Translate(data), 16);
    {
        std::lock_guard lock(g_customMutex);
        g_customData[profile->onlineXuid] = custom;
    }
    ctx.r3.u64 = 0;
}

// HRESULT XamPartyGetBandwidth(DWORD Type, ULONGLONG* Statistic) (ordinal 2832; XAM 0x816AFD68: XMsgInProcessCall(0xF4,
// 0x500004, {a1, a2}, 0), answered by the party service; the signature is Xenia's): a statistic of the party's network
// traffic. Without a party X_PARTY_E_NOT_IN_PARTY; no pointer E_INVALIDARG. In a party the statistic is 0: party chat
// here is carried by our LIVE server, whose traffic the runtime does not count (XAM's own counters, 0x81AA07F0, are
// the party service's). The type is only logged.
GUEST_EXPORT("xam.xex", 2832, XamPartyGetBandwidth)
{
    const uint32_t type = ctx.r3.u32, statistic = ctx.r4.u32;
    if (statistic == 0)
    {
        ctx.r3.u64 = kInvalidArg;
        return;
    }
    if (!PartyOfAny())
    {
        ctx.r3.u64 = kPartyNotInParty;
        return;
    }
    Log("XamPartyGetBandwidth(type {}): 0 (not counted)", type);
    guest::g_memory.Write64(statistic, 0);
    ctx.r3.u64 = 0;
}

// DWORD XamPartySendGameInvites(DWORD UserIndex, DWORD, DWORD, PXOVERLAPPED) (XPartySendGameInvites; XAM 0x816AFBF0, party
// message 0x500002): invites every other member of the user's party to the game session the user hosts or is in.
// Failure is ERROR_FUNCTION_FAILED (the XDK); with an overlapped it completes through it.
GUEST_EXPORT("xam.xex", 2816, XamPartySendGameInvites)
{
    const uint32_t user = ctx.r3.u32, overlapped = ctx.r6.u32;
    uint32_t result = 0;
    if (user >= 4 || !live::SignedIn(user))
        result = 0x4E5; // ERROR_FUNCTION_FAILED
    else if (const auto error = live::InvitePartyToGame(user))
    {
        Log("XamPartySendGameInvites(user {}): {}", user, *error);
        result = 0x4E5;
    }
    if (overlapped)
    {
        kernel::CompleteOverlappedImmediate(overlapped, result);
        ctx.r3.u64 = kernel::kErrorIoPending;
        return;
    }
    ctx.r3.u64 = result;
}

// HRESULT XamPartyGetUserListInternal(PVOID UserList) (XAM 0x816AFBB8): XMsgInProcessCall(0xF4, 0x50000C, &UserList,
// 0). The dash reads a u32 count and 0x80-byte entries from +8 (user index +0x18, flags +0x24: 1 local, 0x80000000
// leader). Party chat is not built yet: nobody is in a party, X_PARTY_E_NOT_IN_PARTY (by analogy with
// XamPartyGetUserList; the party app's handler for 0x50000C was not found).
GUEST_IMPORT(XamPartyGetUserListInternal)
{
    ctx.r3.u64 = kPartyNotInParty;
}
