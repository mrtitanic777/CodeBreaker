// XAM user/profile exports.
//
// The profiles signed in to user indices 0-3 come from kernel/users.h. XAM keeps state bits per user (0x81AA28E0,
// stride 0x290): 1 signed in, 2 logged on to Xbox LIVE, 4 guest; here bit 2 is set while the ports' own LIVE server
// has the profile signed in (kernel::UserOnLive, kernel/xam_live.cpp). XamUserGetXUID's type mask uses the same bits.

#include "kernel/kernel.h"
#include "kernel/enumerator.h"
#include "kernel/overlapped.h"
#include "kernel/profile_store.h"
#include "kernel/users.h"

#include <algorithm>
#include <atomic>
#include <cstring>
#include <mutex>

namespace
{
    constexpr uint32_t kMaxUsers = 4;
    constexpr uint32_t kSigninStateNotSignedIn = 0;
    constexpr uint32_t kSigninStateSignedInLocally = 1;

    constexpr uint32_t kSuccess = 0;
    constexpr uint32_t kErrorNoSuchUser = 0x00000525;      // ERROR_NO_SUCH_USER
    constexpr uint32_t kHresultInvalidArg = 0x80070057;    // E_INVALIDARG
    constexpr uint32_t kHresultNoSuchUser = 0x80070525;    // HRESULT_FROM_WIN32(ERROR_NO_SUCH_USER)

    constexpr uint32_t kSigninInfoOnlineXuidOnly = 1;
    constexpr uint32_t kSigninInfoOfflineXuidOnly = 2;
    constexpr uint32_t kProfileTypeOffline = 1;

    bool SignedIn(uint32_t user)
    {
        return kernel::UserSignedIn(user);
    }

    constexpr uint32_t kStateSignedIn = 1, kStateLive = 2;

    // XAM's state bits for a user (0 when nobody is signed in there).
    uint32_t StateBits(uint32_t user)
    {
        if (user >= kMaxUsers || !SignedIn(user))
            return 0;
        return kStateSignedIn | (kernel::UserOnLive(user) ? kStateLive : 0);
    }

    // The profile's online XUID (0009...) when its account has one, else 0.
    uint64_t OnlineXuid(uint32_t user)
    {
        const auto profile = kernel::users::SignedIn(user);
        return profile && (profile->onlineXuid >> 48) == 0x0009 ? profile->onlineXuid : 0;
    }

    void WriteName(uint32_t user, uint32_t buffer, uint32_t size)
    {
        auto& mem = guest::g_memory;
        const std::string name = kernel::users::SignedIn(user)->gamertag;
        const uint32_t copy = std::min<uint32_t>(uint32_t(name.size()), size ? size - 1 : 0);
        memcpy(mem.Translate(buffer), name.data(), copy);
        if (size)
            mem.Write8(buffer + copy, 0);
    }
}

// DWORD XamUserGetSigninState(DWORD UserIndex) (XAM 0x816DD118): 2 logged on to Xbox LIVE, 1 signed in locally, 0.
GUEST_IMPORT(XamUserGetSigninState)
{
    const uint32_t state = StateBits(ctx.r3.u32);
    ctx.r3.u64 = (state & kStateLive) ? 2 : state ? kSigninStateSignedInLocally : kSigninStateNotSignedIn;
}

// HRESULT XamUserGetSigninInfo(DWORD UserIndex, DWORD Flags, PXUSER_SIGNIN_INFO SigninInfo)
// XUSER_SIGNIN_INFO (40 bytes): xuid (u64), dwInfoFlags, UserSigninState, dwGuestNumber, dwSponsorUserIndex,
// szUserName[16].
GUEST_IMPORT(XamUserGetSigninInfo)
{
    auto& mem = guest::g_memory;
    const uint32_t user = ctx.r3.u32;
    const uint32_t flags = ctx.r4.u32;
    const uint32_t info = ctx.r5.u32;
    if (info == 0)
    {
        ctx.r3.u64 = kHresultInvalidArg;
        return;
    }
    memset(mem.Translate(info), 0, 40);
    if (user >= kMaxUsers || !SignedIn(user))
    {
        ctx.r3.u64 = kHresultNoSuchUser;
        return;
    }
    // XAM 0x816DDB20: flag 1 (online XUID only) wins over flag 2 (offline only); with neither, the online XUID while
    // logged on to LIVE, else the offline one. dwInfoFlags bit 0: the account is Xbox LIVE-enabled.
    const auto profile = kernel::users::SignedIn(user);
    const bool live = (StateBits(user) & kStateLive) != 0;
    if (flags & kSigninInfoOnlineXuidOnly)
        mem.Write64(info + 0x00, OnlineXuid(user));
    else if (flags & kSigninInfoOfflineXuidOnly)
        mem.Write64(info + 0x00, kernel::users::Xuid(user));
    else
        mem.Write64(info + 0x00, live ? OnlineXuid(user) : kernel::users::Xuid(user));
    mem.Write32(info + 0x08, profile && profile->liveEnabled ? 1 : 0);
    mem.Write32(info + 0x0C, live ? 2 : kSigninStateSignedInLocally);
    mem.Write32(info + 0x14, 0xFE); // sponsor user index (none)
    WriteName(user, info + 0x18, 16);
    ctx.r3.u64 = kSuccess;
}

