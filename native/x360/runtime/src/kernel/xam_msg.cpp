// XAM message dispatch (XMsg*) and overlapped completion.
//
// Titles send requests to XAM's internal "apps" (system services) as (app, message, buffer). Each handled message is
// listed explicitly; anything else stops with the app and message ID so it can be researched rather than guessed.

#include "kernel/objects.h"
#include "kernel/overlapped.h"
#include "kernel/profile_store.h"
#include "kernel/spa.h"
#include "kernel/users.h"
#include "kernel/web_call.h"
#include "kernel/xam_matchmaking.h"
#include "kernel/xam_stats.h"
#include "kernel/xam_storage.h"
#include "kernel/xmp.h"
#include "live/client.h"
#include "live/presence.h"
#include "settings.h"
#include "xui/system_ui.h"

#include <windows.h>

#include <algorithm>
#include <chrono>
#include <set>
#include <cstring>
#include <format>
#include <map>
#include <mutex>
#include <thread>
#include <vector>

namespace kernel
{
    uint32_t OnlineGetLogonExtendedStatus(uint32_t buffer); // kernel/xam_live.cpp
    uint32_t WriteTitleXnAddr(uint32_t address);           // kernel/xam_net.cpp
}

namespace
{
    constexpr uint32_t kAppXam = 0xFE;
    constexpr uint32_t kAppXLiveBase = 0xFC;
    constexpr uint32_t kMsgTitleGetDeploymentType = 0x00022005;
    constexpr uint32_t kMsgOnlineGetNatType = 0x00058006;
    constexpr uint32_t kMsgLogonGetHr = 0x00058003;
    constexpr uint32_t kMsg58004 = 0x00058004;
    constexpr uint32_t kMsgPresenceInitialize = 0x00058056;
    bool g_presenceInitialized = false;
    constexpr uint32_t kOnlineLogonDisconnected = 0x001510F1; // XONLINE_S_LOGON_DISCONNECTED
    constexpr uint32_t kNatOpen = 1; // XONLINE_NAT_OPEN
    constexpr uint32_t kAppSocial = 0xF4;
    constexpr uint32_t kMsgSocialGetCapabilities = 0x0050001F;
    constexpr uint32_t kOnlineNotLoggedOn = 0x80151802; // XONLINE_E_LOGON_NOT_LOGGED_ON
    constexpr uint32_t kMsgEnableGuestSignin = 0x00021012;
    // XEnableGuestSignin: whether the Guide's sign-in screens offer guests. The runtime has no Guide sign-in UI (one
    // local profile is always signed in), so the setting is only kept.
    bool g_guestSigninEnabled = true;
    constexpr uint32_t kAppXgi = 0xFB;
    constexpr uint32_t kAppXmp = 0xFA;
    constexpr uint32_t kMsgUserSetContext = 0x000B0006;
    constexpr uint32_t kMsgUserSetProperty = 0x000B0007;
    constexpr uint32_t kMsgUserWriteAchievements = 0x000B0008;
    constexpr uint32_t kAchievementAchieved = 0x20000; // XACHIEVEMENT_DETAILS_ACHIEVED

    // Contexts and properties set by XUserSetContext / XUserSetPropertyEx, per user index. On a console they feed
    // rich presence, sessions and stats uploads; nothing reads them back offline, so they are kept and logged.
    struct UserValue
    {
        uint32_t context = 0;
        std::vector<uint8_t> property;
    };
    std::mutex g_userValuesMutex;
    std::map<std::pair<uint32_t, uint32_t>, UserValue> g_userValues;

    // Rich presence: what XAM would hand Xbox LIVE for these values, formatted here from the title's XLAST
    // (live/presence.h) and sent to our server when it changes.
    std::mutex g_presenceMutex;
    std::map<uint32_t, std::string> g_presenceSent; // per user

    const live::presence::Definitions& PresenceDefinitions()
    {
        static const live::presence::Definitions definitions = [] {
            live::presence::Definitions d;
            const auto xsrc = kernel::spa::Metadata(0x58535243); // 'XSRC'
            std::string error;
            if (xsrc.empty())
                Log("Presence: the SPA has no XLAST (XSRC): no rich presence");
            else if (!live::presence::LoadXsrc(xsrc.data(), xsrc.size(), d, error))
                Log("Presence: XLAST not read ({}): no rich presence", error);
            else
                Log("Presence: {} presence modes, {} contexts with value strings", d.modes.size(), d.contextValues.size());
            return d;
        }();
        return definitions;
    }

    std::string Utf8(const std::u16string& text)
    {
        std::string out;
        for (size_t i = 0; i < text.size(); i++)
        {
            uint32_t c = text[i];
            if (c >= 0xD800 && c < 0xDC00 && i + 1 < text.size())
                c = 0x10000 + ((c - 0xD800) << 10) + (text[++i] - 0xDC00);
            if (c < 0x80)
                out += char(c);
            else if (c < 0x800)
                out += { char(0xC0 | c >> 6), char(0x80 | (c & 0x3F)) };
            else if (c < 0x10000)
                out += { char(0xE0 | c >> 12), char(0x80 | (c >> 6 & 0x3F)), char(0x80 | (c & 0x3F)) };
            else
                out += { char(0xF0 | c >> 18), char(0x80 | (c >> 12 & 0x3F)), char(0x80 | (c >> 6 & 0x3F)),
                         char(0x80 | (c & 0x3F)) };
        }
        return out;
    }

