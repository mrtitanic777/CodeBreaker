// XAM party exports (Xbox LIVE parties). Titles look these up dynamically because older dashboards lack them.

#include "kernel/kernel.h"

namespace
{
    constexpr uint32_t kPartyNotInParty = 0x807D0003; // X_PARTY_E_NOT_IN_PARTY
}

// HRESULT XamPartyGetUserList(DWORD Unknown, PXPARTY_USER_LIST UserList)
// There is no LIVE party, which a console reports as "not in party". The first argument's meaning is not
// documented; it is logged once so a title that depends on it is noticed.
GUEST_EXPORT("xam.xex", 2815, XamPartyGetUserList)
{
    static bool logged = false;
    if (!logged)
    {
        logged = true;
        Log("XamPartyGetUserList first argument = {:08X}", ctx.r3.u32);
    }
    ctx.r3.u64 = kPartyNotInParty;
}

// HRESULT XamPartyGetUserListInternal(PVOID UserList) (XAM 0x816AFBB8): XMsgInProcessCall(0xF4, 0x50000C, &UserList,
// 0). The dash reads a u32 count and 0x80-byte entries from +8 (user index +0x18, flags +0x24: 1 local, 0x80000000
// leader). Party chat is not built yet: nobody is in a party, X_PARTY_E_NOT_IN_PARTY (by analogy with
// XamPartyGetUserList; the party app's handler for 0x50000C was not found).
GUEST_IMPORT(XamPartyGetUserListInternal)
{
    ctx.r3.u64 = kPartyNotInParty;
}