// HRESULT XamUserGetXUID(DWORD UserIndex, DWORD TypeMask, PXUID Xuid)
GUEST_IMPORT(XamUserGetXUID)
{
    auto& mem = guest::g_memory;
    const uint32_t user = ctx.r3.u32;
    const uint32_t typeMask = ctx.r4.u32;
    const uint32_t xuidPtr = ctx.r5.u32;
    if (xuidPtr == 0)
    {
        ctx.r3.u64 = kHresultInvalidArg;
        return;
    }
    mem.Write64(xuidPtr, 0);
    if (user >= kMaxUsers)
    {
        ctx.r3.u64 = kHresultInvalidArg;
        return;
    }
    // XAM 0x816D8510: the type mask against the state bits; LIVE (2) or guest (4) give the online XUID, 1 the offline.
    const uint32_t match = StateBits(user) & typeMask;
    if (match & 6)
        mem.Write64(xuidPtr, OnlineXuid(user));
    else if (match & kProfileTypeOffline)
        mem.Write64(xuidPtr, kernel::users::Xuid(user));
    else
    {
        ctx.r3.u64 = kHresultNoSuchUser;
        return;
    }
    ctx.r3.u64 = kSuccess;
}

// DWORD XamUserGetName(DWORD UserIndex, LPSTR Buffer, DWORD BufferSize)
GUEST_IMPORT(XamUserGetName)
{
    const uint32_t user = ctx.r3.u32;
    if (user >= kMaxUsers || !SignedIn(user))
    {
        ctx.r3.u64 = kErrorNoSuchUser;
        return;
    }
    WriteName(user, ctx.r4.u32, ctx.r5.u32);
    ctx.r3.u64 = kSuccess;
}

// HRESULT XamUserGetDeviceContext(DWORD UserIndex, DWORD DeviceClass, PDWORD DeviceContext)
// The device context for a signed-in user's controller is the user index; there is no microphone.
GUEST_IMPORT(XamUserGetDeviceContext)
{
    auto& mem = guest::g_memory;
    const uint32_t user = ctx.r3.u32;
    mem.Write32(ctx.r5.u32, 0);
    if (SignedIn(user) || (user & 0xFF) == 0xFF)
    {
        mem.Write32(ctx.r5.u32, user);
        ctx.r3.u64 = kSuccess;
    }
    else
    {
        ctx.r3.u64 = 0x8007048F; // HRESULT_FROM_WIN32(ERROR_DEVICE_NOT_CONNECTED)
    }
}

// DWORD XamUserCheckPrivilege(DWORD UserIndex, XPRIVILEGE_TYPE PrivilegeType, PBOOL Result)
// Privileges come from the Xbox LIVE account. The XDK documents ERROR_NOT_LOGGED_ON for an offline user, which the
// local profiles always are (XUSER_INDEX_ANY covers the signed-in users, all offline). A user index nobody is signed
// in to gives ERROR_NO_SUCH_USER, as Xenia answers (TU46 asks for user 1 while loading a world that player 2 joined
// before it was saved).
// Logged on to LIVE (XAM 0x816DD548): privileges 0xBD-0xFF come from the logon reply's privilege bits (XLiveBase
// 0x58008); the ports' own LIVE gives every account Gold: all granted. Outside that range *Result = 0, success.
// XUSER_INDEX_ANY: the first signed-in user that has it.
GUEST_IMPORT(XamUserCheckPrivilege)
{
    constexpr uint32_t kErrorNotLoggedOn = 1245;
    constexpr uint32_t kErrorInvalidParameter = 87;
    const uint32_t user = ctx.r3.u32, privilege = ctx.r4.u32, result = ctx.r5.u32;
    auto& mem = guest::g_memory;
    auto check = [&](uint32_t u) -> uint32_t {
        if (!SignedIn(u))
            return kErrorNoSuchUser;
        if (!(StateBits(u) & kStateLive))
            return kErrorNotLoggedOn;
        if (result)
            mem.Write32(result, privilege >= 0xBD && privilege <= 0xFF ? 1 : 0);
        return kSuccess;
    };
    if (result)
        mem.Write32(result, 0);
    if (user == 0xFF)
    {
        uint32_t error = kErrorNoSuchUser;
        for (uint32_t u = 0; u < kMaxUsers; u++)
        {
            if (!SignedIn(u))
                continue;
            error = check(u);
            if (error == kSuccess && (!result || mem.Read32(result)))
                break;
        }
        ctx.r3.u64 = error;
    }
    else if (user >= kMaxUsers)
        ctx.r3.u64 = kErrorInvalidParameter;
    else
        ctx.r3.u64 = check(user);
}

// DWORD XamDemand(DWORD UserIndex, DWORD Kind, BOOL NoBegin) (XAM 0x8168D6F8 -> 0x81994E78): checks the user's
// Xbox LIVE privilege 0xE0 through XAM's adapters; Kind 1 answers whether that check succeeded, other kinds whether the
// privilege is granted; when the answer is 0 and NoBegin is 0, XAM starts a "demand" (XamBeginDemand: usage reporting
// to LIVE). Our LIVE grants every privilege of the range to a signed-in LIVE user and has no usage reporting.
GUEST_IMPORT(XamDemand)
{
    const uint32_t user = ctx.r3.u32, kind = ctx.r4.u32;
    const bool known = user < kMaxUsers && SignedIn(user) && (StateBits(user) & kStateLive);
    ctx.r3.u64 = kind == 1 ? (known ? 1 : 0) : (known ? 1 : 0);
}