    // A property's value as presence text ({p0x...}): numbers in decimal, strings as they are (the guest's
    // big-endian UTF-16); binary and date values have no text.
    std::string PropertyText(uint32_t id, const std::vector<uint8_t>& d)
    {
        auto be = [&](size_t n) {
            uint64_t v = 0;
            for (size_t i = 0; i < n && i < d.size(); i++)
                v = v << 8 | d[i];
            return v;
        };
        switch (id >> 28)
        {
        case 1: return std::to_string(int32_t(be(4)));
        case 2: return std::to_string(int64_t(be(8)));
        case 3: { const uint64_t v = be(8); double f; std::memcpy(&f, &v, 8); return std::format("{}", f); }
        case 5: { const uint32_t v = uint32_t(be(4)); float f; std::memcpy(&f, &v, 4); return std::format("{}", f); }
        case 4:
        {
            std::u16string text;
            for (size_t i = 0; i + 1 < d.size(); i += 2)
            {
                const char16_t c = char16_t(d[i] << 8 | d[i + 1]);
                if (c == 0)
                    break;
                text += c;
            }
            return Utf8(text);
        }
        default: return {};
        }
    }

    // Formats `user`'s presence; sends it when it changed (or always: after a sign-in, which starts with the title
    // name alone).
    void UpdatePresence(uint32_t user, bool always)
    {
        if (user >= 4)
            return;
        std::map<uint32_t, uint32_t> contexts;
        std::map<uint32_t, std::string> properties;
        {
            std::lock_guard lock(g_userValuesMutex);
            for (const auto& [key, value] : g_userValues)
                if (key.first == user)
                {
                    if (key.second >> 28 == 0)
                        contexts[key.second] = value.context;
                    else
                        properties[key.second] = PropertyText(key.second, value.property);
                }
        }
        const std::string rich = live::presence::Format(PresenceDefinitions(), contexts, properties,
            [](uint32_t id) { return id > 0xFFFF ? std::string() : Utf8(kernel::spa::String(uint16_t(id))); });
        {
            std::lock_guard lock(g_presenceMutex);
            auto [it, inserted] = g_presenceSent.try_emplace(user, rich);
            if (!always && !inserted && it->second == rich)
                return;
            it->second = rich;
        }
        Log("Presence: user {}: \"{}\"", user, rich);
        live::SetPresence(user, kernel::TitleId(), Utf8(kernel::spa::TitleName()), rich);
    }

    // The user a request names: by XUID when non-zero (a signed-in profile's offline XUID), else by index.
    bool ResolveUser(uint32_t userIndex, uint64_t xuid, uint32_t& user)
    {
        if (xuid != 0)
        {
            const auto signedIn = kernel::users::UserOfXuid(xuid);
            if (!signedIn)
                return false;
            user = *signedIn;
            return true;
        }
        user = userIndex;
        return user < 4 && kernel::UserSignedIn(user);
    }

    uint32_t XgiUserSet(uint32_t message, uint32_t buffer, uint32_t length)
    {
        // XGI_XUSER_SET_CONTEXT (0x18): user index, unused, xuid (u64), XUSER_CONTEXT { id, value }.
        // XGI_XUSER_SET_PROPERTY (0x20): user index, unused, xuid (u64), property id, data size, data address.
        // The type of an id is its top 4 bits (XPROPERTYTYPEFROMID: 0 = context, 1 int32, 2 int64, 3 double,
        // 4 wide string, 5 float, 6 binary, 7 datetime).
        auto& mem = guest::g_memory;
        const bool context = message == kMsgUserSetContext;
        if (length != (context ? 0x18u : 0x20u))
            Fatal("XGI message {:08X}: buffer of {} bytes", message, length);
        const uint32_t userIndex = mem.Read32(buffer);
        const uint64_t xuid = mem.Read64(buffer + 8);
        const uint32_t id = mem.Read32(buffer + 0x10);
        const uint32_t type = id >> 28;
        uint32_t user = 0;
        if (!ResolveUser(userIndex, xuid, user))
        {
            // TU46 sets its presence context right after "Play Game" even when nobody is signed in (owner's report,
            // session 15). Xenia Canary and Nexia360 both drop the value and answer success; there is no user to
            // keep it for. (XAM's own result for this case was not found in xam.xex 17559; the title's
            // XUserSetContextEx would turn an error into ERROR_FUNCTION_FAILED either way.)
            Log("XGI: message {:08X} for user {} / XUID {:016X}, who is not signed in: ignored", message, userIndex, xuid);
            return kernel::kErrorSuccess;
        }
        UserValue value;
        if (context)
        {
            if (type != 0)
                Fatal("XUserSetContext with id {:08X}, which is a property", id);
            value.context = mem.Read32(buffer + 0x14);
        }
        else
        {
            const uint32_t size = mem.Read32(buffer + 0x14);
            const uint32_t data = mem.Read32(buffer + 0x18);
            if (type == 0 || type > 7 || data == 0)
                Fatal("XUserSetPropertyEx with id {:08X}, size {}, data {:08X}", id, size, data);
            const uint8_t* bytes = mem.Translate(data);
            value.property.assign(bytes, bytes + size);
        }
        bool changed;
        {
            std::lock_guard lock(g_userValuesMutex);
            auto [it, inserted] = g_userValues.try_emplace({ user, id });
            changed = inserted || it->second.context != value.context || it->second.property != value.property;
            it->second = std::move(value);
            if (changed)
                Log("XGI: user {} {} {:08X} = {}", user, context ? "context" : "property", id,
                    context ? std::format("{}", it->second.context) : std::format("{} bytes", it->second.property.size()));
        }
        if (changed)
            UpdatePresence(user, false);
        return kernel::kErrorSuccess;
    }

