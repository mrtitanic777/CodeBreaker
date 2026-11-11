// XAM party exports (Xbox LIVE parties). Titles look these up dynamically because older dashboards lack them.

#include "kernel/kernel.h"
#include "kernel/overlapped.h"
#include "kernel/users.h"
#include "live/client.h"
#include "log.h"
#include "memory.h"

#include <algorithm>
#include <cstring>
#include <optional>

namespace
{
    constexpr uint32_t kPartyNotInParty = 0x807D0003; // X_PARTY_E_NOT_IN_PARTY
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
        count++;
    }
    mem.Write32(list, count);
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