// DWORD XamUserContentRestrictionGetRating(DWORD UserIndex, DWORD System, PDWORD Rating, PDWORD Unrated)
// (XAM 0x81688040): the Family Settings limit of a rating system, from the console's secured XConfig (category 3):
// 0x0F bit 0x80 = Family Settings on. System 0: 0x19 / 0x16, 1: 0x1A / 0x18, 2: 0x24 / 0x23, 4: 0x28 / 0x27 (only
// with Family Settings on; otherwise ERROR_INVALID_PARAMETER). System 3 (the dashboard's media): the Xbox LIVE
// privilege 0xED gives 7 / 0xFF (0 / 0 when denied), narrowed by 0x26 / 0x25 when Family Settings are on; a failed
// privilege check returns its error unless those were read.
GUEST_IMPORT(XamUserContentRestrictionGetRating)
{
    constexpr uint32_t kErrorInvalidParameter = 87;
    const uint32_t user = ctx.r3.u32, system = ctx.r4.u32, rating = ctx.r5.u32, unrated = ctx.r6.u32;
    auto& mem = guest::g_memory;
    if (rating == 0 || unrated == 0 || (user != 0xFF && user >= kMaxUsers) || system > 4)
    {
        ctx.r3.u64 = kErrorInvalidParameter;
        return;
    }
    const bool family = (kernel::XConfigRead(3, 0x0F).at(0) & 0x80) != 0;
    uint32_t first = 0, second = 0, result = kErrorInvalidParameter;
    if (system == 3)
    {
        bool narrowed = false;
        if (family)
        {
            first = kernel::XConfigRead32(3, 0x26);
            second = kernel::XConfigRead32(3, 0x25);
            narrowed = true;
            result = kSuccess;
        }
        // XamUserCheckPrivilege(user, 0xED): granted for a user on our Xbox LIVE (every account is Gold).
        bool anyone = false, live = false;
        for (uint32_t u = 0; u < kMaxUsers; u++)
            if ((user == 0xFF || u == user) && SignedIn(u))
            {
                anyone = true;
                live = live || (StateBits(u) & kStateLive) != 0;
            }
        if (live)
        {
            first = narrowed ? first & 7 : 7;
            second = narrowed ? second & 0xFF : 0xFF;
            result = kSuccess;
        }
        else if (result != kSuccess)
            result = anyone ? 1245 /* ERROR_NOT_LOGGED_ON */ : kErrorNoSuchUser;
    }
    else if (family)
    {
        static constexpr uint16_t kSettings[5][2] = { { 0x19, 0x16 }, { 0x1A, 0x18 }, { 0x24, 0x23 }, {}, { 0x28, 0x27 } };
        first = kernel::XConfigRead32(3, kSettings[system][0]);
        second = kernel::XConfigRead32(3, kSettings[system][1]);
        result = kSuccess;
    }
    if (result == kSuccess)
    {
        mem.Write32(rating, first);
        mem.Write32(unrated, second);
    }
    ctx.r3.u64 = result;
}

// DWORD XamUserIsUnsafeProgrammingAllowed(DWORD UserIndex, BOOL CheckConsole, PBOOL Allowed) (XAM 0x816AF590):
// *Allowed = 1, then (with CheckConsole) the console's parental setting XConfig 3/0x0F bit 0x80, then the users'
// Xbox LIVE privilege 0xD4 (XamUserCheckPrivilege), which only replaces *Allowed when it succeeds. Offline it
// fails (ERROR_NOT_LOGGED_ON, or ERROR_NO_SUCH_USER with nobody signed in): *Allowed stays 1 and that error is
// returned.
GUEST_IMPORT(XamUserIsUnsafeProgrammingAllowed)
{
    constexpr uint32_t kErrorNotLoggedOn = 1245;
    constexpr uint32_t kErrorInvalidParameter = 87;
    const uint32_t user = ctx.r3.u32, allowed = ctx.r5.u32;
    if (allowed == 0 || (user >= kMaxUsers && user != 0xFF))
    {
        ctx.r3.u64 = kErrorInvalidParameter;
        return;
    }
    if (ctx.r4.u32)
        Fatal("XamUserIsUnsafeProgrammingAllowed with CheckConsole: XConfig 3/0x0F (parental flags) is not modelled");
    guest::g_memory.Write32(allowed, 1);
    const bool anyone = user == 0xFF ? kernel::users::SignedInMask() != 0 : SignedIn(user);
    bool live = false;
    for (uint32_t u = 0; u < kMaxUsers; u++)
        if ((user == 0xFF || u == user) && (StateBits(u) & kStateLive))
            live = true;
    // A LIVE user's privilege 0xD4 is granted (Gold): *Allowed stays 1, success.
    ctx.r3.u64 = live ? kSuccess : anyone ? kErrorNotLoggedOn : kErrorNoSuchUser;
}

// DWORD XamUserGetUserIndexMask(DWORD Flags) (XAM 0x816D8720): bit i for each user slot signed in with every bit of
// Flags set in its state (1 signed in locally, 2 signed in to Xbox LIVE, 4 guest). Offline: local sign-ins only.
GUEST_IMPORT(XamUserGetUserIndexMask)
{
    const uint32_t flags = ctx.r3.u32;
    if (flags & ~0x7u)
        Fatal("XamUserGetUserIndexMask: unknown flags {:08X}", flags);
    uint32_t mask = 0;
    for (uint32_t user = 0; user < kMaxUsers; user++)
    {
        const uint32_t state = StateBits(user);
        if (state && (state & flags) == flags)
            mask |= 1u << user;
    }
    ctx.r3.u64 = mask;
}