    // A real console sends what a title earned to Xbox LIVE after XUserWriteAchievements (the profile sync,
    // UserTracker::ScheduleSync 0x819077D8: xstats syncachievements). Ours: when the user is signed in to our Xbox LIVE
    // server, the title's whole list (earned or not, the protocol's `achievements_update`) goes up on a thread of its
    // own (the request waits for the server's answer); the server keeps it for friends (Compare Games, the gamercard).
    void PushAchievementsToLive(uint32_t user)
    {
        if (!live::SignedIn(user))
            return;
        const uint64_t xuid = kernel::users::Xuid(user);
        const uint32_t titleId = kernel::TitleId();
        std::vector<live::AchievementInfo> list;
        for (const kernel::spa::Achievement& a : kernel::spa::Achievements())
        {
            if (list.size() >= 500) // the protocol's bound
                break;
            live::AchievementInfo info;
            info.id = a.id;
            info.imageId = a.imageId;
            info.gamerscore = a.gamerscore;
            info.flags = a.flags;
            info.label = Utf8(kernel::spa::String(a.labelId));
            info.description = Utf8(kernel::spa::String(a.descriptionId));
            info.unachieved = Utf8(kernel::spa::String(a.unachievedId));
            if (const auto earned = kernel::profile::ReadAchievement(xuid, titleId, a.id);
                earned && (earned->flags & kAchievementAchieved))
            {
                info.flags = earned->flags;
                // FILETIME -> Unix seconds
                const uint64_t seconds = earned->unlockTime / 10000000;
                info.earned = seconds > 11644473600ull ? int64_t(seconds - 11644473600ull) : 0;
            }
            list.push_back(std::move(info));
        }
        std::thread([user, titleId, list = std::move(list)] {
            if (const auto error = live::UpdateAchievements(user, titleId, list))
                Log("Live: the achievements of title {:08X} were not sent: {}", titleId, *error);
        }).detach();
    }

    // XUserWriteAchievements: buffer = XGI_WRITEACHIEVEMENT {DWORD count, PXUSER_ACHIEVEMENT achievements}, each
    // XUSER_ACHIEVEMENT {DWORD user index, DWORD achievement id}. An achievement the user has not earned yet is
    // stored in the title GPD of the profile, earned offline (no ACHIEVED_ONLINE flag), with the current time.
    // Earning one again changes nothing. XAM then queues one toast per user with new achievements (0x8190FBE8): one
    // achievement = notification 0x1B with "<gamerscore>|<label>" (label cut to 32 characters), several = 0x27 with
    // "<count>|<total gamerscore>".
    uint32_t XgiWriteAchievements(uint32_t buffer, uint32_t length)
    {
        auto& mem = guest::g_memory;
        if (length != 8)
            Fatal("XUserWriteAchievements: buffer of {} bytes", length);
        const uint32_t count = mem.Read32(buffer), list = mem.Read32(buffer + 4);
        std::vector<const kernel::spa::Achievement*> earnedBy[4];
        for (uint32_t i = 0; i < count; i++)
        {
            const uint32_t user = mem.Read32(list + i * 8), id = mem.Read32(list + i * 8 + 4);
            if (user >= 4 || !kernel::UserSignedIn(user))
                Fatal("XUserWriteAchievements for user {}, who is not signed in", user);
            const auto& all = kernel::spa::Achievements();
            const auto it = std::find_if(all.begin(), all.end(), [&](const kernel::spa::Achievement& a) { return a.id == id; });
            if (it == all.end())
                Fatal("XUserWriteAchievements: achievement {} is not in the title's SPA", id);
            const uint64_t xuid = kernel::users::Xuid(user);
            if (const auto earned = kernel::profile::ReadAchievement(xuid, kernel::TitleId(), id);
                earned && (earned->flags & kAchievementAchieved))
                continue;
            FILETIME now;
            GetSystemTimeAsFileTime(&now);
            kernel::profile::AchievementRecord record;
            record.id = id;
            record.imageId = it->imageId;
            record.gamerscore = it->gamerscore;
            record.flags = it->flags | kAchievementAchieved;
            record.unlockTime = uint64_t(now.dwHighDateTime) << 32 | now.dwLowDateTime;
            record.label = kernel::spa::String(it->labelId);
            record.description = kernel::spa::String(it->descriptionId);
            record.unachieved = kernel::spa::String(it->unachievedId);
            kernel::profile::WriteAchievement(xuid, kernel::TitleId(), record);
            Log("XGI: user {} earned achievement {} ({} gamerscore)", user, id, it->gamerscore);
            earnedBy[user].push_back(&*it);
        }
        for (uint32_t user = 0; user < 4; user++)
            if (!earnedBy[user].empty())
                PushAchievementsToLive(user);
        for (uint32_t user = 0; user < 4; user++)
        {
            const auto& earned = earnedBy[user];
            if (earned.size() == 1)
            {
                const std::string score = std::to_string(earned[0]->gamerscore) + "|";
                xui::QueueNotification(xui::kNotifyAchievement, user, std::u16string(score.begin(), score.end()) +
                                       kernel::spa::String(earned[0]->labelId).substr(0, 32));
            }
            else if (earned.size() > 1)
            {
                uint32_t total = 0;
                for (const auto* a : earned)
                    total += a->gamerscore;
                const std::string param = std::format("{}|{}", earned.size(), total);
                xui::QueueNotification(xui::kNotifyAchievements, user, std::u16string(param.begin(), param.end()));
            }
        }
        return kernel::kErrorSuccess;
    }