// HRESULT XamUserGetIndexFromXUID(XUID Xuid, DWORD Flags, PDWORD UserIndex): XAM 17559 walks the four users and
// matches the offline XUID when Flags bit 0 (and the user's state) has bit 0, the Xbox LIVE XUID for bits 1 and 2;
// 0x80070525 (no such user) otherwise. Profiles here are signed in locally only, so just the offline XUID matches.
GUEST_IMPORT(XamUserGetIndexFromXUID)
{
    const uint64_t xuid = ctx.r3.u64;
    auto user = (ctx.r4.u32 & 1) ? kernel::users::UserOfXuid(xuid) : std::nullopt;
    for (uint32_t u = 0; !user && (ctx.r4.u32 & 6) && u < kMaxUsers; u++)
        if ((StateBits(u) & kStateLive) && xuid != 0 && OnlineXuid(u) == xuid)
            user = u;
    if (!user)
    {
        ctx.r3.u64 = 0x80070525;
        return;
    }
    guest::g_memory.Write32(ctx.r5.u32, *user);
    ctx.r3.u64 = 0;
}

// VOID XamUserGetDeviceId(DWORD UserIndex, PDWORD DeviceId): the device of the signed-in profile (here always the hard
// drive, 1), 0 if the user index is not signed in or not 0-3 (XAM 17559's check; the dashboard is exempt from its
// title restriction).
GUEST_IMPORT(XamUserGetDeviceId)
{
    const uint32_t user = ctx.r3.u32;
    guest::g_memory.Write32(ctx.r4.u32, user < kernel::users::kMaxUsers && kernel::users::SignedIn(user) ? 1 : 0);
}

// VOID XampWebInstrumentationSetProfileCounts(DWORD, DWORD, DWORD, DWORD): XAM only records the four values (and a
// "set" flag) for its web instrumentation; nothing reads them here.
GUEST_IMPORT(XampWebInstrumentationSetProfileCounts)
{
}

// Web instrumentation reports (XAM 0x816E46A0...): usage reports XAM sends to Xbox LIVE web servers
// (".../msgserver/logstring2.ashx"); a report's settings come from LIVE. Offline nothing is sent, so no report is
// created: *Report = 0 (as XAM does when a report cannot be set up), and XAM's SetUserVar / SendReport / DestroyReport
// do nothing for report 0 (or -1).
// VOID XamWebInstrumentationCreateReport(LPCWSTR Name, HANDLE* Report) = CreateSampledReport(Name, 100, Report).
GUEST_IMPORT(XamWebInstrumentationCreateReport)
{
    if (ctx.r4.u32)
        guest::g_memory.Write32(ctx.r4.u32, 0);
}

// VOID XamWebInstrumentationSetUserVar(HANDLE Report, DWORD Index, LPCWSTR Value)
GUEST_IMPORT(XamWebInstrumentationSetUserVar)
{
    if (ctx.r3.u32 != 0 && ctx.r3.u32 != 0xFFFFFFFF)
        Fatal("XamWebInstrumentationSetUserVar on report {:08X}: the runtime creates no reports", ctx.r3.u32);
}

// VOID XamWebInstrumentationSendReport(HANDLE Report)
GUEST_IMPORT(XamWebInstrumentationSendReport)
{
    if (ctx.r3.u32 != 0 && ctx.r3.u32 != 0xFFFFFFFF)
        Fatal("XamWebInstrumentationSendReport on report {:08X}: the runtime creates no reports", ctx.r3.u32);
}

// VOID XamWebInstrumentationDestroyReport(HANDLE* Report)
GUEST_IMPORT(XamWebInstrumentationDestroyReport)
{
    if (ctx.r3.u32)
    {
        const uint32_t report = guest::g_memory.Read32(ctx.r3.u32);
        if (report != 0 && report != 0xFFFFFFFF)
            Fatal("XamWebInstrumentationDestroyReport on report {:08X}: the runtime creates no reports", report);
    }
}

// DWORD XamUserLogonEx(const XAM_USER_LOGON* Users, DWORD Flags, PXOVERLAPPED Overlapped)
// Users: 4 entries of 0x188 bytes, one per user index: XUID (0 = none) +0, the profile's XAMACCOUNTINFO +8 (0x17C),
// a dword +0x184. The array is the wanted state of each index. XAM 17559 (0x816D7D40) checks the request, queues it
// for its logon worker (0x8199C838: Xbox LIVE logon, profile mounting, XN_SYS_SIGNINCHANGED with the signed-in mask)
// and returns 0, the overlapped staying ERROR_IO_PENDING until the worker completes it with (hr < 0 ?
// ERROR_FUNCTION_FAILED : 0, hr); errors found before queueing complete it at once and are also returned.
// With Flags & 0xC (per index; the dash signs a profile in with 0x25 from "sign in or out"):
//  - XUID 0: an index already signed in is signed out unless Flags & 1 (keep the indices not named);
//  - a XUID with Flags & 4: that profile is signed in to the index (moved from another index it was signed in to);
//  - a XUID without Flags & 4 (sign out, e.g. 0x28): the index must hold that profile (else E_INVALIDARG); signed out.
// A XUID named for two indices fails with 0x80151005; Flags & 0x4000 with E_INVALIDARG. Profiles here are offline,
// so no Xbox LIVE logon happens (XAM switches to its offline flags when the network is down). Flags 0x20's meaning
// is not established (it is passed through); other flags (0x40 no-wait, 0x80 LIVE, ...) and Flags without 0xC stop.
GUEST_IMPORT(XamUserLogonEx)
{
    auto& mem = guest::g_memory;
    const uint32_t users = ctx.r3.u32, flags = ctx.r4.u32, overlapped = ctx.r5.u32;
    auto finish = [&](uint32_t hr) {
        if (overlapped)
            kernel::CompleteOverlapped(overlapped, int32_t(hr) < 0 ? 0x65B : kSuccess, hr, 0);
    };
    if (flags & 0x4000)
    {
        finish(kHresultInvalidArg);
        ctx.r3.u64 = kHresultInvalidArg;
        return;
    }
    if ((flags & ~0x2Du) || !(flags & 0xC) || !users)
        Fatal("XamUserLogonEx(flags {:X}, users {:08X}): only local sign-in/sign-out with flags 0x1/0x4/0x8/0x20 "
            "is implemented", flags, users);
    if (!overlapped)
        Fatal("XamUserLogonEx(flags {:X}): synchronous logon not implemented", flags);

    uint64_t wanted[kMaxUsers];
    for (uint32_t i = 0; i < kMaxUsers; i++)
        wanted[i] = mem.Read64(users + i * 0x188);
    for (uint32_t i = 1; i < kMaxUsers; i++)
        for (uint32_t j = 0; j < i; j++)
            if (wanted[i] != 0 && wanted[i] == wanted[j])
            {
                finish(0x80151005);
                ctx.r3.u64 = 0x80151005;
                return;
            }
    const auto& profiles = kernel::users::Profiles();
    for (uint32_t i = 0; i < kMaxUsers; i++)
        if (wanted[i] != 0 && !(flags & 4) && kernel::users::Xuid(i) != wanted[i])
        {
            finish(kHresultInvalidArg);
            ctx.r3.u64 = kHresultInvalidArg;
            return;
        }

    const uint32_t before = kernel::users::SignedInMask();
    bool changed = false;
    for (uint32_t i = 0; i < kMaxUsers; i++)
    {
        if (wanted[i] == 0)
        {
            if (!(flags & 1) && kernel::UserSignedIn(i))
            {
                kernel::users::SignOut(i);
                changed = true;
            }
        }
        else if (flags & 4)
        {
            const auto profile = std::find_if(profiles.begin(), profiles.end(),
                [&](const kernel::users::Profile& p) { return p.xuid == wanted[i]; });
            if (profile == profiles.end())
                Fatal("XamUserLogonEx: user {}: profile {:016X} is not on this console (XAM's error not established)",
                    i, wanted[i]);
            if (kernel::users::Xuid(i) != wanted[i])
            {
                kernel::users::SignIn(i, size_t(profile - profiles.begin()));
                changed = true;
                // A local profile's toast at its logon; an Xbox LIVE profile's comes with its LIVE logon
                // (XamShowSigninUIp: "signed in to Xbox Live" or "signed in offline").
                if (!profile->liveEnabled)
                    kernel::SignInToast(i, profile->gamertag, false, false);
            }
        }
        else
        {
            kernel::users::SignOut(i);
            changed = true;
        }
    }
    Log("XamUserLogonEx(flags {:X}): signed-in users {:X} -> {:X}", flags, before, kernel::users::SignedInMask());
    if (changed)
        kernel::BroadcastNotification(0x0000000A, kernel::users::SignedInMask()); // XN_SYS_SIGNINCHANGED
    finish(kSuccess);
    ctx.r3.u64 = kSuccess;
}

// HRESULT XamUserGetGamerTag(DWORD UserIndex, LPWSTR Buffer, DWORD cchBuffer) (XAM 0x816D86A8): Buffer NULL or under
// 16 characters: E_INVALIDARG; user index > 3: E_INVALIDARG; not signed in: 0x80070525 (0x816D8470; titles other than
// the dash also get it while XAM logs users on); else the account's gamertag, 15 characters copied and terminated.
GUEST_IMPORT(XamUserGetGamerTag)
{
    auto& mem = guest::g_memory;
    const uint32_t user = ctx.r3.u32, buffer = ctx.r4.u32, length = ctx.r5.u32;
    if (buffer == 0 || length < 16 || user >= kMaxUsers)
    {
        ctx.r3.u64 = kHresultInvalidArg;
        return;
    }
    if (!SignedIn(user))
    {
        ctx.r3.u64 = kHresultNoSuchUser;
        return;
    }
    const std::string name = kernel::users::SignedIn(user)->gamertag;
    for (uint32_t i = 0; i < 15; i++)
        mem.Write16(buffer + i * 2, i < name.size() ? uint8_t(name[i]) : 0);
    mem.Write16(buffer + 30, 0);
    ctx.r3.u64 = kSuccess;
}

// HRESULT XamUserGetUserTenure(DWORD UserIndex, PDWORD, PDWORD, PDWORD) (XAM 0x816D9828) and
// HRESULT XamUserGetSubscriptionType(DWORD UserIndex, PDWORD, PDWORD) (0x816D9AF0): Xbox LIVE account data, asked of
// XAM's app (0xFE messages 0x2300B / 0x2300C) with the user's online XUID. A NULL pointer: E_INVALIDARG. The XUID
// lookup (0x816D8510, flags 2) needs the user signed in to Xbox LIVE (state bit 2): a profile signed in locally gets
// 0x80070525 and the outputs stay untouched. Every profile here is local (no network).
GUEST_IMPORT(XamUserGetUserTenure)
{
    const uint32_t user = ctx.r3.u32;
    if (ctx.r4.u32 == 0 || ctx.r5.u32 == 0 || ctx.r6.u32 == 0)
        ctx.r3.u64 = kHresultInvalidArg;
    else
        ctx.r3.u64 = user >= kMaxUsers ? kHresultInvalidArg : kHresultNoSuchUser;
}