    // Handles a message; returns its X_ERROR/HRESULT result.
    uint32_t DispatchAppMessage(uint32_t app, uint32_t message, uint32_t buffer, uint32_t length)
    {
        auto& mem = guest::g_memory;
        // An older dashboard's XAM numbered some messages differently: they become the 17559 ones (xam_blades.cpp).
        message = kernel::OlderDashMessage(app, message);
        if (app == kAppXam && message == kMsgTitleGetDeploymentType)
        {
            // XTitleGetDeploymentType: buffer = { PDWORD DeploymentType, PXOVERLAPPED Overlapped }.
            const uint32_t typePtr = mem.Read32(buffer);
            if (typePtr)
                mem.Write32(typePtr, Settings().deploymentType);
            return kernel::kErrorSuccess;
        }
        if (app == kAppXam && message == kMsgEnableGuestSignin)
        {
            // XEnableGuestSignin(BOOL fEnable): buffer = &fEnable; the wrapper maps a failure to ERROR_FUNCTION_FAILED.
            const bool enable = mem.Read32(buffer) != 0;
            if (enable != g_guestSigninEnabled)
                Log("XAM: guest sign-in {}", enable ? "enabled" : "disabled");
            g_guestSigninEnabled = enable;
            return kernel::kErrorSuccess;
        }
        if (app == kAppXLiveBase && message == kMsgLogonGetHr)
        {
            // XLiveBase's logon state (XAM 0x818CCE08 case 0: the logon object's last HRESULT): the dashboard asks it
            // at start-up and before online features. XONLINE_S_LOGON_CONNECTION_ESTABLISHED while a profile is
            // logged on to the ports' own Xbox LIVE (kernel/xam_live.cpp), else disconnected.
            for (uint32_t user = 0; user < 4; user++)
                if (kernel::UserOnLive(user))
                    return 0x001510F0;
            return kOnlineLogonDisconnected;
        }
        if (app == kAppXLiveBase && message == kMsg58004)
        {
            // XAM 0x818CCE08 answers this one with a constant: *(DWORD*)buffer = 1, S_OK. The dash asks it at
            // start-up; its name is not known.
            if (buffer)
                mem.Write32(buffer, 1);
            return kernel::kErrorSuccess;
        }
        if (app == kAppXLiveBase && message == kMsgPresenceInitialize)
        {
            // XPresenceInitializeEx (XAM 0x818CCE08 case 0x53 -> 0x818D5978), an in-process call: the first parameter
            // (the user buffer from XOnlineSetupUserBuffer) is unused, the second is an argument list (32 entries of
            // { u32 size; u32 pad; u64 pointer }, count at +0x200) whose entry 0 points at cMaxPeerSubscriptions.
            // XAM allocates that many peer subscriptions once; it needs no Xbox LIVE.
            const uint32_t arguments = length;
            const uint32_t count = arguments ? mem.Read32(arguments + 0x200) : 0;
            const uint32_t peers = count ? mem.Read32(mem.Read32(arguments + 12)) : 0;
            if (peers == 0)
                return 0x80070057; // E_INVALIDARG
            if (peers > 400)
                return 0x8015200E;
            if (g_presenceInitialized)
                return 0x80004005; // E_FAIL: already initialised
            g_presenceInitialized = true;
            return kernel::kErrorSuccess;
        }
        if (app == kAppXLiveBase && message == kMsgOnlineGetNatType)
        {
            // XOnlineGetNatType: buffer = PDWORD NatType. The NAT type is detected at Xbox LIVE logon, which never
            // happens offline. The title's XAPI wrapper itself answers XONLINE_NAT_OPEN when this call fails, and
            // Xenia's netplay fork answers OPEN, so the title sees OPEN either way.
            if (buffer)
                mem.Write32(buffer, kNatOpen);
            return kernel::kErrorSuccess;
        }
        if (app == kAppSocial && message == kMsgSocialGetCapabilities)
        {
            // XSocialGetCapabilities: the XDK says it fails with ERROR_FUNCTION_FAILED unless a user is signed in to
            // Xbox LIVE. The title's wrapper turns this (non-Win32) HRESULT into ERROR_FUNCTION_FAILED.
            return kOnlineNotLoggedOn;
        }
        if (app == kAppXmp)
            if (const auto result = kernel::xmp::Dispatch(message, buffer, length))
                return *result;
        if (app == kAppXgi && (message == kMsgUserSetContext || message == kMsgUserSetProperty))
            return XgiUserSet(message, buffer, length);
        if (app == kAppXgi && message == kMsgUserWriteAchievements)
            return XgiWriteAchievements(buffer, length);
        if (app == kAppXLiveBase && message == 0x00058020)
        {
            // XFriendsCreateEnumerator (dash 0x9288D1E8; XMsgInProcessCall(0xFC, 0x58020, context, arguments)):
            // kernel/xam_live.cpp (XAM 0x818DDF40). Here buffer / length are the two parameters.
            return kernel::FriendsCreateEnumerator(length);
        }
        if (app == kAppXgi && message == 0x000B0010)
        {
            // XSessionCreate (after XamSessionCreateHandle / XamSessionRefObjByHandle): buffer = { session object,
            // flags, public slots, private slots, user index, XSESSION_INFO*, ULONGLONG* nonce } (28 bytes). Offline
            // a session is local: its XSESSION_INFO (XNKID session id 8, XNADDR host address 36, XNKEY 16) gets a
            // random id and key and a zero address (no network), the nonce a random value.
            // A session of a user who is on our Xbox LIVE and that uses presence, stats, matchmaking or arbitration
            // (kernel/xam_matchmaking.h) gets an online id (top nibble 8) and is registered on the server (the host's);
            // the client side of such a session (no host flag) keeps the XSESSION_INFO the title found in a search.
            // Any other is a system link session: a random id (top nibble 0) and this console's address on the PC's
            // network (kernel/xam_net.cpp). (As the Dashboard's runtime.)
            if (length < 28)
                return 0x80070057;
            const uint32_t info = mem.Read32(buffer + 0x14), nonce = mem.Read32(buffer + 0x18);
            const uint32_t flags = mem.Read32(buffer + 4), userIndex = mem.Read32(buffer + 0x10);
            const bool online = kernel::matchmaking::UsesLive(userIndex, flags);
            auto random = [] {
                static uint64_t state = 0x9E3779B97F4A7C15ull ^ uint64_t(std::chrono::steady_clock::now().time_since_epoch().count());
                state ^= state << 13; state ^= state >> 7; state ^= state << 17;
                return state;
            };
            uint64_t id = 0;
            if (info)
            {
                const bool keepInfo = online && !(flags & kernel::matchmaking::kCreateHost) &&
                                      (mem.Read8(info) & 0xF0) == 0x80;
                if (!keepInfo)
                {
                    std::memset(mem.Translate(info), 0, 0x3C);
                    mem.Write64(info, (random() & ~(0xF0ull << 56)) | (online ? 0x80ull << 56 : 0)); // XNKID
                    kernel::WriteTitleXnAddr(info + 8);             // XNADDR
                    mem.Write64(info + 0x2C, random());             // XNKEY
                    mem.Write64(info + 0x34, random());
                }
                id = mem.Read64(info);
            }
            if (nonce)
                mem.Write64(nonce, random());
            Log("XSessionCreate: flags {:X}, {} public / {} private slots, user {}", flags,
                mem.Read32(buffer + 8), mem.Read32(buffer + 0xC), userIndex);
            if (online && id)
                kernel::matchmaking::SessionCreated(mem.Read32(buffer), userIndex, flags, mem.Read32(buffer + 8),
                                                    mem.Read32(buffer + 0xC), id);
            return kernel::kErrorSuccess;
        }
        if (app == kAppXgi)
            if (const auto stats = kernel::stats::Message(message, buffer, length))
                return *stats; // XUserReadStats, XUserResetStatsView, XSessionWriteStats, XSessionFlushStats
        if (app == kAppXLiveBase && message == kernel::storage::kMsgBuildServerPath)
            return kernel::storage::BuildServerPath(buffer); // XStorageBuildServerPath(ByXuid)
        if (app == kAppXgi && message == kernel::matchmaking::kMsgSessionSearchByIds)
            return kernel::matchmaking::SessionSearchByIds(buffer, length); // XSessionSearchByIds
        if (app == kAppXgi && message == kernel::matchmaking::kMsgSessionSearch)
            return kernel::matchmaking::SessionSearch(buffer, length); // XSessionSearch / XSessionSearchEx
        if (app == kAppXgi && kernel::matchmaking::SessionMessage(message, buffer, length))
            return kernel::kErrorSuccess; // delete, join, leave, start, end: mirrored on our Xbox LIVE
        if (app == kAppXgi && message >= 0x000B0011 && message <= 0x000B0021)
        {
            // The rest of a local session's life (delete, join / leave local users, start, end, modify, write and
            // flush stats ...): nothing to publish without Xbox LIVE. Logged once per message.
            static std::set<uint32_t> seen;
            static std::mutex seenMutex;
            std::lock_guard lock(seenMutex);
            if (seen.insert(message).second)
                Log("XGI session message {:08X} ({} bytes): succeeds offline", message, length);
            return kernel::kErrorSuccess;
        }
        if (app == kAppXgi && message == 0x000B0036)
        {
            // XGI 0xB0036 (case 0x33 -> 0x8191B558): XAM's usage statistics record event 0x4064000F in its three
            // recorders (0x8190BBB0); nothing reads them back offline. The dash sends it when the games library
            // opens its title list (0x922C8xxx).
            return kernel::kErrorSuccess;
        }
        if (app == kAppXLiveBase && message == 0x00058055)
            return kernel::OnlineGetLogonExtendedStatus(buffer); // XOnlineGetLogonExtendedStatus (kernel/xam_live.cpp)
        if (app == kAppXLiveBase && message == 0x00058007)
            return kernel::OnlineGetServiceInfo(buffer, length); // XOnlineGetServiceInfo(ServiceId, Info)
        if (app == kAppXLiveBase && message == 0x00058024)
            return kernel::MessageEnumerate(length); // XMessageEnumerate (kernel/xam_live.cpp)
        if (app == kAppXLiveBase && message == 0x00058072)
        {
            // XOnlineGetWebServiceTaskBufferSize (dash 0x9224AD28; XMsgInProcessCall(0xFC, 0x58072, arguments, 0)):
            // kernel/xam_web_service.cpp.
            return kernel::WebServiceTaskBufferSize(buffer);
        }
        if (app == kAppXLiveBase && message == 0x0005800E)
        {
            // XUserAreUsersFriends(UserIndex, Xuids, Count, pfResult, NULL) (title xonline library; Minecraft asks it of
            // the host and each player when it joins a game, ClientConnection): XMsgInProcessCall(0xFC, 0x5800E, args,
            // pfResult), args = { user index, count, the XUIDs (8 bytes each) }. INFER: the layout from Minecraft's
            // request (00000000 00000001 <XUID>) and its source; XAM's handler (0x818CCE08 case 0xE) was not followed.
            // TRUE when every XUID is an accepted friend of the user on our Xbox LIVE.
            const uint32_t user = mem.Read32(buffer), count = mem.Read32(buffer + 4), result = length;
            if (user >= 4 || count == 0 || count > 100 || result == 0)
                return 0x80070057;
            bool all = true;
            const auto friends = live::Friends(user);
            for (uint32_t i = 0; i < count && all; i++)
            {
                const uint64_t xuid = mem.Read64(buffer + 8 + i * 8);
                all = std::any_of(friends.begin(), friends.end(),
                                  [&](const live::Friend& f) { return f.xuid == xuid && f.request.empty(); });
            }
            mem.Write32(result, all ? 1 : 0);
            Log("XUserAreUsersFriends: user {}, {} XUIDs (first {:016X}): {}", user, count, mem.Read64(buffer + 8),
                all ? "friends" : "not all friends");
            return kernel::kErrorSuccess;
        }
        // Exploration only (as for imports): CB_CONTINUE_UNIMPLEMENTED=1 logs the message and fails it with E_FAIL.
        static const bool keepGoing = [] { const char* v = std::getenv("CB_CONTINUE_UNIMPLEMENTED"); return v && v[0] == '1'; }();
        std::string bytes; // the request's first words, to tell what it is
        for (uint32_t i = 0; buffer && buffer < 0xC0000000 && i < 32; i++)
            bytes += std::format("{:02X}{}", mem.Read8(buffer + i), i % 4 == 3 ? " " : "");
        if (!keepGoing)
            Fatal("XAM message {:08X} to app {:02X} (buffer {:08X}, {} bytes: {}) is not implemented", message, app, buffer,
                  length, bytes);
        Log("UNIMPLEMENTED XAM MESSAGE {:08X} to app {:02X} (buffer {:08X}, {} bytes): E_FAIL", message, app, buffer, length);
        return 0x80004005;
    }
}