GUEST_IMPORT(XamUserGetSubscriptionType)
{
    const uint32_t user = ctx.r3.u32;
    if (ctx.r4.u32 == 0 || ctx.r5.u32 == 0)
        ctx.r3.u64 = kHresultInvalidArg;
    else
        ctx.r3.u64 = user >= kMaxUsers ? kHresultInvalidArg : kHresultNoSuchUser;
}

// DWORD XamUserCreateTitlesPlayedEnumerator(DWORD TitleId, DWORD UserIndex, XUID Xuid, DWORD StartingIndex,
//     DWORD cItem, PDWORD pcbBuffer, PHANDLE phEnum) (XAM 0x81670460): user index > 3, cItem 0 or a NULL pointer:
// ERROR_INVALID_PARAMETER. Creates an XGI (0xFB) enumerator (enumerate 0xB0050, close 0xB000B), *pcbBuffer = cItem *
// 0xA8, and initialises it with message 0xB0009 (0x81908FF8; failure: the handle is closed, ERROR_FUNCTION_FAILED):
// Xuid 0 = the signed-in user's own titles (not signed in: failure); another XUID reads a remote profile's titles
// (0x8191FFB0, from Xbox LIVE): not implemented. TitleId is not used for the list.
// Items (0x819090B0 -> 0x81920F88): the dash GPD's (FFFE07D1) section 4 sync list (id 0x100000000) walked from its
// end (the most recently changed title first); StartingIndex entries are skipped; each title record (section 4, id =
// title ID: title ID, achievement counts, gamerscores, ..., last played FILETIME +0x20, UTF-16BE name +0x28) is copied
// as stored into a 0xA8-byte item; a record longer than 0xA8 (or missing) is left out. Up to cItem items per
// XamEnumerate, then ERROR_NO_MORE_FILES. Deviation: the list is read when the enumerator is created (XAM reads the
// GPD on each XamEnumerate).
GUEST_IMPORT(XamUserCreateTitlesPlayedEnumerator)
{
    auto& mem = guest::g_memory;
    const uint32_t user = ctx.r4.u32, start = ctx.r6.u32, count = ctx.r7.u32;
    const uint64_t xuid = ctx.r5.u64;
    const uint32_t sizePtr = ctx.r8.u32, handlePtr = ctx.r9.u32;
    constexpr uint32_t kErrorInvalidParameter = 0x57, kErrorFunctionFailed = 0x65B;
    constexpr uint32_t kItemSize = 0xA8;
    constexpr uint32_t kDashTitleId = 0xFFFE07D1;
    constexpr uint16_t kSectionTitle = 4;
    constexpr uint64_t kSyncListId = 0x100000000;
    if (user >= kMaxUsers || count == 0 || sizePtr == 0 || handlePtr == 0)
    {
        ctx.r3.u64 = kErrorInvalidParameter;
        return;
    }
    if (xuid != 0)
        Fatal("XamUserCreateTitlesPlayedEnumerator: titles of XUID {:016X} (a remote profile, from Xbox LIVE) are not "
            "implemented", xuid);
    mem.Write32(sizePtr, count * kItemSize);
    if (!SignedIn(user))
    {
        ctx.r3.u64 = kErrorFunctionFailed;
        return;
    }

    const uint64_t owner = kernel::users::Xuid(user);
    std::vector<std::vector<uint8_t>> items;
    const auto syncList = kernel::profile::ReadEntry(owner, kDashTitleId, kSectionTitle, kSyncListId);
    const size_t listed = syncList ? syncList->size() / 16 : 0;
    for (size_t i = listed, skipped = 0; i-- > 0;)
    {
        if (skipped < start)
        {
            skipped++;
            continue;
        }
        uint64_t id = 0;
        for (int b = 0; b < 8; b++)
            id = id << 8 | (*syncList)[i * 16 + b];
        const auto record = kernel::profile::ReadEntry(owner, kDashTitleId, kSectionTitle, id);
        if (!record || record->size() > kItemSize)
            continue;
        std::vector<uint8_t> item(kItemSize, 0);
        std::copy(record->begin(), record->end(), item.begin());
        items.push_back(std::move(item));
    }
    Log("XamUserCreateTitlesPlayedEnumerator: user {}: {} titles played (from {}, {} per enumerate)", user,
        items.size(), start, count);

    kernel::EnumeratorInfo info;
    info.appId = 0xFB; // XGI
    info.message = 0xB0050;
    info.closeMessage = 0xB000B;
    info.userIndex = user;
    info.itemsPerEnumerate = count;
    mem.Write32(handlePtr, kernel::CreateHostEnumerator(info, kItemSize, std::move(items)));
    ctx.r3.u64 = kSuccess;
}

namespace
{
    // XamSetLastActiveUserData's 8 bytes and whether they are set (XAM 0x81AA3EF8..0x81AA3F00).
    std::mutex g_lastActiveMutex;
    bool g_lastActiveSet = false;
    uint8_t g_lastActive[8] = { 0, 0, 0, 0xFE, 0xFF, 0xFF, 0xFF, 0xFE };
}