namespace kernel
{
    std::map<uint32_t, std::vector<uint8_t>> UserMatchmakingValues(uint32_t user)
    {
        std::map<uint32_t, std::vector<uint8_t>> out;
        std::lock_guard lock(g_userValuesMutex);
        for (const auto& [key, value] : g_userValues)
        {
            if (key.first != user)
                continue;
            if (key.second >> 28 == 0)
            {
                const uint32_t v = value.context;
                out[key.second] = { uint8_t(v >> 24), uint8_t(v >> 16), uint8_t(v >> 8), uint8_t(v) };
            }
            else
                out[key.second] = value.property;
        }
        return out;
    }

    void RefreshPresence(uint32_t user)
    {
        UpdatePresence(user, true);
    }

    void CompleteOverlapped(uint32_t overlapped, uint32_t result, uint32_t extendedError, uint32_t length)
    {
        auto& mem = guest::g_memory;
        mem.Write32(overlapped + 0x00, result);
        mem.Write32(overlapped + 0x04, length);
        mem.Write32(overlapped + 0x08, kCurrentThreadHandle);
        mem.Write32(overlapped + 0x18, extendedError);
        // A completion routine runs as a user APC on the requesting thread (the one completing here):
        // routine(dwErrorCode, dwNumberOfBytesTransfered, pOverlapped).
        if (const uint32_t routine = mem.Read32(overlapped + 0x10))
        {
            GuestThread* thread = CurrentThread();
            if (thread == nullptr)
                Fatal("Overlapped request {:08X} with a completion routine completed outside a guest thread", overlapped);
            QueueUserApc(thread, { routine, result, length, overlapped });
        }
        if (const uint32_t eventHandle = mem.Read32(overlapped + 0x0C))
        {
            const uint32_t event = LookupHandle(eventHandle);
            if (event == 0)
                Fatal("Overlapped request {:08X} names invalid event handle {:08X}", overlapped, eventHandle);
            auto lock = LockDispatcher();
            mem.Write32(event + header::kSignalState, 1);
            NotifyDispatcherChange(event);
        }
    }

    void BeginOverlapped(uint32_t overlapped)
    {
        auto& mem = guest::g_memory;
        mem.Write32(overlapped + 0x00, kErrorIoPending);
        mem.Write32(overlapped + 0x04, 0);
        mem.Write32(overlapped + 0x18, 0);
        if (const uint32_t eventHandle = mem.Read32(overlapped + 0x0C))
        {
            const uint32_t event = LookupHandle(eventHandle);
            if (event == 0)
                Fatal("Overlapped request {:08X} names invalid event handle {:08X}", overlapped, eventHandle);
            auto lock = LockDispatcher();
            mem.Write32(event + header::kSignalState, 0);
        }
    }

    void CompleteOverlappedWin32(uint32_t overlapped, uint32_t error, uint32_t information)
    {
        constexpr uint32_t kErrorFunctionFailed = 0x65B;
        if (error == kErrorSuccess)
            CompleteOverlapped(overlapped, kErrorSuccess, 0, information);
        else
            CompleteOverlapped(overlapped, kErrorFunctionFailed, 0x80070000 | (error & 0xFFFF), 0);
    }

    void CompleteOverlappedImmediate(uint32_t overlapped, uint32_t result)
    {
        CompleteOverlapped(overlapped, result, result, result == kErrorSuccess ? 0 : 0xFFFFFFFF);
    }
}