// BOOL XamSetLastActiveUserData(const BYTE* Data) (XAM 0x816E3018): keeps 8 bytes for XamGetLastActiveUserData
// (NULL: cleared to {0xFE, 0xFFFFFFFE}); returns TRUE. The dash (export 11034, 0x92188A90) stores {a dash value, the
// user index} and then broadcasts 0x80040016 with the user index << 16. (XAM's 0x816E3078, around title launches,
// moves the "set" state 1 -> 2 -> cleared; not modelled: the dash runs alone.)
GUEST_IMPORT(XamSetLastActiveUserData)
{
    auto& mem = guest::g_memory;
    std::lock_guard lock(g_lastActiveMutex);
    g_lastActiveSet = ctx.r3.u32 != 0;
    if (g_lastActiveSet)
        std::memcpy(g_lastActive, mem.Translate(ctx.r3.u32), 8);
    else
        std::memcpy(g_lastActive, "\x00\x00\x00\xFE\xFF\xFF\xFF\xFE", 8);
    ctx.r3.u64 = 1;
}

// BOOL XamGetLastActiveUserData(PBYTE Data) (XAM 0x816E2F98): if set (and Data non-NULL), copies the 8 bytes, clears
// them and returns TRUE; else FALSE.
GUEST_IMPORT(XamGetLastActiveUserData)
{
    auto& mem = guest::g_memory;
    std::lock_guard lock(g_lastActiveMutex);
    if (ctx.r3.u32 == 0 || !g_lastActiveSet)
    {
        ctx.r3.u64 = 0;
        return;
    }
    std::memcpy(mem.Translate(ctx.r3.u32), g_lastActive, 8);
    g_lastActiveSet = false;
    std::memcpy(g_lastActive, "\x00\x00\x00\xFE\xFF\xFF\xFF\xFE", 8);
    ctx.r3.u64 = 1;
}

// DWORD XamUserGetMembershipTierFromXUID(XUID Xuid) (XAM 0x816DACA8): the user signed in with that XUID (0x816D8618
// flags 3: the offline XUID of a signed-in user, the online XUID of one signed in to LIVE) -> bits 20-23 of its
// XAMACCOUNTINFO's cached user flags (+0x30; 3 Silver, 6 Gold as the dash names them); 0 when no user matches. A
// locally signed-in LIVE profile still answers its cached tier. A profile without an Account file: 0.
GUEST_IMPORT(XamUserGetMembershipTierFromXUID)
{
    uint64_t xuid = ctx.r3.u64;
    ctx.r3.u64 = 0;
    if (xuid == 0)
        return;
    if (!kernel::users::UserOfXuid(xuid))
    {
        // The online XUID of a user logged on to LIVE.
        std::optional<uint64_t> offline;
        for (uint32_t u = 0; u < kMaxUsers && !offline; u++)
            if ((StateBits(u) & kStateLive) && OnlineXuid(u) == xuid)
                offline = kernel::users::Xuid(u);
        if (!offline)
            return;
        xuid = *offline;
    }
    const auto account = kernel::profile::ReadAccount(xuid);
    if (!account || account->info.size() < 0x34)
        return;
    const uint8_t* f = &account->info[0x30];
    const uint32_t flags = uint32_t(f[0]) << 24 | uint32_t(f[1]) << 16 | uint32_t(f[2]) << 8 | f[3];
    ctx.r3.u64 = flags >> 20 & 0xF;
}

// DWORD XamUserGetMembershipTier(DWORD UserIndex) (XAM 0x816DAC48): the signed-in user's cached tier, bits 20-23 of
// its sign-in record's account flags (+0x44 of the 0xA4-byte record, from XAMACCOUNTINFO +0x30); 0 for a user index
// that is not signed in (0x816D8470 fails). The same account flags as XamUserGetMembershipTierFromXUID.
GUEST_IMPORT(XamUserGetMembershipTier)
{
    const uint32_t user = ctx.r3.u32;
    ctx.r3.u64 = 0;
    if (user >= kMaxUsers || !kernel::UserSignedIn(user))
        return;
    const auto account = kernel::profile::ReadAccount(kernel::users::Xuid(user));
    if (!account || account->info.size() < 0x34)
        return;
    const uint8_t* f = &account->info[0x30];
    const uint32_t flags = uint32_t(f[0]) << 24 | uint32_t(f[1]) << 16 | uint32_t(f[2]) << 8 | f[3];
    ctx.r3.u64 = flags >> 20 & 0xF;
}

// HRESULT XamUserGetOnlineXUIDFromOfflineXUID(XUID Offline, PXUID Online) (XAM 0x816DCFC0): the signed-in profile
// with that offline XUID, its online XUID when it has one (0009...; no LIVE logon needed), else 0x80070525.
GUEST_IMPORT(XamUserGetOnlineXUIDFromOfflineXUID)
{
    const auto user = kernel::users::UserOfXuid(ctx.r3.u64);
    const uint64_t online = user ? OnlineXuid(*user) : 0;
    if (online == 0)
    {
        ctx.r3.u64 = kHresultNoSuchUser;
        return;
    }
    if (ctx.r4.u32)
        guest::g_memory.Write64(ctx.r4.u32, online);
    ctx.r3.u64 = kSuccess;
}

// BOOL XamUserIsOnlineEnabled(DWORD UserIndex) (XAM 0x816DAA60): the account's reserved flag 0x20000000 (Xbox LIVE
// account).
GUEST_IMPORT(XamUserIsOnlineEnabled)
{
    const auto profile = ctx.r3.u32 < kMaxUsers ? kernel::users::SignedIn(ctx.r3.u32) : std::nullopt;
    ctx.r3.u64 = profile && profile->liveEnabled ? 1 : 0;
}