namespace
{
    // XMsgStartIORequest(Ex)(App, Message, Overlapped, Buffer, BufferSize[, Options]) (XAM 0x816A2CF0). With an
    // overlapped structure the request completes through it (here at once) and the call returns ERROR_IO_PENDING.
    // Without one XAM uses an event of its own, waits for the request, stores its extended error (the handler's
    // HRESULT) as the thread's last error (RtlSetLastError) and returns 0: callers read the result with
    // XGetOverlappedExtendedError(NULL).
    void StartIORequest(PPCContext& ctx)
    {
        const uint32_t overlapped = ctx.r5.u32;
        if (ctx.r3.u32 == kAppXLiveBase && ctx.r4.u32 >= 0x0005806A && ctx.r4.u32 <= 0x0005806F)
        {
            // XOnlineCallWebService and its Begin / Continue / Close calls (xam_web_call.cpp): through our server.
            // XAM completes each through the overlapped, later: XMsgCompleteIORequest(ov, hr < 0 ? 0x65B : 0, hr, 0).
            // The completion must come after the call returns: the dash's Lua async helper (LuaMultiOverlapped,
            // Dash.Search.lex's RetrieveSearchManifest) re-arms one overlapped for each step and takes one that is
            // already complete when the step returns as "no further step" (the download ended after BeginResponse).
            // So with an overlapped (and no completion routine) the work runs on a host thread.
            auto& mem = guest::g_memory;
            kernel::WebCall call = kernel::WebCallPrepare(ctx, ctx.r4.u32, ctx.r6.u32);
            if (overlapped && !call.sync && mem.Read32(overlapped + 0x10) == 0)
            {
                kernel::BeginOverlapped(overlapped);
                std::thread([call = std::move(call), overlapped] {
                    const uint32_t hr = kernel::WebCallRun(call);
                    kernel::CompleteOverlapped(overlapped, int32_t(hr) < 0 ? 0x65B : 0, hr, 0);
                }).detach();
                ctx.r3.u64 = kernel::kErrorIoPending;
                return;
            }
            const uint32_t hr = kernel::WebCallRun(call);
            if (overlapped)
            {
                kernel::CompleteOverlapped(overlapped, int32_t(hr) < 0 ? 0x65B : 0, hr, 0);
                ctx.r3.u64 = kernel::kErrorIoPending;
                return;
            }
            const uint32_t thread = mem.Read32(ctx.r13.u32 + kernel::pcr::kCurrentThread);
            mem.Write32(thread + kernel::kthread::kLastError, hr);
            ctx.r3.u64 = 0;
            return;
        }
        if (ctx.r3.u32 == kAppXLiveBase && ctx.r4.u32 >= 0x00050000 && ctx.r4.u32 < 0x00058000)
        {
            // An Xbox LIVE service task (an ".ashx" service): 0x50000 | the request schema's index; the message buffer
            // holds the title's CXLiveAsyncTask (dash CXLiveAsyncTask::Initialize 0x9224EF78): +0 ordinal, +8 schema
            // index, +0xC flags (2: no completion, 4: raw response), +0x18 / +0x1C the marshalled request, +0x24 the
            // default count, +0x2C / +0x30 the user buffer and its size. XAM (FUN_818DC3D8) posts the request to the
            // service and unmarshals the reply into the user buffer (FUN_81A3D7E8); our server does both (Shared
            // server/xon_services.py) and returns the user buffer's bytes. Completion as XAM's FUN_818DC188: task+0x30
            // = bytes used, XMsgCompleteIORequest(ov, hr < 0 ? 0x65B : 0, hr, used). No server, or a path it has no
            // service for: XONLINE_E_LOGON_SERVICE_NOT_REQUESTED.
            auto& mem = guest::g_memory;
            constexpr uint32_t kServiceNotRequested = 0x80151100;
            const uint32_t task = ctx.r6.u32 ? mem.Read32(ctx.r6.u32) : 0;
            uint32_t hr = kServiceNotRequested, used = 0, flags = 0;
            if (task)
            {
                const uint32_t index = mem.Read32(task + 8);
                flags = mem.Read32(task + 0xC);
                const uint32_t wire = mem.Read32(task + 0x18), wireSize = mem.Read32(task + 0x1C);
                const uint32_t base = mem.Read32(task + 0x2C), size = mem.Read32(task + 0x30);
                if (index != (ctx.r4.u32 & 0x7FFF))
                    Fatal("Xbox LIVE service task {:05X}: its task names schema index {}", ctx.r4.u32, index);
                if (const uint32_t ordinal = mem.Read32(task) & 0xFFFF; kernel::storage::IsTask(ordinal))
                {
                    // XStorage (kernel/xam_storage.h): the data moves between the title's buffer and our server. Its
                    // download is a raw response (task flag 4, XStorageDownloadToMemory): handled here, so before that check.
                    hr = kernel::storage::RunTask(ordinal, task, used);
                    if (int32_t(hr) >= 0)
                        mem.Write32(task + 0x30, used);
                    Log("Xbox LIVE service task {:05X} (XStorage ordinal {:04X}): {:08X}, {} bytes", ctx.r4.u32, ordinal, hr, used);
                }
                else
                {
                if (flags & 4)
                    Fatal("Xbox LIVE service task {:05X} (schema ordinal {:04X}): raw responses (task flag 4) are not handled",
                          ctx.r4.u32, mem.Read32(task) & 0xFFFF);
                std::vector<uint8_t> request;
                if (wire && wireSize)
                {
                    const uint8_t* p = mem.Translate(wire);
                    request.assign(p, p + wireSize);
                }
                int user = -1;
                for (uint32_t u = 0; u < 4 && user < 0; u++)
                    if (kernel::UserOnLive(u))
                        user = int(u);
                const auto reply = live::XonTask(user, kernel::OnlineSchemaBlob(), index, request, base, size,
                                                 mem.Read32(task + 0x24));
                if (reply)
                {
                    hr = reply->hr;
                    if (int32_t(hr) >= 0 && !reply->native.empty())
                    {
                        if (reply->native.size() > size)
                            Fatal("Xbox LIVE service {}: {} bytes for a {}-byte user buffer", reply->path,
                                  reply->native.size(), size);
                        memcpy(mem.Translate(base), reply->native.data(), reply->native.size());
                        used = uint32_t(reply->native.size());
                    }
                    if (int32_t(hr) >= 0)
                        mem.Write32(task + 0x30, used);
                }
                Log("Xbox LIVE service task {:05X} (ordinal {:04X}) {}: {:08X}, {} bytes", ctx.r4.u32,
                    mem.Read32(task) & 0xFFFF, reply ? reply->path : std::string("(no server)"), hr, used);
                }
            }
            const uint32_t win32 = int32_t(hr) < 0 ? 0x65B : 0;
            if (overlapped)
            {
                if (!(flags & 2))
                    kernel::CompleteOverlapped(overlapped, win32, hr, used);
                ctx.r3.u64 = kernel::kErrorIoPending;
                return;
            }
            const uint32_t thread = mem.Read32(ctx.r13.u32 + kernel::pcr::kCurrentThread);
            mem.Write32(thread + kernel::kthread::kLastError, hr);
            ctx.r3.u64 = 0;
            return;
        }
        const uint32_t result = DispatchAppMessage(ctx.r3.u32, ctx.r4.u32, ctx.r6.u32, ctx.r7.u32);
        if (overlapped)
        {
            kernel::CompleteOverlappedImmediate(overlapped, result);
            ctx.r3.u64 = kernel::kErrorIoPending;
            return;
        }
        const uint32_t thread = guest::g_memory.Read32(ctx.r13.u32 + kernel::pcr::kCurrentThread);
        guest::g_memory.Write32(thread + kernel::kthread::kLastError, result);
        ctx.r3.u64 = 0;
    }
}

// DWORD XMsgStartIORequest(HXAMAPP App, DWORD Message, PXOVERLAPPED Overlapped, PVOID Buffer, DWORD BufferSize)
// The work is done immediately; with an overlapped structure the request completes through it and the call returns
// ERROR_IO_PENDING.
GUEST_IMPORT(XMsgStartIORequest)
{
    StartIORequest(ctx);
}

// DWORD XMsgStartIORequestEx(HXAMAPP App, DWORD Message, PXOVERLAPPED Overlapped, PVOID Buffer, DWORD BufferSize,
//                            PDWORD Options) (XAM 0x816A2CF0): XMsgStartIORequest with an options word that only steers
// how XAM waits for a synchronous request (no overlapped: XAM makes an event and waits on it). Requests complete at
// once here, so the result is the same.
GUEST_IMPORT(XMsgStartIORequestEx)
{
    StartIORequest(ctx);
}

// DWORD XMsgCancelIORequest(PXOVERLAPPED Overlapped, BOOL Wait) (XCancelOverlapped passes Wait = TRUE; the XDK:
// "after the function returns, it is safe to release the XOVERLAPPED structure"). A request that already completed
// is left alone. Only system-UI requests stay pending in this runtime; cancelling one completes it at once with
// ERROR_CANCELLED, so there is never anything to wait for.
GUEST_IMPORT(XMsgCancelIORequest)
{
    const uint32_t overlapped = ctx.r3.u32;
    if (overlapped == 0)
        Fatal("XMsgCancelIORequest without an overlapped structure");
    if (guest::g_memory.Read32(overlapped) == kernel::kErrorIoPending && !kernel::CancelSystemUi(overlapped))
        Fatal("XMsgCancelIORequest: overlapped {:08X} is pending but belongs to no request the runtime knows", overlapped);
    ctx.r3.u64 = kernel::kErrorSuccess;
}

// DWORD XMsgInProcessCall(HXAMAPP App, DWORD Message, PVOID Param1, PVOID Param2): a synchronous request; the
// parameters are the message's buffer and its length.
GUEST_IMPORT(XMsgInProcessCall)
{
    ctx.r3.u64 = DispatchAppMessage(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32);
}

// HRESULT XMsgSystemProcessCall(HXAMAPP App, DWORD Message, PVOID Buffer, DWORD Length) (XAM 0x816A2FF8): copies the
// buffer and schedules the message as a XAM task on the system process; returns 0 once queued (the handler's result
// is dropped), 0x80070057 for an unknown app. Known message: XNuiDelayUI(DWORD) (dash 0x92192058) = app 0xFE
// 0x2B001 with the DWORD: XAM's handler (0x816F9CB8 case 0 -> 0x816AC460) sets the fade value of its Kinect HUD
// elements (0x81809708); no Kinect HUD here, so it is only logged. Other messages stop.
GUEST_IMPORT(XMsgSystemProcessCall)
{
    const uint32_t app = ctx.r3.u32, message = ctx.r4.u32, buffer = ctx.r5.u32, length = ctx.r6.u32;
    constexpr uint32_t kMsgNuiDelayUi = 0x0002B001;
    if (app == kAppXam && message == kMsgNuiDelayUi && buffer != 0 && length == 4)
    {
        Log("XAM: XNuiDelayUI({:08X}) (no Kinect HUD)", guest::g_memory.Read32(buffer));
        ctx.r3.u64 = kernel::kErrorSuccess;
        return;
    }
    Fatal("XMsgSystemProcessCall: app {:02X} message {:08X} (buffer {:08X}, {} bytes) is not implemented", app, message,
        buffer, length);
}

// DWORD XamGetOverlappedResult(PXOVERLAPPED Overlapped, PDWORD Result, BOOL Wait) (XAM 17559 0x816A2898): while
// InternalLow is ERROR_IO_PENDING it returns ERROR_IO_INCOMPLETE, unless Wait is set and the request has an event:
// then it waits for the event (NtWaitForSingleObjectEx, user mode, not alertable, no timeout). Then *Result =
// InternalHigh and it returns InternalLow.
GUEST_IMPORT(XamGetOverlappedResult)
{
    auto& mem = guest::g_memory;
    const uint32_t overlapped = ctx.r3.u32, resultPtr = ctx.r4.u32;
    const bool wait = ctx.r5.u32 != 0;
    if (mem.Read32(overlapped) == kernel::kErrorIoPending)
    {
        const uint32_t eventHandle = mem.Read32(overlapped + 0x0C);
        if (!wait || eventHandle == 0)
        {
            ctx.r3.u64 = kernel::kErrorIoIncomplete;
            return;
        }
        const uint32_t event = kernel::LookupHandle(eventHandle);
        if (event == 0)
            Fatal("XamGetOverlappedResult({:08X}): invalid event handle {:08X}", overlapped, eventHandle);
        kernel::WaitForObjects(&event, 1, false, 0);
    }
    if (resultPtr)
        mem.Write32(resultPtr, mem.Read32(overlapped + 0x04));
    ctx.r3.u64 = mem.Read32(overlapped);
}