// DWORD XamUserGetUserFlags(DWORD UserIndex) (XAM 0x816DAB28): the user record's +0x10 (0 for an index nobody is
// signed in to). Its writer was not found; consumers read bit 0x80000000 as "on Xbox LIVE" (the dash's billing
// notifications, 0x9225BB18) and 0x40000000 as "re-query the subscription" (0x816DD8D0): 0x80000000 on LIVE, else 0.
GUEST_IMPORT(XamUserGetUserFlags)
{
    ctx.r3.u64 = (StateBits(ctx.r3.u32) & kStateLive) ? 0x80000000u : 0;
}

// DWORD XamUserGetAgeGroup(DWORD UserIndex, PDWORD AgeGroup, PXOVERLAPPED Overlapped) (XAM 0x816D9CE0 -> 0x2300E ->
// 0x81827A10): a LIVE web request (service 0x63C) for the account's age group; the dash asks only for users on LIVE
// (0x9225BB18; others it takes as adults). The ports' own Xbox LIVE has no age data: every account is an adult
// (XUSER_AGE_GROUP_ADULT 2). Not on LIVE the request fails: ERROR_FUNCTION_FAILED (0x80151802 as the extended error).
GUEST_IMPORT(XamUserGetAgeGroup)
{
    auto& mem = guest::g_memory;
    const uint32_t user = ctx.r3.u32, agePtr = ctx.r4.u32, overlapped = ctx.r5.u32;
    if (agePtr == 0)
    {
        ctx.r3.u64 = 0x57;
        return;
    }
    const bool live = (StateBits(user) & kStateLive) != 0;
    if (live)
        mem.Write32(agePtr, 2);
    if (overlapped)
    {
        kernel::CompleteOverlapped(overlapped, live ? 0 : 0x65B, live ? 0 : 0x80151802, 0);
        ctx.r3.u64 = 0x3E5;
        return;
    }
    ctx.r3.u64 = live ? 0 : 0x65B;
}

// HRESULT XamGetUserNeedTFA(DWORD UserIndex, PDWORD Value, PBOOL NeedTFA) (XAM 0x816D8988): the user's two-factor
// sign-in state from Xbox LIVE (a per-user value and its "needs TFA" bit). This runtime's Xbox LIVE asks for no second
// factor: both zero for a signed-in user.
GUEST_IMPORT(XamGetUserNeedTFA)
{
    auto& mem = guest::g_memory;
    if (ctx.r3.u32 >= 4 || StateBits(ctx.r3.u32) == 0)
    {
        ctx.r3.u64 = 0x80070000u | kErrorNoSuchUser;
        return;
    }
    if (ctx.r4.u32 && ctx.r5.u32)
    {
        mem.Write32(ctx.r4.u32, 0);
        mem.Write32(ctx.r5.u32, 0);
    }
    ctx.r3.u64 = 0;
}

// HRESULT XamGetUserPresetWindowsLiveCreds(DWORD UserIndex, XUID Xuid, PWSTR Email, DWORD EmailSize, PWSTR Password,
// DWORD PasswordSize) and XamGetUserInMemoryWindowsLiveCreds (the same): the Windows Live ID and password a profile
// keeps for its Xbox LIVE sign-in (XAM 0x816D8890 copies them when the profile has some). This runtime's Xbox LIVE
// needs none: no credentials (the buffers left empty, ERROR_NOT_FOUND).
namespace
{
    void NoWindowsLiveCreds(PPCContext& ctx)
    {
        auto& mem = guest::g_memory;
        if (ctx.r5.u32 && ctx.r6.u32)
            mem.Write16(ctx.r5.u32, 0);
        if (ctx.r7.u32 && ctx.r8.u32)
            mem.Write16(ctx.r7.u32, 0);
        ctx.r3.u64 = 0x80070490; // HRESULT_FROM_WIN32(ERROR_NOT_FOUND)
    }
}

GUEST_IMPORT(XamGetUserPresetWindowsLiveCreds) { NoWindowsLiveCreds(ctx); }
GUEST_IMPORT(XamGetUserInMemoryWindowsLiveCreds) { NoWindowsLiveCreds(ctx); }

// Logon preview mode (XAM 17559: ordinals 1565-1567, 0x816D7760 / 0x816D7778 / 0x816D77A0): a reference count the
// dashboard's sign-in holds while a profile is previewed. XamUserIsLogonPreviewModeEnabled is TRUE while it is not 0,
// XamUserLockLogonPreviewMode adds one (an atomic increment, no result) and XamUserUnlockLogonPreviewMode takes one
// away (nothing keeps it from going below 0 either). On the last unlock XAM also flags the user state (|= 0x10, while a
// profile is signed in) and calls 0x816E2E78 to tell the sign-in; that release hook is not done here: nothing in this
// runtime waits on it. The early NXE dashboards (7258 ... 7371) look Lock up with XexGetProcedureAddress
// (kernel/xam_nxe_dynamic.cpp).
namespace
{
    std::atomic<int32_t> g_logonPreviewLocks = 0;
}

GUEST_IMPORT(XamUserIsLogonPreviewModeEnabled)
{
    ctx.r3.u64 = g_logonPreviewLocks.load() != 0;
}

GUEST_IMPORT(XamUserLockLogonPreviewMode)
{
    g_logonPreviewLocks++;
}

GUEST_IMPORT(XamUserUnlockLogonPreviewMode)
{
    g_logonPreviewLocks--;
}

PPC_FUNC(__imp__XamUserLockLogonPreviewMode);
namespace
{
    kernel::DynamicExportRegistrar g_lockLogonPreviewMode("xam.xex", 1566, __imp__XamUserLockLogonPreviewMode);
}
